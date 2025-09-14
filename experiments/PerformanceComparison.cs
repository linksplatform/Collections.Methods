using System;
using Platform.Diagnostics;
using Platform.Collections.Methods.Tests;

namespace Platform.Collections.Methods.Performance
{
    class PerformanceComparison
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Performance Comparison: With vs Without unchecked blocks");
            Console.WriteLine("========================================================");
            
            const int iterations = 2000;
            const int warmupIterations = 500;
            
            var avlTreeWithUnchecked = new SizedAndThreadedAVLBalancedTree<uint>(iterations + 1000);
            var avlTreeWithoutUnchecked = new SizedAndThreadedAVLBalancedTreeWithoutUnchecked<uint>(iterations + 1000);
            
            Console.WriteLine($"Testing with {iterations} operations per test.");
            Console.WriteLine($"Warmup with {warmupIterations} operations first.");
            Console.WriteLine();

            // Warm up both implementations
            Console.WriteLine("Warming up WITH unchecked blocks...");
            avlTreeWithUnchecked.TestMultipleCreationsAndDeletions(
                avlTreeWithUnchecked.Allocate, 
                avlTreeWithUnchecked.Free, 
                ref avlTreeWithUnchecked.Root, 
                () => avlTreeWithUnchecked.Count, 
                warmupIterations);

            Console.WriteLine("Warming up WITHOUT unchecked blocks...");
            avlTreeWithoutUnchecked.TestMultipleCreationsAndDeletions(
                avlTreeWithoutUnchecked.Allocate, 
                avlTreeWithoutUnchecked.Free, 
                ref avlTreeWithoutUnchecked.Root, 
                () => avlTreeWithoutUnchecked.Count, 
                warmupIterations);

            // Reset for actual testing
            avlTreeWithUnchecked = new SizedAndThreadedAVLBalancedTree<uint>(iterations + 1000);
            avlTreeWithoutUnchecked = new SizedAndThreadedAVLBalancedTreeWithoutUnchecked<uint>(iterations + 1000);

            Console.WriteLine("Warm up completed. Starting performance tests...");
            Console.WriteLine();

            // Test 1: Sequential insertions and deletions
            Console.WriteLine("Test 1: Sequential operations");
            Console.WriteLine("------------------------------");
            
            var timeWithUnchecked = Platform.Diagnostics.Performance.Measure(() =>
            {
                avlTreeWithUnchecked.TestMultipleCreationsAndDeletions(
                    avlTreeWithUnchecked.Allocate, 
                    avlTreeWithUnchecked.Free, 
                    ref avlTreeWithUnchecked.Root, 
                    () => avlTreeWithUnchecked.Count, 
                    iterations);
            });

            // Reset for second test
            avlTreeWithoutUnchecked = new SizedAndThreadedAVLBalancedTreeWithoutUnchecked<uint>(iterations + 1000);
            
            var timeWithoutUnchecked = Platform.Diagnostics.Performance.Measure(() =>
            {
                avlTreeWithoutUnchecked.TestMultipleCreationsAndDeletions(
                    avlTreeWithoutUnchecked.Allocate, 
                    avlTreeWithoutUnchecked.Free, 
                    ref avlTreeWithoutUnchecked.Root, 
                    () => avlTreeWithoutUnchecked.Count, 
                    iterations);
            });

            Console.WriteLine($"WITH unchecked blocks:    {timeWithUnchecked.TotalMilliseconds:F2} ms");
            Console.WriteLine($"WITHOUT unchecked blocks: {timeWithoutUnchecked.TotalMilliseconds:F2} ms");
            
            var sequentialDifference = timeWithoutUnchecked.TotalMilliseconds - timeWithUnchecked.TotalMilliseconds;
            var sequentialPercentage = (sequentialDifference / timeWithUnchecked.TotalMilliseconds) * 100;
            
