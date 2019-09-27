using System;

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member

namespace Platform.Collections.Methods.Trees
{
    /// <summary>
    /// Experimental implementation, don't use it yet.
    /// </summary>
    public abstract class SizeBalancedTreeMethods<TElement> : SizedBinaryTreeMethodsBase<TElement>
    {
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
                        break;
                    }
                    if (FirstIsToTheRightOfSecond(node, left)) // node.Key greater than left.Key
                    {
                        var leftRight = GetRight(left);
                        var leftRightSize = GetSizeOrZero(leftRight);
                        if (GreaterThan(Increment(leftRightSize), rightSize))
                        {
                            if (EqualToZero(leftRightSize) && EqualToZero(rightSize))
                            {
                                SetLeft(node, left);
                                SetRight(node, root);
                                SetSize(node, Add(GetSize(left), Two)); // Two (2) - размер ветки *root (right) и самого node
                                SetLeft(root, Zero);
                                SetSize(root, One);
                                root = node;
                                break;
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
                    else // node.Key less than left.Key
                    {
                        var leftLeft = GetLeft(left);
                        var leftLeftSize = GetSizeOrZero(leftLeft);
                        if (GreaterThan(Increment(leftLeftSize), rightSize))
                        {
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
                        break;
                    }
                    if (FirstIsToTheRightOfSecond(node, right)) // node.Key greater than right.Key
                    {
                        var rightRight = GetRight(right);
                        var rightRightSize = GetSizeOrZero(rightRight);
                        if (GreaterThan(Increment(rightRightSize), leftSize))
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
                        var rightLeft = GetLeft(right);
                        var rightLeftSize = GetSizeOrZero(rightLeft);
                        if (GreaterThan(Increment(rightLeftSize), leftSize))
                        {
                            if (EqualToZero(rightLeftSize) && EqualToZero(leftSize))
                            {
                                SetLeft(node, root);
                                SetRight(node, right);
                                SetSize(node, Add(GetSize(right), Two)); // Two (2) - размер ветки *root (left) и самого node
                                SetRight(root, Zero);
                                SetSize(root, One);
                                root = node;
                                break;
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
                    EnsureNodeInTheTree(node, ref left);
                    var rightLeft = GetLeft(right);
                    var rightLeftSize = GetSizeOrZero(rightLeft);
                    var rightRight = GetRight(right);
                    var rightRightSize = GetSizeOrZero(rightRight);
                    if (GreaterThan(rightRightSize, Decrement(leftSize)))
                    {
                        LeftRotate(ref root);
                    }
                    else if (GreaterThan(rightLeftSize, Decrement(leftSize)))
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
                    EnsureNodeInTheTree(node, ref right);
                    var leftLeft = GetLeft(left);
                    var leftLeftSize = GetSizeOrZero(leftLeft);
                    var leftRight = GetRight(left);
                    var leftRightSize = GetSizeOrZero(leftRight);
                    if (GreaterThan(leftLeftSize, Decrement(rightSize)))
                    {
                        RightRotate(ref root);
                    }
                    else if (GreaterThan(leftRightSize, Decrement(rightSize)))
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
                        if (GreaterThan(leftSize, rightSize))
                        {
                            var replacement = left;
                            while (!EqualToZero(GetRight(replacement)))
                            {
                                replacement = GetRight(replacement);
                            }
                            DetachCore(ref left, replacement);
                            SetLeft(replacement, left);
                            SetRight(replacement, right);
                            FixSize(replacement);
                            root = replacement;
                        }
                        else
                        {
                            var replacement = right;
                            while (!EqualToZero(GetLeft(replacement)))
                            {
                                replacement = GetLeft(replacement);
                            }
                            DetachCore(ref right, replacement);
                            SetLeft(replacement, left);
                            SetRight(replacement, right);
                            FixSize(replacement);
                            root = replacement;
                        }
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
                    break;
                }
            }
        }

        private void EnsureNodeInTheTree(TElement node, ref TElement branch)
        {
            if (EqualToZero(branch))
            {
                throw new InvalidOperationException($"Элемент {node} не содержится в дереве.");
            }
        }
    }
}