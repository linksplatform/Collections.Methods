using Xunit;

namespace Platform.Collections.Methods.Tests
{
    public static class TreesTests
    {
        [Fact]
        public static void MultipleAttachAndDetachTest()
        {
            var sizeBalancedTree = new SizeBalancedTree<uint>(10000);
            sizeBalancedTree.TestMultipleRandomCreationsAndDeletions(sizeBalancedTree.Allocate, sizeBalancedTree.Free, ref sizeBalancedTree.Root, () => sizeBalancedTree.Count, 100);

            var sizeBalancedTree2 = new SizeBalancedTree2<uint>(10000);
            sizeBalancedTree2.TestMultipleRandomCreationsAndDeletions(sizeBalancedTree2.Allocate, sizeBalancedTree2.Free, ref sizeBalancedTree2.Root, () => sizeBalancedTree2.Count, 100);

            var avlTree = new SizedAndThreadedAVLBalancedTree<uint>(10000);
            avlTree.TestMultipleRandomCreationsAndDeletions(avlTree.Allocate, avlTree.Free, ref avlTree.Root, () => avlTree.Count, 100);
        }
    }
}
