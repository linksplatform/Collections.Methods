

namespace Platform::Collections::Methods::Trees
{
    template <typename TElement> class SizeBalancedTreeMethods : public SizedBinaryTreeMethodsBase<TElement>
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

        void AttachCore(TElement* root, TElement node) override
        {
            while (true)
            {
                TElement* left = GetLeftReference(*root);
                TElement leftSize = SizedBinaryTreeMethodsBase<TElement>::GetSizeOrZero(*left);
                TElement* right = GetRightReference(*root);
                TElement rightSize = SizedBinaryTreeMethodsBase<TElement>::GetSizeOrZero(*right);
                if (FirstIsToTheLeftOfSecond(node, *root)) // node.Key less than root.Key
                {
                    if (*left == 0)
                    {
                        SizedBinaryTreeMethodsBase<TElement>::IncrementSize(*root);
                        SetSize(node, 1);
                        *left = node;
                        return;
                    }
                    if (FirstIsToTheLeftOfSecond(node, *left)) // node.Key less than left.Key
                    {
                        if ((leftSize + 1) > rightSize)
                        {
                            SizedBinaryTreeMethodsBase<TElement>::RightRotate(root);
                        }
                        else
                        {
                            SizedBinaryTreeMethodsBase<TElement>::IncrementSize(*root);
                            root = left;
                        }
                    }
                    else  // node.Key greater than left.Key
                    {
                        TElement leftRightSize = SizedBinaryTreeMethodsBase<TElement>::GetSizeOrZero(GetRight(*left));
                        if ((leftRightSize + 1) > rightSize)
                        {
                            if (leftRightSize == 0 && rightSize == 0)
                            {
                                SetLeft(node, *left);
                                SetRight(node, *root);
                                SetSize(node, leftSize + 2); // 2 (2) - node the size of root and a node itself
                                SetLeft(*root, 0);
                                SetSize(*root, 1);
                                *root = node;
                                return;
                            }
                            SizedBinaryTreeMethodsBase<TElement>::LeftRotate(left);
                            SizedBinaryTreeMethodsBase<TElement>::RightRotate(root);
                        }
                        else
                        {
                            SizedBinaryTreeMethodsBase<TElement>::IncrementSize(*root);
                            root = left;
                        }
                    }
                }
                else // node.Key greater than root.Key
                {
                    if (*right == 0)
                    {
                        SizedBinaryTreeMethodsBase<TElement>::IncrementSize(*root);
                        SetSize(node, 1);
                        *right = node;
                        return;
                    }
                    if (FirstIsToTheRightOfSecond(node, *right)) // node.Key greater than right.Key
                    {
                        if ((rightSize + 1) > leftSize)
                        {
                            SizedBinaryTreeMethodsBase<TElement>::LeftRotate(root);
                        }
                        else
                        {
                            SizedBinaryTreeMethodsBase<TElement>::IncrementSize(*root);
                            root = right;
                        }
                    }
                    else // node.Key less than right.Key
                    {
                        TElement rightLeftSize = SizedBinaryTreeMethodsBase<TElement>::GetSizeOrZero(GetLeft(*right));
                        if ((rightLeftSize + 1) > leftSize)
                        {
                            if (rightLeftSize == 0 && leftSize == 0)
                            {
                                SetLeft(node, *root);
                                SetRight(node, *right);
                                SetSize(node, rightSize + 2); // 2 (2) - node the size of root and a node itself
                                SetRight(*root, 0);
                                SetSize(*root, 1);
                                *root = node;
                                return;
                            }
                            SizedBinaryTreeMethodsBase<TElement>::RightRotate(right);
                            SizedBinaryTreeMethodsBase<TElement>::LeftRotate(root);
                        }
                        else
                        {
                            SizedBinaryTreeMethodsBase<TElement>::IncrementSize(*root);
                            root = right;
                        }
                    }
                }
            }
        }

        void DetachCore(TElement* root, TElement node) override
        {
            while (true)
            {
                TElement* left = GetLeftReference(*root);
                TElement leftSize = SizedBinaryTreeMethodsBase<TElement>::GetSizeOrZero(*left);
                TElement* right = GetRightReference(*root);
                TElement rightSize = SizedBinaryTreeMethodsBase<TElement>::GetSizeOrZero(*right);
                if (FirstIsToTheLeftOfSecond(node, *root)) // node.Key less than root.Key
                {
                    TElement decrementedLeftSize = leftSize - 1;
                    if (SizedBinaryTreeMethodsBase<TElement>::GetSizeOrZero(GetRight(*right)) > decrementedLeftSize)
                    {
                        SizedBinaryTreeMethodsBase<TElement>::LeftRotate(root);
                    }
                    else if (SizedBinaryTreeMethodsBase<TElement>::GetSizeOrZero(GetLeft(*right)) > decrementedLeftSize)
                    {
                        SizedBinaryTreeMethodsBase<TElement>::RightRotate(right);
                        SizedBinaryTreeMethodsBase<TElement>::LeftRotate(root);
                    }
                    else
                    {
                        SizedBinaryTreeMethodsBase<TElement>::DecrementSize(*root);
                        root = left;
                    }
                }
                else if (FirstIsToTheRightOfSecond(node, *root)) // node.Key greater than root.Key
                {
                    TElement decrementedRightSize = rightSize - 1;
                    if (SizedBinaryTreeMethodsBase<TElement>::GetSizeOrZero(GetLeft(*left)) > decrementedRightSize)
                    {
                        SizedBinaryTreeMethodsBase<TElement>::RightRotate(root);
                    }
                    else if (SizedBinaryTreeMethodsBase<TElement>::GetSizeOrZero(GetRight(*left)) > decrementedRightSize)
                    {
                        SizedBinaryTreeMethodsBase<TElement>::LeftRotate(left);
                        SizedBinaryTreeMethodsBase<TElement>::RightRotate(root);
                    }
                    else
                    {
                        SizedBinaryTreeMethodsBase<TElement>::DecrementSize(*root);
                        root = right;
                    }
                }
                else // key equals to root.Key
                {
                    if (leftSize > 0 && rightSize > 0)
                    {
                        TElement replacement;
                        if (leftSize > rightSize)
                        {
                            replacement = *left;
                            TElement replacementRight = GetRight(replacement);
                            while (replacementRight != 0)
                            {
                                replacement = replacementRight;
                                replacementRight = GetRight(replacement);
                            }
                            DetachCore(left, replacement);
                        }
                        else
                        {
                            replacement = *right;
                            TElement replacementLeft = GetLeft(replacement);
                            while (replacementLeft != 0)
                            {
                                replacement = replacementLeft;
                                replacementLeft = GetLeft(replacement);
                            }
                            DetachCore(right, replacement);
                        }
                        SetLeft(replacement, *left);
                        SetRight(replacement, *right);
                        SetSize(replacement, leftSize + rightSize);
                        *root = replacement;
                    }
                    else if (leftSize > 0)
                    {
                        *root = *left;
                    }
                    else if (rightSize > 0)
                    {
                        *root = *right;
                    }
                    else
                    {
                        *root = 0;
                    }
                    SizedBinaryTreeMethodsBase<TElement>::ClearNode(node);
                    return;
                }
            }
        }
    };
}