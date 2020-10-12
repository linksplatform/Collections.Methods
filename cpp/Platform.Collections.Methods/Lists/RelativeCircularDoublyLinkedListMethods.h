namespace Platform::Collections::Methods::Lists
{
    template <typename ...> class RelativeCircularDoublyLinkedListMethods;
    template <typename TElement> class RelativeCircularDoublyLinkedListMethods<TElement> : public RelativeDoublyLinkedListMethodsBase<TElement>
    {
        public: void AttachBefore(TElement headElement, TElement baseElement, TElement newElement)
        {
            auto baseElementPrevious = this->GetPrevious(baseElement);
            this->SetPrevious(newElement, baseElementPrevious);
            this->SetNext(newElement, baseElement);
            if (baseElement == this->GetFirst(headElement))
            {
                this->SetFirst(headElement, newElement);
            }
            this->SetNext(baseElementPrevious, newElement);
            this->SetPrevious(baseElement, newElement);
            this->IncrementSize(headElement);
        }

        public: void AttachAfter(TElement headElement, TElement baseElement, TElement newElement)
        {
            auto baseElementNext = this->GetNext(baseElement);
            this->SetPrevious(newElement, baseElement);
            this->SetNext(newElement, baseElementNext);
            if (baseElement == this->GetLast(headElement))
            {
                this->SetLast(headElement, newElement);
            }
            this->SetPrevious(baseElementNext, newElement);
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
                this->SetPrevious(element, element);
                this->SetNext(element, element);
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
            if (elementNext == element)
            {
                this->SetFirst(headElement, 0);
                this->SetLast(headElement, 0);
            }
            else
            {
                this->SetNext(elementPrevious, elementNext);
                this->SetPrevious(elementNext, elementPrevious);
                if (element == this->GetFirst(headElement))
                {
                    this->SetFirst(headElement, elementNext);
                }
                if (element == this->GetLast(headElement))
                {
                    this->SetLast(headElement, elementPrevious);
                }
            }
            this->SetPrevious(element, 0);
            this->SetNext(element, 0);
            this->DecrementSize(headElement);
        }
    };
}
