namespace Platform::Collections::Methods::Trees
{
    template <typename ...> class SizedAndThreadedAVLBalancedTreeMethods;
    template <typename impl_t, typename TElement> class SizedAndThreadedAVLBalancedTreeMethods<impl_t, TElement> : public SizedBinaryTreeMethodsBase<impl_t, TElement>
    {
        public: using base = SizedBinaryTreeMethodsBase<impl_t, TElement>;
        friend base;
        using Polymorph<impl_t>::object;

        private: inline static const std::int32_t _maxPath = 11 * sizeof(TElement) + 4;

        protected: TElement GetRightest(TElement current)
        {
            auto currentRight = object().GetRightOrDefault(current);
            while (currentRight != 0)
            {
                current = currentRight;
                currentRight = object().GetRightOrDefault(current);
            }
            return current;
        }

        protected: TElement GetLeftest(TElement current)
        {
            auto currentLeft = object().GetLeftOrDefault(current);
            while (currentLeft != 0)
            {
                current = currentLeft;
                currentLeft = object().GetLeftOrDefault(current);
            }
            return current;
        }

        public: bool Contains(TElement node, TElement root)
        {
            while (root != 0)
            {
                if (object().FirstIsToTheLeftOfSecond(node, root))
                {
                    root = object().GetLeftOrDefault(root);
                }
                else if (object().FirstIsToTheRightOfSecond(node, root))
                {
                    root = object().GetRightOrDefault(root);
                }
                else
                {
                    return true;
                }
            }
            return false;
        }

        protected: void IncrementBalance(TElement node) { object().SetBalance(node, (std::int8_t)(object().GetBalance(node) + 1)); }

        protected: void DecrementBalance(TElement node) { object().SetBalance(node, (std::int8_t)(object().GetBalance(node) - 1)); }

        protected: TElement GetLeftOrDefault(TElement node) { return object().GetLeftIsChild(node) ? object().GetLeft(node) : 0; }

        protected: TElement GetRightOrDefault(TElement node) { return object().GetRightIsChild(node) ? object().GetRight(node) : 0; }

        protected: bool GetLeftIsChild(TElement node) { return object().GetLeftIsChild(node); }

        protected: void SetLeftIsChild(TElement node, bool value) { object().SetLeftIsChild(node, value); }

        protected: bool GetRightIsChild(TElement node) { return object().GetRightIsChild(node); }

        protected: void SetRightIsChild(TElement node, bool value) { object().SetRightIsChild(node, value); }

        protected: std::int8_t GetBalance(TElement node) { return object().GetBalance(node); }

        protected: void SetBalance(TElement node, std::int8_t value) { object().SetBalance(node, value); }

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
                    if (object().FirstIsToTheLeftOfSecond(node, currentNode))
                    {
                        if (object().GetLeftIsChild(currentNode))
                        {
                            object().IncrementSize(currentNode);
                            path[pathPosition++] = currentNode;
                            currentNode = object().GetLeft(currentNode);
                        }
                        else
                        {
                            object().SetLeft(node, object().GetLeft(currentNode));
                            object().SetRight(node, currentNode);
                            object().SetLeft(currentNode, node);
                            object().SetLeftIsChild(currentNode, true);
                            object().DecrementBalance(currentNode);
                            object().SetSize(node, 1);
                            object().FixSize(currentNode);
                            break;
                        }
                    }
                    else if (object().FirstIsToTheRightOfSecond(node, currentNode))
                    {
                        if (object().GetRightIsChild(currentNode))
                        {
                            object().IncrementSize(currentNode);
                            path[pathPosition++] = currentNode;
                            currentNode = object().GetRight(currentNode);
                        }
                        else
                        {
                            object().SetRight(node, object().GetRight(currentNode));
                            object().SetLeft(node, currentNode);
                            object().SetRight(currentNode, node);
                            object().SetRightIsChild(currentNode, true);
                            object().IncrementBalance(currentNode);
                            object().SetSize(node, 1);
                            object().FixSize(currentNode);
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
                    auto isLeftNode = parent != 0 && currentNode == object().GetLeft(parent);
                    auto currentNodeBalance = object().GetBalance(currentNode);
                    if (currentNodeBalance < -1 || currentNodeBalance > 1)
                    {
                        currentNode = object().Balance(currentNode);
                        if (parent == 0)
                        {
                            *root = currentNode;
                        }
                        else if (isLeftNode)
                        {
                            object().SetLeft(parent, currentNode);
                            object().FixSize(parent);
                        }
                        else
                        {
                            object().SetRight(parent, currentNode);
                            object().FixSize(parent);
                        }
                    }
                    currentNodeBalance = object().GetBalance(currentNode);
                    if (currentNodeBalance == 0 || parent == 0)
                    {
                        break;
                    }
                    if (isLeftNode)
                    {
                        object().DecrementBalance(parent);
                    }
                    else
                    {
                        object().IncrementBalance(parent);
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
                auto rootBalance = object().GetBalance(node);
                if (rootBalance < -1)
                {
                    auto left = object().GetLeft(node);
                    if (object().GetBalance(left) > 0)
                    {
                        object().SetLeft(node, object().LeftRotateWithBalance(left));
                        object().FixSize(node);
                    }
                    node = object().RightRotateWithBalance(node);
                }
                else if (rootBalance > 1)
                {
                    auto right = object().GetRight(node);
                    if (object().GetBalance(right) < 0)
                    {
                        object().SetRight(node, object().RightRotateWithBalance(right));
                        object().FixSize(node);
                    }
                    node = object().LeftRotateWithBalance(node);
                }
                return node;
            }
        }

        protected: TElement LeftRotateWithBalance(TElement node)
        {
            {
                auto right = object().GetRight(node);
                if (object().GetLeftIsChild(right))
                {
                    object().SetRight(node, object().GetLeft(right));
                }
                else
                {
                    object().SetRightIsChild(node, false);
                    object().SetLeftIsChild(right, true);
                }
                object().SetLeft(right, node);
                object().SetSize(right, object().GetSize(node));
                object().FixSize(node);
                auto rootBalance = object().GetBalance(node);
                auto rightBalance = object().GetBalance(right);
                if (rightBalance <= 0)
                {
                    if (rootBalance >= 1)
                    {
                        object().SetBalance(right, (std::int8_t)(rightBalance - 1));
                    }
                    else
                    {
                        object().SetBalance(right, (std::int8_t)(rootBalance + rightBalance - 2));
                    }
                    object().SetBalance(node, (std::int8_t)(rootBalance - 1));
                }
                else
                {
                    if (rootBalance <= rightBalance)
                    {
                        object().SetBalance(right, (std::int8_t)(rootBalance - 2));
                    }
                    else
                    {
                        object().SetBalance(right, (std::int8_t)(rightBalance - 1));
                    }
                    object().SetBalance(node, (std::int8_t)(rootBalance - rightBalance - 1));
                }
                return right;
            }
        }

        protected: TElement RightRotateWithBalance(TElement node)
        {
            {
                auto left = object().GetLeft(node);
                if (object().GetRightIsChild(left))
                {
                    object().SetLeft(node, object().GetRight(left));
                }
                else
                {
                    object().SetLeftIsChild(node, false);
                    object().SetRightIsChild(left, true);
                }
                object().SetRight(left, node);
                object().SetSize(left, object().GetSize(node));
                object().FixSize(node);
                auto rootBalance = object().GetBalance(node);
                auto leftBalance = object().GetBalance(left);
                if (leftBalance <= 0)
                {
                    if (leftBalance > rootBalance)
                    {
                        object().SetBalance(left, (std::int8_t)(leftBalance + 1));
                    }
                    else
                    {
                        object().SetBalance(left, (std::int8_t)(rootBalance + 2));
                    }
                    object().SetBalance(node, (std::int8_t)(rootBalance - leftBalance + 1));
                }
                else
                {
                    if (rootBalance <= -1)
                    {
                        object().SetBalance(left, (std::int8_t)(leftBalance + 1));
                    }
                    else
                    {
                        object().SetBalance(left, (std::int8_t)(rootBalance + leftBalance + 2));
                    }
                    object().SetBalance(node, (std::int8_t)(rootBalance + 1));
                }
                return left;
            }
        }

        protected: TElement GetNext(TElement node)
        {
            auto current = object().GetRight(node);
            if (object().GetRightIsChild(node))
            {
                return object().GetLeftest(current);
            }
            return current;
        }

        protected: TElement GetPrevious(TElement node)
        {
            auto current = object().GetLeft(node);
            if (object().GetLeftIsChild(node))
            {
                return object().GetRightest(current);
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
                    if (object().FirstIsToTheLeftOfSecond(node, currentNode))
                    {
                        if (!object().GetLeftIsChild(currentNode))
                        {
                            throw std::runtime_error("Cannot find a node.");
                        }
                        object().DecrementSize(currentNode);
                        path[pathPosition++] = currentNode;
                        currentNode = object().GetLeft(currentNode);
                    }
                    else if (object().FirstIsToTheRightOfSecond(node, currentNode))
                    {
                        if (!object().GetRightIsChild(currentNode))
                        {
                            throw std::runtime_error("Cannot find a node.");
                        }
                        object().DecrementSize(currentNode);
                        path[pathPosition++] = currentNode;
                        currentNode = object().GetRight(currentNode);
                    }
                    else
                    {
                        break;
                    }
                }
                auto parent = path[--pathPosition];
                auto balanceNode = parent;
                auto isLeftNode = parent != 0 && currentNode == object().GetLeft(parent);
                if (!object().GetLeftIsChild(currentNode))
                {
                    if (!object().GetRightIsChild(currentNode))
                    {
                        if (parent == 0)
                        {
                            *root = 0;
                        }
                        else if (isLeftNode)
                        {
                            object().SetLeftIsChild(parent, false);
                            object().SetLeft(parent, object().GetLeft(currentNode));
                            object().IncrementBalance(parent);
                        }
                        else
                        {
                            object().SetRightIsChild(parent, false);
                            object().SetRight(parent, object().GetRight(currentNode));
                            object().DecrementBalance(parent);
                        }
                    }
                    else
                    {
                        auto successor = object().GetNext(currentNode);
                        object().SetLeft(successor, object().GetLeft(currentNode));
                        auto right = object().GetRight(currentNode);
                        if (parent == 0)
                        {
                            *root = right;
                        }
                        else if (isLeftNode)
                        {
                            object().SetLeft(parent, right);
                            object().IncrementBalance(parent);
                        }
                        else
                        {
                            object().SetRight(parent, right);
                            object().DecrementBalance(parent);
                        }
                    }
                }
                else
                {
                    if (!object().GetRightIsChild(currentNode))
                    {
                        auto predecessor = object().GetPrevious(currentNode);
                        object().SetRight(predecessor, object().GetRight(currentNode));
                        auto leftValue = object().GetLeft(currentNode);
                        if (parent == 0)
                        {
                            *root = leftValue;
                        }
                        else if (isLeftNode)
                        {
                            object().SetLeft(parent, leftValue);
                            object().IncrementBalance(parent);
                        }
                        else
                        {
                            object().SetRight(parent, leftValue);
                            object().DecrementBalance(parent);
                        }
                    }
                    else
                    {
                        auto predecessor = object().GetLeft(currentNode);
                        auto successor = object().GetRight(currentNode);
                        auto successorParent = currentNode;
                        std::int32_t previousPathPosition = ++pathPosition;
                        while (object().GetLeftIsChild(successor))
                        {
                            path[++pathPosition] = successorParent = successor;
                            successor = object().GetLeft(successor);
                            if (successorParent != currentNode)
                            {
                                object().DecrementSize(successorParent);
                            }
                        }
                        path[previousPathPosition] = successor;
                        balanceNode = path[pathPosition];
                        if (successorParent != currentNode)
                        {
                            if (!object().GetRightIsChild(successor))
                            {
                                object().SetLeftIsChild(successorParent, false);
                            }
                            else
                            {
                                object().SetLeft(successorParent, object().GetRight(successor));
                            }
                            object().IncrementBalance(successorParent);
                            object().SetRightIsChild(successor, true);
                            object().SetRight(successor, object().GetRight(currentNode));
                        }
                        else
                        {
                            object().DecrementBalance(currentNode);
                        }
                        while (object().GetRightIsChild(predecessor))
                        {
                            predecessor = object().GetRight(predecessor);
                        }
                        object().SetRight(predecessor, successor);
                        auto left = object().GetLeft(currentNode);
                        object().SetLeftIsChild(successor, true);
                        object().SetLeft(successor, left);
                        object().SetBalance(successor, object().GetBalance(currentNode));
                        object().FixSize(successor);
                        if (parent == 0)
                        {
                            *root = successor;
                        }
                        else if (isLeftNode)
                        {
                            object().SetLeft(parent, successor);
                        }
                        else
                        {
                            object().SetRight(parent, successor);
                        }
                    }
                }
                if (balanceNode != 0)
                {
                    while (true)
                    {
                        auto balanceParent = path[--pathPosition];
                        isLeftNode = balanceParent != 0 && balanceNode == object().GetLeft(balanceParent);
                        auto currentNodeBalance = object().GetBalance(balanceNode);
                        if (currentNodeBalance < -1 || currentNodeBalance > 1)
                        {
                            balanceNode = object().Balance(balanceNode);
                            if (balanceParent == 0)
                            {
                                *root = balanceNode;
                            }
                            else if (isLeftNode)
                            {
                                object().SetLeft(balanceParent, balanceNode);
                            }
                            else
                            {
                                object().SetRight(balanceParent, balanceNode);
                            }
                        }
                        currentNodeBalance = object().GetBalance(balanceNode);
                        if (currentNodeBalance != 0 || balanceParent == 0)
                        {
                            break;
                        }
                        if (isLeftNode)
                        {
                            object().IncrementBalance(balanceParent);
                        }
                        else
                        {
                            object().DecrementBalance(balanceParent);
                        }
                        balanceNode = balanceParent;
                    }
                }
                object().ClearNode(node);
#if USEARRAYPOOL
                ArrayPool.Free(path);
#endif
            }
        }

        protected: void ClearNode(TElement node)
        {
            object().SetLeft(node, 0);
            object().SetRight(node, 0);
            object().SetSize(node, 0);
            object().SetLeftIsChild(node, false);
            object().SetRightIsChild(node, false);
            object().SetBalance(node, 0);
        }
    };
}