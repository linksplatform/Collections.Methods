using Xunit;

namespace Platform.Collections.Methods.Tests
{
    /// <summary>
    /// <para>
    /// Represents the trees tests.
    /// </para>
    /// <para></para>
    /// </summary>
    public static class TreesTests
    {
        private const int _n = 500;

        /// <summary>
        /// <para>
        /// Tests that recursionless size balanced tree multiple attach and detach test.
        /// </para>
        /// <para></para>
        /// </summary>
        [Fact]
        public static void RecursionlessSizeBalancedTreeMultipleAttachAndDetachTest()
        {
            var recursionlessSizeBalancedTree = new RecursionlessSizeBalancedTree<uint>(10000);
            recursionlessSizeBalancedTree.TestMultipleCreationsAndDeletions(recursionlessSizeBalancedTree.Allocate, recursionlessSizeBalancedTree.Free, ref recursionlessSizeBalancedTree.Root, () => recursionlessSizeBalancedTree.Count, _n);
        }

        /// <summary>
        /// <para>
        /// Tests that size balanced tree multiple attach and detach test.
        /// </para>
        /// <para></para>
        /// </summary>
        [Fact]
        public static void SizeBalancedTreeMultipleAttachAndDetachTest()
        {
            var sizeBalancedTree = new SizeBalancedTree<uint>(10000);
            sizeBalancedTree.TestMultipleCreationsAndDeletions(sizeBalancedTree.Allocate, sizeBalancedTree.Free, ref sizeBalancedTree.Root, () => sizeBalancedTree.Count, _n);
        }

        /// <summary>
        /// <para>
        /// Tests that sized and threaded avl balanced tree multiple attach and detach test.
        /// </para>
        /// <para></para>
        /// </summary>
        [Fact]
        public static void SizedAndThreadedAVLBalancedTreeMultipleAttachAndDetachTest()
        {
            var avlTree = new SizedAndThreadedAVLBalancedTree<uint>(10000);
            avlTree.TestMultipleCreationsAndDeletions(avlTree.Allocate, avlTree.Free, ref avlTree.Root, () => avlTree.Count, _n);
        }

        /// <summary>
        /// <para>
        /// Tests that recursionless size balanced tree multiple random attach and detach test.
        /// </para>
        /// <para></para>
        /// </summary>
        [Fact]
        public static void RecursionlessSizeBalancedTreeMultipleRandomAttachAndDetachTest()
        {
            var recursionlessSizeBalancedTree = new RecursionlessSizeBalancedTree<uint>(10000);
            recursionlessSizeBalancedTree.TestMultipleRandomCreationsAndDeletions(ref recursionlessSizeBalancedTree.Root, () => recursionlessSizeBalancedTree.Count, _n);
        }

        /// <summary>
        /// <para>
        /// Tests that size balanced tree multiple random attach and detach test.
        /// </para>
        /// <para></para>
        /// </summary>
        [Fact]
        public static void SizeBalancedTreeMultipleRandomAttachAndDetachTest()
        {
            var sizeBalancedTree = new SizeBalancedTree<uint>(10000);
            sizeBalancedTree.TestMultipleRandomCreationsAndDeletions(ref sizeBalancedTree.Root, () => sizeBalancedTree.Count, _n);
        }

        /// <summary>
        /// <para>
        /// Tests that sized and threaded avl balanced tree multiple random attach and detach test.
        /// </para>
        /// <para></para>
        /// </summary>
        [Fact]
        public static void SizedAndThreadedAVLBalancedTreeMultipleRandomAttachAndDetachTest()
        {
            var avlTree = new SizedAndThreadedAVLBalancedTree<uint>(10000);
            avlTree.TestMultipleRandomCreationsAndDeletions(ref avlTree.Root, () => avlTree.Count, _n);
        }
    }
}
