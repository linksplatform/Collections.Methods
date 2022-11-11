namespace Platform::Collections::Methods::Lists
{
    template <class impl_t, typename ...> class RelativeDoublyLinkedListMethodsBase;
    template <class impl_t, typename TElement> class RelativeDoublyLinkedListMethodsBase<impl_t, TElement> : public DoublyLinkedListMethodsBase<impl_t, TElement>
    {
        public: using Polymorph<impl_t>::object;

        protected: TElement GetFirst(TElement headElement) { return object().GetFirst(headElement); };

        protected: TElement GetLast(TElement headElement) { return object().GetLast(headElement); };

        protected: TElement GetSize(TElement headElement) { return object().GetSize(headElement); };

        protected: void SetFirst(TElement headElement, TElement element) { object().SetFirst(headElement, element); };

        protected: void SetLast(TElement headElement, TElement element) { object().SetLast(headElement, element); };

        protected: void SetSize(TElement headElement, TElement size) { object().SetSize(headElement, size); };

        protected: void IncrementSize(TElement headElement) { object().SetSize(headElement, object().GetSize(headElement) + 1); }

        protected: void DecrementSize(TElement headElement) { object().SetSize(headElement, object().GetSize(headElement) - 1); }
    };
}
