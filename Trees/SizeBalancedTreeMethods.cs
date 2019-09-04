using System;

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member

namespace Platform.Collections.Methods.Trees
{
    public unsafe abstract class SizeBalancedTreeMethods<TElement> : SizedBinaryTreeMethodsBase<TElement>
    {
        protected override void AttachCore(IntPtr root, TElement node)
        {
            while (true)
            {
                var left = GetLeftPointer(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root));
                var leftSize = GetSizeOrZero(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)left));
                var right = GetRightPointer(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root));
                var rightSize = GetSizeOrZero(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)right));
                if (FirstIsToTheLeftOfSecond(node, System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root))) // node.Key less than root.Key
                {
                    if (EqualToZero(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)left)))
                    {
                        IncrementSize(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root));
                        SetSize(node, GetOne());
                        System.Runtime.CompilerServices.Unsafe.Write((void*)left, node);
                        break;
                    }
                    if (FirstIsToTheRightOfSecond(node, System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)left))) // node.Key greater than left.Key
                    {
                        var leftRight = GetRightValue(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)left));
                        var leftRightSize = GetSizeOrZero(leftRight);
                        if (GreaterThan(Increment(leftRightSize), rightSize))
                        {
                            if (EqualToZero(leftRightSize) && EqualToZero(rightSize))
                            {
                                SetLeft(node, System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)left));
                                SetRight(node, System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root));
                                SetSize(node, Add(GetSize(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)left)), GetTwo())); // Two (2) - размер ветки *root (right) и самого node
                                SetLeft(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root), GetZero());
                                SetSize(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root), GetOne());
                                System.Runtime.CompilerServices.Unsafe.Write((void*)root, node);
                                break;
                            }
                            LeftRotate(left);
                            RightRotate(root);
                        }
                        else
                        {
                            IncrementSize(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root));
                            root = left;
                        }
                    }
                    else // node.Key less than left.Key
                    {
                        var leftLeft = GetLeftValue(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)left));
                        var leftLeftSize = GetSizeOrZero(leftLeft);
                        if (GreaterThan(Increment(leftLeftSize), rightSize))
                        {
                            RightRotate(root);
                        }
                        else
                        {
                            IncrementSize(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root));
                            root = left;
                        }
                    }
                }
                else // node.Key greater than root.Key
                {
                    if (EqualToZero(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)right)))
                    {
                        IncrementSize(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root));
                        SetSize(node, GetOne());
                        System.Runtime.CompilerServices.Unsafe.Write((void*)right, node);
                        break;
                    }
                    if (FirstIsToTheRightOfSecond(node, System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)right))) // node.Key greater than right.Key
                    {
                        var rightRight = GetRightValue(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)right));
                        var rightRightSize = GetSizeOrZero(rightRight);
                        if (GreaterThan(Increment(rightRightSize), leftSize))
                        {
                            LeftRotate(root);
                        }
                        else
                        {
                            IncrementSize(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root));
                            root = right;
                        }
                    }
                    else // node.Key less than right.Key
                    {
                        var rightLeft = GetLeftValue(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)right));
                        var rightLeftSize = GetSizeOrZero(rightLeft);
                        if (GreaterThan(Increment(rightLeftSize), leftSize))
                        {
                            if (EqualToZero(rightLeftSize) && EqualToZero(leftSize))
                            {
                                SetLeft(node, System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root));
                                SetRight(node, System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)right));
                                SetSize(node, Add(GetSize(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)right)), GetTwo())); // Two (2) - размер ветки *root (left) и самого node
                                SetRight(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root), GetZero());
                                SetSize(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root), GetOne());
                                System.Runtime.CompilerServices.Unsafe.Write((void*)root, node);
                                break;
                            }
                            RightRotate(right);
                            LeftRotate(root);
                        }
                        else
                        {
                            IncrementSize(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root));
                            root = right;
                        }
                    }
                }
            }
        }

        protected override void DetachCore(IntPtr root, TElement node)
        {
            while (true)
            {
                var left = GetLeftPointer(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root));
                var leftSize = GetSizeOrZero(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)left));
                var right = GetRightPointer(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root));
                var rightSize = GetSizeOrZero(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)right));
                if (FirstIsToTheLeftOfSecond(node, System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root))) // node.Key less than root.Key
                {
                    EnsureNodeInTheTree(node, left);
                    var rightLeft = GetLeftValue(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)right));
                    var rightLeftSize = GetSizeOrZero(rightLeft);
                    var rightRight = GetRightValue(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)right));
                    var rightRightSize = GetSizeOrZero(rightRight);
                    if (GreaterThan(rightRightSize, Decrement(leftSize)))
                    {
                        LeftRotate(root);
                    }
                    else if (GreaterThan(rightLeftSize, Decrement(leftSize)))
                    {
                        RightRotate(right);
                        LeftRotate(root);
                    }
                    else
                    {
                        DecrementSize(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root));
                        root = left;
                    }
                }
                else if (FirstIsToTheRightOfSecond(node, System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root))) // node.Key greater than root.Key
                {
                    EnsureNodeInTheTree(node, right);
                    var leftLeft = GetLeftValue(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)left));
                    var leftLeftSize = GetSizeOrZero(leftLeft);
                    var leftRight = GetRightValue(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)left));
                    var leftRightSize = GetSizeOrZero(leftRight);
                    if (GreaterThan(leftLeftSize, Decrement(rightSize)))
                    {
                        RightRotate(root);
                    }
                    else if (GreaterThan(leftRightSize, Decrement(rightSize)))
                    {
                        LeftRotate(left);
                        RightRotate(root);
                    }
                    else
                    {
                        DecrementSize(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root));
                        root = right;
                    }
                }
                else // key equals to root.Key
                {
                    if (GreaterThanZero(leftSize) && GreaterThanZero(rightSize))
                    {
                        if (GreaterThan(leftSize, rightSize))
                        {
                            var replacement = System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)left);
                            while (!EqualToZero(GetRightValue(replacement)))
                            {
                                replacement = GetRightValue(replacement);
                            }
                            DetachCore(left, replacement);
                            SetLeft(replacement, System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)left));
                            SetRight(replacement, System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)right));
                            FixSize(replacement);
                            System.Runtime.CompilerServices.Unsafe.Write((void*)root, replacement);
                        }
                        else
                        {
                            var replacement = System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)right);
                            while (!EqualToZero(GetLeftValue(replacement)))
                            {
                                replacement = GetLeftValue(replacement);
                            }
                            DetachCore(right, replacement);
                            SetLeft(replacement, System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)left));
                            SetRight(replacement, System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)right));
                            FixSize(replacement);
                            System.Runtime.CompilerServices.Unsafe.Write((void*)root, replacement);
                        }
                    }
                    else if (GreaterThanZero(leftSize))
                    {
                        System.Runtime.CompilerServices.Unsafe.Write((void*)root, System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)left));
                    }
                    else if (GreaterThanZero(rightSize))
                    {
                        System.Runtime.CompilerServices.Unsafe.Write((void*)root, System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)right));
                    }
                    else
                    {
                        System.Runtime.CompilerServices.Unsafe.Write((void*)root, GetZero());
                    }
                    ClearNode(node);
                    break;
                }
            }
        }

        private void EnsureNodeInTheTree(TElement node, IntPtr branch)
        {
            if (EqualToZero(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)branch)))
            {
                throw new InvalidOperationException($"Элемент {node} не содержится в дереве.");
            }
        }
    }
}