#ifndef ___INCLG__SCOLL_TEST__HELPERS__TEST_CLASSES_HPP___
#define ___INCLG__SCOLL_TEST__HELPERS__TEST_CLASSES_HPP___

namespace scoll::test::helpers
{
    struct Base
    {
        int a;

        constexpr Base(int a) : a(a) { }
        constexpr virtual ~Base() = default;

        constexpr bool operator==(const Base& other) const noexcept = default;
    };

    struct Derived : public Base
    {
        char b;

        constexpr Derived(int a, char b) : Base(a), b(b) { }
        constexpr virtual ~Derived() override = default;

        constexpr bool operator==(const Derived& other) const noexcept = default;
    };
}

#endif
