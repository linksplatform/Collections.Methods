namespace Platform::Collections::Methods::Trees
{
    template <typename ...> class SizeBalancedTreeMethods;
    template <typename TSelf, typename TElement> class SizeBalancedTreeMethods<TSelf, TElement> : public SizedBinaryTreeMethodsBase<TSelf, TElement>
    {

      public:
        using base = SizedBinaryTreeMethodsBase<TSelf, TElement>;
        friend base;

        protected: void AttachCore(TElement* root, TElement node)
        {
            if (*root == 0)
            {
                *root = node;
                this->object().IncrementSize(*root);
            }
            else
            {
                this->object().IncrementSize(*root);
                if (this->object().FirstIsToTheLeftOfSecond(node, *root))
                {
                    this->object().AttachCore(this->object().GetLeftReference(*root), node);
                    this->object().LeftMaintain(root);
                }
                else
                {
                    this->object().AttachCore(this->object().GetRightReference(*root), node);
                    this->object().RightMaintain(root);
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
                this->object().DecrementSize(*currentNode);
                if (this->object().FirstIsToTheLeftOfSecond(nodeToDetach, *currentNode))
                {
                    parent = currentNode;
                    currentNode = this->object().GetLeftReference(*currentNode);
                }
                else if (this->object().FirstIsToTheRightOfSecond(nodeToDetach, *currentNode))
                {
                    parent = currentNode;
                    currentNode = this->object().GetRightReference(*currentNode);
                }
                else
                {
                    throw std::runtime_error("Duplicate link found in the tree.");
                }
            }
            auto nodeToDetachLeft = this->object().GetLeft(nodeToDetach);
            auto node = this->object().GetRight(nodeToDetach);
            if (nodeToDetachLeft != 0 && node != 0)
            {
                auto leftestNode = this->object().GetLeftest(node);
                this->object().DetachCore(this->object().GetRightReference(nodeToDetach), leftestNode);
                this->object().SetLeft(leftestNode, nodeToDetachLeft);
                node = this->object().GetRight(nodeToDetach);
                if (node != 0)
                {
                    this->object().SetRight(leftestNode, node);
                    this->object().SetSize(leftestNode, (this->object().GetSize(nodeToDetachLeft) + this->object().GetSize(node)) + 1);
                }
                else
                {
                    this->object().SetSize(leftestNode, this->object().GetSize(nodeToDetachLeft) + 1);
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
            else if (this->object().GetLeft(*parent) == nodeToDetach)
            {
                this->object().SetLeft(*parent, replacementNode);
            }
            else if (this->object().GetRight(*parent) == nodeToDetach)
            {
                this->object().SetRight(*parent, replacementNode);
            }
            this->object().ClearNode(nodeToDetach);
        }

        private: void LeftMaintain(TElement* root)
        {
            if (*root != 0)
            {
                auto rootLeftNode = this->object().GetLeft(*root);
                if (rootLeftNode != 0)
                {
                    auto rootRightNode = this->object().GetRight(*root);
                    auto rootRightNodeSize = this->object().GetSize(rootRightNode);
                    auto rootLeftNodeLeftNode = this->object().GetLeft(rootLeftNode);
                    if (rootLeftNodeLeftNode != 0 &&
                        (rootRightNode == 0 || this->object().GetSize(rootLeftNodeLeftNode) > rootRightNodeSize))
                    {
                        this->object().RightRotate(root);
                    }
                    else
                    {
                        auto rootLeftNodeRightNode = this->object().GetRight(rootLeftNode);
                        if (rootLeftNodeRightNode != 0 &&
                            (rootRightNode == 0 || this->object().GetSize(rootLeftNodeRightNode) > rootRightNodeSize))
                        {
                            this->object().LeftRotate(this->object().GetLeftReference(*root));
                            this->object().RightRotate(root);
                        }
                        else
                        {
                            return;
                        }
                    }
                    this->object().LeftMaintain(this->object().GetLeftReference(*root));
                    this->object().RightMaintain(this->object().GetRightReference(*root));
                    this->object().LeftMaintain(root);
                    this->object().RightMaintain(root);
                }
            }
        }

        private: void RightMaintain(TElement* root)
        {
            if (*root != 0)
            {
                auto rootRightNode = this->object().GetRight(*root);
                if (rootRightNode != 0)
                {
                    auto rootLeftNode = this->object().GetLeft(*root);
                    auto rootLeftNodeSize = this->object().GetSize(rootLeftNode);
                    auto rootRightNodeRightNode = this->object().GetRight(rootRightNode);
                    if (rootRightNodeRightNode != 0 &&
                        (rootLeftNode == 0 || this->object().GetSize(rootRightNodeRightNode) > rootLeftNodeSize))
                    {
                        this->object().LeftRotate(root);
                    }
                    else
                    {
                        auto rootRightNodeLeftNode = this->object().GetLeft(rootRightNode);
                        if (rootRightNodeLeftNode != 0 &&
                            (rootLeftNode == 0 || this->object().GetSize(rootRightNodeLeftNode) > rootLeftNodeSize))
                        {
                            this->object().RightRotate(this->object().GetRightReference(*root));
                            this->object().LeftRotate(root);
                        }
                        else
                        {
                            return;
                        }
                    }
                    this->object().LeftMaintain(this->object().GetLeftReference(*root));
                    this->object().RightMaintain(this->object().GetRightReference(*root));
                    this->object().LeftMaintain(root);
                    this->object().RightMaintain(root);
                }
            }
        }
    };
}
