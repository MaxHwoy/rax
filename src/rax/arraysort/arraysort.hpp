#pragma once

#include <cstdint>
#include <rax/shared.hpp>
#include <rax/action/action.hpp>

namespace rax
{
	template <typename T> class arraysort final
	{
    private:
        static auto default_comparison(const T* x, const T* y) -> std::int32_t
        {
            return *x == *y ? 0 : *x < *y ? -1 : 1;
        }

        RAX_INLINE static void swap_if_greater(T* ptr, std::uint32_t a, std::uint32_t b)
        {
            if (a != b && ptr[a] > ptr[b])
            {
                auto temp = ptr[a];
                ptr[a] = ptr[b];
                ptr[b] = temp;
            }
        }
        RAX_INLINE static void swap_if_greater(T* ptr, std::uint32_t a, std::uint32_t b, comparison<T> comparer)
        {
            if (a != b && comparer(ptr + a, ptr + b) > 0)
            {
                auto temp = ptr[a];
                ptr[a] = ptr[b];
                ptr[b] = temp;
            }
        }

        static auto pick_pivot_and_partition(T* ptr, std::uint32_t lo, std::uint32_t hi) -> std::uint32_t
        {
            auto num = lo + ((hi - lo) >> 1);

            arraysort::swap_if_greater(ptr, lo, num);
            arraysort::swap_if_greater(ptr, lo, hi);
            arraysort::swap_if_greater(ptr, num, hi);

            auto value = ptr[num];

            ptr[num] = ptr[hi - 1];
            ptr[hi - 1] = value;
            
            auto num2 = static_cast<std::int32_t>(lo);
            auto num3 = static_cast<std::int32_t>(hi - 1);

            while (num2 < num3)
            {
                while (ptr[++num2] < value)
                {
                }

                while (value < ptr[--num3])
                {
                }

                if (num2 >= num3)
                {
                    break;
                }

                auto local = ptr[num2];

                ptr[num2] = ptr[num3];
                ptr[num3] = local;
            }

            auto tempo = ptr[num2];

            ptr[num2] = ptr[hi - 1];
            ptr[hi - 1] = tempo;
            
            return num2;
        }
        static auto pick_pivot_and_partition(T* ptr, std::uint32_t lo, std::uint32_t hi, comparison<T> comparer) -> std::uint32_t
        {
            auto num = lo + ((hi - lo) >> 1);

            arraysort::swap_if_greater(ptr, lo, num, comparer);
            arraysort::swap_if_greater(ptr, lo, hi, comparer);
            arraysort::swap_if_greater(ptr, num, hi, comparer);

            auto value = ptr[num];

            ptr[num] = ptr[hi - 1];
            ptr[hi - 1] = value;

            auto num2 = static_cast<std::int32_t>(lo);
            auto num3 = static_cast<std::int32_t>(hi - 1);

            while (num2 < num3)
            {
                while (comparer(ptr + ++num2, &value) < 0)
                {
                }

                while (comparer(&value, ptr + --num3) < 0)
                {
                }

                if (num2 >= num3)
                {
                    break;
                }

                auto local = ptr[num2];

                ptr[num2] = ptr[num3];
                ptr[num3] = local;
            }

            auto tempo = ptr[num2];

            ptr[num2] = ptr[hi - 1];
            ptr[hi - 1] = tempo;

            return num2;
        }

        static void internal_down_heap(T* ptr, std::int32_t i, std::int32_t n, std::int32_t lo)
        {
            auto value = ptr[lo + i - 1];

            while (i <= (n >> 1))
            {
                auto num = i << 1;

                if (num < n && ptr[lo + num - 1] < ptr[lo + num])
                {
                    ++num;
                }

                if (value >= ptr[lo + num - 1])
                {
                    break;
                }

                ptr[lo + i - 1] = ptr[lo + num - 1];

                i = num;
            }

            ptr[lo + i - 1] = value;
        }
        static void internal_down_heap(T* ptr, std::int32_t i, std::int32_t n, std::int32_t lo, comparison<T> comparer)
        {
            auto value = ptr[lo + i - 1];

            while (i <= (n >> 1))
            {
                auto num = i << 1;

                if (num < n && comparer(ptr + lo + num - 1, ptr + lo + num) < 0)
                {
                    ++num;
                }

                if (comparer(&value, ptr + lo + num - 1) >= 0)
                {
                    break;
                }

                ptr[lo + i - 1] = ptr[lo + num - 1];

                i = num;
            }

            ptr[lo + i - 1] = value;
        }

        static void internal_intro_sort(T* ptr, std::uint32_t lo, std::uint32_t hi, std::uint32_t maxdepth)
        {
            while (hi > lo)
            {
                auto num = hi - lo + 1u;

                if (num <= 0x10)
                {
                    switch (num)
                    {
                        case 1:
                            break;

                        case 2:
                            arraysort::swap_if_greater(ptr, lo, hi);
                            break;

                        case 3:
                            arraysort::swap_if_greater(ptr, lo, hi - 1u);
                            arraysort::swap_if_greater(ptr, lo, hi);
                            arraysort::swap_if_greater(ptr, hi - 1u, hi);
                            break;

                        default:
                            arraysort::internal_insertion_sort(ptr, lo, hi);
                            break;
                    }

                    return;
                }

                if (maxdepth == 0)
                {
                    return arraysort::internal_heap_sort(ptr, lo, hi);
                }

                auto next = arraysort::pick_pivot_and_partition(ptr, lo, hi);

                arraysort::internal_intro_sort(ptr, next + 1u, hi, --maxdepth);
                hi = next - 1u;
            }
        }
        static void internal_intro_sort(T* ptr, std::uint32_t lo, std::uint32_t hi, std::uint32_t maxdepth, comparison<T> comparer)
        {
            while (hi > lo)
            {
                auto num = hi - lo + 1u;

                if (num <= 0x10)
                {
                    switch (num)
                    {
                        case 1:
                            break;

                        case 2:
                            arraysort::swap_if_greater(ptr, lo, hi, comparer);
                            break;

                        case 3:
                            arraysort::swap_if_greater(ptr, lo, hi - 1u, comparer);
                            arraysort::swap_if_greater(ptr, lo, hi, comparer);
                            arraysort::swap_if_greater(ptr, hi - 1u, hi, comparer);
                            break;

                        default:
                            arraysort::internal_insertion_sort(ptr, lo, hi, comparer);
                            break;
                    }

                    return;
                }

                if (maxdepth == 0)
                {
                    return arraysort::internal_heap_sort(ptr, lo, hi, comparer);
                }

                auto next = arraysort::pick_pivot_and_partition(ptr, lo, hi, comparer);

                arraysort::internal_intro_sort(ptr, next + 1u, hi, --maxdepth, comparer);
                hi = next - 1u;
            }
        }

        static void internal_insertion_sort(T* ptr, std::uint32_t lo, std::uint32_t hi)
        {
            auto low = static_cast<std::int32_t>(lo);
            auto high = static_cast<std::int32_t>(hi);

            for (std::int32_t i = low; i < high; ++i)
            {
                auto count = i;
                auto value = ptr[i + 1];

                while (count >= low && value < ptr[count])
                {
                    ptr[count + 1] = ptr[count];
                    --count;
                }

                ptr[count + 1] = value;
            }
        }
        static void internal_insertion_sort(T* ptr, std::uint32_t lo, std::uint32_t hi, comparison<T> comparer)
        {
            if (comparer == nullptr)
            {
                comparer = &arraysort::default_comparison;
            }

            auto low = static_cast<std::int32_t>(lo);
            auto high = static_cast<std::int32_t>(hi);

            for (std::int32_t i = low; i < high; ++i)
            {
                auto count = i;
                auto value = ptr[i + 1];

                while (count >= low && comparer(&value, ptr + count) < 0)
                {
                    ptr[count + 1] = ptr[count];
                    --count;
                }

                ptr[count + 1] = value;
            }
        }

        static void internal_heap_sort(T* ptr, std::uint32_t lo, std::uint32_t hi)
        {
            auto low = static_cast<std::int32_t>(lo);
            auto high = static_cast<std::int32_t>(hi);

            auto num = high - low + 1;

            for (std::int32_t i = num >> 1; i >= 1; --i)
            {
                arraysort::internal_down_heap(ptr, i, num, low);
            }

            for (std::int32_t i = num; i > 1; --i)
            {
                auto index = low + i - 1;
                auto tempo = ptr[index];

                ptr[index] = ptr[low];
                ptr[low] = tempo;

                arraysort::internal_down_heap(ptr, 1, i - 1, low);
            }
        }
        static void internal_heap_sort(T* ptr, std::uint32_t lo, std::uint32_t hi, comparison<T> comparer)
        {
            if (comparer == nullptr)
            {
                comparer = &arraysort::default_comparison;
            }

            auto low = static_cast<std::int32_t>(lo);
            auto high = static_cast<std::int32_t>(hi);

            auto num = high - low + 1;

            for (std::int32_t i = num >> 1; i >= 1; --i)
            {
                arraysort::internal_down_heap(ptr, i, num, low, comparer);
            }

            for (std::int32_t i = num; i > 1; --i)
            {
                auto index = low + i - 1;
                auto tempo = ptr[index];

                ptr[index] = ptr[low];
                ptr[low] = tempo;

                arraysort::internal_down_heap(ptr, 1, i - 1, low, comparer);
            }
        }

        static void internal_quick_sort(T* ptr, std::int32_t left, std::int32_t right, std::uint32_t maxdepth)
        {
            do
            {
                if (maxdepth == 0)
                {
                    arraysort::internal_heap_sort(ptr, left, right);
                    return;
                }

                std::int32_t num = left;
                std::int32_t num2 = right;
                std::int32_t num3 = num + ((num2 - num) >> 1);

                arraysort::swap_if_greater(ptr, num, num3);
                arraysort::swap_if_greater(ptr, num, num2);
                arraysort::swap_if_greater(ptr, num3, num2);

                auto value = ptr[num3];

                while (true)
                {
                    if (ptr[num] < value)
                    {
                        ++num;
                        continue;
                    }

                    while (value < ptr[num2])
                    {
                        --num2;
                    }

                    if (num > num2)
                    {
                        break;
                    }

                    if (num < num2)
                    {
                        auto temp = ptr[num];

                        ptr[num] = ptr[num2];
                        ptr[num2] = temp;
                    }

                    ++num;
                    --num2;

                    if (num > num2)
                    {
                        break;
                    }
                }

                --maxdepth;

                if (num2 - left <= right - num)
                {
                    if (left < num2)
                    {
                        arraysort::internal_quick_sort(ptr, left, num2, maxdepth);
                    }

                    left = num;
                }
                else
                {
                    if (num < right)
                    {
                        arraysort::internal_quick_sort(ptr, num, right, maxdepth);
                    }

                    right = num2;
                }

            } while (left < right);
        }
        static void internal_quick_sort(T* ptr, std::int32_t left, std::int32_t right, std::uint32_t maxdepth, comparison<T> comparer)
        {
            do
            {
                if (maxdepth == 0)
                {
                    arraysort::internal_heap_sort(ptr, left, right, comparer);
                    return;
                }

                std::int32_t num = left;
                std::int32_t num2 = right;
                std::int32_t num3 = num + ((num2 - num) >> 1);

                arraysort::swap_if_greater(ptr, num, num3, comparer);
                arraysort::swap_if_greater(ptr, num, num2, comparer);
                arraysort::swap_if_greater(ptr, num3, num2, comparer);

                auto value = ptr[num3];

                while (true)
                {
                    if (comparer(ptr + num, &value) < 0)
                    {
                        ++num;
                        continue;
                    }

                    while (comparer(&value, ptr + num2) < 0)
                    {
                        --num2;
                    }

                    if (num > num2)
                    {
                        break;
                    }

                    if (num < num2)
                    {
                        auto temp = ptr[num];

                        ptr[num] = ptr[num2];
                        ptr[num2] = temp;
                    }

                    ++num;
                    --num2;

                    if (num > num2)
                    {
                        break;
                    }
                }

                --maxdepth;

                if (num2 - left <= right - num)
                {
                    if (left < num2)
                    {
                        arraysort::internal_quick_sort(ptr, left, num2, maxdepth, comparer);
                    }

                    left = num;
                }
                else
                {
                    if (num < right)
                    {
                        arraysort::internal_quick_sort(ptr, num, right, maxdepth, comparer);
                    }

                    right = num2;
                }

            } while (left < right);
        }

	public:
        static void introspective_sort(T* ptr, std::uint32_t start, std::uint32_t length)
        {
            std::uint32_t lo = start;
            std::uint32_t hi = start + length - 1;
            std::uint32_t maxdepth = 0u;

            while (length >= 1u)
            {
                ++maxdepth;
                length >>= 1;
            }

            arraysort::internal_intro_sort(ptr, lo, hi, maxdepth);
        }
        static void introspective_sort(T* ptr, std::uint32_t start, std::uint32_t length, comparison<T> comparer)
        {
            if (comparer == nullptr)
            {
                comparer = &arraysort::default_comparison;
            }

            std::uint32_t lo = start;
            std::uint32_t hi = start + length - 1;
            std::uint32_t maxdepth = 0u;

            while (length >= 1u)
            {
                ++maxdepth;
                length >>= 1;
            }

            arraysort::internal_intro_sort(ptr, lo, hi, maxdepth, comparer);
        }
	
        static void insertion_sort(T* ptr, std::uint32_t start, std::uint32_t length)
        {
            arraysort::internal_insertion_sort(ptr, start, start + length - 1u);
        }
        static void insertion_sort(T* ptr, std::uint32_t start, std::uint32_t length, comparison<T> comparer)
        {
            arraysort::internal_insertion_sort(ptr, start, start + length - 1u, comparer);
        }

        static void heap_sort(T* ptr, std::uint32_t start, std::uint32_t length)
        {
            arraysort::internal_heap_sort(ptr, start, start + length - 1u);
        }
        static void heap_sort(T* ptr, std::uint32_t start, std::uint32_t length, comparison<T> comparer)
        {
            arraysort::internal_heap_sort(ptr, start, start + length - 1u, comparer);
        }

        static void quick_sort(T* ptr, std::uint32_t start, std::uint32_t length)
        {
            arraysort::internal_quick_sort(ptr, start, start + length - 1u, 0x20);
        }
        static void quick_sort(T* ptr, std::uint32_t start, std::uint32_t length, comparison<T> comparer)
        {
            arraysort::internal_quick_sort(ptr, start, start + length - 1u, 0x20, comparer);
        }

        static void merge_sort(T* ptr, std::uint32_t start, std::uint32_t length)
        {

        }
        static void merge_sort(T* ptr, std::uint32_t start, std::uint32_t length, comparison<T> comparer)
        {

        }

        static void selection_sort(T* ptr, std::uint32_t start, std::uint32_t length)
        {
            auto end = start + length;

            for (std::uint32_t i = start; i < end - 1u; ++i)
            {
                auto min = i;

                for (std::uint32_t j = i + 1u; j < end; ++j)
                {
                    if (ptr[j] < ptr[min])
                    {
                        min = j;
                    }
                }

                auto temp = ptr[i];

                ptr[i] = ptr[min];
                ptr[min] = temp;
            }
        }
        static void selection_sort(T* ptr, std::uint32_t start, std::uint32_t length, comparison<T> comparer)
        {
            auto end = start + length;

            for (std::uint32_t i = start; i < end - 1u; ++i)
            {
                auto min = i;

                for (std::uint32_t j = i + 1u; j < end; ++j)
                {
                    if (comparer(ptr + j, ptr + min) < 0)
                    {
                        min = j;
                    }
                }

                auto temp = ptr[i];

                ptr[i] = ptr[min];
                ptr[min] = temp;
            }
        }

        static void bubble_sort(T* ptr, std::uint32_t start, std::uint32_t length)
        {
            auto end = start + length - 1u;

            for (std::uint32_t i = start; i < end; ++i)
            {
                bool swapped = false;

                for (std::uint32_t j = 0u; j < end - i; ++j)
                {
                    if (ptr[j] > ptr[j + 1])
                    {
                        auto temp = ptr[j];

                        ptr[j] = ptr[j + 1];
                        ptr[j + 1] = temp;

                        swapped = true;
                    }
                }

                if (!swapped)
                {
                    break;
                }
            }
        }
        static void bubble_sort(T* ptr, std::uint32_t start, std::uint32_t length, comparison<T> comparer)
        {
            auto end = start + length - 1u;

            for (std::uint32_t i = start; i < end; ++i)
            {
                bool swapped = false;

                for (std::uint32_t j = 0u; j < end - i; ++j)
                {
                    if (comparer(ptr + j, ptr + j + 1u) > 0)
                    {
                        auto temp = ptr[j];

                        ptr[j] = ptr[j + 1];
                        ptr[j + 1] = temp;

                        swapped = true;
                    }
                }

                if (!swapped)
                {
                    break;
                }
            }
        }
    };
}
