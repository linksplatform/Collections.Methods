using Xunit;

namespace Platform.Collections.Methods.Tests
{
    public static class TreesTests
    {
        private const int _n = 100;

        [Fact]
        public static void SizeBalancedTreeMultipleAttachAndDetachTest()
        {
            var sizeBalancedTree = new SizeBalancedTree<uint>(10000);
            sizeBalancedTree.TestMultipleCreationsAndDeletions(sizeBalancedTree.Allocate, sizeBalancedTree.Free, ref sizeBalancedTree.Root, () => sizeBalancedTree.Count, _n);
        }

        [Fact]
        public static void SizeBalancedTree2MultipleAttachAndDetachTest()
        {
            var sizeBalancedTree2 = new SizeBalancedTree2<uint>(10000);
            sizeBalancedTree2.TestMultipleCreationsAndDeletions(sizeBalancedTree2.Allocate, sizeBalancedTree2.Free, ref sizeBalancedTree2.Root, () => sizeBalancedTree2.Count, _n);
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
            var sizeBalancedTree = new SizeBalancedTree<uint>(10000);
            sizeBalancedTree.TestMultipleRandomCreationsAndDeletions(ref sizeBalancedTree.Root, () => sizeBalancedTree.Count, _n);
        }

        [Fact]
        public static void SizeBalancedTree2MultipleRandomAttachAndDetachTest()
        {
            var sizeBalancedTree2 = new SizeBalancedTree2<uint>(10000);
            sizeBalancedTree2.TestMultipleRandomCreationsAndDeletions(ref sizeBalancedTree2.Root, () => sizeBalancedTree2.Count, _n);
        }

        [Fact]
        public static void SizedAndThreadedAVLBalancedTreeMultipleRandomAttachAndDetachTest()
        {
            var avlTree = new SizedAndThreadedAVLBalancedTree<uint>(10000);
            avlTree.TestMultipleRandomCreationsAndDeletions(ref avlTree.Root, () => avlTree.Count, _n);
        }
    }
}
