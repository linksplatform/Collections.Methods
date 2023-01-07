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
                if (FirstIsToTheLeftOfSecond(node, root)) // node.Key less than root.Key
                {
                    if (EqualToZero(left))
                    {
                        IncrementSize(root);
                        SetSize(node, One);
                        left = node;
                        return;
                    }
                    if (FirstIsToTheLeftOfSecond(node, left)) // node.Key less than left.Key
                    {
                        if (GreaterThan(Increment(leftSize), rightSize))
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
                        if (GreaterThan(Increment(leftRightSize), rightSize))
                        {
                            if (EqualToZero(leftRightSize) && EqualToZero(rightSize))
                            {
                                SetLeft(node, left);
                                SetRight(node, root);
                                SetSize(node, Add(leftSize, Two)); // Two (2) - node the size of root and a node itself
                                SetLeft(root, Zero);
                                SetSize(root, One);
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
                    if (EqualToZero(right))
                    {
                        IncrementSize(root);
                        SetSize(node, One);
                        right = node;
                        return;
                    }
                    if (FirstIsToTheRightOfSecond(node, right)) // node.Key greater than right.Key
                    {
                        if (GreaterThan(Increment(rightSize), leftSize))
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
                        if (GreaterThan(Increment(rightLeftSize), leftSize))
                        {
                            if (EqualToZero(rightLeftSize) && EqualToZero(leftSize))
                            {
                                SetLeft(node, root);
                                SetRight(node, right);
                                SetSize(node, Add(rightSize, Two)); // Two (2) - node the size of root and a node itself
                                SetRight(root, Zero);
                                SetSize(root, One);
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
                if (FirstIsToTheLeftOfSecond(node, root)) // node.Key less than root.Key
                {
                    var decrementedLeftSize = Decrement(leftSize);
                    if (GreaterThan(GetSizeOrZero(GetRightOrDefault(right)), decrementedLeftSize))
                    {
                        LeftRotate(ref root);
                    }
                    else if (GreaterThan(GetSizeOrZero(GetLeftOrDefault(right)), decrementedLeftSize))
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
                else if (FirstIsToTheRightOfSecond(node, root)) // node.Key greater than root.Key
                {
                    var decrementedRightSize = Decrement(rightSize);
                    if (GreaterThan(GetSizeOrZero(GetLeftOrDefault(left)), decrementedRightSize))
                    {
                        RightRotate(ref root);
                    }
                    else if (GreaterThan(GetSizeOrZero(GetRightOrDefault(left)), decrementedRightSize))
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
                    if (GreaterThanZero(leftSize) && GreaterThanZero(rightSize))
                    {
                        TElement replacement;
                        if (GreaterThan(leftSize, rightSize))
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
                        SetSize(replacement, Add(leftSize, rightSize));
                        root = replacement;
                    }
                    else if (GreaterThanZero(leftSize))
                    {
                        root = left;
                    }
                    else if (GreaterThanZero(rightSize))
                    {
                        root = right;
                    }
                    else
                    {
                        root = Zero;
                    }
                    ClearNode(node);
                    return;
                }
            }
        }
    }
}
