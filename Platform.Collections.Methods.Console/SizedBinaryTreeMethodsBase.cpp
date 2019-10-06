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
        virtual TElement GetLeftOrDefault(TElement node) { return node == 0 ? 0 : GetLeft(node); }
        virtual TElement GetRightOrDefault(TElement node) { return node == 0 ? 0 : GetRight(node); }
        void IncrementSize(TElement node) { SetSize(node, GetSize(node) + 1); }
        void DecrementSize(TElement node) { SetSize(node, GetSize(node) - 1); }
        TElement GetLeftSize(TElement node) { return GetSizeOrZero(GetLeftOrDefault(node)); }
        TElement GetRightSize(TElement node) { return GetSizeOrZero(GetRightOrDefault(node)); }
        TElement GetSizeOrZero(TElement node) { return node == 0 ? 0 : GetSize(node); }
        void FixSize(TElement node) { SetSize(node, GetLeftSize(node) + GetRightSize(node) + 1); }
        void LeftRotate(TElement* root) { *root = LeftRotate(*root); }
        TElement LeftRotate(TElement root)
        {
            auto right = GetRight(root);
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            if (right == 0)
            {
                throw std::exception("Right is null.");
            }
#endif
            SetRight(root, GetLeft(right));
            SetLeft(right, root);
            SetSize(right, GetSize(root));
            FixSize(root);
            return right;
        }
        void RightRotate(TElement* root) { *root = RightRotate(*root); }
        TElement RightRotate(TElement root)
        {
            auto left = GetLeft(root);
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            if (left == 0)
            {
                throw std::exception("Left is null.");
            }
#endif
            SetLeft(root, GetRight(left));
            SetRight(left, root);
            SetSize(left, GetSize(root));
            FixSize(root);
            return left;
        }
        virtual bool Contains(TElement node, TElement root)
        {
            while (root != 0)
            {
                if (FirstIsToTheLeftOfSecond(node, root)) // node.Key < root.Key
                {
                    root = GetLeft(root);
                }
                else if (FirstIsToTheRightOfSecond(node, root)) // node.Key > root.Key
                {
                    root = GetRight(root);
                }
                else // node.Key == root.Key
                {
                    return true;
                }
            }
            return false;
        }
        virtual void ClearNode(TElement node)
        {
            SetLeft(node, 0);
            SetRight(node, 0);
            SetSize(node, 0);
        }
        void Attach(TElement* root, TElement node)
        {
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            ValidateSizes(*root);
            Debug.WriteLine("--BeforeAttach--");
            Debug.WriteLine(PrintNodes(*root));
            Debug.WriteLine("----------------");
            auto sizeBefore = GetSize(*root);
#endif
            if (*root == 0)
            {
                SetSize(node, 1);
                *root = node;
                return;
            }
            AttachCore(root, node);
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            Debug.WriteLine("--AfterAttach--");
            Debug.WriteLine(PrintNodes(*root));
            Debug.WriteLine("----------------");
            ValidateSizes(*root);
            auto sizeAfter = GetSize(*root);
            if (!IsEquals(MathHelpers.sizeBefore + 1, sizeAfter))
            {
                throw std::exception("Tree was broken after attach.");
            }
#endif
        }

        virtual void AttachCore(TElement* root, TElement node) = 0;
        void Detach(TElement* root, TElement node)
        {
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            ValidateSizes(*root);
            Debug.WriteLine("--BeforeDetach--");
            Debug.WriteLine(PrintNodes(*root));
            Debug.WriteLine("----------------");
            auto sizeBefore = GetSize(*root);
            if (ValueEqualToZero(*root))
            {
                throw std::exception("Элемент с {node} не содержится в дереве.");
            }
#endif
            DetachCore(root, node);
#if ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            Debug.WriteLine("--AfterDetach--");
            Debug.WriteLine(PrintNodes(*root));
            Debug.WriteLine("----------------");
            ValidateSizes(*root);
            auto sizeAfter = GetSize(*root);
            if (!IsEquals(MathHelpers.sizeBefore - 1, sizeAfter))
            {
                throw std::exception("Tree was broken after detach.");
            }
#endif
        }

        virtual void DetachCore(TElement* root, TElement node) = 0;

    };
}
