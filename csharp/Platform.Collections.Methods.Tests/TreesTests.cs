using Xunit;

namespace Platform.Collections.Methods.Tests
{
    public static class TreesTests
    {
        private const int _n = 500;

        [Fact]
        public static void RecursionlessSizeBalancedTreeMultipleAttachAndDetachTest()
        {
            var recursionlessSizeBalancedTree = new RecursionlessSizeBalancedTree<uint>(10000);
            recursionlessSizeBalancedTree.TestMultipleCreationsAndDeletions(recursionlessSizeBalancedTree.Allocate, recursionlessSizeBalancedTree.Free, ref recursionlessSizeBalancedTree.Root, () => recursionlessSizeBalancedTree.Count, _n);
        }

        [Fact]
        public static void SizeBalancedTreeMultipleAttachAndDetachTest()
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
        public static void RecursionlessSizeBalancedTreeMultipleRandomAttachAndDetachTest()
        {
            var recursionlessSizeBalancedTree = new RecursionlessSizeBalancedTree<uint>(10000);
            recursionlessSizeBalancedTree.TestMultipleRandomCreationsAndDeletions(ref recursionlessSizeBalancedTree.Root, () => recursionlessSizeBalancedTree.Count, _n);
        }

        [Fact]
        public static void SizeBalancedTreeMultipleRandomAttachAndDetachTest()
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

        [Fact]
        public static void SizedAndThreadedAVLBalancedTreeValidationTest()
        {
            var avlTree = new SizedAndThreadedAVLBalancedTree<uint>(100);
            
            // Test basic attach operations with validation
            for (uint i = 1; i <= 10; i++)
            {
                var node = avlTree.Allocate();
                avlTree.Attach(ref avlTree.Root, node);
                
                // Validate tree structure after each insertion
                // The validation will run automatically due to ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            }
            
            // Test detach operations with validation  
            for (uint i = 1; i <= 5; i++)
            {
                avlTree.Detach(ref avlTree.Root, i);
                
                // Validation runs automatically after detach
            }
            
            // Test remaining elements
            for (uint i = 6; i <= 10; i++)
            {
                avlTree.Detach(ref avlTree.Root, i);
            }
            
            // Tree should be empty
            Assert.Equal(0U, avlTree.Count);
        }
    }
}
