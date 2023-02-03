namespace Platform::Collections::Methods::Lists
{
    template <class TSelf, typename ...> class AbsoluteOpenDoublyLinkedListMethods;
    template <class TSelf, typename TElement> class AbsoluteOpenDoublyLinkedListMethods<TSelf, TElement> : public AbsoluteDoublyLinkedListMethodsBase<TSelf, TElement>
    {
        public: void AttachBefore(TElement baseElement, TElement newElement)
        {
            auto baseElementPrevious = this->object().GetPrevious(baseElement);
            this->object().SetPrevious(newElement, baseElementPrevious);
            this->object().SetNext(newElement, baseElement);
            if (baseElementPrevious == 0)
            {
                this->object().SetFirst(newElement);
            }
            else
            {
                this->object().SetNext(baseElementPrevious, newElement);
            }
            this->object().SetPrevious(baseElement, newElement);
            this->object().IncrementSize();
        }

        public: void AttachAfter(TElement baseElement, TElement newElement)
        {
            auto baseElementNext = this->object().GetNext(baseElement);
            this->object().SetPrevious(newElement, baseElement);
            this->object().SetNext(newElement, baseElementNext);
            if (baseElementNext == 0)
            {
                this->object().SetLast(newElement);
            }
            else
            {
                this->object().SetPrevious(baseElementNext, newElement);
            }
            this->object().SetNext(baseElement, newElement);
            this->object().IncrementSize();
        }

        public: void AttachAsFirst(TElement element)
        {
            auto first = this->object().GetFirst();
            if (first == 0)
            {
                this->object().SetFirst(element);
                this->object().SetLast(element);
                this->object().SetPrevious(element, 0);
                this->object().SetNext(element, 0);
                this->object().IncrementSize();
            }
            else
            {
                this->object().AttachBefore(first, element);
            }
        }

        public: void AttachAsLast(TElement element)
        {
            auto last = this->object().GetLast();
            if (last == 0)
            {
                this->object().AttachAsFirst(element);
            }
            else
            {
                this->object().AttachAfter(last, element);
            }
        }

        public: void Detach(TElement element)
        {
            auto elementPrevious = this->object().GetPrevious(element);
            auto elementNext = this->object().GetNext(element);
            if (elementPrevious == 0)
            {
                this->object().SetFirst(elementNext);
            }
            else
            {
                this->object().SetNext(elementPrevious, elementNext);
            }
            if (elementNext == 0)
            {
                this->object().SetLast(elementPrevious);
            }
            else
            {
                this->object().SetPrevious(elementNext, elementPrevious);
            }
            this->object().SetPrevious(element, 0);
            this->object().SetNext(element, 0);
            this->object().DecrementSize();
        }
    };
}
