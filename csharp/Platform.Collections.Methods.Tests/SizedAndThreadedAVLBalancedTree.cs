using System;
using System.Collections.Generic;
using System.Text;
using Platform.Numbers;
using Platform.Collections.Methods.Trees;
using Platform.Converters;

namespace Platform.Collections.Methods.Tests
{
    /// <summary>
    /// <para>
    /// Represents the sized and threaded avl balanced tree.
    /// </para>
    /// <para></para>
    /// </summary>
    /// <seealso cref="SizedAndThreadedAVLBalancedTreeMethods{TElement}"/>
    public class SizedAndThreadedAVLBalancedTree<TElement> : SizedAndThreadedAVLBalancedTreeMethods<TElement>
    {
        /// <summary>
        /// <para>
        /// The tree element.
        /// </para>
        /// <para></para>
        /// </summary>
        private struct TreeElement
        {
            /// <summary>
            /// <para>
            /// The size.
            /// </para>
            /// <para></para>
            /// </summary>
            public TElement Size;
            /// <summary>
            /// <para>
            /// The left.
            /// </para>
            /// <para></para>
            /// </summary>
            public TElement Left;
            /// <summary>
            /// <para>
            /// The right.
            /// </para>
            /// <para></para>
            /// </summary>
            public TElement Right;
            /// <summary>
            /// <para>
            /// The balance.
            /// </para>
            /// <para></para>
            /// </summary>
            public sbyte Balance;
            /// <summary>
            /// <para>
            /// The left is child.
            /// </para>
            /// <para></para>
            /// </summary>
            public bool LeftIsChild;
            /// <summary>
            /// <para>
            /// The right is child.
            /// </para>
            /// <para></para>
            /// </summary>
            public bool RightIsChild;
        }

        /// <summary>
        /// <para>
        /// The elements.
        /// </para>
        /// <para></para>
        /// </summary>
        private readonly TreeElement[] _elements;
        /// <summary>
        /// <para>
        /// The allocated.
        /// </para>
        /// <para></para>
        /// </summary>
        private TElement _allocated;

        /// <summary>
        /// <para>
        /// The root.
        /// </para>
        /// <para></para>
        /// </summary>
        public TElement Root;

        /// <summary>
        /// <para>
        /// Gets the count value.
        /// </para>
        /// <para></para>
        /// </summary>
        public TElement Count => GetSizeOrZero(Root);

        /// <summary>
        /// <para>
        /// Initializes a new <see cref="SizedAndThreadedAVLBalancedTree"/> instance.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="capacity">
        /// <para>A capacity.</para>
        /// <para></para>
        /// </param>
        public SizedAndThreadedAVLBalancedTree(int capacity) => (_elements, _allocated) = (new TreeElement[capacity], One);

        /// <summary>
        /// <para>
        /// Allocates this instance.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <exception cref="InvalidOperationException">
        /// <para>Allocated tree element is not empty.</para>
        /// <para></para>
        /// </exception>
        /// <returns>
        /// <para>The element</para>
        /// <para></para>
        /// </returns>
        public TElement Allocate()
        {
            var newNode = _allocated;
            if (IsEmpty(newNode))
            {
                _allocated = Arithmetic.Increment(_allocated);
                return newNode;
            }
            else
            {
                throw new InvalidOperationException("Allocated tree element is not empty.");
            }
        }

        /// <summary>
        /// <para>
        /// Frees the node.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="node">
        /// <para>The node.</para>
        /// <para></para>
        /// </param>
        public void Free(TElement node)
        {
            while (!EqualityComparer.Equals(_allocated, One) && IsEmpty(node))
            {
                var lastNode = Arithmetic.Decrement(_allocated);
                if (EqualityComparer.Equals(lastNode, node))
                {
                    _allocated = lastNode;
                    node = Arithmetic.Decrement(node);
                }
                else
                {
                    return;
                }
            }
        }

        /// <summary>
        /// <para>
        /// Determines whether this instance is empty.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="node">
        /// <para>The node.</para>
        /// <para></para>
        /// </param>
        /// <returns>
        /// <para>The bool</para>
        /// <para></para>
        /// </returns>
        public bool IsEmpty(TElement node) => EqualityComparer<TreeElement>.Default.Equals(GetElement(node), default);

        /// <summary>
        /// <para>
        /// Determines whether this instance first is to the left of second.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="first">
        /// <para>The first.</para>
        /// <para></para>
        /// </param>
        /// <param name="second">
        /// <para>The second.</para>
        /// <para></para>
        /// </param>
        /// <returns>
        /// <para>The bool</para>
        /// <para></para>
        /// </returns>
        protected override bool FirstIsToTheLeftOfSecond(TElement first, TElement second) => Comparer.Compare(first, second) < 0;

        /// <summary>
        /// <para>
        /// Determines whether this instance first is to the right of second.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="first">
        /// <para>The first.</para>
        /// <para></para>
        /// </param>
        /// <param name="second">
        /// <para>The second.</para>
        /// <para></para>
        /// </param>
        /// <returns>
        /// <para>The bool</para>
        /// <para></para>
        /// </returns>
        protected override bool FirstIsToTheRightOfSecond(TElement first, TElement second) => Comparer.Compare(first, second) > 0;

        /// <summary>
        /// <para>
        /// Gets the balance using the specified node.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="node">
        /// <para>The node.</para>
        /// <para></para>
        /// </param>
        /// <returns>
        /// <para>The sbyte</para>
        /// <para></para>
        /// </returns>
        protected override sbyte GetBalance(TElement node) => GetElement(node).Balance;

