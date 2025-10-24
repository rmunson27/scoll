#ifndef ___INCLG__SCOLL__P_ARRAY_HPP___
#define ___INCLG__SCOLL__P_ARRAY_HPP___

#include "builtin_arrays.hpp"
#include <stdexcept>
#include <concepts>

namespace scoll
{
    /// @brief Pointer array. Represents an array as a pointer and a length.
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
        /// @tparam U Used to deduce that `T` is a `const` type.
        /// @param arr 
        template <typename U, size_t Size>
        constexpr p_array(const c_array<U, Size>& arr) noexcept
        requires std::same_as<T, std::add_const_t<U>>
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

        /// @brief Accesses an array element at the specified index.
        ///        If the index is out of range, the behavior is undefined.
        /// @param i 
        /// @return 
        constexpr T& operator[](size_t i) noexcept(noexcept(*(ptr + i))) { return *(ptr + i); }

        /// @brief Accesses an array element at the specified index.
        ///        If the index is out of range, the behavior is undefined.
        /// @param i 
        /// @return 
        constexpr const T& operator[](size_t i) const noexcept(noexcept(*(ptr + i)))
        {
            return *(ptr + i);
        }

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
            else if (i >= len)
            {
                throw std::out_of_range("Value " + std::to_string(i) + " is out of range of the array.");
            }
        }

    public:
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
    
    /// @brief Contains additional generic methods for constructing `r_array` objects.
    namespace p_arrays
    {
        /// @brief Creates a `p_array` from the given C-style array.
        /// @tparam T 
        /// @tparam Size 
        /// @param arr 
        /// @return 
        template <typename T, size_t Size>
        p_array<T> make(c_array<T, Size>& arr) noexcept
        {
            return { arr };
        }

        /// @brief Creates a `p_array` from the given `const` C-style array.
        /// @remark This function can be used to deduce the type arguments and length, which cannot be done when using
        ///         the constructor.
        /// @tparam T 
        /// @tparam Size 
        /// @param arr 
        /// @return 
        template <typename T, size_t Size>
        p_array<const T> make(const c_array<T, Size>& arr) noexcept
        {
            return { arr };
        }

        /// @brief Creates a `p_array` from the given C++ standard library array.
        /// @tparam T 
        /// @tparam Size 
        /// @param arr 
        /// @return 
        template <typename T, size_t Size>
        p_array<T> make(std::array<T, Size>& arr) noexcept
        {
            return { arr };
        }

        /// @brief Creates a `p_array` from the given `const` C++ standard library array.
        /// @remark This function can be used to deduce the type arguments and length, which cannot be done when using
        ///         the constructor.
        /// @tparam T 
        /// @tparam Size 
        /// @param arr 
        /// @return 
        template <typename T, size_t Size>
        p_array<const T> make(const std::array<T, Size>& arr) noexcept
        {
            return { arr };
        }
    }
}

#endif
