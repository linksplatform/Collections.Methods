using System;
using Xunit;
using Platform.Numbers;
using Platform.Collections.Methods.Trees;

namespace Platform.Collections.Methods.Tests
{
    internal static class TestExtensions
    {
        public static void TestMultipleRandomCreationsAndDeletions<TElement>(this SizedBinaryTreeMethodsBase<TElement> tree, Func<TElement> allocate, Action<TElement> free, ref TElement root, Func<TElement> treeCount, int maximumOperationsPerCycle)
        {
            for (var N = 1; N < maximumOperationsPerCycle; N++)
            {
                var currentCount = 0;
                for (var i = 0; i < N; i++)
                {
                    var node = allocate();
                    tree.Attach(ref root, node);
                    currentCount++;
                    Assert.Equal(currentCount, (int)(Integer<TElement>)treeCount());
                }
                for (var i = 1; i <= N; i++)
                {
                    TElement node = (Integer<TElement>)i;
                    if (tree.Contains(node, root))
                    {
                        tree.Detach(ref root, node);
                        free(node);
                        currentCount--;
                        Assert.Equal(currentCount, (int)(Integer<TElement>)treeCount());
                    }
                }
            }
        }
    }
}
