namespace Platform::Collections::Methods::Trees
{
    template <class impl_t, typename ...> class SizedBinaryTreeMethodsBase;
    template <class impl_t, typename TElement> class SizedBinaryTreeMethodsBase<impl_t, TElement> : public GenericCollectionMethodsBase<impl_t, TElement>
    {
        public: using base_t = GenericCollectionMethodsBase<impl_t, TElement>;
        friend base_t;

        protected: TElement* GetLeftReference(TElement node) { return static_cast<impl_t*>(this)->GetLeftReference(node); };

        protected: TElement* GetRightReference(TElement node) { return static_cast<impl_t*>(this)->GetRightReference(node); };

        protected: TElement GetLeft(TElement node) { return static_cast<impl_t*>(this)->GetLeft(node); };

        protected: TElement GetRight(TElement node) { return static_cast<impl_t*>(this)->GetRight(node); };

        protected: TElement GetSize(TElement node) { return static_cast<impl_t*>(this)->GetSize(node); };

        protected: void SetLeft(TElement node, TElement left) { static_cast<impl_t*>(this)->SetLeft(node, left); };

        protected: void SetRight(TElement node, TElement right) { static_cast<impl_t*>(this)->SetRight(node, right); };

        protected: void SetSize(TElement node, TElement size) { static_cast<impl_t*>(this)->SetSize(node, size); };

        protected: bool FirstIsToTheLeftOfSecond(TElement first, TElement second) { return static_cast<impl_t*>(this)->FirstIsToTheLeftOfSecond(first, second); };

        protected: bool FirstIsToTheRightOfSecond(TElement first, TElement second) { return static_cast<impl_t*>(this)->FirstIsToTheRightOfSecond(first, second); };

        protected: TElement GetLeftOrDefault(TElement node) { return node == 0 ? 0 : static_cast<impl_t*>(this)->GetLeft(node); }

        protected: TElement GetRightOrDefault(TElement node) { return node == 0 ? 0 : static_cast<impl_t*>(this)->GetRight(node); }

        protected: void IncrementSize(TElement node) { static_cast<impl_t*>(this)->SetSize(node, static_cast<impl_t*>(this)->GetSize(node) + 1); }

        protected: void DecrementSize(TElement node) { static_cast<impl_t*>(this)->SetSize(node, static_cast<impl_t*>(this)->GetSize(node) - 1); }

        protected: TElement GetLeftSize(TElement node) { return static_cast<impl_t*>(this)->GetSizeOrZero(static_cast<impl_t*>(this)->GetLeftOrDefault(node)); }

        protected: TElement GetRightSize(TElement node) { return static_cast<impl_t*>(this)->GetSizeOrZero(static_cast<impl_t*>(this)->GetRightOrDefault(node)); }

        protected: TElement GetSizeOrZero(TElement node) { return node == 0 ? 0 : static_cast<impl_t*>(this)->GetSize(node); }

        protected: void FixSize(TElement node) { static_cast<impl_t*>(this)->SetSize(node, (static_cast<impl_t*>(this)->GetLeftSize(node) + static_cast<impl_t*>(this)->GetRightSize(node)) + 1); }

        protected: void LeftRotate(TElement* root) { *root = static_cast<impl_t*>(this)->LeftRotate(*root); }

        protected: TElement LeftRotate(TElement root)
        {
            auto right = static_cast<impl_t*>(this)->GetRight(root);
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            if (right == 0)
            {
                throw std::runtime_error("Right is null.");
            }
#endif
            static_cast<impl_t*>(this)->SetRight(root, static_cast<impl_t*>(this)->GetLeft(right));
            static_cast<impl_t*>(this)->SetLeft(right, root);
            static_cast<impl_t*>(this)->SetSize(right, static_cast<impl_t*>(this)->GetSize(root));
            static_cast<impl_t*>(this)->FixSize(root);
            return right;
        }

        protected: void RightRotate(TElement* root) { *root = static_cast<impl_t*>(this)->RightRotate(*root); }

        protected: TElement RightRotate(TElement root)
        {
            auto left = static_cast<impl_t*>(this)->GetLeft(root);
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            if (left == 0)
            {
                throw std::runtime_error("Left is null.");
            }
#endif
            static_cast<impl_t*>(this)->SetLeft(root, static_cast<impl_t*>(this)->GetRight(left));
            static_cast<impl_t*>(this)->SetRight(left, root);
            static_cast<impl_t*>(this)->SetSize(left, static_cast<impl_t*>(this)->GetSize(root));
            static_cast<impl_t*>(this)->FixSize(root);
            return left;
        }

        protected: TElement GetRightest(TElement current)
        {
            auto currentRight = static_cast<impl_t*>(this)->GetRight(current);
            while (currentRight != 0)
            {
                current = currentRight;
                currentRight = static_cast<impl_t*>(this)->GetRight(current);
            }
            return current;
        }

        protected: TElement GetLeftest(TElement current)
        {
            auto currentLeft = static_cast<impl_t*>(this)->GetLeft(current);
            while (currentLeft != 0)
            {
                current = currentLeft;
                currentLeft = static_cast<impl_t*>(this)->GetLeft(current);
            }
            return current;
        }

        protected: TElement GetNext(TElement node) { return static_cast<impl_t*>(this)->GetLeftest(static_cast<impl_t*>(this)->GetRight(node)); }

        protected: TElement GetPrevious(TElement node) { return static_cast<impl_t*>(this)->GetRightest(static_cast<impl_t*>(this)->GetLeft(node)); }

        public: bool Contains(TElement node, TElement root)
        {
            while (root != 0)
            {
                if (static_cast<impl_t*>(this)->FirstIsToTheLeftOfSecond(node, root))
                {
                    root = static_cast<impl_t*>(this)->GetLeft(root);
                }
                else if (static_cast<impl_t*>(this)->FirstIsToTheRightOfSecond(node, root))
                {
                    root = static_cast<impl_t*>(this)->GetRight(root);
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
            static_cast<impl_t*>(this)->SetLeft(node, 0);
            static_cast<impl_t*>(this)->SetRight(node, 0);
            static_cast<impl_t*>(this)->SetSize(node, 0);
        }

        public: void Attach(TElement* root, TElement node)
        {
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            static_cast<impl_t*>(this)->ValidateSizes(*root);
            Debug.WriteLine("--BeforeAttach--");
            Debug.WriteLine(static_cast<impl_t*>(this)->PrintNodes(*root));
            Debug.WriteLine("----------------");
            auto sizeBefore = static_cast<impl_t*>(this)->GetSize(*root);
#endif
            if (*root == 0)
            {
                static_cast<impl_t*>(this)->SetSize(node, 1);
                *root = node;
                return;
            }
            static_cast<impl_t*>(this)->AttachCore(root, node);
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            Debug.WriteLine("--AfterAttach--");
            Debug.WriteLine(static_cast<impl_t*>(this)->PrintNodes(*root));
            Debug.WriteLine("----------------");
            static_cast<impl_t*>(this)->ValidateSizes(*root);
            auto sizeAfter = static_cast<impl_t*>(this)->GetSize(*root);
            if ((sizeBefore + 1) != sizeAfter)
            {
                throw std::runtime_error("Tree was broken after attach.");
            }
#endif
        }

        protected: void AttachCore(TElement* root, TElement node) { static_cast<impl_t*>(this)->AttachCore(root, node); };

        public: void Detach(TElement* root, TElement node)
        {
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            static_cast<impl_t*>(this)->ValidateSizes(*root);
            Debug.WriteLine("--BeforeDetach--");
            Debug.WriteLine(static_cast<impl_t*>(this)->PrintNodes(*root));
            Debug.WriteLine("----------------");
            auto sizeBefore = static_cast<impl_t*>(this)->GetSize(*root);
            if (*root == 0)
            {
                throw std::runtime_error(std::string("Элемент с ").append(Platform::Converters::To<std::string>(node)).append(" не содержится в дереве."));
            }
#endif
            static_cast<impl_t*>(this)->DetachCore(root, node);
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            Debug.WriteLine("--AfterDetach--");
            Debug.WriteLine(static_cast<impl_t*>(this)->PrintNodes(*root));
            Debug.WriteLine("----------------");
            static_cast<impl_t*>(this)->ValidateSizes(*root);
            auto sizeAfter = static_cast<impl_t*>(this)->GetSize(*root);
            if ((sizeBefore - 1) != sizeAfter)
            {
                throw std::runtime_error("Tree was broken after detach.");
            }
#endif
        }

        protected: void DetachCore(TElement* root, TElement node) { static_cast<impl_t*>(this)->DetachCore(root, node); };
    };
}
