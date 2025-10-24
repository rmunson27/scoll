#include "r_array_test.hpp"

using namespace scoll;

static constexpr int int_c_arr[] { 1, 3, 5, 7, 9 };
static constexpr size_t int_arr_size = sizeof(int_c_arr) / sizeof(int);
static constexpr std::array<int, int_arr_size> int_cpp_arr = { 1, 3, 5, 7, 9 };

TEST(RArrayTest, TestConstruction)
{
    int c_arr[int_arr_size];
    std::copy(int_c_arr, int_c_arr + int_arr_size, c_arr);
    auto c_rarr = r_array(c_arr);
    EXPECT_EQ(c_arr, c_rarr.data()) << "Data of constructed r_array was not as expected.";

    auto const_c_rarr = r_array(int_c_arr);
    EXPECT_EQ(int_c_arr, const_c_rarr.data()) << "Data of constructed r_array was not as expected.";

    auto cpp_arr = int_cpp_arr;
    auto cpp_rarr = r_array(cpp_arr);
    EXPECT_EQ(cpp_arr.data(), cpp_rarr.data()) << "Data of constructed r_array was not as expected.";

    r_array<const int, int_arr_size> cpp_const_rarr = { int_cpp_arr };
    EXPECT_EQ(int_cpp_arr.data(), cpp_const_rarr.data()) << "Data of constructed r_array was not as expected.";
}

TEST(RArrayTest, TestMake)
{
    int c_arr[int_arr_size];
    std::copy(int_c_arr, int_c_arr + int_arr_size, c_arr);
    auto c_rarr = r_arrays::make(c_arr);
    EXPECT_EQ(c_arr, c_rarr.data()) << "Data of constructed r_array was not as expected.";

    auto const_c_rarr = r_arrays::make(int_c_arr);
    EXPECT_EQ(int_c_arr, const_c_rarr.data()) << "Data of constructed r_array was not as expected.";

    auto cpp_arr = int_cpp_arr;
    auto cpp_rarr = r_arrays::make(cpp_arr);
    EXPECT_EQ(cpp_arr.data(), cpp_rarr.data()) << "Data of constructed r_array was not as expected.";

    auto cpp_const_rarr = r_arrays::make(int_cpp_arr);
    EXPECT_EQ(int_cpp_arr.data(), cpp_const_rarr.data()) << "Data of constructed r_array was not as expected.";
}

TEST(RArrayTest, TestPArrayConversion)
{
    auto cpp_arr = int_cpp_arr;
    auto rarr = r_arrays::make(cpp_arr);
    auto parr = p_array<int>(rarr);
    EXPECT_EQ(cpp_arr.data(), parr.ptr) << "The converted p_array should point to the same data as the r_array.";
    EXPECT_EQ(int_arr_size, parr.len) << "Length of the converted p_array should match the size of the r_array.";

    auto const_rarr = r_arrays::make(int_cpp_arr);
    auto const_parr = p_array<const int>(const_rarr);
    EXPECT_EQ(int_cpp_arr.data(), const_parr.ptr) << "The converted p_array should point to the same data as the r_array.";
    EXPECT_EQ(int_arr_size, const_parr.len) << "Length of the converted p_array should match the size of the r_array.";
}

TEST(RArrayTest, TestIndexing)
{
    auto cpp_arr = int_cpp_arr;
    auto rarr = r_arrays::make(cpp_arr);
    for (int i = 0; i < int_arr_size; i++)
    {
        EXPECT_EQ(cpp_arr[i], rarr[i]) << "Element at index " << i << " should match the underlying data.";
        rarr[i] = i * 5;
        EXPECT_EQ(i * 5, rarr[i]) << "Element of r_array should have been set.";
        EXPECT_EQ(i * 5, cpp_arr[i]) << "Element set in r_array should be reflected in underlying data.";
    }

    auto const_rarr = r_arrays::make(int_cpp_arr);
    for (int i = 0; i < int_arr_size; i++)
    {
        EXPECT_EQ(int_cpp_arr[i], const_rarr[i]) << "Element at index " << i << " should match the underlying data.";
    }
}

