namespace Platform::Collections::Methods::Trees
{
    template <class impl_t, typename ...> class RecursionlessSizeBalancedTreeMethods;
    template <class impl_t, typename TElement> class RecursionlessSizeBalancedTreeMethods<impl_t, TElement> : public SizedBinaryTreeMethodsBase<impl_t, TElement>
    {
    public:
        using base = SizedBinaryTreeMethodsBase<impl_t, TElement>;
        friend base;
        using Polymorph<impl_t>::object;

        protected: void AttachCore(TElement* root, TElement node)
        {
            while (true)
            {
                auto* left = object().GetLeftReference(*root);
                auto leftSize = object().GetSizeOrZero(*left);
                auto* right = object().GetRightReference(*root);
                auto rightSize = object().GetSizeOrZero(*right);
                if (object().FirstIsToTheLeftOfSecond(node, *root))
                {
                    if (*left == 0)
                    {
                        object().IncrementSize(*root);
                        object().SetSize(node, 1);
                        *left = node;
                        return;
                    }
                    if (object().FirstIsToTheLeftOfSecond(node, *left))
                    {
                        if ((leftSize + 1) > rightSize)
                        {
                            object().RightRotate(root);
                        }
                        else
                        {
                            object().IncrementSize(*root);
                            root = left;
                        }
                    }
                    else
                    {
                        auto leftRightSize = object().GetSizeOrZero(object().GetRight(*left));
                        if ((leftRightSize + 1) > rightSize)
                        {
                            if (leftRightSize == 0 && rightSize == 0)
                            {
                                object().SetLeft(node, *left);
                                object().SetRight(node, *root);
                                object().SetSize(node, leftSize + 2);
                                object().SetLeft(*root, 0);
                                object().SetSize(*root, 1);
                                *root = node;
                                return;
                            }
                            object().LeftRotate(left);
                            object().RightRotate(root);
                        }
                        else
                        {
                            object().IncrementSize(*root);
                            root = left;
                        }
                    }
                }
                else
                {
                    if (*right == 0)
                    {
                        object().IncrementSize(*root);
                        object().SetSize(node, 1);
                        *right = node;
                        return;
                    }
                    if (object().FirstIsToTheRightOfSecond(node, *right))
                    {
                        if ((rightSize + 1) > leftSize)
                        {
                            object().LeftRotate(root);
                        }
                        else
                        {
                            object().IncrementSize(*root);
                            root = right;
                        }
                    }
                    else
                    {
                        auto rightLeftSize = object().GetSizeOrZero(object().GetLeft(*right));
                        if ((rightLeftSize + 1) > leftSize)
                        {
                            if (rightLeftSize == 0 && leftSize == 0)
                            {
                                object().SetLeft(node, *root);
                                object().SetRight(node, *right);
                                object().SetSize(node, rightSize + 2);
                                object().SetRight(*root, 0);
                                object().SetSize(*root, 1);
                                *root = node;
                                return;
                            }
                            object().RightRotate(right);
                            object().LeftRotate(root);
                        }
                        else
                        {
                            object().IncrementSize(*root);
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
                auto* left = object().GetLeftReference(*root);
                auto leftSize = object().GetSizeOrZero(*left);
                auto* right = object().GetRightReference(*root);
                auto rightSize = object().GetSizeOrZero(*right);
                if (object().FirstIsToTheLeftOfSecond(node, *root))
                {
                    auto decrementedLeftSize = leftSize - 1;
                    if (object().GetSizeOrZero(object().GetRightOrDefault(*right)) > decrementedLeftSize)
                    {
                        object().LeftRotate(root);
                    }
                    else if (object().GetSizeOrZero(object().GetLeftOrDefault(*right)) > decrementedLeftSize)
                    {
                        object().RightRotate(right);
                        object().LeftRotate(root);
                    }
                    else
                    {
                        object().DecrementSize(*root);
                        root = left;
                    }
                }
                else if (object().FirstIsToTheRightOfSecond(node, *root))
                {
                    auto decrementedRightSize = rightSize - 1;
                    if (object().GetSizeOrZero(object().GetLeftOrDefault(*left)) > decrementedRightSize)
                    {
                        object().RightRotate(root);
                    }
                    else if (object().GetSizeOrZero(object().GetRightOrDefault(*left)) > decrementedRightSize)
                    {
                        object().LeftRotate(left);
                        object().RightRotate(root);
                    }
                    else
                    {
                        object().DecrementSize(*root);
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
                            replacement = object().GetRightest(*left);
                            object().DetachCore(left, replacement);
                        }
                        else
                        {
                            replacement = object().GetLeftest(*right);
                            object().DetachCore(right, replacement);
                        }
                        object().SetLeft(replacement, *left);
                        object().SetRight(replacement, *right);
                        object().SetSize(replacement, leftSize + rightSize);
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
                    object().ClearNode(node);
                    return;
                }
            }
        }
    };
}
