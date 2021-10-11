#pragma once

#include <cstdint>
#include <rax/shared.hpp>

namespace rax::text
{
	class strex final
	{
	public:
		/// <summary>
		/// Returns string length of any null-terminated sequence of type T.
		/// </summary>
		/// <param name="ptr">Pointer to the beginning of the sequence.</param>
		/// <param name="doNotCountTerminator">True if null-terminator should not be counted as part
		/// of the string length; false if null-terminator should be considered as part of string length.</param>
		/// <returns>Size of null-terminated sequence as unsigned 32 bit integer.</returns>
		template <typename T> RAX_INLINE static auto strlen(const T* ptr, bool doNotCountTerminator) -> std::uint32_t
		{
			auto address = ptr;
			while (*ptr++);
			return ptr - address - doNotCountTerminator;
		}

		/// <summary>
		/// Compares two null-terminated sequences and returns true if they equal each other.
		/// </summary>
		/// <param name="ptr1">Pointer to the first null-terminated sequence.</param>
		/// <param name="ptr2">Pointer to the second null-terminated sequence.</param>
		/// <returns>True if two sequences equal; otherwise, false.</returns>
		template <typename T> RAX_INLINE static bool strcmp(const T* ptr1, const T* ptr2)
		{
			while (*ptr1 || *ptr2)
			{
				if (*ptr1++ != *ptr2++)
				{
					return false;
				}
			}

			return true;
		}
	};
}