            if (sequentialDifference >= 0)
            {
                Console.WriteLine($"Difference: {sequentialDifference:F2} ms ({sequentialPercentage:F1}% slower without unchecked)");
            }
            else
            {
                Console.WriteLine($"Difference: {Math.Abs(sequentialDifference):F2} ms ({Math.Abs(sequentialPercentage):F1}% faster without unchecked)");
            }
            Console.WriteLine();

            // Test 2: Random insertions and deletions
            Console.WriteLine("Test 2: Random operations");
            Console.WriteLine("--------------------------");

            // Reset both trees
            avlTreeWithUnchecked = new SizedAndThreadedAVLBalancedTree<uint>(iterations + 1000);
            avlTreeWithoutUnchecked = new SizedAndThreadedAVLBalancedTreeWithoutUnchecked<uint>(iterations + 1000);

            var randomTimeWithUnchecked = Platform.Diagnostics.Performance.Measure(() =>
            {
                avlTreeWithUnchecked.TestMultipleRandomCreationsAndDeletions(
                    ref avlTreeWithUnchecked.Root, 
                    () => avlTreeWithUnchecked.Count, 
                    iterations);
            });

            // Reset for second random test
            avlTreeWithoutUnchecked = new SizedAndThreadedAVLBalancedTreeWithoutUnchecked<uint>(iterations + 1000);

            var randomTimeWithoutUnchecked = Platform.Diagnostics.Performance.Measure(() =>
            {
                avlTreeWithoutUnchecked.TestMultipleRandomCreationsAndDeletions(
                    ref avlTreeWithoutUnchecked.Root, 
                    () => avlTreeWithoutUnchecked.Count, 
                    iterations);
            });

            Console.WriteLine($"WITH unchecked blocks:    {randomTimeWithUnchecked.TotalMilliseconds:F2} ms");
            Console.WriteLine($"WITHOUT unchecked blocks: {randomTimeWithoutUnchecked.TotalMilliseconds:F2} ms");
            
            var randomDifference = randomTimeWithoutUnchecked.TotalMilliseconds - randomTimeWithUnchecked.TotalMilliseconds;
            var randomPercentage = (randomDifference / randomTimeWithUnchecked.TotalMilliseconds) * 100;
            
            if (randomDifference >= 0)
            {
                Console.WriteLine($"Difference: {randomDifference:F2} ms ({randomPercentage:F1}% slower without unchecked)");
            }
            else
            {
                Console.WriteLine($"Difference: {Math.Abs(randomDifference):F2} ms ({Math.Abs(randomPercentage):F1}% faster without unchecked)");
            }
            Console.WriteLine();
            
            // Summary
            Console.WriteLine("Summary");
            Console.WriteLine("=======");
            Console.WriteLine($"Sequential operations: {Math.Abs(sequentialPercentage):F1}% {(sequentialDifference >= 0 ? "slower without unchecked blocks" : "faster without unchecked blocks")}");
            Console.WriteLine($"Random operations:     {Math.Abs(randomPercentage):F1}% {(randomDifference >= 0 ? "slower without unchecked blocks" : "faster without unchecked blocks")}");
            
            var avgPercentage = (Math.Abs(sequentialPercentage) + Math.Abs(randomPercentage)) / 2;
            var allSlowerWithoutUnchecked = sequentialDifference >= 0 && randomDifference >= 0;
            var allFasterWithoutUnchecked = sequentialDifference < 0 && randomDifference < 0;
            
            if (avgPercentage < 2)
            {
                Console.WriteLine("Performance difference is minimal (< 2%). The overhead check costs appear negligible for this workload.");
            }
            else if (allSlowerWithoutUnchecked)
            {
                Console.WriteLine("unchecked blocks provide consistent performance benefit. Removing overflow checks improves performance.");
            }
            else if (allFasterWithoutUnchecked)
            {
                Console.WriteLine("WITHOUT unchecked blocks performs better. This suggests JIT optimizations are more effective with bounds checking enabled.");
            }
            else
            {
                Console.WriteLine("Mixed results - performance varies by workload pattern.");
            }
        }
    }
}