

namespace Platform::Collections::Methods::Trees
{
    template <typename TElement> class SizeBalancedTreeMethods : SizedBinaryTreeMethodsBase<TElement>
    {
        void AttachCore(TElement* root, TElement node) override
        {
            while (true)
            {
                auto* left = SizedBinaryTreeMethodsBase::GetLeftReference(root);
                auto leftSize = SizedBinaryTreeMethodsBase::GetSizeOrZero(left);
                auto* right = SizedBinaryTreeMethodsBase::GetRightReference(root);
                auto rightSize = SizedBinaryTreeMethodsBase::GetSizeOrZero(right);
                if (SizedBinaryTreeMethodsBase::FirstIsToTheLeftOfSecond(node, root)) // node.Key less than root.Key
                {
                    if (left == 0)
                    {
                        SizedBinaryTreeMethodsBase::IncrementSize(root);
                        SizedBinaryTreeMethodsBase::SetSize(node, 1);
                        left = node;
                        return;
                    }
                    if (SizedBinaryTreeMethodsBase::FirstIsToTheLeftOfSecond(node, left)) // node.Key less than left.Key
                    {
                        if ((leftSize + 1) > rightSize)
                        {
                            SizedBinaryTreeMethodsBase::RightRotate(root);
                        }
                        else
                        {
                            SizedBinaryTreeMethodsBase::IncrementSize(root);
                            root = left;
                        }
                    }
                    else  // node.Key greater than left.Key
                    {
                        auto leftRightSize = SizedBinaryTreeMethodsBase::GetSizeOrZero(SizedBinaryTreeMethodsBase::GetRight(left));
                        if ((leftRightSize + 1) > rightSize)
                        {
                            if (leftRightSize == 0 && rightSize == 0)
                            {
                                SizedBinaryTreeMethodsBase::SetLeft(node, left);
                                SizedBinaryTreeMethodsBase::SetRight(node, root);
                                SizedBinaryTreeMethodsBase::SetSize(node, leftSize + 2); // 2 (2) - node the size of root and a node itself
                                SizedBinaryTreeMethodsBase::SetLeft(root, 0);
                                SizedBinaryTreeMethodsBase::SetSize(root, 1);
                                root = node;
                                return;
                            }
                            SizedBinaryTreeMethodsBase::LeftRotate(left);
                            SizedBinaryTreeMethodsBase::RightRotate(root);
                        }
                        else
                        {
                            SizedBinaryTreeMethodsBase::IncrementSize(root);
                            root = left;
                        }
                    }
                }
                else // node.Key greater than root.Key
                {
                    if (right == 0)
                    {
                        SizedBinaryTreeMethodsBase::IncrementSize(root);
                        SizedBinaryTreeMethodsBase::SetSize(node, 1);
                        right = node;
                        return;
                    }
                    if (SizedBinaryTreeMethodsBase::FirstIsToTheRightOfSecond(node, right)) // node.Key greater than right.Key
                    {
                        if ((rightSize + 1) > leftSize)
                        {
                            SizedBinaryTreeMethodsBase::LeftRotate(root);
                        }
                        else
                        {
                            SizedBinaryTreeMethodsBase::IncrementSize(root);
                            root = right;
                        }
                    }
                    else // node.Key less than right.Key
                    {
                        auto rightLeftSize = SizedBinaryTreeMethodsBase::GetSizeOrZero(SizedBinaryTreeMethodsBase::GetLeft(right));
                        if ((rightLeftSize + 1) > leftSize)
                        {
                            if (rightLeftSize == 0 && leftSize == 0)
                            {
                                SizedBinaryTreeMethodsBase::SetLeft(node, root);
                                SizedBinaryTreeMethodsBase::SetRight(node, right);
                                SizedBinaryTreeMethodsBase::SetSize(node, rightSize + 2); // 2 (2) - node the size of root and a node itself
                                SizedBinaryTreeMethodsBase::SetRight(root, 0);
                                SizedBinaryTreeMethodsBase::SetSize(root, 1);
                                root = node;
                                return;
                            }
                            SizedBinaryTreeMethodsBase::RightRotate(right);
                            SizedBinaryTreeMethodsBase::LeftRotate(root);
                        }
                        else
                        {
                            SizedBinaryTreeMethodsBase::IncrementSize(root);
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
                auto* left = SizedBinaryTreeMethodsBase::GetLeftReference(root);
                auto leftSize = SizedBinaryTreeMethodsBase::GetSizeOrZero(left);
                auto* right = SizedBinaryTreeMethodsBase::GetRightReference(root);
                auto rightSize = SizedBinaryTreeMethodsBase::GetSizeOrZero(right);
                if (SizedBinaryTreeMethodsBase::FirstIsToTheLeftOfSecond(node, root)) // node.Key less than root.Key
                {
                    auto decrementedLeftSize = leftSize - 1;
                    if (SizedBinaryTreeMethodsBase::GetSizeOrZero(SizedBinaryTreeMethodsBase::GetRight(right)) > decrementedLeftSize)
                    {
                        SizedBinaryTreeMethodsBase::LeftRotate(root);
                    }
                    else if (SizedBinaryTreeMethodsBase::GetSizeOrZero(SizedBinaryTreeMethodsBase::GetLeft(right)) > decrementedLeftSize)
                    {
                        SizedBinaryTreeMethodsBase::RightRotate(right);
                        SizedBinaryTreeMethodsBase::LeftRotate(root);
                    }
                    else
                    {
                        SizedBinaryTreeMethodsBase::DecrementSize(root);
                        root = left;
                    }
                }
                else if (SizedBinaryTreeMethodsBase::FirstIsToTheRightOfSecond(node, root)) // node.Key greater than root.Key
                {
                    auto decrementedRightSize = rightSize - 1;
                    if (SizedBinaryTreeMethodsBase::GetSizeOrZero(SizedBinaryTreeMethodsBase::GetLeft(left)) > decrementedRightSize)
                    {
                        SizedBinaryTreeMethodsBase::RightRotate(root);
                    }
                    else if (SizedBinaryTreeMethodsBase::GetSizeOrZero(SizedBinaryTreeMethodsBase::GetRight(left)) > decrementedRightSize)
                    {
                        SizedBinaryTreeMethodsBase::LeftRotate(left);
                        SizedBinaryTreeMethodsBase::RightRotate(root);
                    }
                    else
                    {
                        SizedBinaryTreeMethodsBase::DecrementSize(root);
                        root = right;
                    }
                }
                else // key equals to root.Key
                {
                    if (leftSize > 0 && rightSize > 0)
                    {
                        TElement replacement;
                        if (GreaterThan(leftSize, rightSize))
                        {
                            replacement = left;
                            auto replacementRight = SizedBinaryTreeMethodsBase::GetRight(replacement);
                            while (replacementRight != 0)
                            {
                                replacement = replacementRight;
                                replacementRight = SizedBinaryTreeMethodsBase::GetRight(replacement);
                            }
                            DetachCore(left, replacement);
                        }
                        else
                        {
                            replacement = right;
                            auto replacementLeft = SizedBinaryTreeMethodsBase::GetLeft(replacement);
                            while (replacementLeft != 0)
                            {
                                replacement = replacementLeft;
                                replacementLeft = SizedBinaryTreeMethodsBase::GetLeft(replacement);
                            }
                            DetachCore(right, replacement);
                        }
                        SizedBinaryTreeMethodsBase::SetLeft(replacement, left);
                        SizedBinaryTreeMethodsBase::SetRight(replacement, right);
                        SizedBinaryTreeMethodsBase::SetSize(replacement, leftSize + rightSize);
                        root = replacement;
                    }
                    else if (leftSize > 0)
                    {
                        root = left;
                    }
                    else if (rightSize > 0)
                    {
                        root = right;
                    }
                    else
                    {
                        root = 0;
                    }
                    SizedBinaryTreeMethodsBase::ClearNode(node);
                    return;
                }
            }
        }
    };
}