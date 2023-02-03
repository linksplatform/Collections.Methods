namespace Platform::Collections::Methods::Lists
{
    template <class TSelf, typename ...> class AbsoluteCircularDoublyLinkedListMethods;
    template <class TSelf, typename TElement> class AbsoluteCircularDoublyLinkedListMethods<TSelf, TElement> : public AbsoluteDoublyLinkedListMethodsBase<TSelf, TElement>
    {
        public: void AttachBefore(TElement baseElement, TElement newElement)
        {
            auto baseElementPrevious = this->object().GetPrevious(baseElement);
            this->object().SetPrevious(newElement, baseElementPrevious);
            this->object().SetNext(newElement, baseElement);
            if (baseElement == this->object().GetFirst())
            {
                this->object().SetFirst(newElement);
            }
            this->object().SetNext(baseElementPrevious, newElement);
            this->object().SetPrevious(baseElement, newElement);
            this->object().IncrementSize();
        }

        public: void AttachAfter(TElement baseElement, TElement newElement)
        {
            auto baseElementNext = this->object().GetNext(baseElement);
            this->object().SetPrevious(newElement, baseElement);
            this->object().SetNext(newElement, baseElementNext);
            if (baseElement == this->object().GetLast())
            {
                this->object().SetLast(newElement);
            }
            this->object().SetPrevious(baseElementNext, newElement);
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
                this->object().SetPrevious(element, element);
                this->object().SetNext(element, element);
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
            if (elementNext == element)
            {
                this->object().SetFirst(0);
                this->object().SetLast(0);
            }
            else
            {
                this->object().SetNext(elementPrevious, elementNext);
                this->object().SetPrevious(elementNext, elementPrevious);
                if (element == this->object().GetFirst())
                {
                    this->object().SetFirst(elementNext);
                }
                if (element == this->object().GetLast())
                {
                    this->object().SetLast(elementPrevious);
                }
            }
            this->object().SetPrevious(element, 0);
            this->object().SetNext(element, 0);
            this->object().DecrementSize();
        }
    };
}
