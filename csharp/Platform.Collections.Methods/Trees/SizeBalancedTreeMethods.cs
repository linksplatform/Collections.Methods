using System;
using System.Numerics;

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member

namespace Platform.Collections.Methods.Trees
{
    /// <summary>
    /// <para>
    /// Represents the size balanced tree methods.
    /// </para>
    /// <para></para>
    /// </summary>
    /// <seealso cref="SizedBinaryTreeMethodsBase{TElement}"/>
    public abstract class SizeBalancedTreeMethods<TElement> : SizedBinaryTreeMethodsBase<TElement> where TElement: IUnsignedNumber<TElement>, IComparisonOperators<TElement, TElement, bool>
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
            if (root == TElement.Zero)
            {
                root = node;
                IncrementSize(root);
            }
            else
            {
                IncrementSize(root);
                if (FirstIsToTheLeftOfSecond(node, root))
                {
                    AttachCore(ref GetLeftReference(root), node);
                    LeftMaintain(ref root);
                }
                else
                {
                    AttachCore(ref GetRightReference(root), node);
                    RightMaintain(ref root);
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
        /// <param name="nodeToDetach">
        /// <para>The node to detach.</para>
        /// <para></para>
        /// </param>
        /// <exception cref="InvalidOperationException">
        /// <para>Duplicate link found in the tree.</para>
        /// <para></para>
        /// </exception>
        protected override void DetachCore(ref TElement root, TElement nodeToDetach)
        {
            ref var currentNode = ref root;
            ref var parent = ref root;
            var replacementNode = TElement.Zero;
            while (currentNode != nodeToDetach)
            {
                DecrementSize(currentNode);
                if (FirstIsToTheLeftOfSecond(nodeToDetach, currentNode))
                {
                    parent = ref currentNode;
                    currentNode = ref GetLeftReference(currentNode);
                }
                else if (FirstIsToTheRightOfSecond(nodeToDetach, currentNode))
                {
                    parent = ref currentNode;
                    currentNode = ref GetRightReference(currentNode);
                }
                else
                {
                    throw new InvalidOperationException("Duplicate link found in the tree.");
                }
            }
            var nodeToDetachLeft = GetLeft(nodeToDetach);
            var node = GetRight(nodeToDetach);
            if (nodeToDetachLeft != TElement.Zero && node != TElement.Zero)
            {
                var leftestNode = GetLeftest(node);
                DetachCore(ref GetRightReference(nodeToDetach), leftestNode);
                SetLeft(leftestNode, nodeToDetachLeft);
                node = GetRight(nodeToDetach);
                if (node != TElement.Zero)
                {
                    SetRight(leftestNode, node);
                    SetSize(leftestNode, ((GetSize(nodeToDetachLeft) + GetSize(node))) + TElement.One);
                }
                else
                {
                    SetSize(leftestNode, (GetSize(nodeToDetachLeft)) + TElement.One);
                }
                replacementNode = leftestNode;
            }
            else if (nodeToDetachLeft != TElement.Zero)
            {
                replacementNode = nodeToDetachLeft;
            }
            else if (node != TElement.Zero)
            {
                replacementNode = node;
            }
            if (root == nodeToDetach)
            {
                root = replacementNode;
            }
            else if ((GetLeft(parent) == nodeToDetach))
            {
                SetLeft(parent, replacementNode);
            }
            else if ((GetRight(parent) == nodeToDetach))
            {
                SetRight(parent, replacementNode);
            }
            ClearNode(nodeToDetach);
        }
        private void LeftMaintain(ref TElement root)
        {
            if (root != TElement.Zero)
            {
                var rootLeftNode = GetLeft(root);
                if (rootLeftNode != TElement.Zero)
                {
                    var rootRightNode = GetRight(root);
                    var rootRightNodeSize = GetSize(rootRightNode);
                    var rootLeftNodeLeftNode = GetLeft(rootLeftNode);
                    if (rootLeftNodeLeftNode != TElement.Zero &&
                        (rootRightNode == TElement.Zero || (GetSize(rootLeftNodeLeftNode)) > rootRightNodeSize))
                    {
                        RightRotate(ref root);
                    }
                    else
                    {
                        var rootLeftNodeRightNode = GetRight(rootLeftNode);
                        if (rootLeftNodeRightNode != TElement.Zero &&
                            (rootRightNode == TElement.Zero || (GetSize(rootLeftNodeRightNode)) > rootRightNodeSize))
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
            if (root != TElement.Zero)
            {
                var rootRightNode = GetRight(root);
                if (rootRightNode != TElement.Zero)
                {
                    var rootLeftNode = GetLeft(root);
                    var rootLeftNodeSize = GetSize(rootLeftNode);
                    var rootRightNodeRightNode = GetRight(rootRightNode);
                    if (rootRightNodeRightNode != TElement.Zero &&
                        (rootLeftNode == TElement.Zero || (GetSize(rootRightNodeRightNode)) > rootLeftNodeSize))
                    {
                        LeftRotate(ref root);
                    }
                    else
                    {
                        var rootRightNodeLeftNode = GetLeft(rootRightNode);
                        if (rootRightNodeLeftNode != TElement.Zero &&
                            (rootLeftNode == TElement.Zero || (GetSize(rootRightNodeLeftNode)) > rootLeftNodeSize))
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
