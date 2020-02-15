namespace Platform::Collections::Methods::Trees
{
    template <typename TElement> class SizedAndThreadedAVLBalancedTreeMethods : public SizedBinaryTreeMethodsBase<TElement>
    {
        private: static const std::int32_t _maxPath = 11 * sizeof(TElement) + 4;

        protected: TElement GetRightest(TElement current) override
        {
            auto currentRight = this->GetRightOrDefault(current);
            while (currentRight != 0)
            {
                current = currentRight;
                currentRight = this->GetRightOrDefault(current);
            }
            return current;
        }

        protected: TElement GetLeftest(TElement current) override
        {
            auto currentLeft = this->GetLeftOrDefault(current);
            while (currentLeft != 0)
            {
                current = currentLeft;
                currentLeft = this->GetLeftOrDefault(current);
            }
            return current;
        }

        public: bool Contains(TElement node, TElement root) override
        {
            while (root != 0)
            {
                if (this->FirstIsToTheLeftOfSecond(node, root))
                {
                    root = this->GetLeftOrDefault(root);
                }
                else if (this->FirstIsToTheRightOfSecond(node, root))
                {
                    root = this->GetRightOrDefault(root);
                }
                else
                {
                    return true;
                }
            }
            return false;
        }

        protected: void IncrementBalance(TElement node) { this->SetBalance(node, (std::int8_t)(this->GetBalance(node) + 1)); }

        protected: void DecrementBalance(TElement node) { this->SetBalance(node, (std::int8_t)(this->GetBalance(node) - 1)); }

        protected: TElement GetLeftOrDefault(TElement node) override { return this->GetLeftIsChild(node) ? this->GetLeft(node) : 0; }

        protected: TElement GetRightOrDefault(TElement node) override { return this->GetRightIsChild(node) ? this->GetRight(node) : 0; }

        protected: virtual bool GetLeftIsChild(TElement node) = 0;

        protected: virtual void SetLeftIsChild(TElement node, bool value) = 0;

        protected: virtual bool GetRightIsChild(TElement node) = 0;

        protected: virtual void SetRightIsChild(TElement node, bool value) = 0;

        protected: virtual std::int8_t GetBalance(TElement node) = 0;

        protected: virtual void SetBalance(TElement node, std::int8_t value) = 0;

        protected: void AttachCore(TElement* root, TElement node) override
        {
            {
#if USEARRAYPOOL
                auto path = ArrayPool.Allocate<TElement>(MaxPath);
                auto pathPosition = 0;
                path[pathPosition++] = 0;
#else
                TElement path[_maxPath]; path[0] = 0;
                auto pathPosition = 1;
#endif
                auto currentNode = *root;
                while (true)
                {
                    if (this->FirstIsToTheLeftOfSecond(node, currentNode))
                    {
                        if (this->GetLeftIsChild(currentNode))
                        {
                            this->IncrementSize(currentNode);
                            path[pathPosition++] = currentNode;
                            currentNode = this->GetLeft(currentNode);
                        }
                        else
                        {
                            this->SetLeft(node, this->GetLeft(currentNode));
                            this->SetRight(node, currentNode);
                            this->SetLeft(currentNode, node);
                            this->SetLeftIsChild(currentNode, true);
                            this->DecrementBalance(currentNode);
                            this->SetSize(node, 1);
                            this->FixSize(currentNode);
                            break;
                        }
                    }
                    else if (this->FirstIsToTheRightOfSecond(node, currentNode))
                    {
                        if (this->GetRightIsChild(currentNode))
                        {
                            this->IncrementSize(currentNode);
                            path[pathPosition++] = currentNode;
                            currentNode = this->GetRight(currentNode);
                        }
                        else
                        {
                            this->SetRight(node, this->GetRight(currentNode));
                            this->SetLeft(node, currentNode);
                            this->SetRight(currentNode, node);
                            this->SetRightIsChild(currentNode, true);
                            this->IncrementBalance(currentNode);
                            this->SetSize(node, 1);
                            this->FixSize(currentNode);
                            break;
                        }
                    }
                    else
                    {
                        throw std::runtime_error("Node with the same key already attached to a tree.");
                    }
                }
                while (true)
                {
                    auto parent = path[--pathPosition];
                    auto isLeftNode = parent != 0 && currentNode == this->GetLeft(parent);
                    auto currentNodeBalance = this->GetBalance(currentNode);
                    if (currentNodeBalance < -1 || currentNodeBalance > 1)
                    {
                        currentNode = this->Balance(currentNode);
                        if (parent == 0)
                        {
                            *root = currentNode;
                        }
                        else if (isLeftNode)
                        {
                            this->SetLeft(parent, currentNode);
                            this->FixSize(parent);
                        }
                        else
                        {
                            this->SetRight(parent, currentNode);
                            this->FixSize(parent);
                        }
                    }
                    currentNodeBalance = this->GetBalance(currentNode);
                    if (currentNodeBalance == 0 || parent == 0)
                    {
                        break;
                    }
                    if (isLeftNode)
                    {
                        this->DecrementBalance(parent);
                    }
                    else
                    {
                        this->IncrementBalance(parent);
                    }
                    currentNode = parent;
                }
#if USEARRAYPOOL
                ArrayPool.Free(path);
#endif
            }
        }

        private: TElement Balance(TElement node)
        {
            {
                auto rootBalance = this->GetBalance(node);
                if (rootBalance < -1)
                {
                    auto left = this->GetLeft(node);
                    if (this->GetBalance(left) > 0)
                    {
                        this->SetLeft(node, this->LeftRotateWithBalance(left));
                        this->FixSize(node);
                    }
                    node = this->RightRotateWithBalance(node);
                }
                else if (rootBalance > 1)
                {
                    auto right = this->GetRight(node);
                    if (this->GetBalance(right) < 0)
                    {
                        this->SetRight(node, this->RightRotateWithBalance(right));
                        this->FixSize(node);
                    }
                    node = this->LeftRotateWithBalance(node);
                }
                return node;
            }
        }

        protected: TElement LeftRotateWithBalance(TElement node)
        {
            {
                auto right = this->GetRight(node);
                if (this->GetLeftIsChild(right))
                {
                    this->SetRight(node, this->GetLeft(right));
                }
                else
                {
                    this->SetRightIsChild(node, false);
                    this->SetLeftIsChild(right, true);
                }
                this->SetLeft(right, node);
                this->SetSize(right, this->GetSize(node));
                this->FixSize(node);
                auto rootBalance = this->GetBalance(node);
                auto rightBalance = this->GetBalance(right);
                if (rightBalance <= 0)
                {
                    if (rootBalance >= 1)
                    {
                        this->SetBalance(right, (std::int8_t)(rightBalance - 1));
                    }
                    else
                    {
                        this->SetBalance(right, (std::int8_t)(rootBalance + rightBalance - 2));
                    }
                    this->SetBalance(node, (std::int8_t)(rootBalance - 1));
                }
                else
                {
                    if (rootBalance <= rightBalance)
                    {
                        this->SetBalance(right, (std::int8_t)(rootBalance - 2));
                    }
                    else
                    {
                        this->SetBalance(right, (std::int8_t)(rightBalance - 1));
                    }
                    this->SetBalance(node, (std::int8_t)(rootBalance - rightBalance - 1));
                }
                return right;
            }
        }

        protected: TElement RightRotateWithBalance(TElement node)
        {
            {
                auto left = this->GetLeft(node);
                if (this->GetRightIsChild(left))
                {
                    this->SetLeft(node, this->GetRight(left));
                }
                else
                {
                    this->SetLeftIsChild(node, false);
                    this->SetRightIsChild(left, true);
                }
                this->SetRight(left, node);
                this->SetSize(left, this->GetSize(node));
                this->FixSize(node);
                auto rootBalance = this->GetBalance(node);
                auto leftBalance = this->GetBalance(left);
                if (leftBalance <= 0)
                {
                    if (leftBalance > rootBalance)
                    {
                        this->SetBalance(left, (std::int8_t)(leftBalance + 1));
                    }
                    else
                    {
                        this->SetBalance(left, (std::int8_t)(rootBalance + 2));
                    }
                    this->SetBalance(node, (std::int8_t)(rootBalance - leftBalance + 1));
                }
                else
                {
                    if (rootBalance <= -1)
                    {
                        this->SetBalance(left, (std::int8_t)(leftBalance + 1));
                    }
                    else
                    {
                        this->SetBalance(left, (std::int8_t)(rootBalance + leftBalance + 2));
                    }
                    this->SetBalance(node, (std::int8_t)(rootBalance + 1));
                }
                return left;
            }
        }

        protected: TElement GetNext(TElement node) override
        {
            auto current = this->GetRight(node);
            if (this->GetRightIsChild(node))
            {
                return this->GetLeftest(current);
            }
            return current;
        }

        protected: TElement GetPrevious(TElement node) override
        {
            auto current = this->GetLeft(node);
            if (this->GetLeftIsChild(node))
            {
                return this->GetRightest(current);
            }
            return current;
        }

        protected: void DetachCore(TElement* root, TElement node) override
        {
            {
#if USEARRAYPOOL
                auto path = ArrayPool.Allocate<TElement>(MaxPath);
                auto pathPosition = 0;
                path[pathPosition++] = 0;
#else
                TElement path[_maxPath]; path[0] = 0;
                auto pathPosition = 1;
#endif
                auto currentNode = *root;
                while (true)
                {
                    if (this->FirstIsToTheLeftOfSecond(node, currentNode))
                    {
                        if (!this->GetLeftIsChild(currentNode))
                        {
                            throw std::runtime_error("Cannot find a node.");
                        }
                        this->DecrementSize(currentNode);
                        path[pathPosition++] = currentNode;
                        currentNode = this->GetLeft(currentNode);
                    }
                    else if (this->FirstIsToTheRightOfSecond(node, currentNode))
                    {
                        if (!this->GetRightIsChild(currentNode))
                        {
                            throw std::runtime_error("Cannot find a node.");
                        }
                        this->DecrementSize(currentNode);
                        path[pathPosition++] = currentNode;
                        currentNode = this->GetRight(currentNode);
                    }
                    else
                    {
                        break;
                    }
                }
                auto parent = path[--pathPosition];
                auto balanceNode = parent;
                auto isLeftNode = parent != 0 && currentNode == this->GetLeft(parent);
                if (!this->GetLeftIsChild(currentNode))
                {
                    if (!this->GetRightIsChild(currentNode))
                    {
                        if (parent == 0)
                        {
                            *root = 0;
                        }
                        else if (isLeftNode)
                        {
                            this->SetLeftIsChild(parent, false);
                            this->SetLeft(parent, this->GetLeft(currentNode));
                            this->IncrementBalance(parent);
                        }
                        else
                        {
                            this->SetRightIsChild(parent, false);
                            this->SetRight(parent, this->GetRight(currentNode));
                            this->DecrementBalance(parent);
                        }
                    }
                    else
                    {
                        auto successor = this->GetNext(currentNode);
                        this->SetLeft(successor, this->GetLeft(currentNode));
                        auto right = this->GetRight(currentNode);
                        if (parent == 0)
                        {
                            *root = right;
                        }
                        else if (isLeftNode)
                        {
                            this->SetLeft(parent, right);
                            this->IncrementBalance(parent);
                        }
                        else
                        {
                            this->SetRight(parent, right);
                            this->DecrementBalance(parent);
                        }
                    }
                }
                else
                {
                    if (!this->GetRightIsChild(currentNode))
                    {
                        auto predecessor = this->GetPrevious(currentNode);
                        this->SetRight(predecessor, this->GetRight(currentNode));
                        auto leftValue = this->GetLeft(currentNode);
                        if (parent == 0)
                        {
                            *root = leftValue;
                        }
                        else if (isLeftNode)
                        {
                            this->SetLeft(parent, leftValue);
                            this->IncrementBalance(parent);
                        }
                        else
                        {
                            this->SetRight(parent, leftValue);
                            this->DecrementBalance(parent);
                        }
                    }
                    else
                    {
                        auto predecessor = this->GetLeft(currentNode);
                        auto successor = this->GetRight(currentNode);
                        auto successorParent = currentNode;
                        std::int32_t previousPathPosition = ++pathPosition;
                        while (this->GetLeftIsChild(successor))
                        {
                            path[++pathPosition] = successorParent = successor;
                            successor = this->GetLeft(successor);
                            if (successorParent != currentNode)
                            {
                                this->DecrementSize(successorParent);
                            }
                        }
                        path[previousPathPosition] = successor;
                        balanceNode = path[pathPosition];
                        if (successorParent != currentNode)
                        {
                            if (!this->GetRightIsChild(successor))
                            {
                                this->SetLeftIsChild(successorParent, false);
                            }
                            else
                            {
                                this->SetLeft(successorParent, this->GetRight(successor));
                            }
                            this->IncrementBalance(successorParent);
                            this->SetRightIsChild(successor, true);
                            this->SetRight(successor, this->GetRight(currentNode));
                        }
                        else
                        {
                            this->DecrementBalance(currentNode);
                        }
                        while (this->GetRightIsChild(predecessor))
                        {
                            predecessor = this->GetRight(predecessor);
                        }
                        this->SetRight(predecessor, successor);
                        auto left = this->GetLeft(currentNode);
                        this->SetLeftIsChild(successor, true);
                        this->SetLeft(successor, left);
                        this->SetBalance(successor, this->GetBalance(currentNode));
                        this->FixSize(successor);
                        if (parent == 0)
                        {
                            *root = successor;
                        }
                        else if (isLeftNode)
                        {
                            this->SetLeft(parent, successor);
                        }
                        else
                        {
                            this->SetRight(parent, successor);
                        }
                    }
                }
                if (balanceNode != 0)
                {
                    while (true)
                    {
                        auto balanceParent = path[--pathPosition];
                        isLeftNode = balanceParent != 0 && balanceNode == this->GetLeft(balanceParent);
                        auto currentNodeBalance = this->GetBalance(balanceNode);
                        if (currentNodeBalance < -1 || currentNodeBalance > 1)
                        {
                            balanceNode = this->Balance(balanceNode);
                            if (balanceParent == 0)
                            {
                                *root = balanceNode;
                            }
                            else if (isLeftNode)
                            {
                                this->SetLeft(balanceParent, balanceNode);
                            }
                            else
                            {
                                this->SetRight(balanceParent, balanceNode);
                            }
                        }
                        currentNodeBalance = this->GetBalance(balanceNode);
                        if (currentNodeBalance != 0 || balanceParent == 0)
                        {
                            break;
                        }
                        if (isLeftNode)
                        {
                            this->IncrementBalance(balanceParent);
                        }
                        else
                        {
                            this->DecrementBalance(balanceParent);
                        }
                        balanceNode = balanceParent;
                    }
                }
                this->ClearNode(node);
#if USEARRAYPOOL
                ArrayPool.Free(path);
#endif
            }
        }

        protected: void ClearNode(TElement node) override
        {
            this->SetLeft(node, 0);
            this->SetRight(node, 0);
            this->SetSize(node, 0);
            this->SetLeftIsChild(node, false);
            this->SetRightIsChild(node, false);
            this->SetBalance(node, 0);
        }
    };
}