
namespace Platform::Collections::Methods::Lists
{
    template <typename TElement> class OpenDoublyLinkedListMethods : DoublyLinkedListMethodsBase<TElement>
    {
        void AttachBefore(TElement baseElement, TElement newElement)
        {
            auto baseElementPrevious = DoublyLinkedListMethodsBase::GetPrevious(baseElement);
            DoublyLinkedListMethodsBase::SetPrevious(newElement, baseElementPrevious);
            DoublyLinkedListMethodsBase::SetNext(newElement, baseElement);
            if (baseElementPrevious == 0)
            {
                DoublyLinkedListMethodsBase::SetFirst(newElement);
            }
            else
            {
                DoublyLinkedListMethodsBase::SetNext(baseElementPrevious, newElement);
            }
            DoublyLinkedListMethodsBase::SetPrevious(baseElement, newElement);
            DoublyLinkedListMethodsBase::IncrementSize();
        }

        void AttachAfter(TElement baseElement, TElement newElement)
        {
            auto baseElementNext = DoublyLinkedListMethodsBase::GetNext(baseElement);
            DoublyLinkedListMethodsBase::SetPrevious(newElement, baseElement);
            DoublyLinkedListMethodsBase::SetNext(newElement, baseElementNext);
            if (baseElementNext == 0)
            {
                DoublyLinkedListMethodsBase::SetLast(newElement);
            }
            else
            {
                DoublyLinkedListMethodsBase::SetPrevious(baseElementNext, newElement);
            }
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
                DoublyLinkedListMethodsBase::SetPrevious(element, 0);
                DoublyLinkedListMethodsBase::SetNext(element, 0);
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
            if (elementPrevious == 0)
            {
                DoublyLinkedListMethodsBase::SetFirst(elementNext);
            }
            else
            {
                DoublyLinkedListMethodsBase::SetNext(elementPrevious, elementNext);
            }
            if (elementNext == 0)
            {
                DoublyLinkedListMethodsBase::SetLast(elementPrevious);
            }
            else
            {
                DoublyLinkedListMethodsBase::SetPrevious(elementNext, elementPrevious);
            }
            DoublyLinkedListMethodsBase::SetPrevious(element, 0);
            DoublyLinkedListMethodsBase::SetNext(element, 0);
            DoublyLinkedListMethodsBase::DecrementSize();
        }
    };
}
