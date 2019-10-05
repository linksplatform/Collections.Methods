


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

        virtual TElement* GetLeftReference(TElement node) override = 0;

        virtual TElement* GetRightReference(TElement node) override = 0;

        virtual TElement GetLeft(TElement node) override = 0;

        virtual TElement GetRight(TElement node) override = 0;

        virtual TElement GetSize(TElement node) override = 0;

        virtual void SetLeft(TElement node, TElement left) override = 0;

        virtual void SetRight(TElement node, TElement right) override = 0;

        virtual void SetSize(TElement node, TElement size) override = 0;

        virtual bool FirstIsToTheLeftOfSecond(TElement first, TElement second) override = 0;

        virtual bool FirstIsToTheRightOfSecond(TElement first, TElement second) override = 0;

        // TODO: Link with size of TElement
        static const int MaxPath = 92;

        bool Contains(TElement node, TElement root) override
        {
            while (*root != 0)
            {
                if (FirstIsToTheLeftOfSecond(node, *root)) // node.Key < root.Key
                {
                    *root = GetLeftOrDefault(*root);
                }
                else if (FirstIsToTheRightOfSecond(node, *root)) // node.Key > root.Key
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
        TElement GetLeftOrDefault(TElement node) override { return GetLeftIsChild(node) ? GetLeft(node) : 0; }
        TElement GetRightOrDefault(TElement node) override { return GetRightIsChild(node) ? GetRight(node) : 0; }
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
                    if (FirstIsToTheLeftOfSecond(node, currentNode))
                    {
                        if (GetLeftIsChild(currentNode))
                        {
                            SizedBinaryTreeMethodsBase<TElement>::IncrementSize(currentNode);
                            path[pathPosition++] = currentNode;
                            currentNode = GetLeft(currentNode);
                        }
                        else
                        {
                            // Threads
                            SetLeft(node, GetLeft(currentNode));
                            SetRight(node, currentNode);
                            SetLeft(currentNode, node);
                            SetLeftIsChild(currentNode, true);
                            DecrementBalance(currentNode);
                            SetSize(node, 1);
                            SizedBinaryTreeMethodsBase<TElement>::FixSize(currentNode); // Should be incremented already
                            break;
                        }
                    }
                    else if (FirstIsToTheRightOfSecond(node, currentNode))
                    {
                        if (GetRightIsChild(currentNode))
                        {
                            SizedBinaryTreeMethodsBase<TElement>::IncrementSize(currentNode);
                            path[pathPosition++] = currentNode;
                            currentNode = GetRight(currentNode);
                        }
                        else
                        {
                            // Threads
                            SetRight(node, GetRight(currentNode));
                            SetLeft(node, currentNode);
                            SetRight(currentNode, node);
                            SetRightIsChild(currentNode, true);
                            IncrementBalance(currentNode);
                            SetSize(node, 1);
                            SizedBinaryTreeMethodsBase<TElement>::FixSize(currentNode); // Should be incremented already
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
                    TElement isLeftNode = parent != 0 && currentNode == GetLeft(parent);
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
                            SetLeft(parent, currentNode);
                            SizedBinaryTreeMethodsBase<TElement>::FixSize(parent);
                        }
                        else
                        {
                            SetRight(parent, currentNode);
                            SizedBinaryTreeMethodsBase<TElement>::FixSize(parent);
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
                    TElement *left = GetLeft(node);
                    if (GetBalance(*left) > 0)
                    {
                        SetLeft(node, LeftRotateWithBalance(*left));
                        SizedBinaryTreeMethodsBase<TElement>::FixSize(node);
                    }
                    node = RightRotateWithBalance(node);
                }
                else if (rootBalance > 1)
                {
                    TElement *right = GetRight(node);
                    if (GetBalance(*right) < 0)
                    {
                        SetRight(node, RightRotateWithBalance(*right));
                        SizedBinaryTreeMethodsBase<TElement>::FixSize(node);
                    }
                    node = LeftRotateWithBalance(node);
                }
                return node;
            }
        }

        TElement LeftRotateWithBalance(TElement node)
        {
            {
                TElement *right = GetRight(node);
                if (GetLeftIsChild(*right))
                {
                    SetRight(node, GetLeft(*right));
                }
                else
                {
                    SetRightIsChild(node, false);
                    SetLeftIsChild(*right, true);
                }
                SetLeft(*right, node);
                // Fix size
                SetSize(*right, GetSize(node));
                SizedBinaryTreeMethodsBase<TElement>::FixSize(node);
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
                TElement *left = GetLeft(node);
                if (GetRightIsChild(*left))
                {
                    SetLeft(node, GetRight(*left));
                }
                else
                {
                    SetLeftIsChild(node, false);
                    SetRightIsChild(*left, true);
                }
                SetRight(*left, node);
                // Fix size
                SetSize(*left, GetSize(node));
                SizedBinaryTreeMethodsBase<TElement>::FixSize(node);
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
                TElement current = GetRight(node);
                if (GetRightIsChild(node))
                {
                    while (GetLeftIsChild(current))
                    {
                        current = GetLeft(current);
                    }
                }
                return current;
            }
        }

        TElement GetPrevious(TElement node)
        {
            {
                TElement current = GetLeft(node);
                if (GetLeftIsChild(node))
                {
                    while (GetRightIsChild(current))
                    {
                        current = GetRight(current);
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
                    if (FirstIsToTheLeftOfSecond(node, currentNode))
                    {
                        if (!GetLeftIsChild(currentNode))
                        {
                            throw std::exception("Cannot find a node.");
                        }
                        SizedBinaryTreeMethodsBase<TElement>::DecrementSize(currentNode);
                        path[pathPosition++] = currentNode;
                        currentNode = GetLeft(currentNode);
                    }
                    else if (FirstIsToTheRightOfSecond(node, currentNode))
                    {
                        if (!GetRightIsChild(currentNode))
                        {
                            throw std::exception("Cannot find a node.");
                        }
                        SizedBinaryTreeMethodsBase<TElement>::DecrementSize(currentNode);
                        path[pathPosition++] = currentNode;
                        currentNode = GetRight(currentNode);
                    }
                    else
                    {
                        break;
                    }
                }
                TElement parent = path[--pathPosition];
                TElement balanceNode = parent;
                TElement isLeftNode = parent != 0 && currentNode == GetLeft(parent);
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
                            SetLeft(parent, GetLeft(currentNode));
                            IncrementBalance(parent);
                        }
                        else
                        {
                            SetRightIsChild(parent, false);
                            SetRight(parent, GetRight(currentNode));
                            DecrementBalance(parent);
                        }
                    }
                    else // node has a right child
                    {
                        TElement successor = GetNext(currentNode);
                        SetLeft(successor, GetLeft(currentNode));
                        TElement *right = GetRight(currentNode);
                        if (parent == 0)
                        {
                            *root = *right;
                        }
                        else if (isLeftNode)
                        {
                            SetLeft(parent, *right);
                            IncrementBalance(parent);
                        }
                        else
                        {
                            SetRight(parent, *right);
                            DecrementBalance(parent);
                        }
                    }
                }
                else // node has a left child
                {
                    if (!GetRightIsChild(currentNode))
                    {
                        TElement predecessor = GetPrevious(currentNode);
                        SetRight(predecessor, GetRight(currentNode));
                        TElement leftValue = GetLeft(currentNode);
                        if (parent == 0)
                        {
                            *root = leftValue;
                        }
                        else if (isLeftNode)
                        {
                            SetLeft(parent, leftValue);
                            IncrementBalance(parent);
                        }
                        else
                        {
                            SetRight(parent, leftValue);
                            DecrementBalance(parent);
                        }
                    }
                    else // node has a both children (left and right)
                    {
                        TElement predecessor = GetLeft(currentNode);
                        TElement successor = GetRight(currentNode);
                        TElement successorParent = currentNode;
                        int previousPathPosition = ++pathPosition;
                        // find the immediately next node (and its parent)
                        while (GetLeftIsChild(successor))
                        {
                            path[++pathPosition] = successorParent = successor;
                            successor = GetLeft(successor);
                            if (successorParent != currentNode)
                            {
                                SizedBinaryTreeMethodsBase<TElement>::DecrementSize(successorParent);
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
                                SetLeft(successorParent, GetRight(successor));
                            }
                            IncrementBalance(successorParent);
                            SetRightIsChild(successor, true);
                            SetRight(successor, GetRight(currentNode));
                        }
                        else
                        {
                            DecrementBalance(currentNode);
                        }
                        // set the predecessor's successor link to point to the right place
                        while (GetRightIsChild(predecessor))
                        {
                            predecessor = GetRight(predecessor);
                        }
                        SetRight(predecessor, successor);
                        // prepare 'successor' to replace 'node'
                        TElement *left = GetLeft(currentNode);
                        SetLeftIsChild(successor, true);
                        SetLeft(successor, *left);
                        SetBalance(successor, GetBalance(currentNode));
                        SizedBinaryTreeMethodsBase<TElement>::FixSize(successor);
                        if (parent == 0)
                        {
                            *root = successor;
                        }
                        else if (isLeftNode)
                        {
                            SetLeft(parent, successor);
                        }
                        else
                        {
                            SetRight(parent, successor);
                        }
                    }
                }
                // restore balance
                if (balanceNode != 0)
                {
                    while (true)
                    {
                        TElement balanceParent = path[--pathPosition];
                        isLeftNode = balanceParent != 0 && balanceNode == GetLeft(balanceParent);
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
                                SetLeft(balanceParent, balanceNode);
                            }
                            else
                            {
                                SetRight(balanceParent, balanceNode);
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
            SetLeft(node, 0);
            SetRight(node, 0);
            SetSize(node, 0);
            SetLeftIsChild(node, false);
            SetRightIsChild(node, false);
            SetBalance(node, 0);
        }
    };
}