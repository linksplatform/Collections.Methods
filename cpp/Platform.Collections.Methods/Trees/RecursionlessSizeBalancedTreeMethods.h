namespace Platform::Collections::Methods::Trees
{
    template <class impl_t, typename ...> class RecursionlessSizeBalancedTreeMethods;
    template <class impl_t, typename TElement> class RecursionlessSizeBalancedTreeMethods<impl_t, TElement> : public SizedBinaryTreeMethodsBase<impl_t, TElement>
    {
        using base_t = SizedBinaryTreeMethodsBase<impl_t, TElement>;
        friend base_t;

        protected: void AttachCore(TElement* root, TElement node)
        {
            while (true)
            {
                auto* left = static_cast<impl_t*>(this)->GetLeftReference(*root);
                auto leftSize = static_cast<impl_t*>(this)->GetSizeOrZero(*left);
                auto* right = static_cast<impl_t*>(this)->GetRightReference(*root);
                auto rightSize = static_cast<impl_t*>(this)->GetSizeOrZero(*right);
                if (static_cast<impl_t*>(this)->FirstIsToTheLeftOfSecond(node, *root))
                {
                    if (*left == 0)
                    {
                        static_cast<impl_t*>(this)->IncrementSize(*root);
                        static_cast<impl_t*>(this)->SetSize(node, 1);
                        *left = node;
                        return;
                    }
                    if (static_cast<impl_t*>(this)->FirstIsToTheLeftOfSecond(node, *left))
                    {
                        if ((leftSize + 1) > rightSize)
                        {
                            static_cast<impl_t*>(this)->RightRotate(root);
                        }
                        else
                        {
                            static_cast<impl_t*>(this)->IncrementSize(*root);
                            root = left;
                        }
                    }
                    else
                    {
                        auto leftRightSize = static_cast<impl_t*>(this)->GetSizeOrZero(static_cast<impl_t*>(this)->GetRight(*left));
                        if ((leftRightSize + 1) > rightSize)
                        {
                            if (leftRightSize == 0 && rightSize == 0)
                            {
                                static_cast<impl_t*>(this)->SetLeft(node, *left);
                                static_cast<impl_t*>(this)->SetRight(node, *root);
                                static_cast<impl_t*>(this)->SetSize(node, leftSize + 2);
                                static_cast<impl_t*>(this)->SetLeft(*root, 0);
                                static_cast<impl_t*>(this)->SetSize(*root, 1);
                                *root = node;
                                return;
                            }
                            static_cast<impl_t*>(this)->LeftRotate(left);
                            static_cast<impl_t*>(this)->RightRotate(root);
                        }
                        else
                        {
                            static_cast<impl_t*>(this)->IncrementSize(*root);
                            root = left;
                        }
                    }
                }
                else
                {
                    if (*right == 0)
                    {
                        static_cast<impl_t*>(this)->IncrementSize(*root);
                        static_cast<impl_t*>(this)->SetSize(node, 1);
                        *right = node;
                        return;
                    }
                    if (static_cast<impl_t*>(this)->FirstIsToTheRightOfSecond(node, *right))
                    {
                        if ((rightSize + 1) > leftSize)
                        {
                            static_cast<impl_t*>(this)->LeftRotate(root);
                        }
                        else
                        {
                            static_cast<impl_t*>(this)->IncrementSize(*root);
                            root = right;
                        }
                    }
                    else
                    {
                        auto rightLeftSize = static_cast<impl_t*>(this)->GetSizeOrZero(static_cast<impl_t*>(this)->GetLeft(*right));
                        if ((rightLeftSize + 1) > leftSize)
                        {
                            if (rightLeftSize == 0 && leftSize == 0)
                            {
                                static_cast<impl_t*>(this)->SetLeft(node, *root);
                                static_cast<impl_t*>(this)->SetRight(node, *right);
                                static_cast<impl_t*>(this)->SetSize(node, rightSize + 2);
                                static_cast<impl_t*>(this)->SetRight(*root, 0);
                                static_cast<impl_t*>(this)->SetSize(*root, 1);
                                *root = node;
                                return;
                            }
                            static_cast<impl_t*>(this)->RightRotate(right);
                            static_cast<impl_t*>(this)->LeftRotate(root);
                        }
                        else
                        {
                            static_cast<impl_t*>(this)->IncrementSize(*root);
                            root = right;
                        }
                    }
                }
            }
        }

        protected: void DetachCore(TElement* root, TElement node)
        {
            while (true)
            {
                auto* left = static_cast<impl_t*>(this)->GetLeftReference(*root);
                auto leftSize = static_cast<impl_t*>(this)->GetSizeOrZero(*left);
                auto* right = static_cast<impl_t*>(this)->GetRightReference(*root);
                auto rightSize = static_cast<impl_t*>(this)->GetSizeOrZero(*right);
                if (static_cast<impl_t*>(this)->FirstIsToTheLeftOfSecond(node, *root))
                {
                    auto decrementedLeftSize = leftSize - 1;
                    if (static_cast<impl_t*>(this)->GetSizeOrZero(static_cast<impl_t*>(this)->GetRightOrDefault(*right)) > decrementedLeftSize)
                    {
                        static_cast<impl_t*>(this)->LeftRotate(root);
                    }
                    else if (static_cast<impl_t*>(this)->GetSizeOrZero(static_cast<impl_t*>(this)->GetLeftOrDefault(*right)) > decrementedLeftSize)
                    {
                        static_cast<impl_t*>(this)->RightRotate(right);
                        static_cast<impl_t*>(this)->LeftRotate(root);
                    }
                    else
                    {
                        static_cast<impl_t*>(this)->DecrementSize(*root);
                        root = left;
                    }
                }
                else if (static_cast<impl_t*>(this)->FirstIsToTheRightOfSecond(node, *root))
                {
                    auto decrementedRightSize = rightSize - 1;
                    if (static_cast<impl_t*>(this)->GetSizeOrZero(static_cast<impl_t*>(this)->GetLeftOrDefault(*left)) > decrementedRightSize)
                    {
                        static_cast<impl_t*>(this)->RightRotate(root);
                    }
                    else if (static_cast<impl_t*>(this)->GetSizeOrZero(static_cast<impl_t*>(this)->GetRightOrDefault(*left)) > decrementedRightSize)
                    {
                        static_cast<impl_t*>(this)->LeftRotate(left);
                        static_cast<impl_t*>(this)->RightRotate(root);
                    }
                    else
                    {
                        static_cast<impl_t*>(this)->DecrementSize(*root);
                        root = right;
                    }
                }
                else
                {
                    if (leftSize > 0 && rightSize > 0)
                    {
                        TElement replacement = 0;
                        if (leftSize > rightSize)
                        {
                            replacement = static_cast<impl_t*>(this)->GetRightest(*left);
                            static_cast<impl_t*>(this)->DetachCore(left, replacement);
                        }
                        else
                        {
                            replacement = static_cast<impl_t*>(this)->GetLeftest(*right);
                            static_cast<impl_t*>(this)->DetachCore(right, replacement);
                        }
                        static_cast<impl_t*>(this)->SetLeft(replacement, *left);
                        static_cast<impl_t*>(this)->SetRight(replacement, *right);
                        static_cast<impl_t*>(this)->SetSize(replacement, leftSize + rightSize);
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
                    static_cast<impl_t*>(this)->ClearNode(node);
                    return;
                }
            }
        }
    };
}