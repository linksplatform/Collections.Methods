namespace Platform::Collections::Methods::Trees
{
    template <typename TElement> class SizeBalancedTreeMethods2 : public SizedBinaryTreeMethodsBase<TElement>
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
                this->SetSize(*currentNode, this->GetSize(*currentNode) - 1);
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
                    throw std::exception("Duplicate link found in the tree.");
                }
            }
            auto nodeToDetachLeft = this->GetLeft(nodeToDetach);
            auto node = this->GetRight(nodeToDetach);
            if (nodeToDetachLeft != 0 && node != 0)
            {
                auto minNode = node;
                auto minNodeLeft = this->GetLeft(minNode);
                while (minNodeLeft != 0)
                {
                    minNode = minNodeLeft;
                    minNodeLeft = this->GetLeft(minNode);
                }
                this->DetachCore(this->GetRightReference(nodeToDetach), minNode);
                this->SetLeft(minNode, nodeToDetachLeft);
                node = this->GetRight(nodeToDetach);
                if (node != 0)
                {
                    this->SetRight(minNode, node);
                    this->SetSize(minNode, (this->GetSize(nodeToDetachLeft) + this->GetSize(node)) + 1);
                }
                else
                {
                    this->SetSize(minNode, this->GetSize(nodeToDetachLeft) + 1);
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
                    auto rootLeftNodeLeftNode = this->GetLeft(rootLeftNode);
                    if (rootLeftNodeLeftNode != 0 &&
                        (rootRightNode == 0 || this->GetSize(rootLeftNodeLeftNode) > this->GetSize(rootRightNode)))
                    {
                        this->RightRotate(root);
                    }
                    else
                    {
                        auto rootLeftNodeRightNode = this->GetRight(rootLeftNode);
                        if (rootLeftNodeRightNode != 0 &&
                            (rootRightNode == 0 || this->GetSize(rootLeftNodeRightNode) > this->GetSize(rootRightNode)))
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
                    auto rootRightNodeRightNode = this->GetRight(rootRightNode);
                    if (rootRightNodeRightNode != 0 &&
                        (rootLeftNode == 0 || this->GetSize(rootRightNodeRightNode) > this->GetSize(rootLeftNode)))
                    {
                        this->LeftRotate(root);
                    }
                    else
                    {
                        auto rootRightNodeLeftNode = this->GetLeft(rootRightNode);
                        if (rootRightNodeLeftNode != 0 &&
                            (rootLeftNode == 0 || this->GetSize(rootRightNodeLeftNode) > this->GetSize(rootLeftNode)))
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