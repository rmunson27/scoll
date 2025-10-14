#ifndef ___INCLG__S_COLL__BUILTIN_ARRAYS_HPP___
#define ___INCLG__S_COLL__BUILTIN_ARRAYS_HPP___

#include <array>

namespace s_coll
{
    /// @brief Aliases the C-style array type.
    /// @tparam T 
    /// @tparam Size 
    template <typename T, size_t Size>
    using c_array = T[Size];

    /// @brief Aliases the C++ standard library array type.
    /// @tparam T 
    /// @tparam Size 
    template <typename T, size_t Size>
    using cpp_array = std::array<T, Size>;
}

#endif
