#pragma once

namespace Platform::Interfaces
{
    /// <summary>
    /// <para>
    /// Represents a base class for implementing the Curiously Recurring Template Pattern (CRTP).
    /// This enables static polymorphism where derived classes can be accessed through
    /// the base class interface without virtual function overhead.
    /// </para>
    /// <para>
    /// Based on: https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
    /// </para>
    /// </summary>
    /// <typeparam name="TSelf">The derived class type that inherits from this base class.</typeparam>
    template <typename TSelf>
    class Polymorph
    {
        protected:
            /// <summary>
            /// <para>
            /// Returns a reference to the derived class object.
            /// This method enables static polymorphism by providing access to the
            /// actual derived class implementation through a compile-time cast.
            /// </para>
            /// </summary>
            /// <returns>A reference to the derived class object.</returns>
            constexpr TSelf& object() noexcept
            {
                return static_cast<TSelf&>(*this);
            }

            /// <summary>
            /// <para>
            /// Returns a const reference to the derived class object.
            /// This method enables static polymorphism by providing read-only access to the
            /// actual derived class implementation through a compile-time cast.
            /// </para>
            /// </summary>
            /// <returns>A const reference to the derived class object.</returns>
            constexpr const TSelf& object() const noexcept
            {
                return static_cast<const TSelf&>(*this);
            }
    };
}