#include <rax/string/string.hpp>

#include <memory>
#include <cuchar>

#include <rax/math/math.hpp>
#include <rax/text/strex/strex.hpp>
#include <rax/text/decoder/decoder.hpp>

#pragma warning(disable : 6386)  // buffer overrun warning
#pragma warning(disable : 26495) // always initialize member variables

namespace rax
{
	string::string(const char* ptr)
	{
		this->length_ = rax::text::strex::strlen(ptr, false);

		if (this->length_ > string::kmax_buffer_size)
		{
			auto align = rax::math::align_pow_2(this->length_, 0x04u);
			this->ptr_ = new char[align];
		}
		else
		{
			this->ptr_ = this->buffer_;
		}

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

		if (length == -1)
		{
			this->length_ = 0u;
			this->ptr_ = this->buffer_;
			return;
		}

		this->length_ = static_cast<std::uint32_t>(length);

		if (this->length_ > string::kmax_buffer_size)
		{
			auto align = rax::math::align_pow_2(this->length_, 0x04u);
			this->ptr_ = new char[align];
		}
		else
		{
			this->ptr_ = this->buffer_;
		}

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

		if (length == -1)
		{
			this->length_ = 0u;
			this->ptr_ = this->buffer_;
			return;
		}

		this->length_ = static_cast<std::uint32_t>(length);

		if (this->length_ > string::kmax_buffer_size)
		{
			auto align = rax::math::align_pow_2(this->length_, 0x04u);
			this->ptr_ = new char[align];
		}
		else
		{
			this->ptr_ = this->buffer_;
		}

		rax::text::decoder::utf16_to_utf8_string(ptr, this->ptr_);
	}

	string::string(const char32_t* ptr)
	{
		auto length = rax::text::decoder::utf32_to_utf8_string(ptr, nullptr);

		if (length == -1)
		{
			this->length_ = 0u;
			this->ptr_ = this->buffer_;
			return;
		}

		this->length_ = static_cast<std::uint32_t>(length);

		if (this->length_ > string::kmax_buffer_size)
		{
			auto align = rax::math::align_pow_2(this->length_, 0x04u);
			this->ptr_ = new char[align];
		}
		else
		{
			this->ptr_ = this->buffer_;
		}

		rax::text::decoder::utf32_to_utf8_string(ptr, this->ptr_);
	}

	string::string(const char* ptr, std::uint32_t start, std::uint32_t length)
	{
		this->length_ = length + 1u;

		if (this->length_ > string::kmax_buffer_size)
		{
			auto align = rax::math::align_pow_2(this->length_, 0x04u);
			this->ptr_ = new char[align];
		}
		else
		{
			this->ptr_ = this->buffer_;
		}

		::memcpy(this->ptr_, ptr, length);
		this->ptr_[length] = 0u;
	}

	string::string(const wchar_t* ptr, std::uint32_t start, std::uint32_t length)
	{

	}

	string::string(const char16_t* ptr, std::uint32_t start, std::uint32_t length)
	{

	}

	string::string(const char32_t* ptr, std::uint32_t start, std::uint32_t length)
	{

	}
}
