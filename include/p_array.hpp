#ifndef ___INCLG__S_COLL__P_ARRAY_HPP___
#define ___INCLG__S_COLL__P_ARRAY_HPP___

#include "builtin_arrays.hpp"
#include <concepts>

namespace s_coll
{
    /// @brief A simple struct to represent a pointer to an array and its length.
    /// @tparam T The type of the array elements.
    /// @warning This type can decay to a pointer like a C-style array through an implicit conversion.
    ///          This may be unsafe.
    template <typename T>
    struct p_array
    {
        /// @brief The pointer to the array.
        T* ptr;

        /// @brief The length of the array.
        size_t len;

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

        /// @brief Constructs a default array, with a null pointer and zero length.
        constexpr p_array() noexcept : ptr(nullptr), len(0) { }

        /// @brief Constructs an array from the specified pointer and length. No bounds checking is performed.
        /// @param ptr 
        /// @param len
        constexpr p_array(T * const ptr, size_t len) noexcept
        : ptr(ptr), len(len) { }

        /// @brief Decay constructor. Constructs an array from the specified C-style array.
        ///        No bounds checking is performed.
        /// @param arr 
        template <size_t Size>
        constexpr p_array(c_array<T, Size>& arr) noexcept
        : ptr(arr), len(Size) { }

        /// @brief Decay constructor. Constructs an array from the specified C++ standard library array.
        ///        No bounds checking is performed.
        /// @param arr 
        template <size_t Size>
        constexpr p_array(cpp_array<T, Size>& arr) noexcept
        : ptr(arr.data()), len(Size) { }

        /// @brief Decay constructor. Constructs an array from the specified C++ standard library array.
        ///        No bounds checking is performed.
        /// @tparam U Used to deduce that `T` is a `const` type.
        /// @param arr 
        template <typename U, size_t Size>
        constexpr p_array(const cpp_array<U, Size>& arr) noexcept
        requires std::same_as<T, std::add_const_t<U>>
        : ptr(arr.data()), len(Size) { }

        /// @brief Accesses an element of the array. 
        /// @brief Accesses an array element at the specified index. No bounds checking is performed.
        /// @param i 
        /// @return 
        constexpr T& operator[](size_t i) noexcept { return *(ptr + i); }

        /// @brief Accesses an array element at the specified index. No bounds checking is performed.
        /// @param i 
        /// @return 
        constexpr const T& operator[](size_t i) const noexcept { return *(ptr + i); }

        /// @brief Determines if this array is the same as another array.
        /// @param other 
        /// @return 
        constexpr bool operator==(const p_array<T>& other) const noexcept = default;

        /// @brief Gets a pointer to the first element in the array.
        /// @return 
        constexpr T* begin() noexcept { return ptr; }

        /// @brief Gets a `const` pointer to the first element in the array.
        /// @return 
        constexpr const T* begin() const noexcept { return begin(); }

        /// @brief Gets a `const` pointer to the first element in the array.
        /// @return 
        constexpr const T* cbegin() const noexcept { return ptr; }

        /// @brief Gets a pointer past the last element in the array.
        /// @return 
        constexpr T* end() noexcept { return ptr + len; }

        /// @brief Gets a `const` pointer past the last element in the array.
        /// @return 
        constexpr const T* end() const noexcept { return ptr + len; }

        /// @brief Gets a `const` pointer past the last element in the array.
        /// @return 
        constexpr const T* cend() const noexcept { return end(); }

        /// @brief Gets the size of the array.
        /// @return 
        constexpr size_t size() const noexcept { return len; }

        /// @brief Decay conversion to the underlying pointer.
        constexpr operator pointer() noexcept { return ptr; }

        /// @brief Decay conversion to the underlying pointer as `const`.
        constexpr operator const_pointer() const noexcept { return ptr; }
    };
}

#endif
