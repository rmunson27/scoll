#ifndef ___INCLG__SCOLL__R_ARRAY_HPP___
#define ___INCLG__SCOLL__R_ARRAY_HPP___

#include <algorithm>
#include "p_array.hpp"

namespace scoll
{
    /// @brief Reference array. Represents an array of a fixed size.
    ///        Can only be constructed from C-style arrays or standard C++ arrays.
    /// @remarks This type can be used as a length-safe join of C-style arrays and C++ arrays that does not require
    ///          a (potentially expensive) copy operation to construct.
    /// @tparam T The type of the array elements.
    /// @warning Although C-style arrays can be used to construct these objects, they do not decay automatically
    ///          to pointers, and are compared for equality and ordering similarly to the way C++ standard library
    ///          arrays are.
    template <typename T, size_t Size>
    class r_array
    {
        T * _ptr;

    public:
        /// @brief The type used to describe the size of the array.
        using size_type = size_t;

        /// @brief The type used to describe the difference between two arrays.
        using difference_type = std::ptrdiff_t;

        /// @brief The type of the pointer stored.
        using pointer = T*;

        /// @brief The type of pointer stored as `const`.
        using const_pointer = const T*;

        /// @brief The type of references to array elements.
        using reference = T&;

        /// @brief The type of array elements.
        using value_type = T;
        
        /// @brief The size of arrays of this type.
        static constexpr size_t size_v = Size;

        /// @brief Converts a C-style array to an `r_array`.
        /// @param arr 
        constexpr r_array(c_array<T, Size>& arr) noexcept
        : _ptr(arr) { }

        /// @brief Converts a `const` C-style array to an `r_array`.
        /// @tparam U Used to deduce that `T` is a `const` type.
        /// @param arr 
        template <typename U>
        constexpr r_array(const c_array<U, Size>& arr) noexcept
        requires std::same_as<T, std::add_const_t<U>>
        : _ptr(arr) { }

        /// @brief Converts a standard library C++ array to an `r_array`.
        /// @param arr 
        constexpr r_array(cpp_array<T, Size>& arr) noexcept
        : _ptr(arr.data()) { }

        /// @brief Converts a `const` standard library C++ array to an `r_array`.
        /// @tparam U Used to deduce that `T` is a `const` type.
        /// @param arr 
        template <typename U>
        constexpr r_array(const cpp_array<U, Size>& arr) noexcept
        requires std::same_as<T, std::add_const_t<U>>
        : _ptr(arr.data()) { }

        /// @brief Accesses an element of the array. No bounds checking is performed.
        constexpr T& operator[](size_t i) noexcept { return *(_ptr + i); }

        /// @brief Accesses a `const` element of the array. No bounds checking is performed.
        constexpr const T& operator[](size_t i) const noexcept { return *(_ptr + i); }

        /// @brief Accesses an array element at the specified index with bounds checking.
        /// @param i 
        /// @return 
        /// @throw `std::out_of_range` Thrown if the index is out of range of the collection.
        constexpr T& at(size_t i)
        {
            _check_bounds(i);
            return this->operator[](i);
        }

        /// @brief Accesses an array element at the specified index with bounds checking.
        /// @param i 
        /// @return 
        /// @throw `std::out_of_range` Thrown if the index is out of range of the collection.
        constexpr const T& at(size_t i) const
        {
            _check_bounds(i);
            return this->operator[](i);
        }

    private:
        /// @brief Checks an index within the bounds of the collection.
        /// @param i 
        /// @throw `std::out_of_range`
        constexpr void _check_bounds(size_t i) const
        {
            if (i < 0)
            {
                throw std::out_of_range("Negative value " + std::to_string(i) + " is not a valid array index.");
            }
            else if (i >= Size)
            {
                throw std::out_of_range("Value " + std::to_string(i) + " is out of range of the array.");
            }
        }

    public:
        /// @brief Gets a pointer to the first element in the array.
        /// @return 
        constexpr T* begin() noexcept { return _ptr; }

