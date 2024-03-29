using System.Numerics;

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member

namespace Platform.Collections.Methods.Lists
{
    /// <summary>
    /// <para>
    /// Represents the relative open doubly linked list methods.
    /// </para>
    /// <para></para>
    /// </summary>
    /// <seealso cref="RelativeDoublyLinkedListMethodsBase{TElement}"/>
    public abstract class RelativeOpenDoublyLinkedListMethods<TElement> : RelativeDoublyLinkedListMethodsBase<TElement> where TElement: IUnsignedNumber<TElement>, IComparisonOperators<TElement, TElement, bool>
    {
        /// <summary>
        /// <para>
        /// Attaches the before using the specified head element.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="headElement">
        /// <para>The head element.</para>
        /// <para></para>
        /// </param>
        /// <param name="baseElement">
        /// <para>The base element.</para>
        /// <para></para>
        /// </param>
        /// <param name="newElement">
        /// <para>The new element.</para>
        /// <para></para>
        /// </param>
        public void AttachBefore(TElement headElement, TElement baseElement, TElement newElement)
        {
            var baseElementPrevious = GetPrevious(baseElement);
            SetPrevious(newElement, baseElementPrevious);
            SetNext(newElement, baseElement);
            if (baseElementPrevious == TElement.Zero)
            {
                SetFirst(headElement, newElement);
            }
            else
            {
                SetNext(baseElementPrevious, newElement);
            }
            SetPrevious(baseElement, newElement);
            IncrementSize(headElement);
        }

        /// <summary>
        /// <para>
        /// Attaches the after using the specified head element.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="headElement">
        /// <para>The head element.</para>
        /// <para></para>
        /// </param>
        /// <param name="baseElement">
        /// <para>The base element.</para>
        /// <para></para>
        /// </param>
        /// <param name="newElement">
        /// <para>The new element.</para>
        /// <para></para>
        /// </param>
        public void AttachAfter(TElement headElement, TElement baseElement, TElement newElement)
        {
            var baseElementNext = GetNext(baseElement);
            SetPrevious(newElement, baseElement);
            SetNext(newElement, baseElementNext);
            if (baseElementNext == TElement.Zero)
            {
                SetLast(headElement, newElement);
            }
            else
            {
                SetPrevious(baseElementNext, newElement);
            }
            SetNext(baseElement, newElement);
            IncrementSize(headElement);
        }

        /// <summary>
        /// <para>
        /// Attaches the as first using the specified head element.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="headElement">
        /// <para>The head element.</para>
        /// <para></para>
        /// </param>
        /// <param name="element">
        /// <para>The element.</para>
        /// <para></para>
        /// </param>
        public void AttachAsFirst(TElement headElement, TElement element)
        {
            var first = GetFirst(headElement);
            if (first == TElement.Zero)
            {
                SetFirst(headElement, element);
                SetLast(headElement, element);
                SetPrevious(element, TElement.Zero);
                SetNext(element, TElement.Zero);
                IncrementSize(headElement);
            }
            else
            {
                AttachBefore(headElement, first, element);
            }
        }

        /// <summary>
        /// <para>
        /// Attaches the as last using the specified head element.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="headElement">
        /// <para>The head element.</para>
        /// <para></para>
        /// </param>
        /// <param name="element">
        /// <para>The element.</para>
        /// <para></para>
        /// </param>
        public void AttachAsLast(TElement headElement, TElement element)
        {
            var last = GetLast(headElement);
            if (last == TElement.Zero)
            {
                AttachAsFirst(headElement, element);
            }
            else
            {
                AttachAfter(headElement, last, element);
            }
        }

        /// <summary>
        /// <para>
        /// Detaches the head element.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="headElement">
        /// <para>The head element.</para>
        /// <para></para>
        /// </param>
        /// <param name="element">
        /// <para>The element.</para>
        /// <para></para>
        /// </param>
        public void Detach(TElement headElement, TElement element)
        {
            var elementPrevious = GetPrevious(element);
            var elementNext = GetNext(element);
            if (elementPrevious == TElement.Zero)
            {
                SetFirst(headElement, elementNext);
            }
            else
            {
                SetNext(elementPrevious, elementNext);
            }
            if (elementNext == TElement.Zero)
            {
                SetLast(headElement, elementPrevious);
            }
            else
            {
                SetPrevious(elementNext, elementPrevious);
            }
            SetPrevious(element, TElement.Zero);
            SetNext(element, TElement.Zero);
            DecrementSize(headElement);
        }
    }
}
