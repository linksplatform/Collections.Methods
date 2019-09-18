using System;

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member

namespace Platform.Collections.Methods.Trees
{
    /// <summary>
    /// Experimental implementation, don't use it yet.
    /// </summary>
    public unsafe abstract class SizeBalancedTreeMethods2<TElement> : SizedBinaryTreeMethodsBase<TElement>
    {
        protected override void AttachCore(IntPtr root, TElement newNode)
        {
            if (ValueEqualToZero(root))
            {
                System.Runtime.CompilerServices.Unsafe.Write((void*)root, newNode);
                IncrementSize(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root));
            }
            else
            {
                IncrementSize(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root));
                if (FirstIsToTheLeftOfSecond(newNode, System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root)))
                {
                    AttachCore(GetLeftPointer(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root)), newNode);
                    LeftMaintain(root);
                }
                else
                {
                    AttachCore(GetRightPointer(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root)), newNode);
                    RightMaintain(root);
                }
            }
        }

        protected override void DetachCore(IntPtr root, TElement nodeToDetach)
        {
            if (ValueEqualToZero(root))
            {
                return;
            }
            var currentNode = root;
            var parent = IntPtr.Zero; /* Изначально зануление, так как родителя может и не быть (Корень дерева). */
            var replacementNode = GetZero();
            while (!IsEquals(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)currentNode), nodeToDetach))
            {
                SetSize(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)currentNode), Decrement(GetSize(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)currentNode))));
                if (FirstIsToTheLeftOfSecond(nodeToDetach, System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)currentNode)))
                {
                    parent = currentNode;
                    currentNode = GetLeftPointer(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)currentNode));
                }
                else if (FirstIsToTheRightOfSecond(nodeToDetach, System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)currentNode)))
                {
                    parent = currentNode;
                    currentNode = GetRightPointer(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)currentNode));
                }
                else
                {
                    throw new InvalidOperationException("Duplicate link found in the tree.");
                }
            }
            if (!ValueEqualToZero(GetLeftPointer(nodeToDetach)) && !ValueEqualToZero(GetRightPointer(nodeToDetach)))
            {
                var minNode = GetRightValue(nodeToDetach);
                while (!EqualToZero(GetLeftValue(minNode)))
                {
                    minNode = GetLeftValue(minNode); /* Передвигаемся до минимума */
                }
                DetachCore(GetRightPointer(nodeToDetach), minNode);
                SetLeft(minNode, GetLeftValue(nodeToDetach));
                if (!ValueEqualToZero(GetRightPointer(nodeToDetach)))
                {
                    SetRight(minNode, GetRightValue(nodeToDetach));
                    SetSize(minNode, Increment(Add(GetSize(GetLeftValue(nodeToDetach)), GetSize(GetRightValue(nodeToDetach)))));
                }
                else
                {
                    SetSize(minNode, Increment(GetSize(GetLeftValue(nodeToDetach))));
                }
                replacementNode = minNode;
            }
            else if (!ValueEqualToZero(GetLeftPointer(nodeToDetach)))
            {
                replacementNode = GetLeftValue(nodeToDetach);
            }
            else if (!ValueEqualToZero(GetRightPointer(nodeToDetach)))
            {
                replacementNode = GetRightValue(nodeToDetach);
            }
            if (parent == IntPtr.Zero)
            {
                System.Runtime.CompilerServices.Unsafe.Write((void*)root, replacementNode);
            }
            else if (IsEquals(GetLeftValue(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)parent)), nodeToDetach))
            {
                SetLeft(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)parent), replacementNode);
            }
            else if (IsEquals(GetRightValue(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)parent)), nodeToDetach))
            {
                SetRight(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)parent), replacementNode);
            }
            ClearNode(nodeToDetach);
        }

        private void LeftMaintain(IntPtr root)
        {
            if (!ValueEqualToZero(root))
            {
                var rootLeftNode = GetLeftPointer(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root));
                if (!ValueEqualToZero(rootLeftNode))
                {
                    var rootRightNode = GetRightPointer(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root));
                    var rootLeftNodeLeftNode = GetLeftPointer(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)rootLeftNode));
                    if (!ValueEqualToZero(rootLeftNodeLeftNode) &&
                        (ValueEqualToZero(rootRightNode) || GreaterThan(GetSize(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)rootLeftNodeLeftNode)), GetSize(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)rootRightNode)))))
                    {
                        RightRotate(root);
                    }
                    else
                    {
                        var rootLeftNodeRightNode = GetRightPointer(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)rootLeftNode));
                        if (!ValueEqualToZero(rootLeftNodeRightNode) &&
                            (ValueEqualToZero(rootRightNode) || GreaterThan(GetSize(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)rootLeftNodeRightNode)), GetSize(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)rootRightNode)))))
                        {
                            LeftRotate(GetLeftPointer(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root)));
                            RightRotate(root);
                        }
                        else
                        {
                            return;
                        }
                    }
                    LeftMaintain(GetLeftPointer(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root)));
                    RightMaintain(GetRightPointer(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root)));
                    LeftMaintain(root);
                    RightMaintain(root);
                }
            }
        }

        private void RightMaintain(IntPtr root)
        {
            if (!ValueEqualToZero(root))
            {
                var rootRightNode = GetRightPointer(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root));
                if (!ValueEqualToZero(rootRightNode))
                {
                    var rootLeftNode = GetLeftPointer(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root));
                    var rootRightNodeRightNode = GetRightPointer(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)rootRightNode));
                    if (!ValueEqualToZero(rootRightNodeRightNode) &&
                        (ValueEqualToZero(rootLeftNode) || GreaterThan(GetSize(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)rootRightNodeRightNode)), GetSize(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)rootLeftNode)))))
                    {
                        LeftRotate(root);
                    }
                    else
                    {
                        var rootRightNodeLeftNode = GetLeftPointer(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)rootRightNode));
                        if (!ValueEqualToZero(rootRightNodeLeftNode) &&
                            (ValueEqualToZero(rootLeftNode) || GreaterThan(GetSize(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)rootRightNodeLeftNode)), GetSize(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)rootLeftNode)))))
                        {
                            RightRotate(GetRightPointer(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root)));
                            LeftRotate(root);
                        }
                        else
                        {
                            return;
                        }
                    }
                    LeftMaintain(GetLeftPointer(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root)));
                    RightMaintain(GetRightPointer(System.Runtime.CompilerServices.Unsafe.Read<TElement>((void*)root)));
                    LeftMaintain(root);
                    RightMaintain(root);
                }
            }
        }
    }
}