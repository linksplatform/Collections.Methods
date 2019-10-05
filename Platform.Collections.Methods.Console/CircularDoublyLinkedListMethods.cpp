
namespace Platform::Collections::Methods::Lists
{
    template <typename TElement> class CircularDoublyLinkedListMethods : public DoublyLinkedListMethodsBase<TElement>
    { public:
        void AttachBefore(TElement baseElement, TElement newElement)
        {
            auto baseElementPrevious = this->GetPrevious(baseElement);
            this->SetPrevious(newElement, baseElementPrevious);
            this->SetNext(newElement, baseElement);
            if (baseElement == this->GetFirst())
            {
                this->SetFirst(newElement);
            }
            this->SetNext(baseElementPrevious, newElement);
            this->SetPrevious(baseElement, newElement);
            this->IncrementSize();
        }

        void AttachAfter(TElement baseElement, TElement newElement)
        {
            auto baseElementNext = this->GetNext(baseElement);
            this->SetPrevious(newElement, baseElement);
            this->SetNext(newElement, baseElementNext);
            if (baseElement == this->GetLast())
            {
                this->SetLast(newElement);
            }
            this->SetPrevious(baseElementNext, newElement);
            this->SetNext(baseElement, newElement);
            this->IncrementSize();
        }

        void AttachAsFirst(TElement element)
        {
            auto first = this->GetFirst();
            if (first == 0)
            {
                this->SetFirst(element);
                this->SetLast(element);
                this->SetPrevious(element, element);
                this->SetNext(element, element);
                this->IncrementSize();
            }
            else
            {
                AttachBefore(first, element);
            }
        }

        void AttachAsLast(TElement element)
        {
            auto last = this->GetLast();
            if (last == 0)
            {
                AttachAsFirst(element);
            }
            else
            {
                AttachAfter(last, element);
            }
        }

        void Detach(TElement element)
        {
            auto elementPrevious = this->GetPrevious(element);
            auto elementNext = this->GetNext(element);
            if (elementNext == element)
            {
                this->SetFirst(0);
                this->SetLast(0);
            }
            else
            {
                this->SetNext(elementPrevious, elementNext);
                this->SetPrevious(elementNext, elementPrevious);
                if (element == this->GetFirst())
                {
                    this->SetFirst(elementNext);
                }
                if (element == this->GetLast())
                {
                    this->SetLast(elementPrevious);
                }
            }
            this->SetPrevious(element, 0);
            this->SetNext(element, 0);
            this->DecrementSize();
        }
    };
}
