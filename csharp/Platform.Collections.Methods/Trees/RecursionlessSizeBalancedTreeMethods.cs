using System.Numerics;

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member

namespace Platform.Collections.Methods.Trees
{
    /// <summary>
    /// <para>
    /// Represents the recursionless size balanced tree methods.
    /// </para>
    /// <para></para>
    /// </summary>
    /// <seealso cref="SizedBinaryTreeMethodsBase{TElement}"/>
    public abstract class RecursionlessSizeBalancedTreeMethods<TElement> : SizedBinaryTreeMethodsBase<TElement> where TElement: IUnsignedNumber<TElement>, IComparisonOperators<TElement, TElement, bool>
    {
        /// <summary>
        /// <para>
        /// Attaches the core using the specified root.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="root">
        /// <para>The root.</para>
        /// <para></para>
        /// </param>
        /// <param name="node">
        /// <para>The node.</para>
        /// <para></para>
        /// </param>
        protected override void AttachCore(ref TElement root, TElement node)
        {
            while (true)
            {
                ref var left = ref GetLeftReference(root);
                var leftSize = GetSizeOrZero(left);
                ref var right = ref GetRightReference(root);
                var rightSize = GetSizeOrZero(right);
                if (node < root) // node.Key less than root.Key
                {
                    if (left == TElement.Zero)
                    {
                        IncrementSize(root);
                        SetSize(node, TElement.One);
                        left = node;
                        return;
                    }
                    if (node < left) // node.Key less than left.Key
                    {
                        if ((leftSize + TElement.One) > rightSize)
                        {
                            RightRotate(ref root);
                        }
                        else
                        {
                            IncrementSize(root);
                            root = ref left;
                        }
                    }
                    else  // node.Key greater than left.Key
                    {
                        var leftRightSize = GetSizeOrZero(GetRight(left));
                        if ((leftRightSize + TElement.One) > rightSize)
                        {
                            if (leftRightSize == TElement.Zero && rightSize == TElement.Zero)
                            {
                                SetLeft(node, left);
                                SetRight(node, root);
                                SetSize(node, (leftSize+ TElement.CreateTruncating(2))); // TElement.CreateTruncating(2) (2) - node the size of root and a node itself
                                SetLeft(root, TElement.Zero);
                                SetSize(root, TElement.One);
                                root = node;
                                return;
                            }
                            LeftRotate(ref left);
                            RightRotate(ref root);
                        }
                        else
                        {
                            IncrementSize(root);
                            root = ref left;
                        }
                    }
                }
                else // node.Key greater than root.Key
                {
                    if (right == TElement.Zero)
                    {
                        IncrementSize(root);
                        SetSize(node, TElement.One);
                        right = node;
                        return;
                    }
                    if (node > right) // node.Key greater than right.Key
                    {
                        if ((rightSize + TElement.One) > leftSize)
                        {
                            LeftRotate(ref root);
                        }
                        else
                        {
                            IncrementSize(root);
                            root = ref right;
                        }
                    }
                    else // node.Key less than right.Key
                    {
                        var rightLeftSize = GetSizeOrZero(GetLeft(right));
                        if ((rightLeftSize + TElement.One) > leftSize)
                        {
                            if (rightLeftSize == TElement.Zero && leftSize == TElement.Zero)
                            {
                                SetLeft(node, root);
                                SetRight(node, right);
                                SetSize(node, (rightSize + TElement.CreateTruncating(2))); // TElement.CreateTruncating(2) (2) - node the size of root and a node itself
                                SetRight(root, TElement.Zero);
                                SetSize(root, TElement.One);
                                root = node;
                                return;
                            }
                            RightRotate(ref right);
                            LeftRotate(ref root);
                        }
                        else
                        {
                            IncrementSize(root);
                            root = ref right;
                        }
                    }
                }
            }
        }

        /// <summary>
        /// <para>
        /// Detaches the core using the specified root.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="root">
        /// <para>The root.</para>
        /// <para></para>
        /// </param>
        /// <param name="node">
        /// <para>The node.</para>
        /// <para></para>
        /// </param>
        protected override void DetachCore(ref TElement root, TElement node)
        {
            while (true)
            {
                ref var left = ref GetLeftReference(root);
                var leftSize = GetSizeOrZero(left);
                ref var right = ref GetRightReference(root);
                var rightSize = GetSizeOrZero(right);
                if (node < root) // node.Key less than root.Key
                {
                    var decrementedLeftSize = leftSize - TElement.One;
                    if ((GetSizeOrZero(GetRightOrDefault(right))) > decrementedLeftSize)
                    {
                        LeftRotate(ref root);
                    }
                    else if ((GetSizeOrZero(GetLeftOrDefault(right))) > decrementedLeftSize)
                    {
                        RightRotate(ref right);
                        LeftRotate(ref root);
                    }
                    else
                    {
                        DecrementSize(root);
                        root = ref left;
                    }
                }
                else if (node > root) // node.Key greater than root.Key
                {
                    var decrementedRightSize = rightSize - TElement.One;
                    if ((GetSizeOrZero(GetLeftOrDefault(left))) > decrementedRightSize)
                    {
                        RightRotate(ref root);
                    }
                    else if ((GetSizeOrZero(GetRightOrDefault(left))) > decrementedRightSize)
                    {
                        LeftRotate(ref left);
                        RightRotate(ref root);
                    }
                    else
                    {
                        DecrementSize(root);
                        root = ref right;
                    }
                }
                else // key equals to root.Key
                {
                    if (leftSize > TElement.Zero && rightSize > TElement.Zero)
                    {
                        TElement replacement;
                        if ((leftSize) > rightSize)
                        {
                            replacement = GetRightest(left);
                            DetachCore(ref left, replacement);
                        }
                        else
                        {
                            replacement = GetLeftest(right);
                            DetachCore(ref right, replacement);
                        }
                        SetLeft(replacement, left);
                        SetRight(replacement, right);
                        SetSize(replacement, (leftSize + rightSize));
                        root = replacement;
                    }
                    else if (leftSize > TElement.Zero)
                    {
                        root = left;
                    }
                    else if (rightSize > TElement.Zero)
                    {
                        root = right;
                    }
                    else
                    {
                        root = TElement.Zero;
                    }
                    ClearNode(node);
                    return;
                }
            }
        }
    }
}
