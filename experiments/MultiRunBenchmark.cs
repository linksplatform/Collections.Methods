using System;
using System.Collections.Generic;
using System.Linq;
using Platform.Diagnostics;
using Platform.Collections.Methods.Tests;

namespace Platform.Collections.Methods.Performance
{
    class MultiRunBenchmark
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Multiple Run Performance Comparison: With vs Without unchecked blocks");
            Console.WriteLine("=====================================================================");
            
            const int iterations = 1500;
            const int warmupIterations = 300;
            const int testRuns = 5;
            
            Console.WriteLine($"Testing with {iterations} operations per test, {testRuns} runs each.");
            Console.WriteLine($"Warmup with {warmupIterations} operations first.");
            Console.WriteLine();

            // Warm up both implementations
            var warmupWithUnchecked = new SizedAndThreadedAVLBalancedTree<uint>(warmupIterations + 1000);
            var warmupWithoutUnchecked = new SizedAndThreadedAVLBalancedTreeWithoutUnchecked<uint>(warmupIterations + 1000);
            
            Console.WriteLine("Warming up both implementations...");
            warmupWithUnchecked.TestMultipleCreationsAndDeletions(
                warmupWithUnchecked.Allocate, 
                warmupWithUnchecked.Free, 
                ref warmupWithUnchecked.Root, 
                () => warmupWithUnchecked.Count, 
                warmupIterations);
                
            warmupWithoutUnchecked.TestMultipleCreationsAndDeletions(
                warmupWithoutUnchecked.Allocate, 
                warmupWithoutUnchecked.Free, 
                ref warmupWithoutUnchecked.Root, 
                () => warmupWithoutUnchecked.Count, 
                warmupIterations);

            Console.WriteLine("Warmup completed. Starting performance tests...");
            Console.WriteLine();

            // Sequential operations test
            var sequentialResultsWithUnchecked = new List<double>();
            var sequentialResultsWithoutUnchecked = new List<double>();
            
            Console.WriteLine("Test 1: Sequential operations");
            Console.WriteLine("------------------------------");
            
            for (int run = 1; run <= testRuns; run++)
            {
                Console.Write($"Run {run}/{testRuns}... ");
                
                // Test with unchecked blocks
                var avlTreeWithUnchecked = new SizedAndThreadedAVLBalancedTree<uint>(iterations + 1000);
                var timeWithUnchecked = Platform.Diagnostics.Performance.Measure(() =>
                {
                    avlTreeWithUnchecked.TestMultipleCreationsAndDeletions(
                        avlTreeWithUnchecked.Allocate, 
                        avlTreeWithUnchecked.Free, 
                        ref avlTreeWithUnchecked.Root, 
                        () => avlTreeWithUnchecked.Count, 
                        iterations);
                });
                
                // Test without unchecked blocks
                var avlTreeWithoutUnchecked = new SizedAndThreadedAVLBalancedTreeWithoutUnchecked<uint>(iterations + 1000);
                var timeWithoutUnchecked = Platform.Diagnostics.Performance.Measure(() =>
                {
                    avlTreeWithoutUnchecked.TestMultipleCreationsAndDeletions(
                        avlTreeWithoutUnchecked.Allocate, 
                        avlTreeWithoutUnchecked.Free, 
                        ref avlTreeWithoutUnchecked.Root, 
                        () => avlTreeWithoutUnchecked.Count, 
                        iterations);
                });
                
                sequentialResultsWithUnchecked.Add(timeWithUnchecked.TotalMilliseconds);
                sequentialResultsWithoutUnchecked.Add(timeWithoutUnchecked.TotalMilliseconds);
                
                Console.WriteLine($"WITH: {timeWithUnchecked.TotalMilliseconds:F1}ms, WITHOUT: {timeWithoutUnchecked.TotalMilliseconds:F1}ms");
            }

            // Random operations test
            var randomResultsWithUnchecked = new List<double>();
            var randomResultsWithoutUnchecked = new List<double>();
            
            Console.WriteLine();
            Console.WriteLine("Test 2: Random operations");
            Console.WriteLine("--------------------------");
            
            for (int run = 1; run <= testRuns; run++)
            {
                Console.Write($"Run {run}/{testRuns}... ");
                
                // Test with unchecked blocks
                var avlTreeWithUnchecked = new SizedAndThreadedAVLBalancedTree<uint>(iterations + 1000);
                var timeWithUnchecked = Platform.Diagnostics.Performance.Measure(() =>
                {
                    avlTreeWithUnchecked.TestMultipleRandomCreationsAndDeletions(
                        ref avlTreeWithUnchecked.Root, 
                        () => avlTreeWithUnchecked.Count, 
                        iterations);
                });
                
                // Test without unchecked blocks
                var avlTreeWithoutUnchecked = new SizedAndThreadedAVLBalancedTreeWithoutUnchecked<uint>(iterations + 1000);
                var timeWithoutUnchecked = Platform.Diagnostics.Performance.Measure(() =>
                {
                    avlTreeWithoutUnchecked.TestMultipleRandomCreationsAndDeletions(
                        ref avlTreeWithoutUnchecked.Root, 
                        () => avlTreeWithoutUnchecked.Count, 
                        iterations);
                });
                
                randomResultsWithUnchecked.Add(timeWithUnchecked.TotalMilliseconds);
                randomResultsWithoutUnchecked.Add(timeWithoutUnchecked.TotalMilliseconds);
                
                Console.WriteLine($"WITH: {timeWithUnchecked.TotalMilliseconds:F1}ms, WITHOUT: {timeWithoutUnchecked.TotalMilliseconds:F1}ms");
            }

