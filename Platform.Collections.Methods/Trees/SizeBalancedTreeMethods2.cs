using System;

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member

namespace Platform.Collections.Methods.Trees
{
    public abstract class SizeBalancedTreeMethods2<TElement> : SizedBinaryTreeMethodsBase<TElement>
    {
        protected override void AttachCore(ref TElement root, TElement newNode)
        {
            if (EqualToZero(root))
            {
                root = newNode;
                IncrementSize(root);
            }
            else
            {
                IncrementSize(root);
                if (FirstIsToTheLeftOfSecond(newNode, root))
                {
                    AttachCore(ref GetLeftReference(root), newNode);
                    LeftMaintain(ref root);
                }
                else
                {
                    AttachCore(ref GetRightReference(root), newNode);
                    RightMaintain(ref root);
                }
            }
        }

        protected override void DetachCore(ref TElement root, TElement nodeToDetach)
        {
            if (EqualToZero(root))
            {
                return;
            }
            ref var currentNode = ref root;
            TElement @null = default;
            var parentIsSet = false;
            ref TElement parent = ref @null; /* Изначально зануление, так как родителя может и не быть (Корень дерева). */
            var replacementNode = Zero;
            while (!IsEquals(currentNode, nodeToDetach))
            {
                SetSize(currentNode, Decrement(GetSize(currentNode)));
                if (FirstIsToTheLeftOfSecond(nodeToDetach, currentNode))
                {
                    parent = ref currentNode;
                    parentIsSet = true;
                    currentNode = ref GetLeftReference(currentNode);
                }
                else if (FirstIsToTheRightOfSecond(nodeToDetach, currentNode))
                {
                    parent = ref currentNode;
                    parentIsSet = true;
                    currentNode = ref GetRightReference(currentNode);
                }
                else
                {
                    throw new InvalidOperationException("Duplicate link found in the tree.");
                }
            }
            if (!EqualToZero(GetLeft(nodeToDetach)) && !EqualToZero(GetRight(nodeToDetach)))
            {
                var minNode = GetRight(nodeToDetach);
                while (!EqualToZero(GetLeft(minNode)))
                {
                    minNode = GetLeft(minNode); /* Передвигаемся до минимума */
                }
                DetachCore(ref GetRightReference(nodeToDetach), minNode);
                SetLeft(minNode, GetLeft(nodeToDetach));
                if (!EqualToZero(GetRight(nodeToDetach)))
                {
                    SetRight(minNode, GetRight(nodeToDetach));
                    SetSize(minNode, Increment(Add(GetSize(GetLeft(nodeToDetach)), GetSize(GetRight(nodeToDetach)))));
                }
                else
                {
                    SetSize(minNode, Increment(GetSize(GetLeft(nodeToDetach))));
                }
                replacementNode = minNode;
            }
            else if (!EqualToZero(GetLeft(nodeToDetach)))
            {
                replacementNode = GetLeft(nodeToDetach);
            }
            else if (!EqualToZero(GetRight(nodeToDetach)))
            {
                replacementNode = GetRight(nodeToDetach);
            }
            if (!parentIsSet)
            {
                root = replacementNode;
            }
            else if (IsEquals(GetLeft(parent), nodeToDetach))
            {
                SetLeft(parent, replacementNode);
            }
            else if (IsEquals(GetRight(parent), nodeToDetach))
            {
                SetRight(parent, replacementNode);
            }
            ClearNode(nodeToDetach);
        }

        private void LeftMaintain(ref TElement root)
        {
            if (!EqualToZero(root))
            {
                var rootLeftNode = GetLeft(root);
                if (!EqualToZero(rootLeftNode))
                {
                    var rootRightNode = GetRight(root);
                    var rootLeftNodeLeftNode = GetLeft(rootLeftNode);
                    if (!EqualToZero(rootLeftNodeLeftNode) &&
                        (EqualToZero(rootRightNode) || GreaterThan(GetSize(rootLeftNodeLeftNode), GetSize(rootRightNode))))
                    {
                        RightRotate(ref root);
                    }
                    else
                    {
                        var rootLeftNodeRightNode = GetRight(rootLeftNode);
                        if (!EqualToZero(rootLeftNodeRightNode) &&
                            (EqualToZero(rootRightNode) || GreaterThan(GetSize(rootLeftNodeRightNode), GetSize(rootRightNode))))
                        {
                            LeftRotate(ref GetLeftReference(root));
                            RightRotate(ref root);
                        }
                        else
                        {
                            return;
                        }
                    }
                    LeftMaintain(ref GetLeftReference(root));
                    RightMaintain(ref GetRightReference(root));
                    LeftMaintain(ref root);
                    RightMaintain(ref root);
                }
            }
        }

        private void RightMaintain(ref TElement root)
        {
            if (!EqualToZero(root))
            {
                var rootRightNode = GetRight(root);
                if (!EqualToZero(rootRightNode))
                {
                    var rootLeftNode = GetLeft(root);
                    var rootRightNodeRightNode = GetRight(rootRightNode);
                    if (!EqualToZero(rootRightNodeRightNode) &&
                        (EqualToZero(rootLeftNode) || GreaterThan(GetSize(rootRightNodeRightNode), GetSize(rootLeftNode))))
                    {
                        LeftRotate(ref root);
                    }
                    else
                    {
                        var rootRightNodeLeftNode = GetLeft(rootRightNode);
                        if (!EqualToZero(rootRightNodeLeftNode) &&
                            (EqualToZero(rootLeftNode) || GreaterThan(GetSize(rootRightNodeLeftNode), GetSize(rootLeftNode))))
                        {
                            RightRotate(ref GetRightReference(root));
                            LeftRotate(ref root);
                        }
                        else
                        {
                            return;
                        }
                    }
                    LeftMaintain(ref GetLeftReference(root));
                    RightMaintain(ref GetRightReference(root));
                    LeftMaintain(ref root);
                    RightMaintain(ref root);
                }
            }
        }
    }
}