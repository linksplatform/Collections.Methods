namespace Platform::Collections::Methods::Trees
{
    template <typename ...> class SizedBinaryTreeMethodsBase;
    template <typename TElement> class SizedBinaryTreeMethodsBase<TElement> : public GenericCollectionMethodsBase<TElement>
    {
        protected: virtual TElement* GetLeftReference(TElement node) = 0;

        protected: virtual TElement* GetRightReference(TElement node) = 0;

        protected: virtual TElement GetLeft(TElement node) = 0;

        protected: virtual TElement GetRight(TElement node) = 0;

        protected: virtual TElement GetSize(TElement node) = 0;

        protected: virtual void SetLeft(TElement node, TElement left) = 0;

        protected: virtual void SetRight(TElement node, TElement right) = 0;

        protected: virtual void SetSize(TElement node, TElement size) = 0;

        protected: virtual bool FirstIsToTheLeftOfSecond(TElement first, TElement second) = 0;

        protected: virtual bool FirstIsToTheRightOfSecond(TElement first, TElement second) = 0;

        protected: virtual TElement GetLeftOrDefault(TElement node) { return node == 0 ? 0 : this->GetLeft(node); }

        protected: virtual TElement GetRightOrDefault(TElement node) { return node == 0 ? 0 : this->GetRight(node); }

        protected: void IncrementSize(TElement node) { this->SetSize(node, this->GetSize(node) + 1); }

        protected: void DecrementSize(TElement node) { this->SetSize(node, this->GetSize(node) - 1); }

        protected: TElement GetLeftSize(TElement node) { return this->GetSizeOrZero(this->GetLeftOrDefault(node)); }

        protected: TElement GetRightSize(TElement node) { return this->GetSizeOrZero(this->GetRightOrDefault(node)); }

        protected: TElement GetSizeOrZero(TElement node) { return node == 0 ? 0 : this->GetSize(node); }

        protected: void FixSize(TElement node) { this->SetSize(node, (this->GetLeftSize(node) + this->GetRightSize(node)) + 1); }

        protected: void LeftRotate(TElement* root) { *root = this->LeftRotate(*root); }

        protected: TElement LeftRotate(TElement root)
        {
            auto right = this->GetRight(root);
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            if (right == 0)
            {
                throw std::runtime_error("Right is null.");
            }
#endif
            this->SetRight(root, this->GetLeft(right));
            this->SetLeft(right, root);
            this->SetSize(right, this->GetSize(root));
            this->FixSize(root);
            return right;
        }

        protected: void RightRotate(TElement* root) { *root = this->RightRotate(*root); }

        protected: TElement RightRotate(TElement root)
        {
            auto left = this->GetLeft(root);
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            if (left == 0)
            {
                throw std::runtime_error("Left is null.");
            }
#endif
            this->SetLeft(root, this->GetRight(left));
            this->SetRight(left, root);
            this->SetSize(left, this->GetSize(root));
            this->FixSize(root);
            return left;
        }

        protected: virtual TElement GetRightest(TElement current)
        {
            auto currentRight = this->GetRight(current);
            while (currentRight != 0)
            {
                current = currentRight;
                currentRight = this->GetRight(current);
            }
            return current;
        }

        protected: virtual TElement GetLeftest(TElement current)
        {
            auto currentLeft = this->GetLeft(current);
            while (currentLeft != 0)
            {
                current = currentLeft;
                currentLeft = this->GetLeft(current);
            }
            return current;
        }

        protected: virtual TElement GetNext(TElement node) { return this->GetLeftest(this->GetRight(node)); }

        protected: virtual TElement GetPrevious(TElement node) { return this->GetRightest(this->GetLeft(node)); }

        public: virtual bool Contains(TElement node, TElement root)
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

        protected: virtual void ClearNode(TElement node)
        {
            this->SetLeft(node, 0);
            this->SetRight(node, 0);
            this->SetSize(node, 0);
        }

        public: void Attach(TElement* root, TElement node)
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
            if ((sizeBefore + 1) != sizeAfter)
            {
                throw std::runtime_error("Tree was broken after attach.");
            }
#endif
        }

        protected: virtual void AttachCore(TElement* root, TElement node) = 0;

        public: void Detach(TElement* root, TElement node)
        {
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            this->ValidateSizes(*root);
            Debug.WriteLine("--BeforeDetach--");
            Debug.WriteLine(this->PrintNodes(*root));
            Debug.WriteLine("----------------");
            auto sizeBefore = this->GetSize(*root);
            if (*root == 0)
            {
                throw std::runtime_error(std::string("Элемент с ").append(Platform::Converters::To<std::string>(node)).append(" не содержится в дереве."));
            }
#endif
            this->DetachCore(root, node);
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            Debug.WriteLine("--AfterDetach--");
            Debug.WriteLine(this->PrintNodes(*root));
            Debug.WriteLine("----------------");
            this->ValidateSizes(*root);
            auto sizeAfter = this->GetSize(*root);
            if ((sizeBefore - 1) != sizeAfter)
            {
                throw std::runtime_error("Tree was broken after detach.");
            }
#endif
        }

        protected: virtual void DetachCore(TElement* root, TElement node) = 0;
    };
}
