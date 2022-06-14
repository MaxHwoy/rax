#pragma once

#include <cstdint>
#include <rax/shared.hpp>

namespace rax::text
{
	class strex final
	{
	private:
		RAX_INLINE static bool is_whitespace_latin(std::uint32_t c)
		{
			switch (c)
			{
				case '\t':
				case '\n':
				case '\v':
				case '\f':
				case '\r':
				case ' ':
				case 0xA0:
				case 0x85:
					return true;

				default:
					return false;
			}
		}

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
			return static_cast<std::uint32_t>(ptr - address - doNotCountTerminator);
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

		RAX_INLINE static bool is_latin(std::uint32_t c)
		{
			return c <= 255;
		}

		static bool is_whitespace(std::uint32_t c)
		{
			if (strex::is_latin(c))
			{
				return strex::is_whitespace_latin(c);
			}

			// #TODO - CharUnicodeInfo
			return false;
		}
	};
}
