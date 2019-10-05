
namespace Platform::Collections::Methods::Lists
{
    template <typename TElement> class OpenDoublyLinkedListMethods : public DoublyLinkedListMethodsBase<TElement>
    { public:
        void AttachBefore(TElement baseElement, TElement newElement)
        {
            auto baseElementPrevious = this->GetPrevious(baseElement);
            this->SetPrevious(newElement, baseElementPrevious);
            this->SetNext(newElement, baseElement);
            if (baseElementPrevious == 0)
            {
                this->SetFirst(newElement);
            }
            else
            {
                this->SetNext(baseElementPrevious, newElement);
            }
            this->SetPrevious(baseElement, newElement);
            this->IncrementSize();
        }

        void AttachAfter(TElement baseElement, TElement newElement)
        {
            auto baseElementNext = this->GetNext(baseElement);
            this->SetPrevious(newElement, baseElement);
            this->SetNext(newElement, baseElementNext);
            if (baseElementNext == 0)
            {
                this->SetLast(newElement);
            }
            else
            {
                this->SetPrevious(baseElementNext, newElement);
            }
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
                this->SetPrevious(element, 0);
                this->SetNext(element, 0);
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
            if (elementPrevious == 0)
            {
                this->SetFirst(elementNext);
            }
            else
            {
                this->SetNext(elementPrevious, elementNext);
            }
            if (elementNext == 0)
            {
                this->SetLast(elementPrevious);
            }
            else
            {
                this->SetPrevious(elementNext, elementPrevious);
            }
            this->SetPrevious(element, 0);
            this->SetNext(element, 0);
            this->DecrementSize();
        }
    };
}
