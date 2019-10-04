

namespace Platform::Collections::Methods::Trees
{
    template <typename TElement> class SizeBalancedTreeMethods2 : SizedBinaryTreeMethodsBase<TElement>
    {
        void AttachCore(TElement* root, TElement node) override
        {
            if (root == 0)
            {
                root = node;
                SizedBinaryTreeMethodsBase::IncrementSize(root);
            }
            else
            {
                SizedBinaryTreeMethodsBase::IncrementSize(root);
                if (SizedBinaryTreeMethodsBase::FirstIsToTheLeftOfSecond(node, root))
                {
                    AttachCore(SizedBinaryTreeMethodsBase::GetLeftReference(root), node);
                    LeftMaintain(root);
                }
                else
                {
                    AttachCore(SizedBinaryTreeMethodsBase::GetRightReference(root), node);
                    RightMaintain(root);
                }
            }
        }

        void DetachCore(TElement* root, TElement nodeToDetach) override
        {
            auto* currentNode = root;
            auto* parent = root;
            auto replacementNode = 0;
            while (currentNode != nodeToDetach)
            {
                SizedBinaryTreeMethodsBase::SetSize(currentNode, SizedBinaryTreeMethodsBase::GetSize(currentNode) - 1);
                if (SizedBinaryTreeMethodsBase::FirstIsToTheLeftOfSecond(nodeToDetach, currentNode))
                {
                    parent = currentNode;
                    currentNode = SizedBinaryTreeMethodsBase::GetLeftReference(currentNode);
                }
                else if (SizedBinaryTreeMethodsBase::FirstIsToTheRightOfSecond(nodeToDetach, currentNode))
                {
                    parent = currentNode;
                    currentNode = SizedBinaryTreeMethodsBase::GetRightReference(currentNode);
                }
                else
                {
                    throw std::exception("Duplicate link found in the tree.");
                }
            }
            auto nodeToDetachLeft = SizedBinaryTreeMethodsBase::GetLeft(nodeToDetach);
            auto node = SizedBinaryTreeMethodsBase::GetRight(nodeToDetach);
            if (nodeToDetachLeft != 0 && node != 0)
            {
                auto minNode = node;
                auto minNodeLeft = SizedBinaryTreeMethodsBase::GetLeft(minNode);
                while (minNodeLeft != 0)
                {
                    minNode = minNodeLeft;
                    minNodeLeft = SizedBinaryTreeMethodsBase::GetLeft(minNode);
                }
                DetachCore(SizedBinaryTreeMethodsBase::GetRightReference(nodeToDetach), minNode);
                SizedBinaryTreeMethodsBase::SetLeft(minNode, nodeToDetachLeft);
                node = SizedBinaryTreeMethodsBase::GetRight(nodeToDetach);
                if (node != 0)
                {
                    SizedBinaryTreeMethodsBase::SetRight(minNode, node);
                    SizedBinaryTreeMethodsBase::SetSize(minNode, SizedBinaryTreeMethodsBase::GetSize(nodeToDetachLeft) + SizedBinaryTreeMethodsBase::GetSize(node) + 1);
                }
                else
                {
                    SizedBinaryTreeMethodsBase::SetSize(minNode, SizedBinaryTreeMethodsBase::GetSize(nodeToDetachLeft) + 1);
                }
                replacementNode = minNode;
            }
            else if (nodeToDetachLeft != 0)
            {
                replacementNode = nodeToDetachLeft;
            }
            else if (node != 0)
            {
                replacementNode = node;
            }
            if (root == nodeToDetach)
            {
                root = replacementNode;
            }
            else if (SizedBinaryTreeMethodsBase::GetLeft(parent) == nodeToDetach)
            {
                SizedBinaryTreeMethodsBase::SetLeft(parent, replacementNode);
            }
            else if (SizedBinaryTreeMethodsBase::GetRight(parent) == nodeToDetach)
            {
                SizedBinaryTreeMethodsBase::SetRight(parent, replacementNode);
            }
            SizedBinaryTreeMethodsBase::ClearNode(nodeToDetach);
        }

        void LeftMaintain(TElement* root)
        {
            if (root != 0)
            {
                auto rootLeftNode = SizedBinaryTreeMethodsBase::GetLeft(root);
                if (rootLeftNode != 0)
                {
                    auto rootRightNode = SizedBinaryTreeMethodsBase::GetRight(root);
                    auto rootLeftNodeLeftNode = SizedBinaryTreeMethodsBase::GetLeft(rootLeftNode);
                    if (rootLeftNodeLeftNode != 0 &&
                        (rootRightNode == 0 || SizedBinaryTreeMethodsBase::GetSize(rootLeftNodeLeftNode) > SizedBinaryTreeMethodsBase::GetSize(rootRightNode)))
                    {
                        SizedBinaryTreeMethodsBase::RightRotate(root);
                    }
                    else
                    {
                        auto rootLeftNodeRightNode = SizedBinaryTreeMethodsBase::GetRight(rootLeftNode);
                        if (rootLeftNodeRightNode != 0 &&
                            (rootRightNode == 0 || SizedBinaryTreeMethodsBase::GetSize(rootLeftNodeRightNode) > SizedBinaryTreeMethodsBase::GetSize(rootRightNode)))
                        {
                            SizedBinaryTreeMethodsBase::LeftRotate(SizedBinaryTreeMethodsBase::GetLeftReference(root));
                            SizedBinaryTreeMethodsBase::RightRotate(root);
                        }
                        else
                        {
                            return;
                        }
                    }
                    LeftMaintain(SizedBinaryTreeMethodsBase::GetLeftReference(root));
                    RightMaintain(SizedBinaryTreeMethodsBase::GetRightReference(root));
                    LeftMaintain(root);
                    RightMaintain(root);
                }
            }
        }

        void RightMaintain(TElement* root)
        {
            if (root != 0)
            {
                auto rootRightNode = SizedBinaryTreeMethodsBase::GetRight(root);
                if (rootRightNode != 0)
                {
                    auto rootLeftNode = SizedBinaryTreeMethodsBase::GetLeft(root);
                    auto rootRightNodeRightNode = SizedBinaryTreeMethodsBase::GetRight(rootRightNode);
                    if (rootRightNodeRightNode != 0 &&
                        (rootLeftNode == 0 || SizedBinaryTreeMethodsBase::GetSize(rootRightNodeRightNode) > SizedBinaryTreeMethodsBase::GetSize(rootLeftNode)))
                    {
                        SizedBinaryTreeMethodsBase::LeftRotate(root);
                    }
                    else
                    {
                        auto rootRightNodeLeftNode = SizedBinaryTreeMethodsBase::GetLeft(rootRightNode);
                        if (rootRightNodeLeftNode != 0 &&
                            (rootLeftNode == 0 || SizedBinaryTreeMethodsBase::GetSize(rootRightNodeLeftNode) > SizedBinaryTreeMethodsBase::GetSize(rootLeftNode)))
                        {
                            SizedBinaryTreeMethodsBase::RightRotate(SizedBinaryTreeMethodsBase::GetRightReference(root));
                            SizedBinaryTreeMethodsBase::LeftRotate(root);
                        }
                        else
                        {
                            return;
                        }
                    }
                    LeftMaintain(SizedBinaryTreeMethodsBase::GetLeftReference(root));
                    RightMaintain(SizedBinaryTreeMethodsBase::GetRightReference(root));
                    LeftMaintain(root);
                    RightMaintain(root);
                }
            }
        }
    };
}