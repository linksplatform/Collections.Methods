namespace Platform::Collections::Methods::Lists
{
    template <typename ...> class RelativeOpenDoublyLinkedListMethods;
    template <typename TElement> class RelativeOpenDoublyLinkedListMethods<TElement> : public RelativeDoublyLinkedListMethodsBase<TElement>
    {
        public: void AttachBefore(TElement headElement, TElement baseElement, TElement newElement)
        {
            auto baseElementPrevious = this->GetPrevious(baseElement);
            this->SetPrevious(newElement, baseElementPrevious);
            this->SetNext(newElement, baseElement);
            if (baseElementPrevious == 0)
            {
                this->SetFirst(headElement, newElement);
            }
            else
            {
                this->SetNext(baseElementPrevious, newElement);
            }
            this->SetPrevious(baseElement, newElement);
            this->IncrementSize(headElement);
        }

        public: void AttachAfter(TElement headElement, TElement baseElement, TElement newElement)
        {
            auto baseElementNext = this->GetNext(baseElement);
            this->SetPrevious(newElement, baseElement);
            this->SetNext(newElement, baseElementNext);
            if (baseElementNext == 0)
            {
                this->SetLast(headElement, newElement);
            }
            else
            {
                this->SetPrevious(baseElementNext, newElement);
            }
            this->SetNext(baseElement, newElement);
            this->IncrementSize(headElement);
        }

        public: void AttachAsFirst(TElement headElement, TElement element)
        {
            auto first = this->GetFirst(headElement);
            if (first == 0)
            {
                this->SetFirst(headElement, element);
                this->SetLast(headElement, element);
                this->SetPrevious(element, 0);
                this->SetNext(element, 0);
                this->IncrementSize(headElement);
            }
            else
            {
                this->AttachBefore(headElement, first, element);
            }
        }

        public: void AttachAsLast(TElement headElement, TElement element)
        {
            auto last = this->GetLast(headElement);
            if (last == 0)
            {
                this->AttachAsFirst(headElement, element);
            }
            else
            {
                this->AttachAfter(headElement, last, element);
            }
        }

        public: void Detach(TElement headElement, TElement element)
        {
            auto elementPrevious = this->GetPrevious(element);
            auto elementNext = this->GetNext(element);
            if (elementPrevious == 0)
            {
                this->SetFirst(headElement, elementNext);
            }
            else
            {
                this->SetNext(elementPrevious, elementNext);
            }
            if (elementNext == 0)
            {
                this->SetLast(headElement, elementPrevious);
            }
            else
            {
                this->SetPrevious(elementNext, elementPrevious);
            }
            this->SetPrevious(element, 0);
            this->SetNext(element, 0);
            this->DecrementSize(headElement);
        }
    };
}
