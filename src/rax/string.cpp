#include <rax/string.hpp>

#include <memory>
#include <cuchar>

#include <rax/text/decoder.hpp>
#include <rax/text/encoding.hpp>

#pragma warning(disable : 6386)  // buffer overrun warning
#pragma warning(disable : 26495) // always initialize member variables

namespace rax
{
	string::string(const char* ptr)
	{
		auto actual = rax::text::strex::strlen(ptr, false);
		
		this->length_ = actual - 1u;
		this->allocate_space(actual);
		
		::memcpy(this->ptr_, ptr, actual);
	}

	string::string(const wchar_t* ptr)
	{
		std::int32_t length;

#ifdef _HAS_CXX17
		if constexpr (sizeof(wchar_t) == sizeof(char16_t))
		{
			auto ptr16 = reinterpret_cast<const char16_t*>(ptr);
			length = rax::text::decoder::utf16_to_utf8_string(ptr16, nullptr);
		}
		else
		{
			auto ptr32 = reinterpret_cast<const char32_t*>(ptr);
			length = rax::text::decoder::utf32_to_utf8_string(ptr32, nullptr);
		}
#else
		if (sizeof(wchar_t) == sizeof(char16_t))
		{
			auto ptr16 = reinterpret_cast<const char16_t*>(ptr);
			length = rax::text::decoder::utf16_to_utf8_string(ptr16, nullptr);
		}
		else
		{
			auto ptr32 = reinterpret_cast<const char32_t*>(ptr);
			length = rax::text::decoder::utf32_to_utf8_string(ptr32, nullptr);
		}
#endif

		auto actual = static_cast<std::uint32_t>(length);

		this->length_ = actual - 1u;
		this->allocate_space(actual);

#ifdef _HAS_CXX17
		if constexpr (sizeof(wchar_t) == sizeof(char16_t))
		{
			auto ptr16 = reinterpret_cast<const char16_t*>(ptr);
			rax::text::decoder::utf16_to_utf8_string(ptr16, this->ptr_);
		}
		else
		{
			auto ptr32 = reinterpret_cast<const char32_t*>(ptr);
			rax::text::decoder::utf32_to_utf8_string(ptr32, this->ptr_);
		}
#else
		if (sizeof(wchar_t) == sizeof(char16_t))
		{
			auto ptr16 = reinterpret_cast<const char16_t*>(ptr);
			rax::text::decoder::utf16_to_utf8_string(ptr16, this->ptr_);
		}
		else
		{
			auto ptr32 = reinterpret_cast<const char32_t*>(ptr);
			rax::text::decoder::utf32_to_utf8_string(ptr32, this->ptr_);
		}
#endif
	}

	string::string(const char16_t* ptr)
	{
		auto length = rax::text::decoder::utf16_to_utf8_string(ptr, nullptr);
		auto actual = static_cast<std::uint32_t>(length);

		this->length_ = actual - 1u;
		this->allocate_space(actual);

		rax::text::decoder::utf16_to_utf8_string(ptr, this->ptr_);
	}

	string::string(const char32_t* ptr)
	{
		auto length = rax::text::decoder::utf32_to_utf8_string(ptr, nullptr);
		auto actual = static_cast<std::uint32_t>(length);

		this->length_ = actual - 1u;
		this->allocate_space(actual);

		rax::text::decoder::utf32_to_utf8_string(ptr, this->ptr_);
	}

	string::string(const char* ptr, std::uint32_t start, std::uint32_t length)
	{
		this->length_ = length;
		this->allocate_space(length + 1u);

		::memcpy(this->ptr_, ptr + start, length);
		this->ptr_[length] = 0;
	}

