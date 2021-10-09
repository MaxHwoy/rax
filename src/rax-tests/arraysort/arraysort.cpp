#include <gtest/gtest.h>
#include <rax/arraysort/arraysort.hpp>

namespace rax::tests
{
	template <typename T> using sort_forward = void(*)(T* arr, std::uint32_t start, std::uint32_t length);
	template <typename T> using sort_reverse = void(*)(T* arr, std::uint32_t start, std::uint32_t length, comparison<T> comparer);

	template <typename T> auto reverse_compare(const T* a, const T* b) -> std::int32_t
	{
		if (*a == *b)
		{
			return 0;
		}

		if (*a < *b)
		{
			return 1;
		}

		return -1;
	}

	template <typename T> void fill_randomized(T* ptr, std::uint32_t size)
	{
		for (std::uint32_t i = 0u; i < size; ++i)
		{
			ptr[i] = std::rand(); // #TODO
		}
	}

	template <typename T> void make_random_array(T*& ptr, std::uint32_t* size)
	{
		*size = 0x30; //std::rand();

		ptr = new T[*size];
	}

	template <typename T> void free_random_array(T* ptr)
	{
		delete[] ptr;
	}

	template <typename T> bool check_increasing(const T* ptr, std::uint32_t size)
	{
		for (std::uint32_t i = 1u; i < size; ++i)
		{
			if (ptr[i - 1] > ptr[i])
			{
				return false;
			}
		}

		return true;
	}

	template <typename T> bool check_decreasing(const T* ptr, std::uint32_t size)
	{
		for (std::uint32_t i = 1u; i < size; ++i)
		{
			if (ptr[i - 1] < ptr[i])
			{
				return false;
			}
		}

		return true;
	}

	template <typename T> void perform_sort_test(sort_forward<T> forward, sort_reverse<T> reverse)
	{
		T* arr_fo = nullptr;
		T* arr_re = nullptr;
		std::uint32_t size_fo;
		std::uint32_t size_re;

		make_random_array<T>(arr_fo, &size_fo);
		make_random_array<T>(arr_re, &size_re);

		fill_randomized<T>(arr_fo, size_fo);
		fill_randomized<T>(arr_re, size_re);

		forward(arr_fo, 0u, size_fo);
		reverse(arr_re, 0u, size_re, &reverse_compare);

		//for (std::uint32_t i = 0u; i < size_fo; ++i)
		//{
		//	::printf("[%d] ", arr_fo[i]);
		//}
		//
		//::printf("\n");
		//
		//for (std::uint32_t i = 0u; i < size_re; ++i)
		//{
		//	::printf("[%d] ", arr_re[i]);
		//}
		//
		//::printf("\n");

		EXPECT_TRUE(check_increasing<T>(arr_fo, size_fo));
		EXPECT_TRUE(check_decreasing<T>(arr_re, size_re));

		free_random_array<T>(arr_fo);
		free_random_array<T>(arr_re);
	}

	TEST(rax_arraysort, introspective_sort)
	{
		perform_sort_test<std::int32_t>(&rax::arraysort<std::int32_t>::introspective_sort, &rax::arraysort<std::int32_t>::introspective_sort);
	}

	TEST(rax_arraysort, insertion_sort)
	{
		perform_sort_test<std::int32_t>(&rax::arraysort<std::int32_t>::insertion_sort, &rax::arraysort<std::int32_t>::insertion_sort);
	}

	TEST(rax_arraysort, heap_sort)
	{
		perform_sort_test<std::int32_t>(&rax::arraysort<std::int32_t>::heap_sort, &rax::arraysort<std::int32_t>::heap_sort);
	}
}
