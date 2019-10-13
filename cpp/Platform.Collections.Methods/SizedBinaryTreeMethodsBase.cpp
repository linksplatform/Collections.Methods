namespace Platform::Collections::Methods::Trees
{
    template <typename TElement> class SizedBinaryTreeMethodsBase : public GenericCollectionMethodsBase<TElement>
    {
    public:
        virtual TElement* GetLeftReference(TElement node) = 0;

        virtual TElement* GetRightReference(TElement node) = 0;

        virtual TElement GetLeft(TElement node) = 0;

        virtual TElement GetRight(TElement node) = 0;

        virtual TElement GetSize(TElement node) = 0;

        virtual void SetLeft(TElement node, TElement left) = 0;

        virtual void SetRight(TElement node, TElement right) = 0;

        virtual void SetSize(TElement node, TElement size) = 0;

        virtual bool FirstIsToTheLeftOfSecond(TElement first, TElement second) = 0;

        virtual bool FirstIsToTheRightOfSecond(TElement first, TElement second) = 0;

        virtual TElement GetLeftOrDefault(TElement node) { return node == 0 ? 0 : this->GetLeft(node); }

        virtual TElement GetRightOrDefault(TElement node) { return node == 0 ? 0 : this->GetRight(node); }

        void IncrementSize(TElement node) { this->SetSize(node, this->GetSize(node) + 1); }

        void DecrementSize(TElement node) { this->SetSize(node, this->GetSize(node) - 1); }

        TElement GetLeftSize(TElement node) { return this->GetSizeOrZero(this->GetLeftOrDefault(node)); }

        TElement GetRightSize(TElement node) { return this->GetSizeOrZero(this->GetRightOrDefault(node)); }

        TElement GetSizeOrZero(TElement node) { return node == 0 ? 0 : this->GetSize(node); }

        void FixSize(TElement node) { this->SetSize(node, (this->GetLeftSize(node) + this->GetRightSize(node)) + 1); }

        void LeftRotate(TElement* root) { *root = this->LeftRotate(*root); }

        TElement LeftRotate(TElement root)
        {
            auto right = this->GetRight(root);
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            if (right == 0)
            {
                throw std::exception("Right is null.");
            }
#endif
            this->SetRight(root, this->GetLeft(right));
            this->SetLeft(right, root);
            this->SetSize(right, this->GetSize(root));
            this->FixSize(root);
            return right;
        }

        void RightRotate(TElement* root) { *root = this->RightRotate(*root); }

        TElement RightRotate(TElement root)
        {
            auto left = this->GetLeft(root);
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            if (left == 0)
            {
                throw std::exception("Left is null.");
            }
#endif
            this->SetLeft(root, this->GetRight(left));
            this->SetRight(left, root);
            this->SetSize(left, this->GetSize(root));
            this->FixSize(root);
            return left;
        }

        virtual bool Contains(TElement node, TElement root)
        {
            while (root != 0)
            {
                if (this->FirstIsToTheLeftOfSecond(node, root))
                {
                    root = this->GetLeft(root);
                }
                else if (this->FirstIsToTheRightOfSecond(node, root))
                {
                    root = this->GetRight(root);
                }
                else
                {
                    return true;
                }
            }
            return false;
        }

        virtual void ClearNode(TElement node)
        {
            this->SetLeft(node, 0);
            this->SetRight(node, 0);
            this->SetSize(node, 0);
        }

        void Attach(TElement* root, TElement node)
        {
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            this->ValidateSizes(*root);
            Debug.WriteLine("--BeforeAttach--");
            Debug.WriteLine(this->PrintNodes(*root));
            Debug.WriteLine("----------------");
            auto sizeBefore = this->GetSize(*root);
#endif
            if (*root == 0)
            {
                this->SetSize(node, 1);
                *root = node;
                return;
            }
            this->AttachCore(root, node);
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            Debug.WriteLine("--AfterAttach--");
            Debug.WriteLine(this->PrintNodes(*root));
            Debug.WriteLine("----------------");
            this->ValidateSizes(*root);
            auto sizeAfter = this->GetSize(*root);
            if (!IsEquals(MathHelpers.(sizeBefore) + 1, sizeAfter))
            {
                throw std::exception("Tree was broken after attach.");
            }
#endif
        }

        virtual void AttachCore(TElement* root, TElement node) = 0;

        void Detach(TElement* root, TElement node)
        {
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            this->ValidateSizes(*root);
            Debug.WriteLine("--BeforeDetach--");
            Debug.WriteLine(this->PrintNodes(*root));
            Debug.WriteLine("----------------");
            auto sizeBefore = this->GetSize(*root);
            if (this->ValueEqualToZero(*root))
            {
                throw std::exception("Элемент с {node} не содержится в дереве.");
            }
#endif
            this->DetachCore(root, node);
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            Debug.WriteLine("--AfterDetach--");
            Debug.WriteLine(this->PrintNodes(*root));
            Debug.WriteLine("----------------");
            this->ValidateSizes(*root);
            auto sizeAfter = this->GetSize(*root);
            if (!IsEquals(MathHelpers.(sizeBefore) - 1, sizeAfter))
            {
                throw std::exception("Tree was broken after detach.");
            }
#endif
        }

        virtual void DetachCore(TElement* root, TElement node) = 0;
    };
}
