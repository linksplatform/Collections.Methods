namespace Platform::Collections::Methods::Tests
{
    template <std::size_t N, typename ...> class SizedAndThreadedAVLBalancedTree;
    template <std::size_t N, typename TElement> class SizedAndThreadedAVLBalancedTree<N, TElement> : public Platform::Collections::Methods::Trees::SizedAndThreadedAVLBalancedTreeMethods<SizedAndThreadedAVLBalancedTree<N, TElement>, TElement>
    {
        using base_t = Platform::Collections::Methods::Trees::SizedAndThreadedAVLBalancedTreeMethods<SizedAndThreadedAVLBalancedTree<N, TElement>, TElement>;
        friend base_t;
        friend base_t::base_t;

        struct TreeElement
        {
            public: TElement Size = 0;
            public: TElement Left = 0;
            public: TElement Right = 0;
            public: std::int8_t Balance = 0;
            public: bool LeftIsChild = 0;
            public: bool RightIsChild = 0;
        };

        private: TreeElement _elements[N] = { {0} };
        private: TElement _allocated = 0;

        public: TElement Root = 0;

        public: TElement Count() { return this->GetSizeOrZero(Root); }

        public: SizedAndThreadedAVLBalancedTree() { _allocated = 1; }

        public: TElement Allocate()
        {
            auto newNode = _allocated;
            if (this->IsEmpty(newNode))
            {
                _allocated = _allocated + 1;
                return newNode;
            }
            else
            {
                throw std::runtime_error("Allocated tree element is not empty.");
            }
        }

        public: void Free(TElement node)
        {
            while (_allocated != 1 && this->IsEmpty(node))
            {
                auto lastNode = _allocated - 1;
                if (lastNode == node)
                {
                    _allocated = lastNode;
                    node = node - 1;
                }
                else
                {
                    return;
                }
            }
        }

        public: bool IsEmpty(TElement node) { return iszero(this->GetElement(node), sizeof(TreeElement)); }

        protected: bool FirstIsToTheLeftOfSecond(TElement first, TElement second) { return first < second; }

        protected: bool FirstIsToTheRightOfSecond(TElement first, TElement second) { return first > second; }

        protected: std::int8_t GetBalance(TElement node) { return this->GetElement(node)->Balance; }

        protected: bool GetLeftIsChild(TElement node) { return this->GetElement(node)->LeftIsChild; }

        protected: TElement* GetLeftReference(TElement node) { return &GetElement(node)->Left; }

        protected: TElement GetLeft(TElement node) { return this->GetElement(node)->Left; }

        protected: bool GetRightIsChild(TElement node) { return this->GetElement(node)->RightIsChild; }

        protected: TElement* GetRightReference(TElement node) { return &GetElement(node)->Right; }

        protected: TElement GetRight(TElement node) { return this->GetElement(node)->Right; }

        protected: TElement GetSize(TElement node) { return this->GetElement(node)->Size; }

        protected: void SetBalance(TElement node, std::int8_t value) { this->GetElement(node)->Balance = value; }

        protected: void SetLeft(TElement node, TElement left) { this->GetElement(node)->Left = left; }

        protected: void SetLeftIsChild(TElement node, bool value) { this->GetElement(node)->LeftIsChild = value; }

        protected: void SetRight(TElement node, TElement right) { this->GetElement(node)->Right = right; }

        protected: void SetRightIsChild(TElement node, bool value) { this->GetElement(node)->RightIsChild = value; }

        protected: void SetSize(TElement node, TElement size) { this->GetElement(node)->Size = size; }

        private: TreeElement* GetElement(TElement node) { return &_elements[node]; }
    };
}
