using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;
using Platform.Numbers;
using Platform.Collections.Methods.Trees;
using Platform.Converters;

namespace Platform.Collections.Methods.Tests
{
    public class SizedAndThreadedAVLBalancedTree<TElement> : SizedAndThreadedAVLBalancedTreeMethods<TElement> where TElement: IUnsignedNumber<TElement>, IComparisonOperators<TElement, TElement, bool>
    {
        private struct TreeElement
        {
            public TElement Size;
            public TElement Left;
            public TElement Right;
            public sbyte Balance;
            public bool LeftIsChild;
            public bool RightIsChild;
        }
        private readonly TreeElement[] _elements;
        private TElement _allocated;

        public TElement Root;

        public TElement Count => GetSizeOrZero(Root);

        public SizedAndThreadedAVLBalancedTree(int capacity) => (_elements, _allocated) = (new TreeElement[capacity], TElement.One);

        public TElement Allocate()
        {
            var newNode = _allocated;
            if (IsEmpty(newNode))
            {
                _allocated = _allocated+TElement.One;
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
                var lastNode = _allocated-TElement.One;
                if ((lastNode == node))
                {
                    _allocated = lastNode;
                    node = node-TElement.One;
                }
                else
                {
                    return;
                }
            }
        }

        public bool IsEmpty(TElement node) => EqualityComparer<TreeElement>.Default.Equals(GetElement(node), default);

        protected override bool FirstIsToTheLeftOfSecond(TElement first, TElement second) => first < second;

        protected override bool FirstIsToTheRightOfSecond(TElement first, TElement second) => first > second;

        protected override sbyte GetBalance(TElement node) => GetElement(node).Balance;

        protected override bool GetLeftIsChild(TElement node) => GetElement(node).LeftIsChild;

        protected override ref TElement GetLeftReference(TElement node) => ref GetElement(node).Left;

        protected override TElement GetLeft(TElement node) => GetElement(node).Left;

        protected override bool GetRightIsChild(TElement node) => GetElement(node).RightIsChild;

        protected override ref TElement GetRightReference(TElement node) => ref GetElement(node).Right;

        protected override TElement GetRight(TElement node) => GetElement(node).Right;

        protected override TElement GetSize(TElement node) => GetElement(node).Size;

        protected override void PrintNodeValue(TElement node, StringBuilder sb) => sb.Append(node);

        protected override void SetBalance(TElement node, sbyte value) => GetElement(node).Balance = value;

        protected override void SetLeft(TElement node, TElement left) => GetElement(node).Left = left;

        protected override void SetLeftIsChild(TElement node, bool value) => GetElement(node).LeftIsChild = value;

        protected override void SetRight(TElement node, TElement right) => GetElement(node).Right = right;

        protected override void SetRightIsChild(TElement node, bool value) => GetElement(node).RightIsChild = value;

        protected override void SetSize(TElement node, TElement size) => GetElement(node).Size = size;
        private ref TreeElement GetElement(TElement node) => ref _elements[UncheckedConverter<TElement, long>.Default.Convert(node)];
    }
}
