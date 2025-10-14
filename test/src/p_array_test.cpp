#include "p_array_test.hpp"
#include "gtest/gtest.h"

using namespace s_coll;
using namespace s_coll::test::helpers;

static constexpr Base base_c_arr[] = { { 1 }, { 4 }, { 5 }, { 8 }, { 9 } };
static constexpr size_t base_arr_size = sizeof(base_c_arr) / sizeof(Base);
static constexpr auto base_cpp_arr = std::array<Base, base_arr_size>({ Base(1), { 2 }, { 3 }, { 4 }, { 5 } });

static constexpr Derived derv_c_arr[] = { { 1, 'a' }, { 3, 'c' }, { 5, 'e' }, { 7, 'g' } };
static constexpr size_t derv_arr_size = sizeof(derv_c_arr) / sizeof(Derived);
static constexpr auto derv_cpp_arr = std::array<Derived, derv_arr_size>({
    Derived(1, 'a'), { 3, 'c' }, { 5, 'e' }, { 7, 'g' }
});

TEST(PArrayTest, TestDefaultConstruction)
{
    auto parr = p_array<const Base>();
    EXPECT_EQ(nullptr, parr.ptr) << "Pointer should be null for default-constructed p_array.";
    EXPECT_EQ(0, parr.len) << "Length should be zero for default-constructed p_array.";
}

TEST(PArrayTest, TestPointerConstruction)
{
    // Partial array
    p_array<const Base> parr = { base_c_arr, 3 };
    EXPECT_EQ(base_c_arr, parr.ptr) << "Pointer of p_array should match the provided base array.";
    EXPECT_EQ(3, parr.len) << "Length of p_array should match the provided length.";

    // Full array
    parr = { base_c_arr, 5 };
    EXPECT_EQ(base_c_arr, parr.ptr) << "Pointer of p_array should match the provided base array.";
    EXPECT_EQ(5, parr.len) << "Length of p_array should match the provided length.";

    // Derived array
    parr = { derv_c_arr, 2 };
    EXPECT_EQ(derv_c_arr, parr.ptr) << "Pointer of p_array should match the provided derived array.";
    EXPECT_EQ(2, parr.len) << "Length of p_array should match the provided length.";
}

TEST(PArrayTest, TestConversion)
{
    p_array<const Base> base_parr = base_c_arr;
    EXPECT_EQ(base_c_arr, base_parr.ptr) << "Pointer of p_array should match the provided base array.";
    EXPECT_EQ(base_arr_size, base_parr.len) << "Length of p_array should match the size of the provided base array.";

    // Test C-style arrays
    base_parr = derv_c_arr;
    EXPECT_EQ(derv_c_arr, base_parr.ptr) << "Pointer of p_array should match the provided derived array.";
    EXPECT_EQ(derv_arr_size, base_parr.len) << "Length of p_array should match the size of the provided derived array.";

    base_parr = base_c_arr;
    p_array<const Derived> other_parr = derv_c_arr;
    base_parr = other_parr;
    EXPECT_EQ(derv_c_arr, base_parr.ptr) << "Pointer of p_array should match the provided derived array.";
    EXPECT_EQ(derv_arr_size, base_parr.len) << "Length of p_array should match the size of the provided derived array.";

    // Test C++ arrays
    base_parr = base_cpp_arr;
    EXPECT_EQ(base_cpp_arr.data(), base_parr.ptr) << "Pointer of p_array should match the provided base C++ array.";
    EXPECT_EQ(base_arr_size, base_parr.len) << "Length of p_array should match the size of the provided base C++ array.";

    base_parr = derv_cpp_arr;
    EXPECT_EQ(derv_cpp_arr.data(), base_parr.ptr) << "Pointer of p_array should match the provided derived C++ array.";
    EXPECT_EQ(derv_arr_size, base_parr.len) << "Length of p_array should match the size of the provided derived C++ array.";

    // Test non-const C++ arrays
    std::array<Base, base_arr_size> bs = base_cpp_arr;
    p_array<Base> non_const_base_parr = bs;
    EXPECT_EQ(bs.data(), non_const_base_parr.ptr) << "Pointer of p_array should match the provided C++ array.";
    EXPECT_EQ(bs.size(), non_const_base_parr.len) << "Length of p_array should match the size of the provided C++ array.";

    // Test decay to a pointer
    Base * non_const_base_ptr = non_const_base_parr;
    EXPECT_EQ(non_const_base_parr.ptr, non_const_base_ptr) << "A non-const p_array should decay to its non-const pointer.";

    const Base * const_base_ptr = base_parr;
    EXPECT_EQ(base_parr.ptr, const_base_ptr) << "A const p_array should decay to its const pointer.";
}

