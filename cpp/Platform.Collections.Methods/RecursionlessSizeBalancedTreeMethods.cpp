namespace Platform::Collections::Methods::Trees
{
    template <typename TElement> class RecursionlessSizeBalancedTreeMethods : public SizedBinaryTreeMethodsBase<TElement>
    {
    public:
        void AttachCore(TElement* root, TElement node) override
        {
            while (true)
            {
                auto* left = this->GetLeftReference(*root);
                auto leftSize = this->GetSizeOrZero(*left);
                auto* right = this->GetRightReference(*root);
                auto rightSize = this->GetSizeOrZero(*right);
                if (this->FirstIsToTheLeftOfSecond(node, *root))
                {
                    if (*left == 0)
                    {
                        this->IncrementSize(*root);
                        this->SetSize(node, 1);
                        *left = node;
                        return;
                    }
                    if (this->FirstIsToTheLeftOfSecond(node, *left))
                    {
                        if ((leftSize + 1) > rightSize)
                        {
                            this->RightRotate(root);
                        }
                        else
                        {
                            this->IncrementSize(*root);
                            root = left;
                        }
                    }
                    else
                    {
                        auto leftRightSize = this->GetSizeOrZero(this->GetRight(*left));
                        if ((leftRightSize + 1) > rightSize)
                        {
                            if (leftRightSize == 0 && rightSize == 0)
                            {
                                this->SetLeft(node, *left);
                                this->SetRight(node, *root);
                                this->SetSize(node, leftSize + 2);
                                this->SetLeft(*root, 0);
                                this->SetSize(*root, 1);
                                *root = node;
                                return;
                            }
                            this->LeftRotate(left);
                            this->RightRotate(root);
                        }
                        else
                        {
                            this->IncrementSize(*root);
                            root = left;
                        }
                    }
                }
                else
                {
                    if (*right == 0)
                    {
                        this->IncrementSize(*root);
                        this->SetSize(node, 1);
                        *right = node;
                        return;
                    }
                    if (this->FirstIsToTheRightOfSecond(node, *right))
                    {
                        if ((rightSize + 1) > leftSize)
                        {
                            this->LeftRotate(root);
                        }
                        else
                        {
                            this->IncrementSize(*root);
                            root = right;
                        }
                    }
                    else
                    {
                        auto rightLeftSize = this->GetSizeOrZero(this->GetLeft(*right));
                        if ((rightLeftSize + 1) > leftSize)
                        {
                            if (rightLeftSize == 0 && leftSize == 0)
                            {
                                this->SetLeft(node, *root);
                                this->SetRight(node, *right);
                                this->SetSize(node, rightSize + 2);
                                this->SetRight(*root, 0);
                                this->SetSize(*root, 1);
                                *root = node;
                                return;
                            }
                            this->RightRotate(right);
                            this->LeftRotate(root);
                        }
                        else
                        {
                            this->IncrementSize(*root);
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
                auto* left = this->GetLeftReference(*root);
                auto leftSize = this->GetSizeOrZero(*left);
                auto* right = this->GetRightReference(*root);
                auto rightSize = this->GetSizeOrZero(*right);
                if (this->FirstIsToTheLeftOfSecond(node, *root))
                {
                    auto decrementedLeftSize = leftSize - 1;
                    if (this->GetSizeOrZero(this->GetRight(*right)) > decrementedLeftSize)
                    {
                        this->LeftRotate(root);
                    }
                    else if (this->GetSizeOrZero(this->GetLeft(*right)) > decrementedLeftSize)
                    {
                        this->RightRotate(right);
                        this->LeftRotate(root);
                    }
                    else
                    {
                        this->DecrementSize(*root);
                        root = left;
                    }
                }
                else if (this->FirstIsToTheRightOfSecond(node, *root))
                {
                    auto decrementedRightSize = rightSize - 1;
                    if (this->GetSizeOrZero(this->GetLeft(*left)) > decrementedRightSize)
                    {
                        this->RightRotate(root);
                    }
                    else if (this->GetSizeOrZero(this->GetRight(*left)) > decrementedRightSize)
                    {
                        this->LeftRotate(left);
                        this->RightRotate(root);
                    }
                    else
                    {
                        this->DecrementSize(*root);
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
                            replacement = this->GetRightest(*left);
                            this->DetachCore(left, replacement);
                        }
                        else
                        {
                            replacement = this->GetLeftest(*right);
                            this->DetachCore(right, replacement);
                        }
                        this->SetLeft(replacement, *left);
                        this->SetRight(replacement, *right);
                        this->SetSize(replacement, leftSize + rightSize);
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
                    this->ClearNode(node);
                    return;
                }
            }
        }
    };
}