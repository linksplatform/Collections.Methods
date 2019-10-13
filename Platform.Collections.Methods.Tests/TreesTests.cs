using Xunit;

namespace Platform.Collections.Methods.Tests
{
    public static class TreesTests
    {
        private const int _n = 500;

        [Fact]
        public static void SizeBalancedTreeMultipleAttachAndDetachTest()
        {
            var recursionlessSizeBalancedTree = new RecursionlessSizeBalancedTree<uint>(10000);
            recursionlessSizeBalancedTree.TestMultipleCreationsAndDeletions(recursionlessSizeBalancedTree.Allocate, recursionlessSizeBalancedTree.Free, ref recursionlessSizeBalancedTree.Root, () => recursionlessSizeBalancedTree.Count, _n);
        }

        [Fact]
        public static void SizeBalancedTree2MultipleAttachAndDetachTest()
        {
            var sizeBalancedTree = new SizeBalancedTree<uint>(10000);
            sizeBalancedTree.TestMultipleCreationsAndDeletions(sizeBalancedTree.Allocate, sizeBalancedTree.Free, ref sizeBalancedTree.Root, () => sizeBalancedTree.Count, _n);
        }

        [Fact]
        public static void SizedAndThreadedAVLBalancedTreeMultipleAttachAndDetachTest()
        {
            var avlTree = new SizedAndThreadedAVLBalancedTree<uint>(10000);
            avlTree.TestMultipleCreationsAndDeletions(avlTree.Allocate, avlTree.Free, ref avlTree.Root, () => avlTree.Count, _n);
        }

        [Fact]
        public static void SizeBalancedTreeMultipleRandomAttachAndDetachTest()
        {
            var recursionlessSizeBalancedTree = new RecursionlessSizeBalancedTree<uint>(10000);
            recursionlessSizeBalancedTree.TestMultipleRandomCreationsAndDeletions(ref recursionlessSizeBalancedTree.Root, () => recursionlessSizeBalancedTree.Count, _n);
        }

        [Fact]
        public static void SizeBalancedTree2MultipleRandomAttachAndDetachTest()
        {
            var sizeBalancedTree = new SizeBalancedTree<uint>(10000);
            sizeBalancedTree.TestMultipleRandomCreationsAndDeletions(ref sizeBalancedTree.Root, () => sizeBalancedTree.Count, _n);
        }

        [Fact]
        public static void SizedAndThreadedAVLBalancedTreeMultipleRandomAttachAndDetachTest()
        {
            var avlTree = new SizedAndThreadedAVLBalancedTree<uint>(10000);
            avlTree.TestMultipleRandomCreationsAndDeletions(ref avlTree.Root, () => avlTree.Count, _n);
        }
    }
}
