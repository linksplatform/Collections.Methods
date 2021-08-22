using System;
using System.Collections.Generic;
using Xunit;
using Platform.Collections.Methods.Trees;
using Platform.Converters;

namespace Platform.Collections.Methods.Tests
{
    /// <summary>
    /// <para>
    /// Represents the test extensions.
    /// </para>
    /// <para></para>
    /// </summary>
    public static class TestExtensions
    {
        /// <summary>
        /// <para>
        /// Tests the multiple creations and deletions using the specified tree.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <typeparam name="TElement">
        /// <para>The element.</para>
        /// <para></para>
        /// </typeparam>
        /// <param name="tree">
        /// <para>The tree.</para>
        /// <para></para>
        /// </param>
        /// <param name="allocate">
        /// <para>The allocate.</para>
        /// <para></para>
        /// </param>
        /// <param name="free">
        /// <para>The free.</para>
        /// <para></para>
        /// </param>
        /// <param name="root">
        /// <para>The root.</para>
        /// <para></para>
        /// </param>
        /// <param name="treeCount">
        /// <para>The tree count.</para>
        /// <para></para>
        /// </param>
        /// <param name="maximumOperationsPerCycle">
        /// <para>The maximum operations per cycle.</para>
        /// <para></para>
        /// </param>
        public static void TestMultipleCreationsAndDeletions<TElement>(this SizedBinaryTreeMethodsBase<TElement> tree, Func<TElement> allocate, Action<TElement> free, ref TElement root, Func<TElement> treeCount, int maximumOperationsPerCycle)
        {
            for (var N = 1; N < maximumOperationsPerCycle; N++)
            {
                var currentCount = 0;
                for (var i = 0; i < N; i++)
                {
                    var node = allocate();
                    tree.Attach(ref root, node);
                    currentCount++;
                    Assert.Equal(currentCount, (int)UncheckedConverter<TElement, int>.Default.Convert(treeCount()));
                }
                for (var i = 1; i <= N; i++)
                {
                    TElement node = UncheckedConverter<int, TElement>.Default.Convert(i);
                    if (tree.Contains(node, root))
                    {
                        tree.Detach(ref root, node);
                        free(node);
                        currentCount--;
                        Assert.Equal(currentCount, (int)UncheckedConverter<TElement, int>.Default.Convert(treeCount()));
                    }
                }
            }
        }

        /// <summary>
        /// <para>
        /// Tests the multiple random creations and deletions using the specified tree.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <typeparam name="TElement">
        /// <para>The element.</para>
        /// <para></para>
        /// </typeparam>
        /// <param name="tree">
        /// <para>The tree.</para>
        /// <para></para>
        /// </param>
        /// <param name="root">
        /// <para>The root.</para>
        /// <para></para>
        /// </param>
        /// <param name="treeCount">
        /// <para>The tree count.</para>
        /// <para></para>
        /// </param>
        /// <param name="maximumOperationsPerCycle">
        /// <para>The maximum operations per cycle.</para>
        /// <para></para>
        /// </param>
        public static void TestMultipleRandomCreationsAndDeletions<TElement>(this SizedBinaryTreeMethodsBase<TElement> tree, ref TElement root, Func<TElement> treeCount, int maximumOperationsPerCycle)
        {
            var random = new System.Random(0);
            var added = new HashSet<TElement>();
            var currentCount = 0;
            for (var N = 1; N < maximumOperationsPerCycle; N++)
            {
                for (var i = 0; i < N; i++)
                {
                    var node = UncheckedConverter<int, TElement>.Default.Convert(random.Next(1, N));
                    if (added.Add(node))
                    {
                        tree.Attach(ref root, node);
                        currentCount++;
                        Assert.Equal(currentCount, (int)UncheckedConverter<TElement, int>.Default.Convert(treeCount()));
                    }
                }
                for (var i = 1; i <= N; i++)
                {
                    TElement node = UncheckedConverter<int, TElement>.Default.Convert(random.Next(1, N));
                    if (tree.Contains(node, root))
                    {
                        tree.Detach(ref root, node);
                        currentCount--;
                        Assert.Equal(currentCount, (int)UncheckedConverter<TElement, int>.Default.Convert(treeCount()));
                        added.Remove(node);
                    }
                }
            }
        }
    }
}
