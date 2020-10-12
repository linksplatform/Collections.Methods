namespace Platform::Collections::Methods::Lists
{
    template <typename ...> class DoublyLinkedListMethodsBase;
    template <typename TElement> class DoublyLinkedListMethodsBase<TElement> : public GenericCollectionMethodsBase<TElement>
    {
        protected: virtual TElement GetPrevious(TElement element) = 0;

        protected: virtual TElement GetNext(TElement element) = 0;

        protected: virtual void SetPrevious(TElement element, TElement previous) = 0;

        protected: virtual void SetNext(TElement element, TElement next) = 0;
    };
}
