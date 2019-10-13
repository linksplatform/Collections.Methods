using Platform.Diagnostics;

namespace Platform.Collections.Methods.Tests.Console
{
    class Program
    {
        static void Main(string[] args)
        {
            const int _n = 2000;
            var sizeBalancedTree = new SizeBalancedTree<uint>(10000);
            var sizeBalancedTree2 = new SizeBalancedTree2<uint>(10000);
            var avlTree = new SizedAndThreadedAVLBalancedTree<uint>(10000);

            var ts1 = Performance.Measure(() =>
            {
                sizeBalancedTree.TestMultipleCreationsAndDeletions(sizeBalancedTree.Allocate, sizeBalancedTree.Free, ref sizeBalancedTree.Root, () => sizeBalancedTree.Count, _n);
            });
            System.Console.WriteLine($"{(int)ts1.TotalMilliseconds}ms (in order) for SizeBalancedTreeMethods");

            var ts2 = Performance.Measure(() =>
            {
                sizeBalancedTree2.TestMultipleCreationsAndDeletions(sizeBalancedTree2.Allocate, sizeBalancedTree2.Free, ref sizeBalancedTree2.Root, () => sizeBalancedTree2.Count, _n);
            });
            System.Console.WriteLine($"{(int)ts2.TotalMilliseconds}ms (in order) for SizeBalancedTreeMethods2");

            var ts3 = Performance.Measure(() =>
            {
                avlTree.TestMultipleCreationsAndDeletions(avlTree.Allocate, avlTree.Free, ref avlTree.Root, () => avlTree.Count, _n);
            });
            System.Console.WriteLine($"{(int)ts3.TotalMilliseconds}ms (in order) for SizedAndThreadedAVLBalancedTreeMethods");

            var ts4 = Performance.Measure(() =>
            {
                sizeBalancedTree.TestMultipleRandomCreationsAndDeletions(ref sizeBalancedTree.Root, () => sizeBalancedTree.Count, _n);
            });
            System.Console.WriteLine($"{(int)ts4.TotalMilliseconds}ms (random) for SizeBalancedTreeMethods");

            var ts5 = Performance.Measure(() =>
            {
                sizeBalancedTree2.TestMultipleRandomCreationsAndDeletions(ref sizeBalancedTree2.Root, () => sizeBalancedTree2.Count, _n);
            });
            System.Console.WriteLine($"{(int)ts5.TotalMilliseconds}ms (random) for SizeBalancedTreeMethods2");

            var ts6 = Performance.Measure(() =>
            {
                avlTree.TestMultipleRandomCreationsAndDeletions(ref avlTree.Root, () => avlTree.Count, _n);
            });
            System.Console.WriteLine($"{(int)ts6.TotalMilliseconds}ms (random) for SizedAndThreadedAVLBalancedTreeMethods");
        }
    }
}