TEST(RArrayTest, TestAt)
{
    auto cpp_arr = int_cpp_arr;
    auto rarr = r_arrays::make(cpp_arr);
    for (int i = 0; i < int_arr_size; i++)
    {
        EXPECT_EQ(cpp_arr[i], rarr.at(i)) << "Element at index " << i << " should match the underlying data.";
        rarr[i] = i * 5;
        EXPECT_EQ(i * 5, rarr.at(i)) << "Element of r_array should have been set.";
        EXPECT_EQ(i * 5, cpp_arr.at(i)) << "Element set in r_array should be reflected in underlying data.";
    }
    EXPECT_THROW(rarr.at(int_arr_size), std::out_of_range)
        << "Indexing outside the bounds of the array should cause an exception to be thrown.";

    auto const_rarr = r_arrays::make(int_cpp_arr);
    for (int i = 0; i < int_arr_size; i++)
    {
        EXPECT_EQ(int_cpp_arr[i], const_rarr[i]) << "Element at index " << i << " should match the underlying data.";
    }
    EXPECT_THROW(const_rarr.at(int_arr_size), std::out_of_range)
        << "Indexing outside the bounds of the array should cause an exception to be thrown.";
}

TEST(RArrayTest, TestIteration)
{
    // Test getting elements of the const r_array
    auto const_rarr = r_arrays::make(int_cpp_arr);
    int i = 0;
    for (const auto& elt : const_rarr)
    {
        EXPECT_EQ(int_cpp_arr[i++], elt) << "Element at index " << i << " of r_array was not as expected.";
    }

    // Test getting and setting elements of a non-const r_array
    auto cpp_arr = int_cpp_arr;
    auto rarr = r_arrays::make(cpp_arr);
    i = 0;
    for (auto& elt : rarr)
    {
        EXPECT_EQ(cpp_arr[i], elt) << "Element at index " << i << " of r_array was not as expected.";

        int new_elt = 2 * (i + 2);
        elt = new_elt;
        EXPECT_EQ(new_elt, elt) << "Element at r_array index " << i << " was not set properly.";

        EXPECT_EQ(cpp_arr[i++], elt) << "Element set on r_array should be reflected in the underlying storage.";
    }
}

TEST(RArrayTest, TestEquality)
{
    auto arr = int_cpp_arr;
    auto arr2 = int_cpp_arr;
    ASSERT_NE(arr.data(), arr2.data()) << "Data must be unequal for this component of the test to work.";

    auto r_arr = r_arrays::make(arr);
    EXPECT_EQ(r_arr, r_arr) << "Any r_array should be equal to itself.";
    
    auto r_arr2 = r_arrays::make(arr2);
    EXPECT_EQ(r_arr, r_arr2) << "Two r_arrays with the same elements should be equal.";

    int arr3[] = { 2, 4, 6, 8, 12 };
    auto r_arr3 = r_arrays::make(arr3);
    EXPECT_NE(r_arr, r_arr3) << "Two r_arrays with different elements should be unequal.";
}

TEST(RArrayTest, TestCompare)
{
    using ordering = std::strong_ordering;

    std::array<char, 5> apple_arr = { 'a', 'p', 'p', 'l', 'e' };
    auto apple_arr_2 = apple_arr;
    ASSERT_NE(apple_arr.data(), apple_arr_2.data()) << "Data must be unequal for this component of the test to work.";

    auto apple_r_arr = r_arrays::make(apple_arr);
    EXPECT_EQ(ordering::equal, apple_r_arr <=> apple_r_arr) << "Any r_array should compare as equal to itself.";

    auto apple_r_arr_2 = r_arrays::make(apple_arr_2);
    EXPECT_EQ(ordering::equal, apple_r_arr <=> apple_r_arr_2)
        << "Two r_arrays with the same elements should compare as equal.";
    
    std::array<char, 5> batch_arr = { 'b', 'a', 't', 'c', 'h' };
    auto batch_r_arr = r_arrays::make(batch_arr);
    EXPECT_EQ(ordering::less, apple_r_arr <=> batch_r_arr) << "The r_arrays should be compared lexicographically.";
}