        /// @brief Gets a `const` pointer to the first element in the array.
        /// @return 
        constexpr const T* begin() const noexcept { return _ptr; }

        /// @brief Gets a `const` pointer to the first element in the array.
        /// @return 
        constexpr const T* cbegin() const noexcept { return begin(); }

        /// @brief Gets a pointer past the last element in the array.
        /// @return 
        constexpr T* end() noexcept { return _ptr + Size; }

        /// @brief Gets a `const` pointer past the last element in the array.
        /// @return 
        constexpr const T* end() const noexcept { return _ptr + Size; }

        /// @brief Gets a `const` pointer past the last element in the array.
        /// @return 
        constexpr const T* cend() const noexcept { return end(); }

        /// @brief Gets the size of arrays of this type.
        /// @return 
        static constexpr size_t size() noexcept { return Size; }

        /// @brief Implicit conversion to a `p_array`.
        constexpr operator p_array<T>() noexcept { return { _ptr, Size }; }

        /// @brief Implicit conversion to a `const p_array`.
        constexpr operator p_array<const T>() const noexcept { return { _ptr, Size }; }

        /// @brief Gets the underlying pointer to the array data.
        /// @return 
        constexpr pointer data() noexcept { return _ptr; }

        /// @brief Gets the underlying `const` pointer to the array data.
        /// @return 
        constexpr const_pointer data() const noexcept { return _ptr; }

        /// @brief Determines if this `r_array` contains the same elements as another (according to object
        ///        equality on the element type).
        /// @param other 
        /// @return 
        constexpr bool operator==(const r_array<T, Size>& other) const
        noexcept(noexcept(std::declval<T>() != std::declval<T>()))
        requires std::equality_comparable<T>
        {
            for (size_t i = 0; i < Size; i++)
            {
                if (this->operator[](i) != other[i]) return false;
            }

            return true;
        }

        /// @brief Determines the ordering between this `r_array` and another.
        /// @param other 
        /// @return 
        constexpr auto operator<=>(const r_array<T, Size>& other) const
        noexcept(noexcept(std::lexicographical_compare_three_way(cbegin(), cend(), other.cbegin(), other.cend())))
        -> decltype(std::declval<T>() <=> std::declval<T>())
        {
            return std::lexicographical_compare_three_way(cbegin(), cend(), other.cbegin(), other.cend());
        }
    };

    /// @brief Contains additional generic methods for constructing `r_array` objects.
    namespace r_arrays
    {
        /// @brief Creates an `r_array` from the given C-style array.
        /// @tparam T 
        /// @tparam Size 
        /// @param arr 
        /// @return 
        template <typename T, size_t Size>
        r_array<T, Size> make(c_array<T, Size>& arr) noexcept
        {
            return { arr };
        }

        /// @brief Creates an `r_array` from the given `const` C-style array.
        /// @remark This function can be used to deduce the type arguments and length, which cannot be done when using
        ///         the constructor.
        /// @tparam T 
        /// @tparam Size 
        /// @param arr 
        /// @return 
        template <typename T, size_t Size>
        r_array<const T, Size> make(const c_array<T, Size>& arr) noexcept
        {
            return { arr };
        }

        /// @brief Creates an `r_array` from the given C++ standard library array.
        /// @tparam T 
        /// @tparam Size 
        /// @param arr 
        /// @return 
        template <typename T, size_t Size>
        r_array<T, Size> make(std::array<T, Size>& arr) noexcept
        {
            return { arr };
        }

        /// @brief Creates an `r_array` from the given `const` C++ standard library array.
        /// @remark This function can be used to deduce the type arguments and length, which cannot be done when using
        ///         the constructor.
        /// @tparam T 
        /// @tparam Size 
        /// @param arr 
        /// @return 
        template <typename T, size_t Size>
        r_array<const T, Size> make(const std::array<T, Size>& arr) noexcept
        {
            return { arr };
        }
    }
}

#endif
