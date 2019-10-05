

namespace Platform::Collections::Methods::Trees
{
    template <typename TElement> class SizeBalancedTreeMethods : public SizedBinaryTreeMethodsBase<TElement>
    { public:
        void AttachCore(TElement* root, TElement node) override
        {
            while (true)
            {
                TElement* left = this->GetLeftReference(*root);
                TElement leftSize = this->GetSizeOrZero(*left);
                TElement* right = this->GetRightReference(*root);
                TElement rightSize = this->GetSizeOrZero(*right);
                if (this->FirstIsToTheLeftOfSecond(node, *root)) // node.Key less than root.Key
                {
                    if (*left == 0)
                    {
                        this->IncrementSize(*root);
                        this->SetSize(node, 1);
                        *left = node;
                        return;
                    }
                    if (this->FirstIsToTheLeftOfSecond(node, *left)) // node.Key less than left.Key
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
                    else  // node.Key greater than left.Key
                    {
                        TElement leftRightSize = this->GetSizeOrZero(this->GetRight(*left));
                        if ((leftRightSize + 1) > rightSize)
                        {
                            if (leftRightSize == 0 && rightSize == 0)
                            {
                                this->SetLeft(node, *left);
                                this->SetRight(node, *root);
                                this->SetSize(node, leftSize + 2); // 2 (2) - node the size of root and a node itself
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
                else // node.Key greater than root.Key
                {
                    if (*right == 0)
                    {
                        this->IncrementSize(*root);
                        this->SetSize(node, 1);
                        *right = node;
                        return;
                    }
                    if (this->FirstIsToTheRightOfSecond(node, *right)) // node.Key greater than right.Key
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
                    else // node.Key less than right.Key
                    {
                        TElement rightLeftSize = this->GetSizeOrZero(this->GetLeft(*right));
                        if ((rightLeftSize + 1) > leftSize)
                        {
                            if (rightLeftSize == 0 && leftSize == 0)
                            {
                                this->SetLeft(node, *root);
                                this->SetRight(node, *right);
                                this->SetSize(node, rightSize + 2); // 2 (2) - node the size of root and a node itself
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
                TElement* left = this->GetLeftReference(*root);
                TElement leftSize = this->GetSizeOrZero(*left);
                TElement* right = this->GetRightReference(*root);
                TElement rightSize = this->GetSizeOrZero(*right);
                if (this->FirstIsToTheLeftOfSecond(node, *root)) // node.Key less than root.Key
                {
                    TElement decrementedLeftSize = leftSize - 1;
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
                else if (this->FirstIsToTheRightOfSecond(node, *root)) // node.Key greater than root.Key
                {
                    TElement decrementedRightSize = rightSize - 1;
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
                else // key equals to root.Key
                {
                    if (leftSize > 0 && rightSize > 0)
                    {
                        TElement replacement;
                        if (leftSize > rightSize)
                        {
                            replacement = *left;
                            TElement replacementRight = this->GetRight(replacement);
                            while (replacementRight != 0)
                            {
                                replacement = replacementRight;
                                replacementRight = this->GetRight(replacement);
                            }
                            DetachCore(left, replacement);
                        }
                        else
                        {
                            replacement = *right;
                            TElement replacementLeft = this->GetLeft(replacement);
                            while (replacementLeft != 0)
                            {
                                replacement = replacementLeft;
                                replacementLeft = this->GetLeft(replacement);
                            }
                            DetachCore(right, replacement);
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