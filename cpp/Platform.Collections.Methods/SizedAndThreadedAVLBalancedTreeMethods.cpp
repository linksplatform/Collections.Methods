namespace Platform::Collections::Methods::Trees
{
    template <typename TElement> class SizedAndThreadedAVLBalancedTreeMethods : public SizedBinaryTreeMethodsBase<TElement>
    {
    public:
        static const int MaxPath = 92;

        bool Contains(TElement node, TElement root) override
        {
            while (root != 0)
            {
                if (this->FirstIsToTheLeftOfSecond(node, root))
                {
                    root = GetLeftOrDefault(root);
                }
                else if (this->FirstIsToTheRightOfSecond(node, root))
                {
                    root = GetRightOrDefault(root);
                }
                else
                {
                    return true;
                }
            }
            return false;
        }

        void IncrementBalance(TElement node) { SetBalance(node, (std::int8_t)(GetBalance(node) + 1)); }

        void DecrementBalance(TElement node) { SetBalance(node, (std::int8_t)(GetBalance(node) - 1)); }

        TElement GetLeftOrDefault(TElement node) override { return GetLeftIsChild(node) ? this->GetLeft(node) : 0; }

        TElement GetRightOrDefault(TElement node) override { return GetRightIsChild(node) ? this->GetRight(node) : 0; }

        virtual bool GetLeftIsChild(TElement node) = 0;

        virtual void SetLeftIsChild(TElement node, bool value) = 0;

        virtual bool GetRightIsChild(TElement node) = 0;

        virtual void SetRightIsChild(TElement node, bool value) = 0;

        virtual std::int8_t GetBalance(TElement node) = 0;

        virtual void SetBalance(TElement node, std::int8_t value) = 0;

        void AttachCore(TElement* root, TElement node) override
        {
            {
#if USEARRAYPOOL
                auto path = ArrayPool.Allocate<TElement>(MaxPath);
                auto pathPosition = 0;
                path[pathPosition++] = 0;
#else
                TElement path[MaxPath] = { {0} };
                auto pathPosition = 1;
#endif
                auto currentNode = *root;
                while (true)
                {
                    if (this->FirstIsToTheLeftOfSecond(node, currentNode))
                    {
                        if (GetLeftIsChild(currentNode))
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
                            SetLeftIsChild(currentNode, true);
                            DecrementBalance(currentNode);
                            this->SetSize(node, 1);
                            this->FixSize(currentNode);
                            break;
                        }
                    }
                    else if (this->FirstIsToTheRightOfSecond(node, currentNode))
                    {
                        if (GetRightIsChild(currentNode))
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
                            SetRightIsChild(currentNode, true);
                            IncrementBalance(currentNode);
                            this->SetSize(node, 1);
                            this->FixSize(currentNode);
                            break;
                        }
                    }
                    else
                    {
                        throw std::exception("Node with the same key already attached to a tree.");
                    }
                }
                while (true)
                {
                    auto parent = path[--pathPosition];
                    auto isLeftNode = parent != 0 && currentNode == this->GetLeft(parent);
                    auto currentNodeBalance = GetBalance(currentNode);
                    if (currentNodeBalance < -1 || currentNodeBalance > 1)
                    {
                        currentNode = Balance(currentNode);
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
                    currentNodeBalance = GetBalance(currentNode);
                    if (currentNodeBalance == 0 || parent == 0)
                    {
                        break;
                    }
                    if (isLeftNode)
                    {
                        DecrementBalance(parent);
                    }
                    else
                    {
                        IncrementBalance(parent);
                    }
                    currentNode = parent;
                }
#if USEARRAYPOOL
                ArrayPool.Free(path);
#endif
            }
        }

        TElement Balance(TElement node)
        {
            {
                auto rootBalance = GetBalance(node);
                if (rootBalance < -1)
                {
                    auto left = this->GetLeft(node);
                    if (GetBalance(left) > 0)
                    {
                        this->SetLeft(node, LeftRotateWithBalance(left));
                        this->FixSize(node);
                    }
                    node = RightRotateWithBalance(node);
                }
                else if (rootBalance > 1)
                {
                    auto right = this->GetRight(node);
                    if (GetBalance(right) < 0)
                    {
                        this->SetRight(node, RightRotateWithBalance(right));
                        this->FixSize(node);
                    }
                    node = LeftRotateWithBalance(node);
                }
                return node;
            }
        }

        TElement LeftRotateWithBalance(TElement node)
        {
            {
                auto right = this->GetRight(node);
                if (GetLeftIsChild(right))
                {
                    this->SetRight(node, this->GetLeft(right));
                }
                else
                {
                    SetRightIsChild(node, false);
                    SetLeftIsChild(right, true);
                }
                this->SetLeft(right, node);
                this->SetSize(right, this->GetSize(node));
                this->FixSize(node);
                auto rootBalance = GetBalance(node);
                auto rightBalance = GetBalance(right);
                if (rightBalance <= 0)
                {
                    if (rootBalance >= 1)
                    {
                        SetBalance(right, (std::int8_t)(rightBalance - 1));
                    }
                    else
                    {
                        SetBalance(right, (std::int8_t)(rootBalance + rightBalance - 2));
                    }
                    SetBalance(node, (std::int8_t)(rootBalance - 1));
                }
                else
                {
                    if (rootBalance <= rightBalance)
                    {
                        SetBalance(right, (std::int8_t)(rootBalance - 2));
                    }
                    else
                    {
                        SetBalance(right, (std::int8_t)(rightBalance - 1));
                    }
                    SetBalance(node, (std::int8_t)(rootBalance - rightBalance - 1));
                }
                return right;
            }
        }

        TElement RightRotateWithBalance(TElement node)
        {
            {
                auto left = this->GetLeft(node);
                if (GetRightIsChild(left))
                {
                    this->SetLeft(node, this->GetRight(left));
                }
                else
                {
                    SetLeftIsChild(node, false);
                    SetRightIsChild(left, true);
                }
                this->SetRight(left, node);
                this->SetSize(left, this->GetSize(node));
                this->FixSize(node);
                auto rootBalance = GetBalance(node);
                auto leftBalance = GetBalance(left);
                if (leftBalance <= 0)
                {
                    if (leftBalance > rootBalance)
                    {
                        SetBalance(left, (std::int8_t)(leftBalance + 1));
                    }
                    else
                    {
                        SetBalance(left, (std::int8_t)(rootBalance + 2));
                    }
                    SetBalance(node, (std::int8_t)(rootBalance - leftBalance + 1));
                }
                else
                {
                    if (rootBalance <= -1)
                    {
                        SetBalance(left, (std::int8_t)(leftBalance + 1));
                    }
                    else
                    {
                        SetBalance(left, (std::int8_t)(rootBalance + leftBalance + 2));
                    }
                    SetBalance(node, (std::int8_t)(rootBalance + 1));
                }
                return left;
            }
        }

        TElement GetNext(TElement node)
        {
            {
                auto current = this->GetRight(node);
                if (GetRightIsChild(node))
                {
                    while (GetLeftIsChild(current))
                    {
                        current = this->GetLeft(current);
                    }
                }
                return current;
            }
        }

        TElement GetPrevious(TElement node)
        {
            {
                auto current = this->GetLeft(node);
                if (GetLeftIsChild(node))
                {
                    while (GetRightIsChild(current))
                    {
                        current = this->GetRight(current);
                    }
                }
                return current;
            }
        }

        void DetachCore(TElement* root, TElement node) override
        {
            {
#if USEARRAYPOOL
                auto path = ArrayPool.Allocate<TElement>(MaxPath);
                auto pathPosition = 0;
                path[pathPosition++] = 0;
#else
                TElement path[MaxPath] = { {0} };
                auto pathPosition = 1;
#endif
                auto currentNode = *root;
                while (true)
                {
                    if (this->FirstIsToTheLeftOfSecond(node, currentNode))
                    {
                        if (!GetLeftIsChild(currentNode))
                        {
                            throw std::exception("Cannot find a node.");
                        }
                        this->DecrementSize(currentNode);
                        path[pathPosition++] = currentNode;
                        currentNode = this->GetLeft(currentNode);
                    }
                    else if (this->FirstIsToTheRightOfSecond(node, currentNode))
                    {
                        if (!GetRightIsChild(currentNode))
                        {
                            throw std::exception("Cannot find a node.");
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
                if (!GetLeftIsChild(currentNode))
                {
                    if (!GetRightIsChild(currentNode))
                    {
                        if (parent == 0)
                        {
                            *root = 0;
                        }
                        else if (isLeftNode)
                        {
                            SetLeftIsChild(parent, false);
                            this->SetLeft(parent, this->GetLeft(currentNode));
                            IncrementBalance(parent);
                        }
                        else
                        {
                            SetRightIsChild(parent, false);
                            this->SetRight(parent, this->GetRight(currentNode));
                            DecrementBalance(parent);
                        }
                    }
                    else
                    {
                        auto successor = GetNext(currentNode);
                        this->SetLeft(successor, this->GetLeft(currentNode));
                        auto right = this->GetRight(currentNode);
                        if (parent == 0)
                        {
                            *root = right;
                        }
                        else if (isLeftNode)
                        {
                            this->SetLeft(parent, right);
                            IncrementBalance(parent);
                        }
                        else
                        {
                            this->SetRight(parent, right);
                            DecrementBalance(parent);
                        }
                    }
                }
                else
                {
                    if (!GetRightIsChild(currentNode))
                    {
                        auto predecessor = GetPrevious(currentNode);
                        this->SetRight(predecessor, this->GetRight(currentNode));
                        auto leftValue = this->GetLeft(currentNode);
                        if (parent == 0)
                        {
                            *root = leftValue;
                        }
                        else if (isLeftNode)
                        {
                            this->SetLeft(parent, leftValue);
                            IncrementBalance(parent);
                        }
                        else
                        {
                            this->SetRight(parent, leftValue);
                            DecrementBalance(parent);
                        }
                    }
                    else
                    {
                        auto predecessor = this->GetLeft(currentNode);
                        auto successor = this->GetRight(currentNode);
                        auto successorParent = currentNode;
                        int previousPathPosition = ++pathPosition;
                        while (GetLeftIsChild(successor))
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
                            if (!GetRightIsChild(successor))
                            {
                                SetLeftIsChild(successorParent, false);
                            }
                            else
                            {
                                this->SetLeft(successorParent, this->GetRight(successor));
                            }
                            IncrementBalance(successorParent);
                            SetRightIsChild(successor, true);
                            this->SetRight(successor, this->GetRight(currentNode));
                        }
                        else
                        {
                            DecrementBalance(currentNode);
                        }
                        while (GetRightIsChild(predecessor))
                        {
                            predecessor = this->GetRight(predecessor);
                        }
                        this->SetRight(predecessor, successor);
                        auto left = this->GetLeft(currentNode);
                        SetLeftIsChild(successor, true);
                        this->SetLeft(successor, left);
                        SetBalance(successor, GetBalance(currentNode));
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
                        auto currentNodeBalance = GetBalance(balanceNode);
                        if (currentNodeBalance < -1 || currentNodeBalance > 1)
                        {
                            balanceNode = Balance(balanceNode);
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
                        currentNodeBalance = GetBalance(balanceNode);
                        if (currentNodeBalance != 0 || balanceParent == 0)
                        {
                            break;
                        }
                        if (isLeftNode)
                        {
                            IncrementBalance(balanceParent);
                        }
                        else
                        {
                            DecrementBalance(balanceParent);
                        }
                        balanceNode = balanceParent;
                    }
                }
                ClearNode(node);
#if USEARRAYPOOL
                ArrayPool.Free(path);
#endif
            }
        }

        void ClearNode(TElement node) override
        {
            this->SetLeft(node, 0);
            this->SetRight(node, 0);
            this->SetSize(node, 0);
            SetLeftIsChild(node, false);
            SetRightIsChild(node, false);
            SetBalance(node, 0);
        }
    };
}