namespace Platform::Collections::Methods::Lists
{
    template <class impl_t, typename ...> class RelativeDoublyLinkedListMethodsBase;
    template <class impl_t, typename TElement> class RelativeDoublyLinkedListMethodsBase<impl_t, TElement> : public DoublyLinkedListMethodsBase<impl_t, TElement>
    {
        public: using base_t = DoublyLinkedListMethodsBase<impl_t, TElement>;
        friend base_t;

        protected: TElement GetFirst(TElement headElement) { return static_cast<impl_t*>(this)->GetFirst(headElement); };

        protected: TElement GetLast(TElement headElement) { return static_cast<impl_t*>(this)->GetLast(headElement); };

        protected: TElement GetSize(TElement headElement) { return static_cast<impl_t*>(this)->GetSize(headElement); };

        protected: void SetFirst(TElement headElement, TElement element) { static_cast<impl_t*>(this)->SetFirst(headElement, element); };

        protected: void SetLast(TElement headElement, TElement element) { static_cast<impl_t*>(this)->SetLast(headElement, element); };

        protected: void SetSize(TElement headElement, TElement size) { static_cast<impl_t*>(this)->SetSize(headElement, size); };

        protected: void IncrementSize(TElement headElement) { static_cast<impl_t*>(this)->SetSize(headElement, static_cast<impl_t*>(this)->GetSize(headElement) + 1); }

        protected: void DecrementSize(TElement headElement) { static_cast<impl_t*>(this)->SetSize(headElement, static_cast<impl_t*>(this)->GetSize(headElement) - 1); }
    };
}
