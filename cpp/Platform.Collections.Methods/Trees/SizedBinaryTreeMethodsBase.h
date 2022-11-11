namespace Platform::Collections::Methods::Trees
{
    template <typename TSelf, typename ...> class SizedBinaryTreeMethodsBase;
    template <typename TSelf, typename TElement> class SizedBinaryTreeMethodsBase<TSelf, TElement> : public GenericCollectionMethodsBase<TSelf, TElement>
    {
        protected: TElement* GetLeftReference(TElement node) { return this->object().GetLeftReference(node); };

        protected: TElement* GetRightReference(TElement node) { return this->object().GetRightReference(node); };

        protected: TElement GetLeft(TElement node) { return this->object().GetLeft(node); };

        protected: TElement GetRight(TElement node) { return this->object().GetRight(node); };

        protected: TElement GetSize(TElement node) { return this->object().GetSize(node); };

        protected: void SetLeft(TElement node, TElement left) { this->object().SetLeft(node, left); };

        protected: void SetRight(TElement node, TElement right) { this->object().SetRight(node, right); };

        protected: void SetSize(TElement node, TElement size) { this->object().SetSize(node, size); };

        protected: bool FirstIsToTheLeftOfSecond(TElement first, TElement second) { return this->object().FirstIsToTheLeftOfSecond(first, second); };

        protected: bool FirstIsToTheRightOfSecond(TElement first, TElement second) { return this->object().FirstIsToTheRightOfSecond(first, second); };

        protected: TElement GetLeftOrDefault(TElement node) { return node == 0 ? 0 : this->object().GetLeft(node); }

        protected: TElement GetRightOrDefault(TElement node) { return node == 0 ? 0 : this->object().GetRight(node); }

        protected: void IncrementSize(TElement node) { this->object().SetSize(node, this->object().GetSize(node) + 1); }

        protected: void DecrementSize(TElement node) { this->object().SetSize(node, this->object().GetSize(node) - 1); }

        protected: TElement GetLeftSize(TElement node) { return this->object().GetSizeOrZero(this->object().GetLeftOrDefault(node)); }

        protected: TElement GetRightSize(TElement node) { return this->object().GetSizeOrZero(this->object().GetRightOrDefault(node)); }

        protected: TElement GetSizeOrZero(TElement node) { return node == 0 ? 0 : this->object().GetSize(node); }

        protected: void FixSize(TElement node) { this->object().SetSize(node, (this->object().GetLeftSize(node) + this->object().GetRightSize(node)) + 1); }

        protected: void LeftRotate(TElement* root) { *root = this->object().LeftRotate(*root); }

        protected: TElement LeftRotate(TElement root)
        {
            auto right = this->object().GetRight(root);
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            if (right == 0)
            {
                throw std::runtime_error("Right is null.");
            }
#endif
            this->object().SetRight(root, this->object().GetLeft(right));
            this->object().SetLeft(right, root);
            this->object().SetSize(right, this->object().GetSize(root));
            this->object().FixSize(root);
            return right;
        }

        protected: void RightRotate(TElement* root) { *root = this->object().RightRotate(*root); }

        protected: TElement RightRotate(TElement root)
        {
            auto left = this->object().GetLeft(root);
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            if (left == 0)
            {
                throw std::runtime_error("Left is null.");
            }
#endif
            this->object().SetLeft(root, this->object().GetRight(left));
            this->object().SetRight(left, root);
            this->object().SetSize(left, this->object().GetSize(root));
            this->object().FixSize(root);
            return left;
        }

        protected: TElement GetRightest(TElement current)
        {
            auto currentRight = this->object().GetRight(current);
            while (currentRight != 0)
            {
                current = currentRight;
                currentRight = this->object().GetRight(current);
            }
            return current;
        }

        protected: TElement GetLeftest(TElement current)
        {
            auto currentLeft = this->object().GetLeft(current);
            while (currentLeft != 0)
            {
                current = currentLeft;
                currentLeft = this->object().GetLeft(current);
            }
            return current;
        }

        protected: TElement GetNext(TElement node) { return this->object().GetLeftest(this->object().GetRight(node)); }

        protected: TElement GetPrevious(TElement node) { return this->object().GetRightest(this->object().GetLeft(node)); }

        public: bool Contains(TElement node, TElement root)
        {
            while (root != 0)
            {
                if (this->object().FirstIsToTheLeftOfSecond(node, root))
                {
                    root = this->object().GetLeft(root);
                }
                else if (this->object().FirstIsToTheRightOfSecond(node, root))
                {
                    root = this->object().GetRight(root);
                }
                else
                {
                    return true;
                }
            }
            return false;
        }

        protected: void ClearNode(TElement node)
        {
            this->object().SetLeft(node, 0);
            this->object().SetRight(node, 0);
            this->object().SetSize(node, 0);
        }

        public: void Attach(TElement* root, TElement node)
        {
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            this->object().ValidateSizes(*root);
            Debug.WriteLine("--BeforeAttach--");
            Debug.WriteLine(this->object().PrintNodes(*root));
            Debug.WriteLine("----------------");
            auto sizeBefore = this->object().GetSize(*root);
#endif
            if (*root == 0)
            {
                this->object().SetSize(node, 1);
                *root = node;
                return;
            }
            this->object().AttachCore(root, node);
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            Debug.WriteLine("--AfterAttach--");
            Debug.WriteLine(this->object().PrintNodes(*root));
            Debug.WriteLine("----------------");
            this->object().ValidateSizes(*root);
            auto sizeAfter = this->object().GetSize(*root);
            if ((sizeBefore + 1) != sizeAfter)
            {
                throw std::runtime_error("Tree was broken after attach.");
            }
#endif
        }

        protected: void AttachCore(TElement* root, TElement node) { this->object().AttachCore(root, node); };

        public: void Detach(TElement* root, TElement node)
        {
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            this->object().ValidateSizes(*root);
            Debug.WriteLine("--BeforeDetach--");
            Debug.WriteLine(this->object().PrintNodes(*root));
            Debug.WriteLine("----------------");
            auto sizeBefore = this->object().GetSize(*root);
            if (*root == 0)
            {
                throw std::runtime_error(std::string("Элемент с ").append(Platform::Converters::To<std::string>(node)).append(" не содержится в дереве."));
            }
#endif
            this->object().DetachCore(root, node);
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            Debug.WriteLine("--AfterDetach--");
            Debug.WriteLine(this->object().PrintNodes(*root));
            Debug.WriteLine("----------------");
            this->object().ValidateSizes(*root);
            auto sizeAfter = this->object().GetSize(*root);
            if ((sizeBefore - 1) != sizeAfter)
            {
                throw std::runtime_error("Tree was broken after detach.");
            }
#endif
        }

        protected: void DetachCore(TElement* root, TElement node) { this->object().DetachCore(root, node); };
    };
}
