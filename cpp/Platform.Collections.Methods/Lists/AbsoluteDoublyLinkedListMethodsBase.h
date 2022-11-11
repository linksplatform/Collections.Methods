namespace Platform::Collections::Methods::Lists
{
    template <class impl_t, typename ...> class AbsoluteDoublyLinkedListMethodsBase;
    template <class impl_t, typename TElement> class AbsoluteDoublyLinkedListMethodsBase<impl_t, TElement> : public DoublyLinkedListMethodsBase<impl_t, TElement>
    {
        public: using Polymorph<impl_t>::object;

        protected: TElement GetFirst() { return object().GetFirst(); };

        protected: TElement GetLast() { return object().GetLast(); };

        protected: TElement GetSize() { return object().GetSize(); };

        protected: void SetFirst(TElement element) { object().SetFirst(element); };

        protected: void SetLast(TElement element) { object().SetLast(element); };

        protected: void SetSize(TElement size) { object().SetSize(size); };

        protected: void IncrementSize() { object().SetSize(object().GetSize() + 1); }

        protected: void DecrementSize() { object().SetSize(object().GetSize() - 1); }
    };
}
