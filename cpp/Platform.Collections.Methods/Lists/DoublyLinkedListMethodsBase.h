namespace Platform::Collections::Methods::Lists
{
    template <class impl_t, typename ...> class DoublyLinkedListMethodsBase;
    template <class impl_t, typename TElement> class DoublyLinkedListMethodsBase<impl_t, TElement> : public GenericCollectionMethodsBase<impl_t, TElement>
    {
        public: using base_t = GenericCollectionMethodsBase<impl_t, TElement>;
        friend base_t;

        protected: TElement GetPrevious(TElement element) { return static_cast<impl_t*>(this)->GetPrevious(element); };

        protected: TElement GetNext(TElement element) { return static_cast<impl_t*>(this)->GetNext(element); };

        protected: void SetPrevious(TElement element, TElement previous) { static_cast<impl_t*>(this)->SetPrevious(element, previous); };

        protected: void SetNext(TElement element, TElement next) { static_cast<impl_t*>(this)->SetNext(element, next); };
    };
}
