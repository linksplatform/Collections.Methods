namespace Platform::Collections::Methods::Trees
{
    template <typename ...> class SizeBalancedTreeMethods;
    template <typename impl_t, typename TElement> class SizeBalancedTreeMethods<impl_t, TElement> : public SizedBinaryTreeMethodsBase<impl_t, TElement>
    {

      public:
        using Polymorph<impl_t>::object;
        using base = SizedBinaryTreeMethodsBase<impl_t, TElement>;
        friend base;

        protected: void AttachCore(TElement* root, TElement node)
        {
            if (*root == 0)
            {
                *root = node;
                object().IncrementSize(*root);
            }
            else
            {
                object().IncrementSize(*root);
                if (object().FirstIsToTheLeftOfSecond(node, *root))
                {
                    object().AttachCore(object().GetLeftReference(*root), node);
                    object().LeftMaintain(root);
                }
                else
                {
                    object().AttachCore(object().GetRightReference(*root), node);
                    object().RightMaintain(root);
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
                object().DecrementSize(*currentNode);
                if (object().FirstIsToTheLeftOfSecond(nodeToDetach, *currentNode))
                {
                    parent = currentNode;
                    currentNode = object().GetLeftReference(*currentNode);
                }
                else if (object().FirstIsToTheRightOfSecond(nodeToDetach, *currentNode))
                {
                    parent = currentNode;
                    currentNode = object().GetRightReference(*currentNode);
                }
                else
                {
                    throw std::runtime_error("Duplicate link found in the tree.");
                }
            }
            auto nodeToDetachLeft = object().GetLeft(nodeToDetach);
            auto node = object().GetRight(nodeToDetach);
            if (nodeToDetachLeft != 0 && node != 0)
            {
                auto leftestNode = object().GetLeftest(node);
                object().DetachCore(object().GetRightReference(nodeToDetach), leftestNode);
                object().SetLeft(leftestNode, nodeToDetachLeft);
                node = object().GetRight(nodeToDetach);
                if (node != 0)
                {
                    object().SetRight(leftestNode, node);
                    object().SetSize(leftestNode, (object().GetSize(nodeToDetachLeft) + object().GetSize(node)) + 1);
                }
                else
                {
                    object().SetSize(leftestNode, object().GetSize(nodeToDetachLeft) + 1);
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
            else if (object().GetLeft(*parent) == nodeToDetach)
            {
                object().SetLeft(*parent, replacementNode);
            }
            else if (object().GetRight(*parent) == nodeToDetach)
            {
                object().SetRight(*parent, replacementNode);
            }
            object().ClearNode(nodeToDetach);
        }

        private: void LeftMaintain(TElement* root)
        {
            if (*root != 0)
            {
                auto rootLeftNode = object().GetLeft(*root);
                if (rootLeftNode != 0)
                {
                    auto rootRightNode = object().GetRight(*root);
                    auto rootRightNodeSize = object().GetSize(rootRightNode);
                    auto rootLeftNodeLeftNode = object().GetLeft(rootLeftNode);
                    if (rootLeftNodeLeftNode != 0 &&
                        (rootRightNode == 0 || object().GetSize(rootLeftNodeLeftNode) > rootRightNodeSize))
                    {
                        object().RightRotate(root);
                    }
                    else
                    {
                        auto rootLeftNodeRightNode = object().GetRight(rootLeftNode);
                        if (rootLeftNodeRightNode != 0 &&
                            (rootRightNode == 0 || object().GetSize(rootLeftNodeRightNode) > rootRightNodeSize))
                        {
                            object().LeftRotate(object().GetLeftReference(*root));
                            object().RightRotate(root);
                        }
                        else
                        {
                            return;
                        }
                    }
                    object().LeftMaintain(object().GetLeftReference(*root));
                    object().RightMaintain(object().GetRightReference(*root));
                    object().LeftMaintain(root);
                    object().RightMaintain(root);
                }
            }
        }

        private: void RightMaintain(TElement* root)
        {
            if (*root != 0)
            {
                auto rootRightNode = object().GetRight(*root);
                if (rootRightNode != 0)
                {
                    auto rootLeftNode = object().GetLeft(*root);
                    auto rootLeftNodeSize = object().GetSize(rootLeftNode);
                    auto rootRightNodeRightNode = object().GetRight(rootRightNode);
                    if (rootRightNodeRightNode != 0 &&
                        (rootLeftNode == 0 || object().GetSize(rootRightNodeRightNode) > rootLeftNodeSize))
                    {
                        object().LeftRotate(root);
                    }
                    else
                    {
                        auto rootRightNodeLeftNode = object().GetLeft(rootRightNode);
                        if (rootRightNodeLeftNode != 0 &&
                            (rootLeftNode == 0 || object().GetSize(rootRightNodeLeftNode) > rootLeftNodeSize))
                        {
                            object().RightRotate(object().GetRightReference(*root));
                            object().LeftRotate(root);
                        }
                        else
                        {
                            return;
                        }
                    }
                    object().LeftMaintain(object().GetLeftReference(*root));
                    object().RightMaintain(object().GetRightReference(*root));
                    object().LeftMaintain(root);
                    object().RightMaintain(root);
                }
            }
        }
    };
}
