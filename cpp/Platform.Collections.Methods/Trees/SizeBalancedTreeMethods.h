namespace Platform::Collections::Methods::Trees
{
    template <typename ...> class SizeBalancedTreeMethods;
    template <typename impl_t, typename TElement> class SizeBalancedTreeMethods<impl_t, TElement> : public SizedBinaryTreeMethodsBase<impl_t, TElement>
    {
        using base_t = SizedBinaryTreeMethodsBase<impl_t, TElement>;
        friend base_t;

        protected: void AttachCore(TElement* root, TElement node)
        {
            if (*root == 0)
            {
                *root = node;
                static_cast<impl_t*>(this)->IncrementSize(*root);
            }
            else
            {
                static_cast<impl_t*>(this)->IncrementSize(*root);
                if (static_cast<impl_t*>(this)->FirstIsToTheLeftOfSecond(node, *root))
                {
                    static_cast<impl_t*>(this)->AttachCore(static_cast<impl_t*>(this)->GetLeftReference(*root), node);
                    static_cast<impl_t*>(this)->LeftMaintain(root);
                }
                else
                {
                    static_cast<impl_t*>(this)->AttachCore(static_cast<impl_t*>(this)->GetRightReference(*root), node);
                    static_cast<impl_t*>(this)->RightMaintain(root);
                }
            }
        }

        protected: void DetachCore(TElement* root, TElement nodeToDetach)
        {
            auto* currentNode = root;
            auto* parent = root;
            auto replacementNode = 0;
            while (*currentNode != nodeToDetach)
            {
                static_cast<impl_t*>(this)->DecrementSize(*currentNode);
                if (static_cast<impl_t*>(this)->FirstIsToTheLeftOfSecond(nodeToDetach, *currentNode))
                {
                    parent = currentNode;
                    currentNode = static_cast<impl_t*>(this)->GetLeftReference(*currentNode);
                }
                else if (static_cast<impl_t*>(this)->FirstIsToTheRightOfSecond(nodeToDetach, *currentNode))
                {
                    parent = currentNode;
                    currentNode = static_cast<impl_t*>(this)->GetRightReference(*currentNode);
                }
                else
                {
                    throw std::runtime_error("Duplicate link found in the tree.");
                }
            }
            auto nodeToDetachLeft = static_cast<impl_t*>(this)->GetLeft(nodeToDetach);
            auto node = static_cast<impl_t*>(this)->GetRight(nodeToDetach);
            if (nodeToDetachLeft != 0 && node != 0)
            {
                auto leftestNode = static_cast<impl_t*>(this)->GetLeftest(node);
                static_cast<impl_t*>(this)->DetachCore(static_cast<impl_t*>(this)->GetRightReference(nodeToDetach), leftestNode);
                static_cast<impl_t*>(this)->SetLeft(leftestNode, nodeToDetachLeft);
                node = static_cast<impl_t*>(this)->GetRight(nodeToDetach);
                if (node != 0)
                {
                    static_cast<impl_t*>(this)->SetRight(leftestNode, node);
                    static_cast<impl_t*>(this)->SetSize(leftestNode, (static_cast<impl_t*>(this)->GetSize(nodeToDetachLeft) + static_cast<impl_t*>(this)->GetSize(node)) + 1);
                }
                else
                {
                    static_cast<impl_t*>(this)->SetSize(leftestNode, static_cast<impl_t*>(this)->GetSize(nodeToDetachLeft) + 1);
                }
                replacementNode = leftestNode;
            }
            else if (nodeToDetachLeft != 0)
            {
                replacementNode = nodeToDetachLeft;
            }
            else if (node != 0)
            {
                replacementNode = node;
            }
            if (*root == nodeToDetach)
            {
                *root = replacementNode;
            }
            else if (static_cast<impl_t*>(this)->GetLeft(*parent) == nodeToDetach)
            {
                static_cast<impl_t*>(this)->SetLeft(*parent, replacementNode);
            }
            else if (static_cast<impl_t*>(this)->GetRight(*parent) == nodeToDetach)
            {
                static_cast<impl_t*>(this)->SetRight(*parent, replacementNode);
            }
            static_cast<impl_t*>(this)->ClearNode(nodeToDetach);
        }

        private: void LeftMaintain(TElement* root)
        {
            if (*root != 0)
            {
                auto rootLeftNode = static_cast<impl_t*>(this)->GetLeft(*root);
                if (rootLeftNode != 0)
                {
                    auto rootRightNode = static_cast<impl_t*>(this)->GetRight(*root);
                    auto rootRightNodeSize = static_cast<impl_t*>(this)->GetSize(rootRightNode);
                    auto rootLeftNodeLeftNode = static_cast<impl_t*>(this)->GetLeft(rootLeftNode);
                    if (rootLeftNodeLeftNode != 0 &&
                        (rootRightNode == 0 || static_cast<impl_t*>(this)->GetSize(rootLeftNodeLeftNode) > rootRightNodeSize))
                    {
                        static_cast<impl_t*>(this)->RightRotate(root);
                    }
                    else
                    {
                        auto rootLeftNodeRightNode = static_cast<impl_t*>(this)->GetRight(rootLeftNode);
                        if (rootLeftNodeRightNode != 0 &&
                            (rootRightNode == 0 || static_cast<impl_t*>(this)->GetSize(rootLeftNodeRightNode) > rootRightNodeSize))
                        {
                            static_cast<impl_t*>(this)->LeftRotate(static_cast<impl_t*>(this)->GetLeftReference(*root));
                            static_cast<impl_t*>(this)->RightRotate(root);
                        }
                        else
                        {
                            return;
                        }
                    }
                    static_cast<impl_t*>(this)->LeftMaintain(static_cast<impl_t*>(this)->GetLeftReference(*root));
                    static_cast<impl_t*>(this)->RightMaintain(static_cast<impl_t*>(this)->GetRightReference(*root));
                    static_cast<impl_t*>(this)->LeftMaintain(root);
                    static_cast<impl_t*>(this)->RightMaintain(root);
                }
            }
        }

        private: void RightMaintain(TElement* root)
        {
            if (*root != 0)
            {
                auto rootRightNode = static_cast<impl_t*>(this)->GetRight(*root);
                if (rootRightNode != 0)
                {
                    auto rootLeftNode = static_cast<impl_t*>(this)->GetLeft(*root);
                    auto rootLeftNodeSize = static_cast<impl_t*>(this)->GetSize(rootLeftNode);
                    auto rootRightNodeRightNode = static_cast<impl_t*>(this)->GetRight(rootRightNode);
                    if (rootRightNodeRightNode != 0 &&
                        (rootLeftNode == 0 || static_cast<impl_t*>(this)->GetSize(rootRightNodeRightNode) > rootLeftNodeSize))
                    {
                        static_cast<impl_t*>(this)->LeftRotate(root);
                    }
                    else
                    {
                        auto rootRightNodeLeftNode = static_cast<impl_t*>(this)->GetLeft(rootRightNode);
                        if (rootRightNodeLeftNode != 0 &&
                            (rootLeftNode == 0 || static_cast<impl_t*>(this)->GetSize(rootRightNodeLeftNode) > rootLeftNodeSize))
                        {
                            static_cast<impl_t*>(this)->RightRotate(static_cast<impl_t*>(this)->GetRightReference(*root));
                            static_cast<impl_t*>(this)->LeftRotate(root);
                        }
                        else
                        {
                            return;
                        }
                    }
                    static_cast<impl_t*>(this)->LeftMaintain(static_cast<impl_t*>(this)->GetLeftReference(*root));
                    static_cast<impl_t*>(this)->RightMaintain(static_cast<impl_t*>(this)->GetRightReference(*root));
                    static_cast<impl_t*>(this)->LeftMaintain(root);
                    static_cast<impl_t*>(this)->RightMaintain(root);
                }
            }
        }
    };
}