	string::string(const wchar_t* ptr, std::uint32_t start, std::uint32_t length)
	{
		std::int32_t actual;

#ifdef _HAS_CXX17
		if constexpr (sizeof(wchar_t) == sizeof(char16_t))
		{
			auto ptr16 = reinterpret_cast<const char16_t*>(ptr + start);
			actual = rax::text::decoder::utf16_to_utf8_string(ptr16, nullptr, length);
		}
		else
		{
			auto ptr32 = reinterpret_cast<const char32_t*>(ptr + start);
			actual = rax::text::decoder::utf32_to_utf8_string(ptr32, nullptr, length);
		}
#else
		if (sizeof(wchar_t) == sizeof(char16_t))
		{
			auto ptr16 = reinterpret_cast<const char16_t*>(ptr + start);
			actual = rax::text::decoder::utf16_to_utf8_string(ptr16, nullptr, length);
		}
		else
		{
			auto ptr32 = reinterpret_cast<const char32_t*>(ptr + start);
			actual = rax::text::decoder::utf32_to_utf8_string(ptr32, nullptr, length);
		}
#endif // _HAS_CXX17

		this->length_ = static_cast<std::uint32_t>(actual);
		this->allocate_space(this->length_ + 1u);

#ifdef _HAS_CXX17
		if constexpr (sizeof(wchar_t) == sizeof(char16_t))
		{
			auto ptr16 = reinterpret_cast<const char16_t*>(ptr + start);
			rax::text::decoder::utf16_to_utf8_string(ptr16, this->ptr_, length);
		}
		else
		{
			auto ptr32 = reinterpret_cast<const char32_t*>(ptr + start);
			rax::text::decoder::utf32_to_utf8_string(ptr32, this->ptr_, length);
		}
#else
		if (sizeof(wchar_t) == sizeof(char16_t))
		{
			auto ptr16 = reinterpret_cast<const char16_t*>(ptr + start);
			rax::text::decoder::utf16_to_utf8_string(ptr16, this->ptr_, length);
		}
		else
		{
			auto ptr32 = reinterpret_cast<const char32_t*>(ptr + start);
			rax::text::decoder::utf32_to_utf8_string(ptr32, this->ptr_, length);
		}
#endif // _HAS_CXX17

		this->ptr_[this->length_] = 0;
	}

	string::string(const char16_t* ptr, std::uint32_t start, std::uint32_t length)
	{
		auto actual = rax::text::decoder::utf16_to_utf8_string(ptr + start, nullptr, length);
		
		this->length_ = static_cast<std::uint32_t>(actual);
		this->allocate_space(this->length_ + 1u);

		rax::text::decoder::utf16_to_utf8_string(ptr + start, this->ptr_, length);
		this->ptr_[this->length_] = 0;
	}

	string::string(const char32_t* ptr, std::uint32_t start, std::uint32_t length)
	{
		auto actual = rax::text::decoder::utf32_to_utf8_string(ptr + start, nullptr, length);

		this->length_ = static_cast<std::uint32_t>(actual);
		this->allocate_space(this->length_ + 1u);

		rax::text::decoder::utf32_to_utf8_string(ptr + start, this->ptr_, length);
		this->ptr_[this->length_] = 0;
	}

	string::string(const char* ptr, std::uint32_t start, std::uint32_t length, rax::text::encoding_page enc)
	{
		auto topage = rax::text::encoding_page::UTF8;
		auto actual = rax::text::encoding::convert(enc, topage, ptr, nullptr, start, length);

		if (actual == -1)
		{
			this->ptr_ = this->buffer_;
			this->buffer_[0] = 0;
			this->length_ = 0u;
			return;
		}

		this->length_ = static_cast<std::uint32_t>(actual);
		this->allocate_space(this->length_ + 1u);

		rax::text::encoding::convert(enc, topage, ptr, this->ptr_, start, length);
		this->buffer_[this->length_] = 0;
	}

	string::string(const refarray<char>& arr) : string(arr.as_native(), 0u, arr.length())
	{
	}

	string::string(const refarray<wchar_t>& arr) : string(arr.as_native(), 0u, arr.length())
	{
	}

	string::string(const refarray<char16_t>& arr) : string(arr.as_native(), 0u, arr.length())
	{
	}

	string::string(const refarray<char32_t>& arr) : string(arr.as_native(), 0u, arr.length())
	{
	}

	string::string(const refarray<char>& arr, std::uint32_t start, std::uint32_t length) : string(arr.as_native(), start, length)
	{
	}

