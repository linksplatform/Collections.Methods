namespace Platform::Collections::Methods::Trees
{
    template <typename TSelf, typename ...> class RecursionlessSizeBalancedTreeMethods;
    template <typename TSelf, typename TElement> class RecursionlessSizeBalancedTreeMethods<TSelf, TElement> : public SizedBinaryTreeMethodsBase<TSelf, TElement>
    {
    public:
        using base = SizedBinaryTreeMethodsBase<TSelf, TElement>;
        friend base;

        protected: void AttachCore(TElement* root, TElement node)
        {
            while (true)
            {
                auto* left = this->object().GetLeftReference(*root);
                auto leftSize = this->object().GetSizeOrZero(*left);
                auto* right = this->object().GetRightReference(*root);
                auto rightSize = this->object().GetSizeOrZero(*right);
                if (this->object().FirstIsToTheLeftOfSecond(node, *root))
                {
                    if (*left == 0)
                    {
                        this->object().IncrementSize(*root);
                        this->object().SetSize(node, 1);
                        *left = node;
                        return;
                    }
                    if (this->object().FirstIsToTheLeftOfSecond(node, *left))
                    {
                        if ((leftSize + 1) > rightSize)
                        {
                            this->object().RightRotate(root);
                        }
                        else
                        {
                            this->object().IncrementSize(*root);
                            root = left;
                        }
                    }
                    else
                    {
                        auto leftRightSize = this->object().GetSizeOrZero(this->object().GetRight(*left));
                        if ((leftRightSize + 1) > rightSize)
                        {
                            if (leftRightSize == 0 && rightSize == 0)
                            {
                                this->object().SetLeft(node, *left);
                                this->object().SetRight(node, *root);
                                this->object().SetSize(node, leftSize + 2);
                                this->object().SetLeft(*root, 0);
                                this->object().SetSize(*root, 1);
                                *root = node;
                                return;
                            }
                            this->object().LeftRotate(left);
                            this->object().RightRotate(root);
                        }
                        else
                        {
                            this->object().IncrementSize(*root);
                            root = left;
                        }
                    }
                }
                else
                {
                    if (*right == 0)
                    {
                        this->object().IncrementSize(*root);
                        this->object().SetSize(node, 1);
                        *right = node;
                        return;
                    }
                    if (this->object().FirstIsToTheRightOfSecond(node, *right))
                    {
                        if ((rightSize + 1) > leftSize)
                        {
                            this->object().LeftRotate(root);
                        }
                        else
                        {
                            this->object().IncrementSize(*root);
                            root = right;
                        }
                    }
                    else
                    {
                        auto rightLeftSize = this->object().GetSizeOrZero(this->object().GetLeft(*right));
                        if ((rightLeftSize + 1) > leftSize)
                        {
                            if (rightLeftSize == 0 && leftSize == 0)
                            {
                                this->object().SetLeft(node, *root);
                                this->object().SetRight(node, *right);
                                this->object().SetSize(node, rightSize + 2);
                                this->object().SetRight(*root, 0);
                                this->object().SetSize(*root, 1);
                                *root = node;
                                return;
                            }
                            this->object().RightRotate(right);
                            this->object().LeftRotate(root);
                        }
                        else
                        {
                            this->object().IncrementSize(*root);
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
                auto* left = this->object().GetLeftReference(*root);
                auto leftSize = this->object().GetSizeOrZero(*left);
                auto* right = this->object().GetRightReference(*root);
                auto rightSize = this->object().GetSizeOrZero(*right);
                if (this->object().FirstIsToTheLeftOfSecond(node, *root))
                {
                    auto decrementedLeftSize = leftSize - 1;
                    if (this->object().GetSizeOrZero(this->object().GetRightOrDefault(*right)) > decrementedLeftSize)
                    {
                        this->object().LeftRotate(root);
                    }
                    else if (this->object().GetSizeOrZero(this->object().GetLeftOrDefault(*right)) > decrementedLeftSize)
                    {
                        this->object().RightRotate(right);
                        this->object().LeftRotate(root);
                    }
                    else
                    {
                        this->object().DecrementSize(*root);
                        root = left;
                    }
                }
                else if (this->object().FirstIsToTheRightOfSecond(node, *root))
                {
                    auto decrementedRightSize = rightSize - 1;
                    if (this->object().GetSizeOrZero(this->object().GetLeftOrDefault(*left)) > decrementedRightSize)
                    {
                        this->object().RightRotate(root);
                    }
                    else if (this->object().GetSizeOrZero(this->object().GetRightOrDefault(*left)) > decrementedRightSize)
                    {
                        this->object().LeftRotate(left);
                        this->object().RightRotate(root);
                    }
                    else
                    {
                        this->object().DecrementSize(*root);
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
                            replacement = this->object().GetRightest(*left);
                            this->object().DetachCore(left, replacement);
                        }
                        else
                        {
                            replacement = this->object().GetLeftest(*right);
                            this->object().DetachCore(right, replacement);
                        }
                        this->object().SetLeft(replacement, *left);
                        this->object().SetRight(replacement, *right);
                        this->object().SetSize(replacement, leftSize + rightSize);
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
                    this->object().ClearNode(node);
                    return;
                }
            }
        }
    };
}