            // Calculate statistics
            Console.WriteLine();
            Console.WriteLine("Results Summary");
            Console.WriteLine("===============");
            Console.WriteLine();
            
            // Sequential results
            var seqAvgWith = sequentialResultsWithUnchecked.Average();
            var seqAvgWithout = sequentialResultsWithoutUnchecked.Average();
            var seqStdWith = CalculateStandardDeviation(sequentialResultsWithUnchecked);
            var seqStdWithout = CalculateStandardDeviation(sequentialResultsWithoutUnchecked);
            var seqDifference = seqAvgWithout - seqAvgWith;
            var seqPercentage = (seqDifference / seqAvgWith) * 100;
            
            Console.WriteLine("Sequential Operations:");
            Console.WriteLine($"  WITH unchecked:    {seqAvgWith:F1} ± {seqStdWith:F1} ms");
            Console.WriteLine($"  WITHOUT unchecked: {seqAvgWithout:F1} ± {seqStdWithout:F1} ms");
            Console.WriteLine($"  Difference:        {seqDifference:F1} ms ({seqPercentage:F1}%)");
            
            // Random results  
            var randAvgWith = randomResultsWithUnchecked.Average();
            var randAvgWithout = randomResultsWithoutUnchecked.Average();
            var randStdWith = CalculateStandardDeviation(randomResultsWithUnchecked);
            var randStdWithout = CalculateStandardDeviation(randomResultsWithoutUnchecked);
            var randDifference = randAvgWithout - randAvgWith;
            var randPercentage = (randDifference / randAvgWith) * 100;
            
            Console.WriteLine();
            Console.WriteLine("Random Operations:");
            Console.WriteLine($"  WITH unchecked:    {randAvgWith:F1} ± {randStdWith:F1} ms");
            Console.WriteLine($"  WITHOUT unchecked: {randAvgWithout:F1} ± {randStdWithout:F1} ms");
            Console.WriteLine($"  Difference:        {randDifference:F1} ms ({randPercentage:F1}%)");
            
            Console.WriteLine();
            Console.WriteLine("Analysis:");
            Console.WriteLine("--------");
            
            if (Math.Abs(seqPercentage) < 3 && Math.Abs(randPercentage) < 3)
            {
                Console.WriteLine("Performance difference is minimal (< 3%) for both workloads.");
                Console.WriteLine("The overhead of bounds checking appears negligible in this tree implementation.");
            }
            else if (seqPercentage < 0 && randPercentage < 0)
            {
                Console.WriteLine("WITHOUT unchecked blocks consistently performs better.");
                Console.WriteLine("This suggests JIT optimizations work better with bounds checking enabled.");
            }
            else if (seqPercentage > 0 && randPercentage > 0)
            {
                Console.WriteLine("unchecked blocks consistently provide performance benefit.");
                Console.WriteLine("Removing overflow checks improves performance for this workload.");
            }
            else
            {
                Console.WriteLine("Mixed results - performance impact varies by workload pattern:");
                if (seqPercentage < 0)
                    Console.WriteLine("- Sequential operations perform better with bounds checking");
                else
                    Console.WriteLine("- Sequential operations benefit from unchecked blocks");
                    
                if (randPercentage < 0)
                    Console.WriteLine("- Random operations perform better with bounds checking");
                else
                    Console.WriteLine("- Random operations benefit from unchecked blocks");
            }
            
            Console.WriteLine();
            Console.WriteLine("Recommendation:");
            if (Math.Abs(seqPercentage) < 2 && Math.Abs(randPercentage) < 2)
            {
                Console.WriteLine("Keep current unchecked blocks for code clarity without performance concerns.");
            }
            else if ((seqPercentage < -2 && randPercentage < -2) || (Math.Abs(seqPercentage) > 5 && seqPercentage < 0))
            {
                Console.WriteLine("Consider removing unchecked blocks for performance benefit.");
            }
            else if ((seqPercentage > 2 && randPercentage > 2) || (seqPercentage > 5))
            {
                Console.WriteLine("Keep unchecked blocks for performance benefit.");
            }
            else
            {
                Console.WriteLine("Performance impact is mixed - keep current unchecked blocks for overflow protection.");
            }
        }
        
        static double CalculateStandardDeviation(List<double> values)
        {
            var avg = values.Average();
            var sumSquaredDiffs = values.Sum(value => Math.Pow(value - avg, 2));
            return Math.Sqrt(sumSquaredDiffs / values.Count);
        }
    }
}