	string::string(const refarray<wchar_t>& arr, std::uint32_t start, std::uint32_t length) : string(arr.as_native(), start, length)
	{
	}

	string::string(const refarray<char16_t>& arr, std::uint32_t start, std::uint32_t length) : string(arr.as_native(), start, length)
	{
	}

	string::string(const refarray<char32_t>& arr, std::uint32_t start, std::uint32_t length) : string(arr.as_native(), start, length)
	{
	}

	string::string(const refarray<char>& arr, std::uint32_t start, std::uint32_t length, text::encoding_page enc) : string(arr.as_native(), start, length, enc)
	{
	}

	string::string(const std::string& str) : string(str.c_str())
	{
	}

	string::string(const std::wstring& str) : string(str.c_str())
	{
	}

	string::string(const std::u16string& str) : string(str.c_str())
	{
	}

	string::string(const std::u32string& str) : string(str.c_str())
	{
	}

	string::string(char c, std::uint32_t count)
	{
		this->length_ = count;
		this->allocate_space(this->length_ + 1u);

		::memset(this->ptr_, c, this->length_);
		this->ptr_[this->length_] = 0;
	}

	string::string(const string& other)
	{
		this->length_ = other.length_;
		
		if (other.ptr_ == other.buffer_)
		{
			this->ptr_ = this->buffer_;

			*reinterpret_cast<std::uint64_t*>(this->ptr_ + 0x00) = *reinterpret_cast<std::uint64_t*>(other.ptr_ + 0x00);
			*reinterpret_cast<std::uint64_t*>(this->ptr_ + 0x08) = *reinterpret_cast<std::uint64_t*>(other.ptr_ + 0x08);

#ifndef TARGET_64BIT
			*reinterpret_cast<std::uint64_t*>(this->ptr_ + 0x10) = *reinterpret_cast<std::uint64_t*>(other.ptr_ + 0x10);
#else
			*reinterpret_cast<std::uint32_t*>(this->ptr_ + 0x10) = *reinterpret_cast<std::uint32_t*>(other.ptr_ + 0x10);
#endif // TARGET_64BIT
		}
		else
		{
			auto count = this->length_ + 1u;
			this->ptr_ = new char[count];
			::memcpy(this->ptr_, other.ptr_, count);
		}
	}

	string& string::operator=(const string& other)
	{
		if (this != &other)
		{
			this->length_ = other.length_;

			if (other.ptr_ == other.buffer_)
			{
				this->ptr_ = this->buffer_;

				*reinterpret_cast<std::uint64_t*>(this->ptr_ + 0x00) = *reinterpret_cast<std::uint64_t*>(other.ptr_ + 0x00);
				*reinterpret_cast<std::uint64_t*>(this->ptr_ + 0x08) = *reinterpret_cast<std::uint64_t*>(other.ptr_ + 0x08);

#ifndef TARGET_64BIT
				* reinterpret_cast<std::uint64_t*>(this->ptr_ + 0x10) = *reinterpret_cast<std::uint64_t*>(other.ptr_ + 0x10);
#else
				* reinterpret_cast<std::uint32_t*>(this->ptr_ + 0x10) = *reinterpret_cast<std::uint32_t*>(other.ptr_ + 0x10);
#endif // TARGET_64BIT
			}
			else
			{
				auto count = this->length_ + 1u;
				this->ptr_ = new char[count];
				::memcpy(this->ptr_, other.ptr_, count);
			}
		}

		return *this;
	}

	//
	// casts
	//

	string::operator const char*()
	{
		return this->ptr_;
	}

	string::operator std::string()
	{
		auto actual = this->length_ + 1u;
		auto result = std::string(actual, 0);
		::memcpy(&result[0], this->ptr_, actual);
		return result;
	}

