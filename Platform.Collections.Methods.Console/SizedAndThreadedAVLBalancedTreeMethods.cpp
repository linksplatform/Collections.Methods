


#if USEARRAYPOOL
#endif

namespace Platform::Collections::Methods::Trees
{
    /// <summary>
    /// Combination of Size, Height (AVL), and threads.
    /// </summary>
    /// <remarks>
    /// Based on: <a href="https://github.com/programmatom/TreeLib/blob/master/TreeLib/TreeLib/Generated/AVLTreeList.cs">TreeLib.AVLTreeList</a>.
    /// Which itself based on: <a href="https://github.com/GNOME/glib/blob/master/glib/gtree.c">GNOME/glib/gtree</a>.
    /// </remarks>
    template <typename TElement> class SizedAndThreadedAVLBalancedTreeMethods : SizedBinaryTreeMethodsBase<TElement>
    {
        // TODO: Link with size of TElement
        const int MaxPath = 92;

        bool Contains(TElement node, TElement root) override
        {
            while (root != 0)
            {
                if (SizedBinaryTreeMethodsBase::FirstIsToTheLeftOfSecond(node, root)) // node.Key < root.Key
                {
                    root = GetLeftOrDefault(root);
                }
                else if (SizedBinaryTreeMethodsBase::FirstIsToTheRightOfSecond(node, root)) // node.Key > root.Key
                {
                    root = GetRightOrDefault(root);
                }
                else // node.Key == root.Key
                {
                    return true;
                }
            }
            return false;
        }

        
        void IncrementBalance(TElement node) { return SetBalance(node, (std::int8_t)(GetBalance(node) + 1)); }
        void DecrementBalance(TElement node) { return SetBalance(node, (std::int8_t)(GetBalance(node) - 1)); }
        TElement GetLeftOrDefault(TElement node) override { return GetLeftIsChild(node) ? SizedBinaryTreeMethodsBase::GetLeft(node) : 0; }
        TElement GetRightOrDefault(TElement node) override { return GetRightIsChild(node) ? SizedBinaryTreeMethodsBase::GetRight(node) : 0; }
        virtual bool GetLeftIsChild(TElement node) = 0;
        virtual void SetLeftIsChild(TElement node, bool value) = 0;
        virtual bool GetRightIsChild(TElement node) = 0;
        virtual void SetRightIsChild(TElement node, bool value) = 0;
        virtual std::int8_t GetBalance(TElement node) = 0;
        virtual void SetBalance(TElement node, std::int8_t value) = 0;

        void AttachCore(TElement* root, TElement node) override
        {
            {
                // TODO: Check what is faster to use simple array or array from array pool
                // TODO: Try to use stackalloc as an optimization (requires code generation, because of generics)
#if USEARRAYPOOL
                auto path = ArrayPool.Allocate<TElement>(MaxPath);
                auto pathPosition = 0;
                path[pathPosition++] = 0;
#else
                auto path = new TElement[MaxPath];
                auto pathPosition = 1;
#endif
                auto currentNode = root;
                while (true)
                {
                    if (SizedBinaryTreeMethodsBase::FirstIsToTheLeftOfSecond(node, currentNode))
                    {
                        if (GetLeftIsChild(currentNode))
                        {
                            SizedBinaryTreeMethodsBase::IncrementSize(currentNode);
                            path[pathPosition++] = currentNode;
                            currentNode = SizedBinaryTreeMethodsBase::GetLeft(currentNode);
                        }
                        else
                        {
                            // Threads
                            SizedBinaryTreeMethodsBase::SetLeft(node, SizedBinaryTreeMethodsBase::GetLeft(currentNode));
                            SizedBinaryTreeMethodsBase::SetRight(node, currentNode);
                            SizedBinaryTreeMethodsBase::SetLeft(currentNode, node);
                            SetLeftIsChild(currentNode, true);
                            DecrementBalance(currentNode);
                            SizedBinaryTreeMethodsBase::SetSize(node, 1);
                            SizedBinaryTreeMethodsBase::FixSize(currentNode); // Should be incremented already
                            break;
                        }
                    }
                    else if (SizedBinaryTreeMethodsBase::FirstIsToTheRightOfSecond(node, currentNode))
                    {
                        if (GetRightIsChild(currentNode))
                        {
                            SizedBinaryTreeMethodsBase::IncrementSize(currentNode);
                            path[pathPosition++] = currentNode;
                            currentNode = SizedBinaryTreeMethodsBase::GetRight(currentNode);
                        }
                        else
                        {
                            // Threads
                            SizedBinaryTreeMethodsBase::SetRight(node, SizedBinaryTreeMethodsBase::GetRight(currentNode));
                            SizedBinaryTreeMethodsBase::SetLeft(node, currentNode);
                            SizedBinaryTreeMethodsBase::SetRight(currentNode, node);
                            SetRightIsChild(currentNode, true);
                            IncrementBalance(currentNode);
                            SizedBinaryTreeMethodsBase::SetSize(node, 1);
                            SizedBinaryTreeMethodsBase::FixSize(currentNode); // Should be incremented already
                            break;
                        }
                    }
                    else
                    {
                        throw std::exception("Node with the same key already attached to a tree.");
                    }
                }
                // Restore balance. This is the goodness of a non-recursive
                // implementation, when we are done with balancing we 'break'
                // the loop and we are done.
                while (true)
                {
                    auto parent = path[--pathPosition];
                    auto isLeftNode = parent != 0 && currentNode == SizedBinaryTreeMethodsBase::GetLeft(parent);
                    auto currentNodeBalance = GetBalance(currentNode);
                    if (currentNodeBalance < -1 || currentNodeBalance > 1)
                    {
                        currentNode = Balance(currentNode);
                        if (parent == 0)
                        {
                            root = currentNode;
                        }
                        else if (isLeftNode)
                        {
                            SizedBinaryTreeMethodsBase::SetLeft(parent, currentNode);
                            SizedBinaryTreeMethodsBase::FixSize(parent);
                        }
                        else
                        {
                            SizedBinaryTreeMethodsBase::SetRight(parent, currentNode);
                            SizedBinaryTreeMethodsBase::FixSize(parent);
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
                    auto left = SizedBinaryTreeMethodsBase::GetLeft(node);
                    if (GetBalance(left) > 0)
                    {
                        SizedBinaryTreeMethodsBase::SetLeft(node, LeftRotateWithBalance(left));
                        SizedBinaryTreeMethodsBase::FixSize(node);
                    }
                    node = RightRotateWithBalance(node);
                }
                else if (rootBalance > 1)
                {
                    auto right = SizedBinaryTreeMethodsBase::GetRight(node);
                    if (GetBalance(right) < 0)
                    {
                        SizedBinaryTreeMethodsBase::SetRight(node, RightRotateWithBalance(right));
                        SizedBinaryTreeMethodsBase::FixSize(node);
                    }
                    node = LeftRotateWithBalance(node);
                }
                return node;
            }
        }

        TElement LeftRotateWithBalance(TElement node)
        {
            {
                auto right = SizedBinaryTreeMethodsBase::GetRight(node);
                if (GetLeftIsChild(right))
                {
                    SizedBinaryTreeMethodsBase::SetRight(node, SizedBinaryTreeMethodsBase::GetLeft(right));
                }
                else
                {
                    SetRightIsChild(node, false);
                    SetLeftIsChild(right, true);
                }
                SizedBinaryTreeMethodsBase::SetLeft(right, node);
                // Fix size
                SizedBinaryTreeMethodsBase::SetSize(right, SizedBinaryTreeMethodsBase::GetSize(node));
                SizedBinaryTreeMethodsBase::FixSize(node);
                // Fix balance
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
                auto left = SizedBinaryTreeMethodsBase::GetLeft(node);
                if (GetRightIsChild(left))
                {
                    SizedBinaryTreeMethodsBase::SetLeft(node, SizedBinaryTreeMethodsBase::GetRight(left));
                }
                else
                {
                    SetLeftIsChild(node, false);
                    SetRightIsChild(left, true);
                }
                SizedBinaryTreeMethodsBase::SetRight(left, node);
                // Fix size
                SizedBinaryTreeMethodsBase::SetSize(left, SizedBinaryTreeMethodsBase::GetSize(node));
                SizedBinaryTreeMethodsBase::FixSize(node);
                // Fix balance
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
                auto current = SizedBinaryTreeMethodsBase::GetRight(node);
                if (GetRightIsChild(node))
                {
                    while (GetLeftIsChild(current))
                    {
                        current = SizedBinaryTreeMethodsBase::GetLeft(current);
                    }
                }
                return current;
            }
        }

        TElement GetPrevious(TElement node)
        {
            {
                auto current = SizedBinaryTreeMethodsBase::GetLeft(node);
                if (GetLeftIsChild(node))
                {
                    while (GetRightIsChild(current))
                    {
                        current = SizedBinaryTreeMethodsBase::GetRight(current);
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
                auto path = new TElement[MaxPath];
                auto pathPosition = 1;
#endif
                auto currentNode = root;
                while (true)
                {
                    if (SizedBinaryTreeMethodsBase::FirstIsToTheLeftOfSecond(node, currentNode))
                    {
                        if (!GetLeftIsChild(currentNode))
                        {
                            throw std::exception("Cannot find a node.");
                        }
                        SizedBinaryTreeMethodsBase::DecrementSize(currentNode);
                        path[pathPosition++] = currentNode;
                        currentNode = SizedBinaryTreeMethodsBase::GetLeft(currentNode);
                    }
                    else if (SizedBinaryTreeMethodsBase::FirstIsToTheRightOfSecond(node, currentNode))
                    {
                        if (!GetRightIsChild(currentNode))
                        {
                            throw std::exception("Cannot find a node.");
                        }
                        SizedBinaryTreeMethodsBase::DecrementSize(currentNode);
                        path[pathPosition++] = currentNode;
                        currentNode = SizedBinaryTreeMethodsBase::GetRight(currentNode);
                    }
                    else
                    {
                        break;
                    }
                }
                auto parent = path[--pathPosition];
                auto balanceNode = parent;
                auto isLeftNode = parent != 0 && currentNode == SizedBinaryTreeMethodsBase::GetLeft(parent);
                if (!GetLeftIsChild(currentNode))
                {
                    if (!GetRightIsChild(currentNode)) // node has no children
                    {
                        if (parent == 0)
                        {
                            root = 0;
                        }
                        else if (isLeftNode)
                        {
                            SetLeftIsChild(parent, false);
                            SizedBinaryTreeMethodsBase::SetLeft(parent, SizedBinaryTreeMethodsBase::GetLeft(currentNode));
                            IncrementBalance(parent);
                        }
                        else
                        {
                            SetRightIsChild(parent, false);
                            SizedBinaryTreeMethodsBase::SetRight(parent, SizedBinaryTreeMethodsBase::GetRight(currentNode));
                            DecrementBalance(parent);
                        }
                    }
                    else // node has a right child
                    {
                        auto successor = GetNext(currentNode);
                        SizedBinaryTreeMethodsBase::SetLeft(successor, SizedBinaryTreeMethodsBase::GetLeft(currentNode));
                        auto right = SizedBinaryTreeMethodsBase::GetRight(currentNode);
                        if (parent == 0)
                        {
                            root = right;
                        }
                        else if (isLeftNode)
                        {
                            SizedBinaryTreeMethodsBase::SetLeft(parent, right);
                            IncrementBalance(parent);
                        }
                        else
                        {
                            SizedBinaryTreeMethodsBase::SetRight(parent, right);
                            DecrementBalance(parent);
                        }
                    }
                }
                else // node has a left child
                {
                    if (!GetRightIsChild(currentNode))
                    {
                        auto predecessor = GetPrevious(currentNode);
                        SizedBinaryTreeMethodsBase::SetRight(predecessor, SizedBinaryTreeMethodsBase::GetRight(currentNode));
                        auto leftValue = SizedBinaryTreeMethodsBase::GetLeft(currentNode);
                        if (parent == 0)
                        {
                            root = leftValue;
                        }
                        else if (isLeftNode)
                        {
                            SizedBinaryTreeMethodsBase::SetLeft(parent, leftValue);
                            IncrementBalance(parent);
                        }
                        else
                        {
                            SizedBinaryTreeMethodsBase::SetRight(parent, leftValue);
                            DecrementBalance(parent);
                        }
                    }
                    else // node has a both children (left and right)
                    {
                        auto predecessor = SizedBinaryTreeMethodsBase::GetLeft(currentNode);
                        auto successor = SizedBinaryTreeMethodsBase::GetRight(currentNode);
                        auto successorParent = currentNode;
                        int previousPathPosition = ++pathPosition;
                        // find the immediately next node (and its parent)
                        while (GetLeftIsChild(successor))
                        {
                            path[++pathPosition] = successorParent = successor;
                            successor = SizedBinaryTreeMethodsBase::GetLeft(successor);
                            if (successorParent != currentNode)
                            {
                                SizedBinaryTreeMethodsBase::DecrementSize(successorParent);
                            }
                        }
                        path[previousPathPosition] = successor;
                        balanceNode = path[pathPosition];
                        // remove 'successor' from the tree
                        if (successorParent != currentNode)
                        {
                            if (!GetRightIsChild(successor))
                            {
                                SetLeftIsChild(successorParent, false);
                            }
                            else
                            {
                                SizedBinaryTreeMethodsBase::SetLeft(successorParent, SizedBinaryTreeMethodsBase::GetRight(successor));
                            }
                            IncrementBalance(successorParent);
                            SetRightIsChild(successor, true);
                            SizedBinaryTreeMethodsBase::SetRight(successor, SizedBinaryTreeMethodsBase::GetRight(currentNode));
                        }
                        else
                        {
                            DecrementBalance(currentNode);
                        }
                        // set the predecessor's successor link to point to the right place
                        while (GetRightIsChild(predecessor))
                        {
                            predecessor = SizedBinaryTreeMethodsBase::GetRight(predecessor);
                        }
                        SizedBinaryTreeMethodsBase::SetRight(predecessor, successor);
                        // prepare 'successor' to replace 'node'
                        auto left = SizedBinaryTreeMethodsBase::GetLeft(currentNode);
                        SetLeftIsChild(successor, true);
                        SizedBinaryTreeMethodsBase::SetLeft(successor, left);
                        SetBalance(successor, GetBalance(currentNode));
                        SizedBinaryTreeMethodsBase::FixSize(successor);
                        if (parent == 0)
                        {
                            root = successor;
                        }
                        else if (isLeftNode)
                        {
                            SizedBinaryTreeMethodsBase::SetLeft(parent, successor);
                        }
                        else
                        {
                            SizedBinaryTreeMethodsBase::SetRight(parent, successor);
                        }
                    }
                }
                // restore balance
                if (balanceNode != 0)
                {
                    while (true)
                    {
                        auto balanceParent = path[--pathPosition];
                        isLeftNode = balanceParent != 0 && balanceNode == SizedBinaryTreeMethodsBase::GetLeft(balanceParent);
                        auto currentNodeBalance = GetBalance(balanceNode);
                        if (currentNodeBalance < -1 || currentNodeBalance > 1)
                        {
                            balanceNode = Balance(balanceNode);
                            if (balanceParent == 0)
                            {
                                root = balanceNode;
                            }
                            else if (isLeftNode)
                            {
                                SizedBinaryTreeMethodsBase::SetLeft(balanceParent, balanceNode);
                            }
                            else
                            {
                                SizedBinaryTreeMethodsBase::SetRight(balanceParent, balanceNode);
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
            SizedBinaryTreeMethodsBase::SetLeft(node, 0);
            SizedBinaryTreeMethodsBase::SetRight(node, 0);
            SizedBinaryTreeMethodsBase::SetSize(node, 0);
            SetLeftIsChild(node, false);
            SetRightIsChild(node, false);
            SetBalance(node, 0);
        }
    };
}