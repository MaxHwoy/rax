#pragma once

#include <cstdint>
#include <string>
#include <rax/shared.hpp>
#include <rax/refarray/refarray.hpp>

namespace rax
{
	class encoding;

	class string final
	{
	private:
#ifdef TARGET_64BIT
		// on 64 bit platforms ptr_ will take 8 bytes of memory, so for
		// alignment purposes we limit the internal buffer to 0x14 char16s
		// to make the total size of 0x14 + 0x08 + 0x04 = 0x20 bytes
		static inline const std::uint32_t kmax_buffer_size = 0x14;
#else
		// on 32 bit platforms ptr_ will take 4 bytes of memory, so with
		// alignment of 0x20 bytes we can take the internal buffer to be of
		// size 0x18, which is 4 chars bigger than on x64 platforms
		static inline const std::uint32_t kmax_buffer_size = 0x18;
#endif // TARGET_64BIT

		// by default we store strings in unicode/utf8 encoding
		char* ptr_;
		std::uint32_t length_;
		char buffer_[kmax_buffer_size];

	public:
		string(const char* ptr);
		string(const wchar_t* ptr);
		string(const char16_t* ptr);
		string(const char32_t* ptr);
		string(const char* ptr, std::uint32_t start, std::uint32_t length);
		string(const wchar_t* ptr, std::uint32_t start, std::uint32_t length);
		string(const char16_t* ptr, std::uint32_t start, std::uint32_t length);
		string(const char32_t* ptr, std::uint32_t start, std::uint32_t length);
		string(const char* ptr, std::uint32_t start, std::uint32_t length, encoding enc);
		string(const refarray<char>& arr);
		string(const refarray<wchar_t>& arr);
		string(const refarray<char16_t>& arr);
		string(const refarray<char32_t>& arr);
		string(const refarray<char>& arr, std::uint32_t start, std::uint32_t length);
		string(const refarray<wchar_t>& arr, std::uint32_t start, std::uint32_t length);
		string(const refarray<char16_t>& arr, std::uint32_t start, std::uint32_t length);
		string(const refarray<char32_t>& arr, std::uint32_t start, std::uint32_t length);
		string(const refarray<char>& arr, std::uint32_t start, std::uint32_t length, encoding enc);
		string(const std::string& str);
		string(const std::wstring& str);
		string(const std::u16string& str);
		string(const std::u32string& str);
		string(char16_t c, std::uint32_t count);
		string(const string& other);
		string(string&& other);
		string& operator=(const string& str);
		RAX_INLINE ~string()
		{
			// if not nullptr and not allocated locally, cleanup
			if (this->ptr_ != nullptr && this->ptr_ != this->buffer_)
			{
				delete[] this->ptr_;
				this->ptr_ = nullptr;
				this->length_ = 0u;
			}
		}

		RAX_INLINE auto operator[](std::uint32_t index) const -> char
		{
			assert(index < this->length_);
			return this->ptr_[index];
		}
		RAX_INLINE auto operator[](std::int32_t index) const -> char
		{
			assert(static_cast<std::uint32_t>(index) < this->length_);
			return this->ptr_[index];
		}
		RAX_INLINE auto length() const -> std::uint32_t
		{
			return this->length_;
		}
		RAX_INLINE auto as_native() const -> const char*
		{
			return this->ptr_;
		}

		operator std::string();
		operator std::wstring();
		operator std::u16string();
		operator std::u32string();

		friend bool operator==(const string& lhs, const string& rhs);
		friend bool operator!=(const string& lhs, const string& rhs);
		friend bool operator==(const string& lhs, const char* rhs);
		friend bool operator!=(const string& lhs, const char* rhs);
		friend bool operator==(const string& lhs, const wchar_t* rhs);
		friend bool operator!=(const string& lhs, const wchar_t* rhs);
		friend bool operator!=(const string& lhs, const char16_t* rhs);
		friend bool operator!=(const string& lhs, const char16_t* rhs);
		friend bool operator!=(const string& lhs, const char32_t* rhs);
		friend bool operator!=(const string& lhs, const char32_t* rhs);
		friend bool operator==(const string& lhs, const std::string& rhs);
		friend bool operator!=(const string& lhs, const std::string& rhs);
		friend bool operator==(const string& lhs, const std::wstring& rhs);
		friend bool operator!=(const string& lhs, const std::wstring& rhs);
		friend bool operator==(const string& lhs, const std::u16string& rhs);
		friend bool operator!=(const string& lhs, const std::u16string& rhs);
		friend bool operator==(const string& lhs, const std::u32string& rhs);
		friend bool operator!=(const string& lhs, const std::u32string& rhs);
	};
}
