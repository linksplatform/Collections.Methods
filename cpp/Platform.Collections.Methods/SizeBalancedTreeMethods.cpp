namespace Platform::Collections::Methods::Trees
{
    template <typename TElement> class SizeBalancedTreeMethods : public SizedBinaryTreeMethodsBase<TElement>
    {
    public:
        void AttachCore(TElement* root, TElement node) override
        {
            if (*root == 0)
            {
                *root = node;
                this->IncrementSize(*root);
            }
            else
            {
                this->IncrementSize(*root);
                if (this->FirstIsToTheLeftOfSecond(node, *root))
                {
                    this->AttachCore(this->GetLeftReference(*root), node);
                    this->LeftMaintain(root);
                }
                else
                {
                    this->AttachCore(this->GetRightReference(*root), node);
                    this->RightMaintain(root);
                }
            }
        }

        void DetachCore(TElement* root, TElement nodeToDetach) override
        {
            auto* currentNode = root;
            auto* parent = root;
            auto replacementNode = 0;
            while (*currentNode != nodeToDetach)
            {
                this->DecrementSize(*currentNode);
                if (this->FirstIsToTheLeftOfSecond(nodeToDetach, *currentNode))
                {
                    parent = currentNode;
                    currentNode = this->GetLeftReference(*currentNode);
                }
                else if (this->FirstIsToTheRightOfSecond(nodeToDetach, *currentNode))
                {
                    parent = currentNode;
                    currentNode = this->GetRightReference(*currentNode);
                }
                else
                {
                    throw std::runtime_error("Duplicate link found in the tree.");
                }
            }
            auto nodeToDetachLeft = this->GetLeft(nodeToDetach);
            auto node = this->GetRight(nodeToDetach);
            if (nodeToDetachLeft != 0 && node != 0)
            {
                auto leftestNode = this->GetLeftest(node);
                this->DetachCore(this->GetRightReference(nodeToDetach), leftestNode);
                this->SetLeft(leftestNode, nodeToDetachLeft);
                node = this->GetRight(nodeToDetach);
                if (node != 0)
                {
                    this->SetRight(leftestNode, node);
                    this->SetSize(leftestNode, (this->GetSize(nodeToDetachLeft) + this->GetSize(node)) + 1);
                }
                else
                {
                    this->SetSize(leftestNode, this->GetSize(nodeToDetachLeft) + 1);
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
            else if (this->GetLeft(*parent) == nodeToDetach)
            {
                this->SetLeft(*parent, replacementNode);
            }
            else if (this->GetRight(*parent) == nodeToDetach)
            {
                this->SetRight(*parent, replacementNode);
            }
            this->ClearNode(nodeToDetach);
        }

        void LeftMaintain(TElement* root)
        {
            if (*root != 0)
            {
                auto rootLeftNode = this->GetLeft(*root);
                if (rootLeftNode != 0)
                {
                    auto rootRightNode = this->GetRight(*root);
                    auto rootRightNodeSize = this->GetSize(rootRightNode);
                    auto rootLeftNodeLeftNode = this->GetLeft(rootLeftNode);
                    if (rootLeftNodeLeftNode != 0 &&
                        (rootRightNode == 0 || this->GetSize(rootLeftNodeLeftNode) > rootRightNodeSize))
                    {
                        this->RightRotate(root);
                    }
                    else
                    {
                        auto rootLeftNodeRightNode = this->GetRight(rootLeftNode);
                        if (rootLeftNodeRightNode != 0 &&
                            (rootRightNode == 0 || this->GetSize(rootLeftNodeRightNode) > rootRightNodeSize))
                        {
                            this->LeftRotate(this->GetLeftReference(*root));
                            this->RightRotate(root);
                        }
                        else
                        {
                            return;
                        }
                    }
                    this->LeftMaintain(this->GetLeftReference(*root));
                    this->RightMaintain(this->GetRightReference(*root));
                    this->LeftMaintain(root);
                    this->RightMaintain(root);
                }
            }
        }

        void RightMaintain(TElement* root)
        {
            if (*root != 0)
            {
                auto rootRightNode = this->GetRight(*root);
                if (rootRightNode != 0)
                {
                    auto rootLeftNode = this->GetLeft(*root);
                    auto rootLeftNodeSize = this->GetSize(rootLeftNode);
                    auto rootRightNodeRightNode = this->GetRight(rootRightNode);
                    if (rootRightNodeRightNode != 0 &&
                        (rootLeftNode == 0 || this->GetSize(rootRightNodeRightNode) > rootLeftNodeSize))
                    {
                        this->LeftRotate(root);
                    }
                    else
                    {
                        auto rootRightNodeLeftNode = this->GetLeft(rootRightNode);
                        if (rootRightNodeLeftNode != 0 &&
                            (rootLeftNode == 0 || this->GetSize(rootRightNodeLeftNode) > rootLeftNodeSize))
                        {
                            this->RightRotate(this->GetRightReference(*root));
                            this->LeftRotate(root);
                        }
                        else
                        {
                            return;
                        }
                    }
                    this->LeftMaintain(this->GetLeftReference(*root));
                    this->RightMaintain(this->GetRightReference(*root));
                    this->LeftMaintain(root);
                    this->RightMaintain(root);
                }
            }
        }
    };
}