        /// <summary>
        /// <para>
        /// Determines whether this instance get left is child.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="node">
        /// <para>The node.</para>
        /// <para></para>
        /// </param>
        /// <returns>
        /// <para>The bool</para>
        /// <para></para>
        /// </returns>
        protected override bool GetLeftIsChild(TElement node) => GetElement(node).LeftIsChild;

        /// <summary>
        /// <para>
        /// Gets the left reference using the specified node.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="node">
        /// <para>The node.</para>
        /// <para></para>
        /// </param>
        /// <returns>
        /// <para>The ref element</para>
        /// <para></para>
        /// </returns>
        protected override ref TElement GetLeftReference(TElement node) => ref GetElement(node).Left;

        /// <summary>
        /// <para>
        /// Gets the left using the specified node.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="node">
        /// <para>The node.</para>
        /// <para></para>
        /// </param>
        /// <returns>
        /// <para>The element</para>
        /// <para></para>
        /// </returns>
        protected override TElement GetLeft(TElement node) => GetElement(node).Left;

        /// <summary>
        /// <para>
        /// Determines whether this instance get right is child.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="node">
        /// <para>The node.</para>
        /// <para></para>
        /// </param>
        /// <returns>
        /// <para>The bool</para>
        /// <para></para>
        /// </returns>
        protected override bool GetRightIsChild(TElement node) => GetElement(node).RightIsChild;

        /// <summary>
        /// <para>
        /// Gets the right reference using the specified node.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="node">
        /// <para>The node.</para>
        /// <para></para>
        /// </param>
        /// <returns>
        /// <para>The ref element</para>
        /// <para></para>
        /// </returns>
        protected override ref TElement GetRightReference(TElement node) => ref GetElement(node).Right;

        /// <summary>
        /// <para>
        /// Gets the right using the specified node.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="node">
        /// <para>The node.</para>
        /// <para></para>
        /// </param>
        /// <returns>
        /// <para>The element</para>
        /// <para></para>
        /// </returns>
        protected override TElement GetRight(TElement node) => GetElement(node).Right;

        /// <summary>
        /// <para>
        /// Gets the size using the specified node.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="node">
        /// <para>The node.</para>
        /// <para></para>
        /// </param>
        /// <returns>
        /// <para>The element</para>
        /// <para></para>
        /// </returns>
        protected override TElement GetSize(TElement node) => GetElement(node).Size;

        /// <summary>
        /// <para>
        /// Prints the node value using the specified node.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="node">
        /// <para>The node.</para>
        /// <para></para>
        /// </param>
        /// <param name="sb">
        /// <para>The sb.</para>
        /// <para></para>
        /// </param>
        protected override void PrintNodeValue(TElement node, StringBuilder sb) => sb.Append(node);

        /// <summary>
        /// <para>
        /// Sets the balance using the specified node.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="node">
        /// <para>The node.</para>
        /// <para></para>
        /// </param>
        /// <param name="value">
        /// <para>The value.</para>
        /// <para></para>
        /// </param>
        protected override void SetBalance(TElement node, sbyte value) => GetElement(node).Balance = value;

        /// <summary>
        /// <para>
        /// Sets the left using the specified node.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="node">
        /// <para>The node.</para>
        /// <para></para>
        /// </param>
        /// <param name="left">
        /// <para>The left.</para>
        /// <para></para>
        /// </param>
        protected override void SetLeft(TElement node, TElement left) => GetElement(node).Left = left;

        /// <summary>
        /// <para>
        /// Sets the left is child using the specified node.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="node">
        /// <para>The node.</para>
        /// <para></para>
        /// </param>
        /// <param name="value">
        /// <para>The value.</para>
        /// <para></para>
        /// </param>
        protected override void SetLeftIsChild(TElement node, bool value) => GetElement(node).LeftIsChild = value;

        /// <summary>
        /// <para>
        /// Sets the right using the specified node.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="node">
        /// <para>The node.</para>
        /// <para></para>
        /// </param>
        /// <param name="right">
        /// <para>The right.</para>
        /// <para></para>
        /// </param>
        protected override void SetRight(TElement node, TElement right) => GetElement(node).Right = right;

        /// <summary>
        /// <para>
        /// Sets the right is child using the specified node.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="node">
        /// <para>The node.</para>
        /// <para></para>
        /// </param>
        /// <param name="value">
        /// <para>The value.</para>
        /// <para></para>
        /// </param>
        protected override void SetRightIsChild(TElement node, bool value) => GetElement(node).RightIsChild = value;

        /// <summary>
        /// <para>
        /// Sets the size using the specified node.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="node">
        /// <para>The node.</para>
        /// <para></para>
        /// </param>
        /// <param name="size">
        /// <para>The size.</para>
        /// <para></para>
        /// </param>
        protected override void SetSize(TElement node, TElement size) => GetElement(node).Size = size;

        /// <summary>
        /// <para>
        /// Gets the element using the specified node.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="node">
        /// <para>The node.</para>
        /// <para></para>
        /// </param>
        /// <returns>
        /// <para>The ref tree element</para>
        /// <para></para>
        /// </returns>
        private ref TreeElement GetElement(TElement node) => ref _elements[UncheckedConverter<TElement, long>.Default.Convert(node)];
    }
}
