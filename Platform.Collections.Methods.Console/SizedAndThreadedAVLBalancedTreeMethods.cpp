


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
    template <typename TElement> class SizedAndThreadedAVLBalancedTreeMethods : public SizedBinaryTreeMethodsBase<TElement>
    { public:
        // TODO: Link with size of TElement
        static const int MaxPath = 92;

        bool Contains(TElement node, TElement root) override
        {
            while (*root != 0)
            {
                if (this->FirstIsToTheLeftOfSecond(node, *root)) // node.Key < root.Key
                {
                    *root = GetLeftOrDefault(*root);
                }
                else if (this->FirstIsToTheRightOfSecond(node, *root)) // node.Key > root.Key
                {
                    *root = GetRightOrDefault(*root);
                }
                else // node.Key == root.Key
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
                // TODO: Check what is faster to use simple array or array from array pool
                // TODO: Try to use stackalloc as an optimization (requires code generation, because of generics)
#if USEARRAYPOOL
                TElement path = ArrayPool.Allocate<TElement>(MaxPath);
                TElement pathPosition = 0;
                path[pathPosition++] = 0;
#else
                TElement path = new TElement[MaxPath];
                TElement pathPosition = 1;
#endif
                TElement currentNode = *root;
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
                            // Threads
                            this->SetLeft(node, this->GetLeft(currentNode));
                            this->SetRight(node, currentNode);
                            this->SetLeft(currentNode, node);
                            SetLeftIsChild(currentNode, true);
                            DecrementBalance(currentNode);
                            this->SetSize(node, 1);
                            this->FixSize(currentNode); // Should be incremented already
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
                            // Threads
                            this->SetRight(node, this->GetRight(currentNode));
                            this->SetLeft(node, currentNode);
                            this->SetRight(currentNode, node);
                            SetRightIsChild(currentNode, true);
                            IncrementBalance(currentNode);
                            this->SetSize(node, 1);
                            this->FixSize(currentNode); // Should be incremented already
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
                    TElement parent = path[--pathPosition];
                    TElement isLeftNode = parent != 0 && currentNode == this->GetLeft(parent);
                    TElement currentNodeBalance = GetBalance(currentNode);
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
                TElement rootBalance = GetBalance(node);
                if (rootBalance < -1)
                {
                    TElement *left = this->GetLeft(node);
                    if (GetBalance(*left) > 0)
                    {
                        this->SetLeft(node, LeftRotateWithBalance(*left));
                        this->FixSize(node);
                    }
                    node = RightRotateWithBalance(node);
                }
                else if (rootBalance > 1)
                {
                    TElement *right = this->GetRight(node);
                    if (GetBalance(*right) < 0)
                    {
                        this->SetRight(node, RightRotateWithBalance(*right));
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
                TElement *right = this->GetRight(node);
                if (GetLeftIsChild(*right))
                {
                    this->SetRight(node, this->GetLeft(*right));
                }
                else
                {
                    SetRightIsChild(node, false);
                    SetLeftIsChild(*right, true);
                }
                this->SetLeft(*right, node);
                // Fix size
                this->SetSize(*right, this->GetSize(node));
                this->FixSize(node);
                // Fix balance
                TElement rootBalance = GetBalance(node);
                TElement rightBalance = GetBalance(*right);
                if (rightBalance <= 0)
                {
                    if (rootBalance >= 1)
                    {
                        SetBalance(*right, (std::int8_t)(rightBalance - 1));
                    }
                    else
                    {
                        SetBalance(*right, (std::int8_t)(rootBalance + rightBalance - 2));
                    }
                    SetBalance(node, (std::int8_t)(rootBalance - 1));
                }
                else
                {
                    if (rootBalance <= rightBalance)
                    {
                        SetBalance(*right, (std::int8_t)(rootBalance - 2));
                    }
                    else
                    {
                        SetBalance(*right, (std::int8_t)(rightBalance - 1));
                    }
                    SetBalance(node, (std::int8_t)(rootBalance - rightBalance - 1));
                }
                return right;
            }
        }

        TElement RightRotateWithBalance(TElement node)
        {
            {
                TElement *left = this->GetLeft(node);
                if (GetRightIsChild(*left))
                {
                    this->SetLeft(node, this->GetRight(*left));
                }
                else
                {
                    SetLeftIsChild(node, false);
                    SetRightIsChild(*left, true);
                }
                this->SetRight(*left, node);
                // Fix size
                this->SetSize(*left, this->GetSize(node));
                this->FixSize(node);
                // Fix balance
                TElement rootBalance = GetBalance(node);
                TElement leftBalance = GetBalance(*left);
                if (leftBalance <= 0)
                {
                    if (leftBalance > rootBalance)
                    {
                        SetBalance(*left, (std::int8_t)(leftBalance + 1));
                    }
                    else
                    {
                        SetBalance(*left, (std::int8_t)(rootBalance + 2));
                    }
                    SetBalance(node, (std::int8_t)(rootBalance - leftBalance + 1));
                }
                else
                {
                    if (rootBalance <= -1)
                    {
                        SetBalance(*left, (std::int8_t)(leftBalance + 1));
                    }
                    else
                    {
                        SetBalance(*left, (std::int8_t)(rootBalance + leftBalance + 2));
                    }
                    SetBalance(node, (std::int8_t)(rootBalance + 1));
                }
                return left;
            }
        }

        TElement GetNext(TElement node)
        {
            {
                TElement current = this->GetRight(node);
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
                TElement current = this->GetLeft(node);
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
                TElement path = ArrayPool.Allocate<TElement>(MaxPath);
                TElement pathPosition = 0;
                path[pathPosition++] = 0;
#else
                TElement path = new TElement[MaxPath];
                TElement pathPosition = 1;
#endif
                TElement currentNode = *root;
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
                TElement parent = path[--pathPosition];
                TElement balanceNode = parent;
                TElement isLeftNode = parent != 0 && currentNode == this->GetLeft(parent);
                if (!GetLeftIsChild(currentNode))
                {
                    if (!GetRightIsChild(currentNode)) // node has no children
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
                    else // node has a right child
                    {
                        TElement successor = GetNext(currentNode);
                        this->SetLeft(successor, this->GetLeft(currentNode));
                        TElement *right = this->GetRight(currentNode);
                        if (parent == 0)
                        {
                            *root = *right;
                        }
                        else if (isLeftNode)
                        {
                            this->SetLeft(parent, *right);
                            IncrementBalance(parent);
                        }
                        else
                        {
                            this->SetRight(parent, *right);
                            DecrementBalance(parent);
                        }
                    }
                }
                else // node has a left child
                {
                    if (!GetRightIsChild(currentNode))
                    {
                        TElement predecessor = GetPrevious(currentNode);
                        this->SetRight(predecessor, this->GetRight(currentNode));
                        TElement leftValue = this->GetLeft(currentNode);
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
                    else // node has a both children (left and right)
                    {
                        TElement predecessor = this->GetLeft(currentNode);
                        TElement successor = this->GetRight(currentNode);
                        TElement successorParent = currentNode;
                        int previousPathPosition = ++pathPosition;
                        // find the immediately next node (and its parent)
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
                        // remove 'successor' from the tree
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
                        // set the predecessor's successor link to point to the right place
                        while (GetRightIsChild(predecessor))
                        {
                            predecessor = this->GetRight(predecessor);
                        }
                        this->SetRight(predecessor, successor);
                        // prepare 'successor' to replace 'node'
                        TElement *left = this->GetLeft(currentNode);
                        SetLeftIsChild(successor, true);
                        this->SetLeft(successor, *left);
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
                // restore balance
                if (balanceNode != 0)
                {
                    while (true)
                    {
                        TElement balanceParent = path[--pathPosition];
                        isLeftNode = balanceParent != 0 && balanceNode == this->GetLeft(balanceParent);
                        TElement currentNodeBalance = GetBalance(balanceNode);
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