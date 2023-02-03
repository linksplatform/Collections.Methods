namespace Platform::Collections::Methods::Trees
{
    template <typename ...> class SizedAndThreadedAVLBalancedTreeMethods;
    template <typename TSelf, typename TElement> class SizedAndThreadedAVLBalancedTreeMethods<TSelf, TElement> : public SizedBinaryTreeMethodsBase<TSelf, TElement>
    {
        public: using base = SizedBinaryTreeMethodsBase<TSelf, TElement>;
        friend base;

        private: inline static const std::int32_t _maxPath = 11 * sizeof(TElement) + 4;

        protected: TElement GetRightest(TElement current)
        {
            auto currentRight = this->object().GetRightOrDefault(current);
            while (currentRight != 0)
            {
                current = currentRight;
                currentRight = this->object().GetRightOrDefault(current);
            }
            return current;
        }

        protected: TElement GetLeftest(TElement current)
        {
            auto currentLeft = this->object().GetLeftOrDefault(current);
            while (currentLeft != 0)
            {
                current = currentLeft;
                currentLeft = this->object().GetLeftOrDefault(current);
            }
            return current;
        }

        public: bool Contains(TElement node, TElement root)
        {
            while (root != 0)
            {
                if (this->object().FirstIsToTheLeftOfSecond(node, root))
                {
                    root = this->object().GetLeftOrDefault(root);
                }
                else if (this->object().FirstIsToTheRightOfSecond(node, root))
                {
                    root = this->object().GetRightOrDefault(root);
                }
                else
                {
                    return true;
                }
            }
            return false;
        }

        protected: void IncrementBalance(TElement node) { this->object().SetBalance(node, (std::int8_t)(this->object().GetBalance(node) + 1)); }

        protected: void DecrementBalance(TElement node) { this->object().SetBalance(node, (std::int8_t)(this->object().GetBalance(node) - 1)); }

        protected: TElement GetLeftOrDefault(TElement node) { return this->object().GetLeftIsChild(node) ? this->object().GetLeft(node) : 0; }

        protected: TElement GetRightOrDefault(TElement node) { return this->object().GetRightIsChild(node) ? this->object().GetRight(node) : 0; }

        protected: void AttachCore(TElement* root, TElement node)
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
                    if (this->object().FirstIsToTheLeftOfSecond(node, currentNode))
                    {
                        if (this->object().GetLeftIsChild(currentNode))
                        {
                            this->object().IncrementSize(currentNode);
                            path[pathPosition++] = currentNode;
                            currentNode = this->object().GetLeft(currentNode);
                        }
                        else
                        {
                            this->object().SetLeft(node, this->object().GetLeft(currentNode));
                            this->object().SetRight(node, currentNode);
                            this->object().SetLeft(currentNode, node);
                            this->object().SetLeftIsChild(currentNode, true);
                            this->object().DecrementBalance(currentNode);
                            this->object().SetSize(node, 1);
                            this->object().FixSize(currentNode);
                            break;
                        }
                    }
                    else if (this->object().FirstIsToTheRightOfSecond(node, currentNode))
                    {
                        if (this->object().GetRightIsChild(currentNode))
                        {
                            this->object().IncrementSize(currentNode);
                            path[pathPosition++] = currentNode;
                            currentNode = this->object().GetRight(currentNode);
                        }
                        else
                        {
                            this->object().SetRight(node, this->object().GetRight(currentNode));
                            this->object().SetLeft(node, currentNode);
                            this->object().SetRight(currentNode, node);
                            this->object().SetRightIsChild(currentNode, true);
                            this->object().IncrementBalance(currentNode);
                            this->object().SetSize(node, 1);
                            this->object().FixSize(currentNode);
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
                    auto isLeftNode = parent != 0 && currentNode == this->object().GetLeft(parent);
                    auto currentNodeBalance = this->object().GetBalance(currentNode);
                    if (currentNodeBalance < -1 || currentNodeBalance > 1)
                    {
                        currentNode = this->object().Balance(currentNode);
                        if (parent == 0)
                        {
                            *root = currentNode;
                        }
                        else if (isLeftNode)
                        {
                            this->object().SetLeft(parent, currentNode);
                            this->object().FixSize(parent);
                        }
                        else
                        {
                            this->object().SetRight(parent, currentNode);
                            this->object().FixSize(parent);
                        }
                    }
                    currentNodeBalance = this->object().GetBalance(currentNode);
                    if (currentNodeBalance == 0 || parent == 0)
                    {
                        break;
                    }
                    if (isLeftNode)
                    {
                        this->object().DecrementBalance(parent);
                    }
                    else
                    {
                        this->object().IncrementBalance(parent);
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
                auto rootBalance = this->object().GetBalance(node);
                if (rootBalance < -1)
                {
                    auto left = this->object().GetLeft(node);
                    if (this->object().GetBalance(left) > 0)
                    {
                        this->object().SetLeft(node, this->object().LeftRotateWithBalance(left));
                        this->object().FixSize(node);
                    }
                    node = this->object().RightRotateWithBalance(node);
                }
                else if (rootBalance > 1)
                {
                    auto right = this->object().GetRight(node);
                    if (this->object().GetBalance(right) < 0)
                    {
                        this->object().SetRight(node, this->object().RightRotateWithBalance(right));
                        this->object().FixSize(node);
                    }
                    node = this->object().LeftRotateWithBalance(node);
                }
                return node;
            }
        }

        protected: TElement LeftRotateWithBalance(TElement node)
        {
            {
                auto right = this->object().GetRight(node);
                if (this->object().GetLeftIsChild(right))
                {
                    this->object().SetRight(node, this->object().GetLeft(right));
                }
                else
                {
                    this->object().SetRightIsChild(node, false);
                    this->object().SetLeftIsChild(right, true);
                }
                this->object().SetLeft(right, node);
                this->object().SetSize(right, this->object().GetSize(node));
                this->object().FixSize(node);
                auto rootBalance = this->object().GetBalance(node);
                auto rightBalance = this->object().GetBalance(right);
                if (rightBalance <= 0)
                {
                    if (rootBalance >= 1)
                    {
                        this->object().SetBalance(right, (std::int8_t)(rightBalance - 1));
                    }
                    else
                    {
                        this->object().SetBalance(right, (std::int8_t)(rootBalance + rightBalance - 2));
                    }
                    this->object().SetBalance(node, (std::int8_t)(rootBalance - 1));
                }
                else
                {
                    if (rootBalance <= rightBalance)
                    {
                        this->object().SetBalance(right, (std::int8_t)(rootBalance - 2));
                    }
                    else
                    {
                        this->object().SetBalance(right, (std::int8_t)(rightBalance - 1));
                    }
                    this->object().SetBalance(node, (std::int8_t)(rootBalance - rightBalance - 1));
                }
                return right;
            }
        }

        protected: TElement RightRotateWithBalance(TElement node)
        {
            {
                auto left = this->object().GetLeft(node);
                if (this->object().GetRightIsChild(left))
                {
                    this->object().SetLeft(node, this->object().GetRight(left));
                }
                else
                {
                    this->object().SetLeftIsChild(node, false);
                    this->object().SetRightIsChild(left, true);
                }
                this->object().SetRight(left, node);
                this->object().SetSize(left, this->object().GetSize(node));
                this->object().FixSize(node);
                auto rootBalance = this->object().GetBalance(node);
                auto leftBalance = this->object().GetBalance(left);
                if (leftBalance <= 0)
                {
                    if (leftBalance > rootBalance)
                    {
                        this->object().SetBalance(left, (std::int8_t)(leftBalance + 1));
                    }
                    else
                    {
                        this->object().SetBalance(left, (std::int8_t)(rootBalance + 2));
                    }
                    this->object().SetBalance(node, (std::int8_t)(rootBalance - leftBalance + 1));
                }
                else
                {
                    if (rootBalance <= -1)
                    {
                        this->object().SetBalance(left, (std::int8_t)(leftBalance + 1));
                    }
                    else
                    {
                        this->object().SetBalance(left, (std::int8_t)(rootBalance + leftBalance + 2));
                    }
                    this->object().SetBalance(node, (std::int8_t)(rootBalance + 1));
                }
                return left;
            }
        }

        protected: TElement GetNext(TElement node)
        {
            auto current = this->object().GetRight(node);
            if (this->object().GetRightIsChild(node))
            {
                return this->object().GetLeftest(current);
            }
            return current;
        }

        protected: TElement GetPrevious(TElement node)
        {
            auto current = this->object().GetLeft(node);
            if (this->object().GetLeftIsChild(node))
            {
                return this->object().GetRightest(current);
            }
            return current;
        }

        protected: void DetachCore(TElement* root, TElement node)
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
                    if (this->object().FirstIsToTheLeftOfSecond(node, currentNode))
                    {
                        if (!this->object().GetLeftIsChild(currentNode))
                        {
                            throw std::runtime_error("Cannot find a node.");
                        }
                        this->object().DecrementSize(currentNode);
                        path[pathPosition++] = currentNode;
                        currentNode = this->object().GetLeft(currentNode);
                    }
                    else if (this->object().FirstIsToTheRightOfSecond(node, currentNode))
                    {
                        if (!this->object().GetRightIsChild(currentNode))
                        {
                            throw std::runtime_error("Cannot find a node.");
                        }
                        this->object().DecrementSize(currentNode);
                        path[pathPosition++] = currentNode;
                        currentNode = this->object().GetRight(currentNode);
                    }
                    else
                    {
                        break;
                    }
                }
                auto parent = path[--pathPosition];
                auto balanceNode = parent;
                auto isLeftNode = parent != 0 && currentNode == this->object().GetLeft(parent);
                if (!this->object().GetLeftIsChild(currentNode))
                {
                    if (!this->object().GetRightIsChild(currentNode))
                    {
                        if (parent == 0)
                        {
                            *root = 0;
                        }
                        else if (isLeftNode)
                        {
                            this->object().SetLeftIsChild(parent, false);
                            this->object().SetLeft(parent, this->object().GetLeft(currentNode));
                            this->object().IncrementBalance(parent);
                        }
                        else
                        {
                            this->object().SetRightIsChild(parent, false);
                            this->object().SetRight(parent, this->object().GetRight(currentNode));
                            this->object().DecrementBalance(parent);
                        }
                    }
                    else
                    {
                        auto successor = this->object().GetNext(currentNode);
                        this->object().SetLeft(successor, this->object().GetLeft(currentNode));
                        auto right = this->object().GetRight(currentNode);
                        if (parent == 0)
                        {
                            *root = right;
                        }
                        else if (isLeftNode)
                        {
                            this->object().SetLeft(parent, right);
                            this->object().IncrementBalance(parent);
                        }
                        else
                        {
                            this->object().SetRight(parent, right);
                            this->object().DecrementBalance(parent);
                        }
                    }
                }
                else
                {
                    if (!this->object().GetRightIsChild(currentNode))
                    {
                        auto predecessor = this->object().GetPrevious(currentNode);
                        this->object().SetRight(predecessor, this->object().GetRight(currentNode));
                        auto leftValue = this->object().GetLeft(currentNode);
                        if (parent == 0)
                        {
                            *root = leftValue;
                        }
                        else if (isLeftNode)
                        {
                            this->object().SetLeft(parent, leftValue);
                            this->object().IncrementBalance(parent);
                        }
                        else
                        {
                            this->object().SetRight(parent, leftValue);
                            this->object().DecrementBalance(parent);
                        }
                    }
                    else
                    {
                        auto predecessor = this->object().GetLeft(currentNode);
                        auto successor = this->object().GetRight(currentNode);
                        auto successorParent = currentNode;
                        std::int32_t previousPathPosition = ++pathPosition;
                        while (this->object().GetLeftIsChild(successor))
                        {
                            path[++pathPosition] = successorParent = successor;
                            successor = this->object().GetLeft(successor);
                            if (successorParent != currentNode)
                            {
                                this->object().DecrementSize(successorParent);
                            }
                        }
                        path[previousPathPosition] = successor;
                        balanceNode = path[pathPosition];
                        if (successorParent != currentNode)
                        {
                            if (!this->object().GetRightIsChild(successor))
                            {
                                this->object().SetLeftIsChild(successorParent, false);
                            }
                            else
                            {
                                this->object().SetLeft(successorParent, this->object().GetRight(successor));
                            }
                            this->object().IncrementBalance(successorParent);
                            this->object().SetRightIsChild(successor, true);
                            this->object().SetRight(successor, this->object().GetRight(currentNode));
                        }
                        else
                        {
                            this->object().DecrementBalance(currentNode);
                        }
                        while (this->object().GetRightIsChild(predecessor))
                        {
                            predecessor = this->object().GetRight(predecessor);
                        }
                        this->object().SetRight(predecessor, successor);
                        auto left = this->object().GetLeft(currentNode);
                        this->object().SetLeftIsChild(successor, true);
                        this->object().SetLeft(successor, left);
                        this->object().SetBalance(successor, this->object().GetBalance(currentNode));
                        this->object().FixSize(successor);
                        if (parent == 0)
                        {
                            *root = successor;
                        }
                        else if (isLeftNode)
                        {
                            this->object().SetLeft(parent, successor);
                        }
                        else
                        {
                            this->object().SetRight(parent, successor);
                        }
                    }
                }
                if (balanceNode != 0)
                {
                    while (true)
                    {
                        auto balanceParent = path[--pathPosition];
                        isLeftNode = balanceParent != 0 && balanceNode == this->object().GetLeft(balanceParent);
                        auto currentNodeBalance = this->object().GetBalance(balanceNode);
                        if (currentNodeBalance < -1 || currentNodeBalance > 1)
                        {
                            balanceNode = this->object().Balance(balanceNode);
                            if (balanceParent == 0)
                            {
                                *root = balanceNode;
                            }
                            else if (isLeftNode)
                            {
                                this->object().SetLeft(balanceParent, balanceNode);
                            }
                            else
                            {
                                this->object().SetRight(balanceParent, balanceNode);
                            }
                        }
                        currentNodeBalance = this->object().GetBalance(balanceNode);
                        if (currentNodeBalance != 0 || balanceParent == 0)
                        {
                            break;
                        }
                        if (isLeftNode)
                        {
                            this->object().IncrementBalance(balanceParent);
                        }
                        else
                        {
                            this->object().DecrementBalance(balanceParent);
                        }
                        balanceNode = balanceParent;
                    }
                }
                this->object().ClearNode(node);
#if USEARRAYPOOL
                ArrayPool.Free(path);
#endif
            }
        }

        protected: void ClearNode(TElement node)
        {
            this->object().SetLeft(node, 0);
            this->object().SetRight(node, 0);
            this->object().SetSize(node, 0);
            this->object().SetLeftIsChild(node, false);
            this->object().SetRightIsChild(node, false);
            this->object().SetBalance(node, 0);
        }
    };
}
