using System;
using System.Numerics;
using System.Text;
using Platform.Collections.Methods.Trees;

namespace Platform.Collections.Methods.Tests
{
    public class PartiallyLockFreeSizeBalancedTree<TElement> : PartiallyLockFreeSizeBalancedTreeMethods<TElement> 
        where TElement: IUnsignedNumber<TElement>, IComparisonOperators<TElement, TElement, bool>
    {
        private struct TreeElement
        {
            public TElement Size;
            public TElement Left;
            public TElement Right;
        }
        
        private readonly TreeElement[] _elements;
        private TElement _allocated;

        public TElement Root;

        public TElement Count => GetSizeOrZero(Root);

        public PartiallyLockFreeSizeBalancedTree(int capacity, int balancingThreshold = 10) 
            : base(balancingThreshold)
        {
            _elements = new TreeElement[capacity];
            _allocated = TElement.One;
        }

        public TElement Allocate()
        {
            var newNode = _allocated;
            if (IsEmpty(newNode))
            {
                _allocated = _allocated + TElement.One;
                return newNode;
            }
            else
            {
                throw new InvalidOperationException("Allocated tree element is not empty.");
            }
        }

        public void Free(TElement node)
        {
            while ((_allocated != TElement.One) && IsEmpty(node))
            {
                var lastNode = _allocated - TElement.One;
                if ((lastNode == node))
                {
                    _allocated = lastNode;
                    node = node - TElement.One;
                }
                else
                {
                    break;
                }
            }
        }

        protected override ref TElement GetLeftReference(TElement node) => ref _elements[int.CreateTruncating(node)].Left;
        protected override ref TElement GetRightReference(TElement node) => ref _elements[int.CreateTruncating(node)].Right;
        protected override TElement GetLeft(TElement node) => _elements[int.CreateTruncating(node)].Left;
        protected override TElement GetRight(TElement node) => _elements[int.CreateTruncating(node)].Right;
        protected override TElement GetSize(TElement node) => _elements[int.CreateTruncating(node)].Size;
        protected override void SetLeft(TElement node, TElement left) => _elements[int.CreateTruncating(node)].Left = left;
        protected override void SetRight(TElement node, TElement right) => _elements[int.CreateTruncating(node)].Right = right;
        protected override void SetSize(TElement node, TElement size) => _elements[int.CreateTruncating(node)].Size = size;

        protected override bool FirstIsToTheLeftOfSecond(TElement first, TElement second) => first < second;
        protected override bool FirstIsToTheRightOfSecond(TElement first, TElement second) => first > second;

        protected override void PrintNodeValue(TElement node, StringBuilder sb) => sb.Append($":{node}");

        private bool IsEmpty(TElement node) => GetSize(node) == TElement.Zero;

        /// <summary>
        /// <para>
        /// Performs delayed balancing operation on the tree root.
        /// </para>
        /// <para></para>
        /// </summary>
        protected override void PerformDelayedBalancing()
        {
            // Apply balancing to the root when threshold is reached
            if (Root != TElement.Zero)
            {
                ApplyDelayedBalancing(ref Root);
            }
        }
    }
}