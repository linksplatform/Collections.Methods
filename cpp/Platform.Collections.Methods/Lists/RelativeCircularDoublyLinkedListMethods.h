namespace Platform::Collections::Methods::Lists
{
    template <class TSelf, typename ...> class RelativeCircularDoublyLinkedListMethods;
    template <class TSelf, typename TElement> class RelativeCircularDoublyLinkedListMethods<TSelf, TElement> : public RelativeDoublyLinkedListMethodsBase<TSelf, TElement>
    {
        public: void AttachBefore(TElement headElement, TElement baseElement, TElement newElement)
        {
            auto baseElementPrevious = this->object().GetPrevious(baseElement);
            this->object().SetPrevious(newElement, baseElementPrevious);
            this->object().SetNext(newElement, baseElement);
            if (baseElement == this->object().GetFirst(headElement))
            {
                this->object().SetFirst(headElement, newElement);
            }
            this->object().SetNext(baseElementPrevious, newElement);
            this->object().SetPrevious(baseElement, newElement);
            this->object().IncrementSize(headElement);
        }

        public: void AttachAfter(TElement headElement, TElement baseElement, TElement newElement)
        {
            auto baseElementNext = this->object().GetNext(baseElement);
            this->object().SetPrevious(newElement, baseElement);
            this->object().SetNext(newElement, baseElementNext);
            if (baseElement == this->object().GetLast(headElement))
            {
                this->object().SetLast(headElement, newElement);
            }
            this->object().SetPrevious(baseElementNext, newElement);
            this->object().SetNext(baseElement, newElement);
            this->object().IncrementSize(headElement);
        }

        public: void AttachAsFirst(TElement headElement, TElement element)
        {
            auto first = this->object().GetFirst(headElement);
            if (first == 0)
            {
                this->object().SetFirst(headElement, element);
                this->object().SetLast(headElement, element);
                this->object().SetPrevious(element, element);
                this->object().SetNext(element, element);
                this->object().IncrementSize(headElement);
            }
            else
            {
                this->object().AttachBefore(headElement, first, element);
            }
        }

        public: void AttachAsLast(TElement headElement, TElement element)
        {
            auto last = this->object().GetLast(headElement);
            if (last == 0)
            {
                this->object().AttachAsFirst(headElement, element);
            }
            else
            {
                this->object().AttachAfter(headElement, last, element);
            }
        }

        public: void Detach(TElement headElement, TElement element)
        {
            auto elementPrevious = this->object().GetPrevious(element);
            auto elementNext = this->object().GetNext(element);
            if (elementNext == element)
            {
                this->object().SetFirst(headElement, 0);
                this->object().SetLast(headElement, 0);
            }
            else
            {
                this->object().SetNext(elementPrevious, elementNext);
                this->object().SetPrevious(elementNext, elementPrevious);
                if (element == this->object().GetFirst(headElement))
                {
                    this->object().SetFirst(headElement, elementNext);
                }
                if (element == this->object().GetLast(headElement))
                {
                    this->object().SetLast(headElement, elementPrevious);
                }
            }
            this->object().SetPrevious(element, 0);
            this->object().SetNext(element, 0);
            this->object().DecrementSize(headElement);
        }
    };
}
