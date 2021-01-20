namespace Platform::Collections::Methods::Lists
{
    template <class impl_t, typename ...> class AbsoluteDoublyLinkedListMethodsBase;
    template <class impl_t, typename TElement> class AbsoluteDoublyLinkedListMethodsBase<impl_t, TElement> : public DoublyLinkedListMethodsBase<impl_t, TElement>
    {
        public: using base_t = DoublyLinkedListMethodsBase<impl_t, TElement>;
        friend base_t;

        protected: TElement GetFirst() { return static_cast<impl_t*>(this)->GetFirst(); };

        protected: TElement GetLast() { return static_cast<impl_t*>(this)->GetLast(); };

        protected: TElement GetSize() { return static_cast<impl_t*>(this)->GetSize(); };

        protected: void SetFirst(TElement element) { static_cast<impl_t*>(this)->SetFirst(element); };

        protected: void SetLast(TElement element) { static_cast<impl_t*>(this)->SetLast(element); };

        protected: void SetSize(TElement size) { static_cast<impl_t*>(this)->SetSize(size); };

        protected: void IncrementSize() { static_cast<impl_t*>(this)->SetSize(static_cast<impl_t*>(this)->GetSize() + 1); }

        protected: void DecrementSize() { static_cast<impl_t*>(this)->SetSize(static_cast<impl_t*>(this)->GetSize() - 1); }
    };
}
