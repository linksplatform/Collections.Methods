namespace Platform::Collections::Methods::Trees
{
    template <typename ...> class SizedAndThreadedAVLBalancedTreeMethods;
    template <typename impl_t, typename TElement> class SizedAndThreadedAVLBalancedTreeMethods<impl_t, TElement> : public SizedBinaryTreeMethodsBase<impl_t, TElement>
    {
        public: using base_t = SizedBinaryTreeMethodsBase<impl_t, TElement>;
        friend base_t;

        private: inline static const std::int32_t _maxPath = 11 * sizeof(TElement) + 4;

        protected: TElement GetRightest(TElement current)
        {
            auto currentRight = static_cast<impl_t*>(this)->GetRightOrDefault(current);
            while (currentRight != 0)
            {
                current = currentRight;
                currentRight = static_cast<impl_t*>(this)->GetRightOrDefault(current);
            }
            return current;
        }

        protected: TElement GetLeftest(TElement current)
        {
            auto currentLeft = static_cast<impl_t*>(this)->GetLeftOrDefault(current);
            while (currentLeft != 0)
            {
                current = currentLeft;
                currentLeft = static_cast<impl_t*>(this)->GetLeftOrDefault(current);
            }
            return current;
        }

        public: bool Contains(TElement node, TElement root)
        {
            while (root != 0)
            {
                if (static_cast<impl_t*>(this)->FirstIsToTheLeftOfSecond(node, root))
                {
                    root = static_cast<impl_t*>(this)->GetLeftOrDefault(root);
                }
                else if (static_cast<impl_t*>(this)->FirstIsToTheRightOfSecond(node, root))
                {
                    root = static_cast<impl_t*>(this)->GetRightOrDefault(root);
                }
                else
                {
                    return true;
                }
            }
            return false;
        }

        protected: void IncrementBalance(TElement node) { static_cast<impl_t*>(this)->SetBalance(node, (std::int8_t)(static_cast<impl_t*>(this)->GetBalance(node) + 1)); }

        protected: void DecrementBalance(TElement node) { static_cast<impl_t*>(this)->SetBalance(node, (std::int8_t)(static_cast<impl_t*>(this)->GetBalance(node) - 1)); }

        protected: TElement GetLeftOrDefault(TElement node) { return static_cast<impl_t*>(this)->GetLeftIsChild(node) ? static_cast<impl_t*>(this)->GetLeft(node) : 0; }

        protected: TElement GetRightOrDefault(TElement node) { return static_cast<impl_t*>(this)->GetRightIsChild(node) ? static_cast<impl_t*>(this)->GetRight(node) : 0; }

        protected: bool GetLeftIsChild(TElement node) { return static_cast<impl_t*>(this)->GetLeftIsChild(node); }

        protected: void SetLeftIsChild(TElement node, bool value) { static_cast<impl_t*>(this)->SetLeftIsChild(node, value); }

        protected: bool GetRightIsChild(TElement node) { return static_cast<impl_t*>(this)->GetRightIsChild(node); }

        protected: void SetRightIsChild(TElement node, bool value) { static_cast<impl_t*>(this)->SetRightIsChild(node, value); }

        protected: std::int8_t GetBalance(TElement node) { return static_cast<impl_t*>(this)->GetBalance(node); }

        protected: void SetBalance(TElement node, std::int8_t value) { static_cast<impl_t*>(this)->SetBalance(node, value); }

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
                    if (static_cast<impl_t*>(this)->FirstIsToTheLeftOfSecond(node, currentNode))
                    {
                        if (static_cast<impl_t*>(this)->GetLeftIsChild(currentNode))
                        {
                            static_cast<impl_t*>(this)->IncrementSize(currentNode);
                            path[pathPosition++] = currentNode;
                            currentNode = static_cast<impl_t*>(this)->GetLeft(currentNode);
                        }
                        else
                        {
                            static_cast<impl_t*>(this)->SetLeft(node, static_cast<impl_t*>(this)->GetLeft(currentNode));
                            static_cast<impl_t*>(this)->SetRight(node, currentNode);
                            static_cast<impl_t*>(this)->SetLeft(currentNode, node);
                            static_cast<impl_t*>(this)->SetLeftIsChild(currentNode, true);
                            static_cast<impl_t*>(this)->DecrementBalance(currentNode);
                            static_cast<impl_t*>(this)->SetSize(node, 1);
                            static_cast<impl_t*>(this)->FixSize(currentNode);
                            break;
                        }
                    }
                    else if (static_cast<impl_t*>(this)->FirstIsToTheRightOfSecond(node, currentNode))
                    {
                        if (static_cast<impl_t*>(this)->GetRightIsChild(currentNode))
                        {
                            static_cast<impl_t*>(this)->IncrementSize(currentNode);
                            path[pathPosition++] = currentNode;
                            currentNode = static_cast<impl_t*>(this)->GetRight(currentNode);
                        }
                        else
                        {
                            static_cast<impl_t*>(this)->SetRight(node, static_cast<impl_t*>(this)->GetRight(currentNode));
                            static_cast<impl_t*>(this)->SetLeft(node, currentNode);
                            static_cast<impl_t*>(this)->SetRight(currentNode, node);
                            static_cast<impl_t*>(this)->SetRightIsChild(currentNode, true);
                            static_cast<impl_t*>(this)->IncrementBalance(currentNode);
                            static_cast<impl_t*>(this)->SetSize(node, 1);
                            static_cast<impl_t*>(this)->FixSize(currentNode);
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
                    auto isLeftNode = parent != 0 && currentNode == static_cast<impl_t*>(this)->GetLeft(parent);
                    auto currentNodeBalance = static_cast<impl_t*>(this)->GetBalance(currentNode);
                    if (currentNodeBalance < -1 || currentNodeBalance > 1)
                    {
                        currentNode = static_cast<impl_t*>(this)->Balance(currentNode);
                        if (parent == 0)
                        {
                            *root = currentNode;
                        }
                        else if (isLeftNode)
                        {
                            static_cast<impl_t*>(this)->SetLeft(parent, currentNode);
                            static_cast<impl_t*>(this)->FixSize(parent);
                        }
                        else
                        {
                            static_cast<impl_t*>(this)->SetRight(parent, currentNode);
                            static_cast<impl_t*>(this)->FixSize(parent);
                        }
                    }
                    currentNodeBalance = static_cast<impl_t*>(this)->GetBalance(currentNode);
                    if (currentNodeBalance == 0 || parent == 0)
                    {
                        break;
                    }
                    if (isLeftNode)
                    {
                        static_cast<impl_t*>(this)->DecrementBalance(parent);
                    }
                    else
                    {
                        static_cast<impl_t*>(this)->IncrementBalance(parent);
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
                auto rootBalance = static_cast<impl_t*>(this)->GetBalance(node);
                if (rootBalance < -1)
                {
                    auto left = static_cast<impl_t*>(this)->GetLeft(node);
                    if (static_cast<impl_t*>(this)->GetBalance(left) > 0)
                    {
                        static_cast<impl_t*>(this)->SetLeft(node, static_cast<impl_t*>(this)->LeftRotateWithBalance(left));
                        static_cast<impl_t*>(this)->FixSize(node);
                    }
                    node = static_cast<impl_t*>(this)->RightRotateWithBalance(node);
                }
                else if (rootBalance > 1)
                {
                    auto right = static_cast<impl_t*>(this)->GetRight(node);
                    if (static_cast<impl_t*>(this)->GetBalance(right) < 0)
                    {
                        static_cast<impl_t*>(this)->SetRight(node, static_cast<impl_t*>(this)->RightRotateWithBalance(right));
                        static_cast<impl_t*>(this)->FixSize(node);
                    }
                    node = static_cast<impl_t*>(this)->LeftRotateWithBalance(node);
                }
                return node;
            }
        }

        protected: TElement LeftRotateWithBalance(TElement node)
        {
            {
                auto right = static_cast<impl_t*>(this)->GetRight(node);
                if (static_cast<impl_t*>(this)->GetLeftIsChild(right))
                {
                    static_cast<impl_t*>(this)->SetRight(node, static_cast<impl_t*>(this)->GetLeft(right));
                }
                else
                {
                    static_cast<impl_t*>(this)->SetRightIsChild(node, false);
                    static_cast<impl_t*>(this)->SetLeftIsChild(right, true);
                }
                static_cast<impl_t*>(this)->SetLeft(right, node);
                static_cast<impl_t*>(this)->SetSize(right, static_cast<impl_t*>(this)->GetSize(node));
                static_cast<impl_t*>(this)->FixSize(node);
                auto rootBalance = static_cast<impl_t*>(this)->GetBalance(node);
                auto rightBalance = static_cast<impl_t*>(this)->GetBalance(right);
                if (rightBalance <= 0)
                {
                    if (rootBalance >= 1)
                    {
                        static_cast<impl_t*>(this)->SetBalance(right, (std::int8_t)(rightBalance - 1));
                    }
                    else
                    {
                        static_cast<impl_t*>(this)->SetBalance(right, (std::int8_t)(rootBalance + rightBalance - 2));
                    }
                    static_cast<impl_t*>(this)->SetBalance(node, (std::int8_t)(rootBalance - 1));
                }
                else
                {
                    if (rootBalance <= rightBalance)
                    {
                        static_cast<impl_t*>(this)->SetBalance(right, (std::int8_t)(rootBalance - 2));
                    }
                    else
                    {
                        static_cast<impl_t*>(this)->SetBalance(right, (std::int8_t)(rightBalance - 1));
                    }
                    static_cast<impl_t*>(this)->SetBalance(node, (std::int8_t)(rootBalance - rightBalance - 1));
                }
                return right;
            }
        }

        protected: TElement RightRotateWithBalance(TElement node)
        {
            {
                auto left = static_cast<impl_t*>(this)->GetLeft(node);
                if (static_cast<impl_t*>(this)->GetRightIsChild(left))
                {
                    static_cast<impl_t*>(this)->SetLeft(node, static_cast<impl_t*>(this)->GetRight(left));
                }
                else
                {
                    static_cast<impl_t*>(this)->SetLeftIsChild(node, false);
                    static_cast<impl_t*>(this)->SetRightIsChild(left, true);
                }
                static_cast<impl_t*>(this)->SetRight(left, node);
                static_cast<impl_t*>(this)->SetSize(left, static_cast<impl_t*>(this)->GetSize(node));
                static_cast<impl_t*>(this)->FixSize(node);
                auto rootBalance = static_cast<impl_t*>(this)->GetBalance(node);
                auto leftBalance = static_cast<impl_t*>(this)->GetBalance(left);
                if (leftBalance <= 0)
                {
                    if (leftBalance > rootBalance)
                    {
                        static_cast<impl_t*>(this)->SetBalance(left, (std::int8_t)(leftBalance + 1));
                    }
                    else
                    {
                        static_cast<impl_t*>(this)->SetBalance(left, (std::int8_t)(rootBalance + 2));
                    }
                    static_cast<impl_t*>(this)->SetBalance(node, (std::int8_t)(rootBalance - leftBalance + 1));
                }
                else
                {
                    if (rootBalance <= -1)
                    {
                        static_cast<impl_t*>(this)->SetBalance(left, (std::int8_t)(leftBalance + 1));
                    }
                    else
                    {
                        static_cast<impl_t*>(this)->SetBalance(left, (std::int8_t)(rootBalance + leftBalance + 2));
                    }
                    static_cast<impl_t*>(this)->SetBalance(node, (std::int8_t)(rootBalance + 1));
                }
                return left;
            }
        }

        protected: TElement GetNext(TElement node)
        {
            auto current = static_cast<impl_t*>(this)->GetRight(node);
            if (static_cast<impl_t*>(this)->GetRightIsChild(node))
            {
                return static_cast<impl_t*>(this)->GetLeftest(current);
            }
            return current;
        }

        protected: TElement GetPrevious(TElement node)
        {
            auto current = static_cast<impl_t*>(this)->GetLeft(node);
            if (static_cast<impl_t*>(this)->GetLeftIsChild(node))
            {
                return static_cast<impl_t*>(this)->GetRightest(current);
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
                    if (static_cast<impl_t*>(this)->FirstIsToTheLeftOfSecond(node, currentNode))
                    {
                        if (!static_cast<impl_t*>(this)->GetLeftIsChild(currentNode))
                        {
                            throw std::runtime_error("Cannot find a node.");
                        }
                        static_cast<impl_t*>(this)->DecrementSize(currentNode);
                        path[pathPosition++] = currentNode;
                        currentNode = static_cast<impl_t*>(this)->GetLeft(currentNode);
                    }
                    else if (static_cast<impl_t*>(this)->FirstIsToTheRightOfSecond(node, currentNode))
                    {
                        if (!static_cast<impl_t*>(this)->GetRightIsChild(currentNode))
                        {
                            throw std::runtime_error("Cannot find a node.");
                        }
                        static_cast<impl_t*>(this)->DecrementSize(currentNode);
                        path[pathPosition++] = currentNode;
                        currentNode = static_cast<impl_t*>(this)->GetRight(currentNode);
                    }
                    else
                    {
                        break;
                    }
                }
                auto parent = path[--pathPosition];
                auto balanceNode = parent;
                auto isLeftNode = parent != 0 && currentNode == static_cast<impl_t*>(this)->GetLeft(parent);
                if (!static_cast<impl_t*>(this)->GetLeftIsChild(currentNode))
                {
                    if (!static_cast<impl_t*>(this)->GetRightIsChild(currentNode))
                    {
                        if (parent == 0)
                        {
                            *root = 0;
                        }
                        else if (isLeftNode)
                        {
                            static_cast<impl_t*>(this)->SetLeftIsChild(parent, false);
                            static_cast<impl_t*>(this)->SetLeft(parent, static_cast<impl_t*>(this)->GetLeft(currentNode));
                            static_cast<impl_t*>(this)->IncrementBalance(parent);
                        }
                        else
                        {
                            static_cast<impl_t*>(this)->SetRightIsChild(parent, false);
                            static_cast<impl_t*>(this)->SetRight(parent, static_cast<impl_t*>(this)->GetRight(currentNode));
                            static_cast<impl_t*>(this)->DecrementBalance(parent);
                        }
                    }
                    else
                    {
                        auto successor = static_cast<impl_t*>(this)->GetNext(currentNode);
                        static_cast<impl_t*>(this)->SetLeft(successor, static_cast<impl_t*>(this)->GetLeft(currentNode));
                        auto right = static_cast<impl_t*>(this)->GetRight(currentNode);
                        if (parent == 0)
                        {
                            *root = right;
                        }
                        else if (isLeftNode)
                        {
                            static_cast<impl_t*>(this)->SetLeft(parent, right);
                            static_cast<impl_t*>(this)->IncrementBalance(parent);
                        }
                        else
                        {
                            static_cast<impl_t*>(this)->SetRight(parent, right);
                            static_cast<impl_t*>(this)->DecrementBalance(parent);
                        }
                    }
                }
                else
                {
                    if (!static_cast<impl_t*>(this)->GetRightIsChild(currentNode))
                    {
                        auto predecessor = static_cast<impl_t*>(this)->GetPrevious(currentNode);
                        static_cast<impl_t*>(this)->SetRight(predecessor, static_cast<impl_t*>(this)->GetRight(currentNode));
                        auto leftValue = static_cast<impl_t*>(this)->GetLeft(currentNode);
                        if (parent == 0)
                        {
                            *root = leftValue;
                        }
                        else if (isLeftNode)
                        {
                            static_cast<impl_t*>(this)->SetLeft(parent, leftValue);
                            static_cast<impl_t*>(this)->IncrementBalance(parent);
                        }
                        else
                        {
                            static_cast<impl_t*>(this)->SetRight(parent, leftValue);
                            static_cast<impl_t*>(this)->DecrementBalance(parent);
                        }
                    }
                    else
                    {
                        auto predecessor = static_cast<impl_t*>(this)->GetLeft(currentNode);
                        auto successor = static_cast<impl_t*>(this)->GetRight(currentNode);
                        auto successorParent = currentNode;
                        std::int32_t previousPathPosition = ++pathPosition;
                        while (static_cast<impl_t*>(this)->GetLeftIsChild(successor))
                        {
                            path[++pathPosition] = successorParent = successor;
                            successor = static_cast<impl_t*>(this)->GetLeft(successor);
                            if (successorParent != currentNode)
                            {
                                static_cast<impl_t*>(this)->DecrementSize(successorParent);
                            }
                        }
                        path[previousPathPosition] = successor;
                        balanceNode = path[pathPosition];
                        if (successorParent != currentNode)
                        {
                            if (!static_cast<impl_t*>(this)->GetRightIsChild(successor))
                            {
                                static_cast<impl_t*>(this)->SetLeftIsChild(successorParent, false);
                            }
                            else
                            {
                                static_cast<impl_t*>(this)->SetLeft(successorParent, static_cast<impl_t*>(this)->GetRight(successor));
                            }
                            static_cast<impl_t*>(this)->IncrementBalance(successorParent);
                            static_cast<impl_t*>(this)->SetRightIsChild(successor, true);
                            static_cast<impl_t*>(this)->SetRight(successor, static_cast<impl_t*>(this)->GetRight(currentNode));
                        }
                        else
                        {
                            static_cast<impl_t*>(this)->DecrementBalance(currentNode);
                        }
                        while (static_cast<impl_t*>(this)->GetRightIsChild(predecessor))
                        {
                            predecessor = static_cast<impl_t*>(this)->GetRight(predecessor);
                        }
                        static_cast<impl_t*>(this)->SetRight(predecessor, successor);
                        auto left = static_cast<impl_t*>(this)->GetLeft(currentNode);
                        static_cast<impl_t*>(this)->SetLeftIsChild(successor, true);
                        static_cast<impl_t*>(this)->SetLeft(successor, left);
                        static_cast<impl_t*>(this)->SetBalance(successor, static_cast<impl_t*>(this)->GetBalance(currentNode));
                        static_cast<impl_t*>(this)->FixSize(successor);
                        if (parent == 0)
                        {
                            *root = successor;
                        }
                        else if (isLeftNode)
                        {
                            static_cast<impl_t*>(this)->SetLeft(parent, successor);
                        }
                        else
                        {
                            static_cast<impl_t*>(this)->SetRight(parent, successor);
                        }
                    }
                }
                if (balanceNode != 0)
                {
                    while (true)
                    {
                        auto balanceParent = path[--pathPosition];
                        isLeftNode = balanceParent != 0 && balanceNode == static_cast<impl_t*>(this)->GetLeft(balanceParent);
                        auto currentNodeBalance = static_cast<impl_t*>(this)->GetBalance(balanceNode);
                        if (currentNodeBalance < -1 || currentNodeBalance > 1)
                        {
                            balanceNode = static_cast<impl_t*>(this)->Balance(balanceNode);
                            if (balanceParent == 0)
                            {
                                *root = balanceNode;
                            }
                            else if (isLeftNode)
                            {
                                static_cast<impl_t*>(this)->SetLeft(balanceParent, balanceNode);
                            }
                            else
                            {
                                static_cast<impl_t*>(this)->SetRight(balanceParent, balanceNode);
                            }
                        }
                        currentNodeBalance = static_cast<impl_t*>(this)->GetBalance(balanceNode);
                        if (currentNodeBalance != 0 || balanceParent == 0)
                        {
                            break;
                        }
                        if (isLeftNode)
                        {
                            static_cast<impl_t*>(this)->IncrementBalance(balanceParent);
                        }
                        else
                        {
                            static_cast<impl_t*>(this)->DecrementBalance(balanceParent);
                        }
                        balanceNode = balanceParent;
                    }
                }
                static_cast<impl_t*>(this)->ClearNode(node);
#if USEARRAYPOOL
                ArrayPool.Free(path);
#endif
            }
        }

        protected: void ClearNode(TElement node)
        {
            static_cast<impl_t*>(this)->SetLeft(node, 0);
            static_cast<impl_t*>(this)->SetRight(node, 0);
            static_cast<impl_t*>(this)->SetSize(node, 0);
            static_cast<impl_t*>(this)->SetLeftIsChild(node, false);
            static_cast<impl_t*>(this)->SetRightIsChild(node, false);
            static_cast<impl_t*>(this)->SetBalance(node, 0);
        }
    };
}