	string::operator std::wstring()
	{
		std::int32_t actual;

#ifdef _HAS_CXX17
		if constexpr (sizeof(wchar_t) == sizeof(char16_t))
		{
			actual = rax::text::decoder::utf8_to_utf16_string(this->ptr_, nullptr);
		}
		else
		{
			actual = rax::text::decoder::utf8_to_utf32_string(this->ptr_, nullptr);
		}
#else
		if (sizeof(wchar_t) == sizeof(char16_t))
		{
			actual = rax::text::decoder::utf8_to_utf16_string(this->ptr_, nullptr);
		}
		else
		{
			actual = rax::text::decoder::utf8_to_utf32_string(this->ptr_, nullptr);
		}
#endif // _HAS_CXX17

		auto result = std::wstring();

		if (actual == -1)
		{
			return result;
		}

		result.reserve(static_cast<std::uint32_t>(actual));

#ifdef _HAS_CXX17
		if constexpr (sizeof(wchar_t) == sizeof(char16_t))
		{
			rax::text::decoder::utf8_to_utf16_string(this->ptr_, reinterpret_cast<char16_t*>(&result[0]));
		}
		else
		{
			rax::text::decoder::utf8_to_utf32_string(this->ptr_, reinterpret_cast<char32_t*>(&result[0]));
		}
#else
		if (sizeof(wchar_t) == sizeof(char16_t))
		{
			rax::text::decoder::utf8_to_utf16_string(this->ptr_, reinterpret_cast<char16_t*>(&result[0]));
		}
		else
		{
			rax::text::decoder::utf8_to_utf32_string(this->ptr_, reinterpret_cast<char32_t*>(&result[0]));
		}
#endif // _HAS_CXX17

		return result;
	}

	string::operator std::u16string()
	{
		auto actual = rax::text::decoder::utf8_to_utf16_string(this->ptr_, nullptr);

		auto result = std::u16string();

		if (actual == -1)
		{
			return result;
		}

		result.reserve(static_cast<std::uint32_t>(actual));

		rax::text::decoder::utf8_to_utf16_string(this->ptr_, reinterpret_cast<char16_t*>(&result[0]));

		return result;
	}

	string::operator std::u32string()
	{
		auto actual = rax::text::decoder::utf8_to_utf32_string(this->ptr_, nullptr);

		auto result = std::u32string();

		if (actual == -1)
		{
			return result;
		}

		result.reserve(static_cast<std::uint32_t>(actual));

		rax::text::decoder::utf8_to_utf32_string(this->ptr_, reinterpret_cast<char32_t*>(&result[0]));

		return result;
	}

	//
	// operators == and !=
	//

	bool operator==(const string& lhs, const string& rhs)
	{
		return rax::text::strex::strcmp(lhs.as_native(), rhs.as_native());
	}

	bool operator!=(const string& lhs, const string& rhs)
	{
		return !rax::text::strex::strcmp(lhs.as_native(), rhs.as_native());
	}

	bool operator==(const string& lhs, const char* rhs)
	{
		return rax::text::strex::strcmp(lhs.as_native(), rhs);
	}

	bool operator!=(const string& lhs, const char* rhs)
	{
		return !rax::text::strex::strcmp(lhs.as_native(), rhs);
	}

	//
	// operators + and +=
	//

	auto operator+(const string& lhs, const string& rhs) -> string
	{
		auto result = string(lhs.length_ + rhs.length_);

		::memcpy(result.ptr_, lhs.ptr_, lhs.length_);
		::memcpy(result.ptr_ + lhs.length_, rhs.ptr_, rhs.length_);

		result.ptr_[result.length_] = 0;
		return result;
	}

	auto operator+(const string& lhs, const char* rhs) -> string
	{
		auto length = rax::text::strex::strlen(rhs, true);
		auto result = string(lhs.length_ + length);

		::memcpy(result.ptr_, lhs.ptr_, lhs.length_);
		::memcpy(result.ptr_ + lhs.length_, rhs, length);

		result.ptr_[result.length_] = 0;
		return result;
	}

	auto operator+(const char* lhs, const string& rhs) -> string
	{
		return rhs + lhs;
	}

	//
	// instance
	//

	auto string::substring(std::uint32_t start) -> string
	{
		assert(start < this->length_);
		return string(this->ptr_ + start);
	}

	auto string::substring(std::uint32_t start, std::uint32_t length) -> string
	{
		assert(start + length <= this->length_);
		return string(this->ptr_, start, length);
	}

	//
	// static
	//



}
