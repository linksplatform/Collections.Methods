using System.Threading.Tasks;
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
        public static void PartiallyLockFreeSizeBalancedTreeMultipleAttachAndDetachTest()
        {
            var lockFreeTree = new PartiallyLockFreeSizeBalancedTree<uint>(10000, balancingThreshold: 5);
            lockFreeTree.TestMultipleCreationsAndDeletions(lockFreeTree.Allocate, lockFreeTree.Free, ref lockFreeTree.Root, () => lockFreeTree.Count, _n);
        }

        [Fact]
        public static void PartiallyLockFreeSizeBalancedTreeMultipleRandomAttachAndDetachTest()
        {
            var lockFreeTree = new PartiallyLockFreeSizeBalancedTree<uint>(10000, balancingThreshold: 10);
            lockFreeTree.TestMultipleRandomCreationsAndDeletions(ref lockFreeTree.Root, () => lockFreeTree.Count, _n);
        }

        [Fact]
        public static void PartiallyLockFreeSizeBalancedTreeDelayedBalancingTest()
        {
            var lockFreeTree = new PartiallyLockFreeSizeBalancedTree<uint>(1000, balancingThreshold: 5);
            
            // Insert elements without triggering balancing
            for (uint i = 1; i <= 4; i++)
            {
                var node = lockFreeTree.Allocate();
                lockFreeTree.Attach(ref lockFreeTree.Root, node);
            }
            
            // At this point, balancing should not have been triggered
            Assert.Equal(4, lockFreeTree.LockFreeAttachCount);
            Assert.Equal(5, lockFreeTree.BalancingThreshold);
            
            // Insert one more element to trigger balancing
            var triggerNode = lockFreeTree.Allocate();
            lockFreeTree.Attach(ref lockFreeTree.Root, triggerNode);
            
            // After triggering, the counter should reset
            Assert.Equal(0, lockFreeTree.LockFreeAttachCount);
            Assert.Equal(5u, lockFreeTree.Count);
        }

        [Fact]
        public static void PartiallyLockFreeSizeBalancedTreeForceBalancingTest()
        {
            var lockFreeTree = new PartiallyLockFreeSizeBalancedTree<uint>(1000, balancingThreshold: 10);
            
            // Insert a few elements
            for (uint i = 1; i <= 3; i++)
            {
                var node = lockFreeTree.Allocate();
                lockFreeTree.Attach(ref lockFreeTree.Root, node);
            }
            
            Assert.Equal(3, lockFreeTree.LockFreeAttachCount);
            
            // Force balancing manually
            lockFreeTree.ForceBalancing();
            
            // Counter should be reset after force balancing
            Assert.Equal(0, lockFreeTree.LockFreeAttachCount);
            Assert.Equal(3u, lockFreeTree.Count);
        }

        [Fact]
        public static async Task PartiallyLockFreeSizeBalancedTreeConcurrentInsertionTest()
        {
            var lockFreeTree = new PartiallyLockFreeSizeBalancedTree<uint>(10000, balancingThreshold: 50);
            const int numberOfTasks = 10;
            const int insertsPerTask = 20;
            
            // Create tasks that will insert elements concurrently
            var tasks = new Task[numberOfTasks];
            for (int i = 0; i < numberOfTasks; i++)
            {
                int taskId = i;
                tasks[i] = Task.Run(() =>
                {
                    for (int j = 0; j < insertsPerTask; j++)
                    {
                        var node = lockFreeTree.Allocate();
                        lockFreeTree.Attach(ref lockFreeTree.Root, node);
                    }
                });
            }
            
            // Wait for all tasks to complete
            await Task.WhenAll(tasks);
            
            // Verify final count
            Assert.Equal((uint)(numberOfTasks * insertsPerTask), lockFreeTree.Count);
            
            // Clean up resources
            lockFreeTree.Dispose();
        }
    }
}
