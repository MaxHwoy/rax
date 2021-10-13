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
		this->length_ = rax::text::strex::strlen(ptr, false);
		this->allocate_space();
		::memcpy(this->ptr_, ptr, this->length_);
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

		this->length_ = static_cast<std::uint32_t>(length);
		this->allocate_space();

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

		this->length_ = static_cast<std::uint32_t>(length);
		this->allocate_space();

		rax::text::decoder::utf16_to_utf8_string(ptr, this->ptr_);
	}

	string::string(const char32_t* ptr)
	{
		auto length = rax::text::decoder::utf32_to_utf8_string(ptr, nullptr);

		this->length_ = static_cast<std::uint32_t>(length);
		this->allocate_space();

		rax::text::decoder::utf32_to_utf8_string(ptr, this->ptr_);
	}

	string::string(const char* ptr, std::uint32_t start, std::uint32_t length)
	{
		this->length_ = length + 1u;
		this->allocate_space();

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

		this->length_ = static_cast<std::uint32_t>(actual + 1);
		this->allocate_space();

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

		this->ptr_[actual] = 0;
	}

	string::string(const char16_t* ptr, std::uint32_t start, std::uint32_t length)
	{
		auto actual = rax::text::decoder::utf16_to_utf8_string(ptr + start, nullptr, length);
		
		this->length_ = static_cast<std::uint32_t>(actual + 1);
		this->allocate_space();

		rax::text::decoder::utf16_to_utf8_string(ptr + start, this->ptr_, length);
		this->ptr_[actual] = 0;
	}

	string::string(const char32_t* ptr, std::uint32_t start, std::uint32_t length)
	{
		auto actual = rax::text::decoder::utf32_to_utf8_string(ptr + start, nullptr, length);

		this->length_ = static_cast<std::uint32_t>(actual + 1);
		this->allocate_space();

		rax::text::decoder::utf32_to_utf8_string(ptr + start, this->ptr_, length);
		this->ptr_[actual] = 0;
	}

	string::string(const char* ptr, std::uint32_t start, std::uint32_t length, rax::text::encoding_page enc)
	{
		auto topage = rax::text::encoding_page::UTF8;
		auto actual = rax::text::encoding::convert(enc, topage, ptr, nullptr, start, length);

		if (actual == -1)
		{
			this->ptr_ = nullptr;
			this->length_ = 0u;
			return;
		}

		this->length_ = static_cast<std::uint32_t>(actual + 1);
		this->allocate_space();

		rax::text::encoding::convert(enc, topage, ptr, this->ptr_, start, length);
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
		this->length_ = count + 1u;
		this->allocate_space();

		::memset(this->ptr_, c, this->length_);
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
			this->ptr_ = new char[this->length_];
			::memcpy(this->ptr_, other.ptr_, this->length_);
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
				this->ptr_ = new char[this->length_];
				::memcpy(this->ptr_, other.ptr_, this->length_);
			}
		}

		return *this;
	}

	string::operator const char*()
	{
		return this->ptr_;
	}

	string::operator std::string()
	{
		auto result = std::string(this->length_, 0);
		::memcpy(&result[0], this->ptr_, this->length_);
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
}