TEST(PArrayTest, TestEquality)
{
    int ints_1_1[] = { 5, 2, 1, 4 };
    int ints_1_2[] = { 5, 2, 1, 4 };
    int ints_2[] = { 4, 3, 1, 1 };

    p_array<int> ints_1_1_parr_1 = ints_1_1;
    p_array<int> ints_1_1_parr_2 = ints_1_1;
    p_array<int> ints_1_1_parr_diffSize = { ints_1_1, 3 }; // Size differs
    p_array<int> ints_1_2_parr = ints_1_2;
    p_array<int> ints_2_parr = ints_2;

    EXPECT_EQ(ints_1_1_parr_1, ints_1_1_parr_1) << "Any p_array should be equal to itself.";
    EXPECT_EQ(ints_1_1_parr_1, ints_1_1_parr_2)
        << "Any p_array should be equal to another p_array of the same length representing the same array.";
    EXPECT_NE(ints_1_1_parr_1, ints_1_1_parr_diffSize)
        << "Any p_arrays representing the same array but of different sizes should be unequal.";
    EXPECT_NE(ints_1_1_parr_1, ints_1_2_parr) << "Any p_arrays representing two different arrays should be unequal.";
    EXPECT_NE(ints_1_1_parr_1, ints_2_parr) << "Any p_arrays representing two different arrays should be unequal.";
}

TEST(PArrayTest, TestSize)
{
    int ints[] = { 5, 2, 3, 1 };
    p_array<int> ints_parr = { ints, 3 };
    EXPECT_EQ(3, ints_parr.len) << "Control test failed: length of the p_array should be as specified.";
    EXPECT_EQ(3, ints_parr.size()) << "Size of the p_array should match the length.";
}

TEST(PArrayTest, TestIndexing)
{
    std::array<Derived, derv_arr_size> derv = derv_cpp_arr;
    p_array<Base> base_parr = derv;
    p_array<Derived> derv_parr = derv;

    // Check getting indexes of base array
    for (int i = 0; i < base_parr.len; i++)
    {
        EXPECT_EQ(Base(1 + 2 * i), base_parr[i]) << "Element at base p_array index " << i << " was not as expected.";
    }

    // Check getting indexes of derived array
    for (int i = 0; i < derv_parr.len; i++)
    {
        EXPECT_EQ(Derived(1 + 2 * i, char('a' + 2 * i)), derv_parr[i])
            << "Element at derived p_array index " << i << " was not as expected.";
    }

    // Check setting indexes of derived array
    for (int i = 0; i < derv_parr.len; i++)
    {
        derv_parr[i] = { 3 * i + 2, char(i + 1) };
        EXPECT_EQ(Derived(3 * i + 2, char(i + 1)), derv_parr[i]);
    }
}

TEST(PArrayTest, TestIteration)
{
    std::array<Derived, derv_arr_size> derv = derv_cpp_arr;
    p_array<Base> base_parr = derv;
    p_array<Derived> derv_parr = derv;

    // Test getting elements of base p_array
    int i = 0;
    for (const auto& elt : base_parr)
    {
        EXPECT_EQ(derv[i++], elt) << "Element at index " << i << " of base p_array was not as expected.";
    }

    // Test getting elements of derived p_array
    i = 0;
    for (const auto& elt : derv_parr)
    {
        EXPECT_EQ(derv[i++], elt) << "Element at index " << i << " of derived p_array was not as expected.";
    }

    // Test setting elements of derived p_array
    i = 0;
    for (auto& elt : derv_parr)
    {
        Derived new_elt = { 2 * (i + 2), char('a' + 3 * i) };
        elt = new_elt;
        EXPECT_EQ(new_elt, elt) << "Element at index " << i << " of derived p_array was not set properly.";
    }
}
