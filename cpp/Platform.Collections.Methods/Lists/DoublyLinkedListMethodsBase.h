namespace Platform::Collections::Methods::Lists
{
    template <class impl_t, typename ...> class DoublyLinkedListMethodsBase;
    template <class impl_t, typename TElement> class DoublyLinkedListMethodsBase<impl_t, TElement> : public GenericCollectionMethodsBase<impl_t, TElement>
    {
        public: using Polymorph<impl_t>::object;

        protected: TElement GetPrevious(TElement element) { return object().GetPrevious(element); };

        protected: TElement GetNext(TElement element) { return object().GetNext(element); };

        protected: void SetPrevious(TElement element, TElement previous) { object().SetPrevious(element, previous); };

        protected: void SetNext(TElement element, TElement next) { object().SetNext(element, next); };
    };
}
