#include "p_array_test.hpp"

using namespace s_coll;

static constexpr int int_c_arr[] { 1, 3, 5, 7, 9 };
static constexpr size_t int_arr_size = sizeof(int_c_arr) / sizeof(int);
static constexpr std::array<int, int_arr_size> int_cpp_arr = { 1, 3, 5, 7, 9 };

TEST(PArrayTest, TestDefaultConstruction)
{
    auto parr = p_array<int>();
    EXPECT_EQ(nullptr, parr.ptr) << "Pointer should be null for default-constructed p_array.";
    EXPECT_EQ(0, parr.len) << "Length should be zero for default-constructed p_array.";
}

TEST(PArrayTest, TestPointerConstruction)
{
    // Partial array
    p_array<const int> parr = { int_c_arr, 3 };
    EXPECT_EQ(int_c_arr, parr.ptr) << "Pointer of p_array should match the provided array.";
    EXPECT_EQ(3, parr.len) << "Length of p_array should match the provided length.";

    // Full array
    parr = { int_c_arr, 5 };
    EXPECT_EQ(int_c_arr, parr.ptr) << "Pointer of p_array should match the provided array.";
    EXPECT_EQ(5, parr.len) << "Length of p_array should match the provided length.";
}

TEST(PArrayTest, TestConversion)
{
    // Test C-style arrays
    p_array<const int> int_parr = int_c_arr;
    EXPECT_EQ(int_c_arr, int_parr.ptr) << "Pointer of p_array should match the provided array.";
    EXPECT_EQ(int_arr_size, int_parr.len) << "Length of p_array should match the size of the provided array.";

    // Test C++ standard library arrays
    int_parr = int_cpp_arr;
    EXPECT_EQ(int_cpp_arr.data(), int_parr.ptr) << "Pointer of p_array should match the provided C++ array.";
    EXPECT_EQ(int_arr_size, int_parr.len) << "Length of p_array should match the size of the provided C++ array.";

    // Test non-const C++ arrays
    std::array<int, int_arr_size> bs = int_cpp_arr;
    p_array<int> non_const_int_parr = bs;
    EXPECT_EQ(bs.data(), non_const_int_parr.ptr) << "Pointer of p_array should match the provided C++ array.";
    EXPECT_EQ(bs.size(), non_const_int_parr.len) << "Length of p_array should match the size of the provided C++ array.";

    // Test decay to a pointer
    int * non_const_int_ptr = non_const_int_parr;
    EXPECT_EQ(non_const_int_parr.ptr, non_const_int_ptr) << "A non-const p_array should decay to its non-const pointer.";

    const int * const_base_ptr = int_parr;
    EXPECT_EQ(int_parr.ptr, const_base_ptr) << "A const p_array should decay to its const pointer.";
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
    std::array<int, int_arr_size> ints = int_cpp_arr;
    p_array<int> int_parr = ints;

    // Check getting indexes of the p_array
    for (int i = 0; i < int_parr.len; i++)
    {
        EXPECT_EQ(1 + 2 * i, int_parr[i]) << "Element at p_array index " << i << " was not as expected.";
    }

    // Check setting indexes of the p_array
    for (int i = 0; i < int_parr.len; i++)
    {
        int_parr[i] = 3 * i + 2;
        EXPECT_EQ(3 * i + 2, int_parr[i]) << "Element at p_array index " << i << " was not set properly.";
    }
}

TEST(PArrayTest, TestIteration)
{
    std::array<int, int_arr_size> ints = int_cpp_arr;
    p_array<int> int_parr = ints;

    // Test getting elements of the p_array
    int i = 0;
    for (const auto& elt : int_parr)
    {
        EXPECT_EQ(ints[i++], elt) << "Element at index " << i << " of p_array was not as expected.";
    }

    // Test setting elements of the p_array
    i = 0;
    for (auto& elt : int_parr)
    {
        int new_elt = 2 * (i + 2);
        elt = new_elt;
        EXPECT_EQ(new_elt, elt) << "Element at p_array index " << i << " was not set properly.";
    }
}
