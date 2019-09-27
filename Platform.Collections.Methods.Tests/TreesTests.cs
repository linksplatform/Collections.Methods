using Xunit;

namespace Platform.Collections.Methods.Tests
{
    public static class TreesTests
    {
        [Fact]
        public static void MultipleAttachAndDetachTest()
        {
            var sizeBalancedTree = new SizeBalancedTree<uint>(10000);
            sizeBalancedTree.TestMultipleCreationsAndDeletions(sizeBalancedTree.Allocate, sizeBalancedTree.Free, ref sizeBalancedTree.Root, () => sizeBalancedTree.Count, 100);

            var sizeBalancedTree2 = new SizeBalancedTree2<uint>(10000);
            sizeBalancedTree2.TestMultipleCreationsAndDeletions(sizeBalancedTree2.Allocate, sizeBalancedTree2.Free, ref sizeBalancedTree2.Root, () => sizeBalancedTree2.Count, 100);

            var avlTree = new SizedAndThreadedAVLBalancedTree<uint>(10000);
            avlTree.TestMultipleCreationsAndDeletions(avlTree.Allocate, avlTree.Free, ref avlTree.Root, () => avlTree.Count, 100);
        }

        [Fact]
        public static void MultipleRandomAttachAndDetachTest()
        {
            var sizeBalancedTree = new SizeBalancedTree<uint>(10000);
            sizeBalancedTree.TestMultipleRandomCreationsAndDeletions(ref sizeBalancedTree.Root, () => sizeBalancedTree.Count, 100);

            var sizeBalancedTree2 = new SizeBalancedTree2<uint>(10000);
            sizeBalancedTree2.TestMultipleRandomCreationsAndDeletions(ref sizeBalancedTree2.Root, () => sizeBalancedTree2.Count, 100);

            var avlTree = new SizedAndThreadedAVLBalancedTree<uint>(10000);
            avlTree.TestMultipleRandomCreationsAndDeletions(ref avlTree.Root, () => avlTree.Count, 100);
        }
    }
}
