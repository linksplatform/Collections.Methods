
namespace Platform::Collections::Methods::Lists
{
    template <typename TElement> class CircularDoublyLinkedListMethods : DoublyLinkedListMethodsBase<TElement>
    {
        void AttachBefore(TElement baseElement, TElement newElement)
        {
            auto baseElementPrevious = DoublyLinkedListMethodsBase::GetPrevious(baseElement);
            DoublyLinkedListMethodsBase::SetPrevious(newElement, baseElementPrevious);
            DoublyLinkedListMethodsBase::SetNext(newElement, baseElement);
            if (baseElement == DoublyLinkedListMethodsBase::GetFirst())
            {
                DoublyLinkedListMethodsBase::SetFirst(newElement);
            }
            DoublyLinkedListMethodsBase::SetNext(baseElementPrevious, newElement);
            DoublyLinkedListMethodsBase::SetPrevious(baseElement, newElement);
            DoublyLinkedListMethodsBase::IncrementSize();
        }

        void AttachAfter(TElement baseElement, TElement newElement)
        {
            auto baseElementNext = DoublyLinkedListMethodsBase::GetNext(baseElement);
            DoublyLinkedListMethodsBase::SetPrevious(newElement, baseElement);
            DoublyLinkedListMethodsBase::SetNext(newElement, baseElementNext);
            if (baseElement == DoublyLinkedListMethodsBase::GetLast())
            {
                DoublyLinkedListMethodsBase::SetLast(newElement);
            }
            DoublyLinkedListMethodsBase::SetPrevious(baseElementNext, newElement);
            DoublyLinkedListMethodsBase::SetNext(baseElement, newElement);
            DoublyLinkedListMethodsBase::IncrementSize();
        }

        void AttachAsFirst(TElement element)
        {
            auto first = DoublyLinkedListMethodsBase::GetFirst();
            if (first == 0)
            {
                DoublyLinkedListMethodsBase::SetFirst(element);
                DoublyLinkedListMethodsBase::SetLast(element);
                DoublyLinkedListMethodsBase::SetPrevious(element, element);
                DoublyLinkedListMethodsBase::SetNext(element, element);
                DoublyLinkedListMethodsBase::IncrementSize();
            }
            else
            {
                AttachBefore(first, element);
            }
        }

        void AttachAsLast(TElement element)
        {
            auto last = DoublyLinkedListMethodsBase::GetLast();
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
            auto elementPrevious = DoublyLinkedListMethodsBase::GetPrevious(element);
            auto elementNext = DoublyLinkedListMethodsBase::GetNext(element);
            if (elementNext == element)
            {
                DoublyLinkedListMethodsBase::SetFirst(0);
                DoublyLinkedListMethodsBase::SetLast(0);
            }
            else
            {
                DoublyLinkedListMethodsBase::SetNext(elementPrevious, elementNext);
                DoublyLinkedListMethodsBase::SetPrevious(elementNext, elementPrevious);
                if (element == DoublyLinkedListMethodsBase::GetFirst())
                {
                    DoublyLinkedListMethodsBase::SetFirst(elementNext);
                }
                if (element == DoublyLinkedListMethodsBase::GetLast())
                {
                    DoublyLinkedListMethodsBase::SetLast(elementPrevious);
                }
            }
            DoublyLinkedListMethodsBase::SetPrevious(element, 0);
            DoublyLinkedListMethodsBase::SetNext(element, 0);
            DoublyLinkedListMethodsBase::DecrementSize();
        }
    };
}
