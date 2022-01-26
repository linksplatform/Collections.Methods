namespace Platform::Collections::Methods::Trees
{
    template <class impl_t, typename ...> class SizedBinaryTreeMethodsBase;
    template <class impl_t, typename TElement> class SizedBinaryTreeMethodsBase<impl_t, TElement> : public GenericCollectionMethodsBase<impl_t, TElement>
    {
        public: using Polymorph<impl_t>::object;

        protected: TElement* GetLeftReference(TElement node) { return object().GetLeftReference(node); };

        protected: TElement* GetRightReference(TElement node) { return object().GetRightReference(node); };

        protected: TElement GetLeft(TElement node) { return object().GetLeft(node); };

        protected: TElement GetRight(TElement node) { return object().GetRight(node); };

        protected: TElement GetSize(TElement node) { return object().GetSize(node); };

        protected: void SetLeft(TElement node, TElement left) { object().SetLeft(node, left); };

        protected: void SetRight(TElement node, TElement right) { object().SetRight(node, right); };

        protected: void SetSize(TElement node, TElement size) { object().SetSize(node, size); };

        protected: bool FirstIsToTheLeftOfSecond(TElement first, TElement second) { return object().FirstIsToTheLeftOfSecond(first, second); };

        protected: bool FirstIsToTheRightOfSecond(TElement first, TElement second) { return object().FirstIsToTheRightOfSecond(first, second); };

        protected: TElement GetLeftOrDefault(TElement node) { return node == 0 ? 0 : object().GetLeft(node); }

        protected: TElement GetRightOrDefault(TElement node) { return node == 0 ? 0 : object().GetRight(node); }

        protected: void IncrementSize(TElement node) { object().SetSize(node, object().GetSize(node) + 1); }

        protected: void DecrementSize(TElement node) { object().SetSize(node, object().GetSize(node) - 1); }

        protected: TElement GetLeftSize(TElement node) { return object().GetSizeOrZero(object().GetLeftOrDefault(node)); }

        protected: TElement GetRightSize(TElement node) { return object().GetSizeOrZero(object().GetRightOrDefault(node)); }

        protected: TElement GetSizeOrZero(TElement node) { return node == 0 ? 0 : object().GetSize(node); }

        protected: void FixSize(TElement node) { object().SetSize(node, (object().GetLeftSize(node) + object().GetRightSize(node)) + 1); }

        protected: void LeftRotate(TElement* root) { *root = object().LeftRotate(*root); }

        protected: TElement LeftRotate(TElement root)
        {
            auto right = object().GetRight(root);
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            if (right == 0)
            {
                throw std::runtime_error("Right is null.");
            }
#endif
            object().SetRight(root, object().GetLeft(right));
            object().SetLeft(right, root);
            object().SetSize(right, object().GetSize(root));
            object().FixSize(root);
            return right;
        }

        protected: void RightRotate(TElement* root) { *root = object().RightRotate(*root); }

        protected: TElement RightRotate(TElement root)
        {
            auto left = object().GetLeft(root);
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            if (left == 0)
            {
                throw std::runtime_error("Left is null.");
            }
#endif
            object().SetLeft(root, object().GetRight(left));
            object().SetRight(left, root);
            object().SetSize(left, object().GetSize(root));
            object().FixSize(root);
            return left;
        }

        protected: TElement GetRightest(TElement current)
        {
            auto currentRight = object().GetRight(current);
            while (currentRight != 0)
            {
                current = currentRight;
                currentRight = object().GetRight(current);
            }
            return current;
        }

        protected: TElement GetLeftest(TElement current)
        {
            auto currentLeft = object().GetLeft(current);
            while (currentLeft != 0)
            {
                current = currentLeft;
                currentLeft = object().GetLeft(current);
            }
            return current;
        }

        protected: TElement GetNext(TElement node) { return object().GetLeftest(object().GetRight(node)); }

        protected: TElement GetPrevious(TElement node) { return object().GetRightest(object().GetLeft(node)); }

        public: bool Contains(TElement node, TElement root)
        {
            while (root != 0)
            {
                if (object().FirstIsToTheLeftOfSecond(node, root))
                {
                    root = object().GetLeft(root);
                }
                else if (object().FirstIsToTheRightOfSecond(node, root))
                {
                    root = object().GetRight(root);
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
            object().SetLeft(node, 0);
            object().SetRight(node, 0);
            object().SetSize(node, 0);
        }

        public: void Attach(TElement* root, TElement node)
        {
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            object().ValidateSizes(*root);
            Debug.WriteLine("--BeforeAttach--");
            Debug.WriteLine(object().PrintNodes(*root));
            Debug.WriteLine("----------------");
            auto sizeBefore = object().GetSize(*root);
#endif
            if (*root == 0)
            {
                object().SetSize(node, 1);
                *root = node;
                return;
            }
            object().AttachCore(root, node);
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            Debug.WriteLine("--AfterAttach--");
            Debug.WriteLine(object().PrintNodes(*root));
            Debug.WriteLine("----------------");
            object().ValidateSizes(*root);
            auto sizeAfter = object().GetSize(*root);
            if ((sizeBefore + 1) != sizeAfter)
            {
                throw std::runtime_error("Tree was broken after attach.");
            }
#endif
        }

        protected: void AttachCore(TElement* root, TElement node) { object().AttachCore(root, node); };

        public: void Detach(TElement* root, TElement node)
        {
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            object().ValidateSizes(*root);
            Debug.WriteLine("--BeforeDetach--");
            Debug.WriteLine(object().PrintNodes(*root));
            Debug.WriteLine("----------------");
            auto sizeBefore = object().GetSize(*root);
            if (*root == 0)
            {
                throw std::runtime_error(std::string("Элемент с ").append(Platform::Converters::To<std::string>(node)).append(" не содержится в дереве."));
            }
#endif
            object().DetachCore(root, node);
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            Debug.WriteLine("--AfterDetach--");
            Debug.WriteLine(object().PrintNodes(*root));
            Debug.WriteLine("----------------");
            object().ValidateSizes(*root);
            auto sizeAfter = object().GetSize(*root);
            if ((sizeBefore - 1) != sizeAfter)
            {
                throw std::runtime_error("Tree was broken after detach.");
            }
#endif
        }

        protected: void DetachCore(TElement* root, TElement node) { object().DetachCore(root, node); };
    };
}
