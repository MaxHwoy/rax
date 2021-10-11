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
			this->ptr_ = new char16_t[align];
		}
		else
		{
			this->ptr_ = this->buffer_;
		}

		// cannot do memcpy, do for-loop iteration instead
		for (std::uint32_t i = 0u; i < this->length_; ++i)
		{
			this->ptr_[i] = ptr[i];
		}
	}

	string::string(const wchar_t* ptr)
	{
		this->length_ = rax::text::strex::strlen(ptr, false);

		if (this->length_ > string::kmax_buffer_size)
		{
			auto align = rax::math::align_pow_2(this->length_, 0x04u);
			this->ptr_ = new char16_t[align];
		}
		else
		{
			this->ptr_ = this->buffer_;
		}

#if defined(_HAS_CXX17)
		if constexpr (sizeof(wchar_t) == sizeof(char16_t))
		{
			// if wchar_t size is 2 bytes, go ahead and do memcpy
			::memcpy(this->ptr_, ptr, this->length_ * sizeof(char16_t));
		}
		else
		{
			// otherwise we have no option other than for-looping
			for (std::uint32_t i = 0u; i < this->length_; ++i)
			{
				this->ptr_[i] = static_cast<char16_t>(ptr[i]);
			}
		}
#else
		if (sizeof(wchar_t) == sizeof(char16_t))
		{
			// if wchar_t size is 2 bytes, go ahead and do memcpy
			::memcpy(this->ptr_, ptr, this->length_ * sizeof(char16_t));
		}
		else
		{
			// otherwise we have no option other than for-looping
			for (std::uint32_t i = 0u; i < this->length_; ++i)
			{
				this->ptr_[i] = static_cast<char16_t>(ptr[i]);
			}
		}
#endif
	}

	string::string(const char16_t* ptr)
	{
		this->length_ = rax::text::strex::strlen(ptr, false);

		if (this->length_ > string::kmax_buffer_size)
		{
			auto align = rax::math::align_pow_2(this->length_, 0x04u);
			this->ptr_ = new char16_t[align];
		}
		else
		{
			this->ptr_ = this->buffer_;
		}

		::memcpy(this->ptr_, ptr, this->length_ * sizeof(char16_t));
	}

	string::string(const char32_t* ptr)
	{
		this->length_ = rax::text::strex::strlen(ptr, false);

		if (this->length_ > string::kmax_buffer_size)
		{
			auto align = rax::math::align_pow_2(this->length_, 0x04u);
			this->ptr_ = new char16_t[align];
		}
		else
		{
			this->ptr_ = this->buffer_;
		}

		// cannot do memcpy, do for-loop iteration instead
		for (std::uint32_t i = 0u; i < this->length_; ++i)
		{
			this->ptr_[i] = static_cast<char16_t>(ptr[i]);
		}
	}

	string::string(const char* ptr, std::uint32_t start, std::uint32_t length)
	{
		this->length_ = length;

		if (this->length_ > string::kmax_buffer_size)
		{
			auto align = rax::math::align_pow_2(this->length_, 0x04u);
			this->ptr_ = new char16_t[align];
		}
		else
		{
			this->ptr_ = this->buffer_;
		}

		// cannot do memcpy, do for-loop iteration instead
		for (std::uint32_t i = 0u; i < this->length_; ++i)
		{
			this->ptr_[i] = ptr[i];
		}
	}

	string::string(const wchar_t* ptr, std::uint32_t start, std::uint32_t length)
	{
		this->length_ = length;

		if (this->length_ > string::kmax_buffer_size)
		{
			auto align = rax::math::align_pow_2(this->length_, 0x04u);
			this->ptr_ = new char16_t[align];
		}
		else
		{
			this->ptr_ = this->buffer_;
		}

#if defined(_HAS_CXX17)
		if constexpr (sizeof(wchar_t) == sizeof(char16_t))
		{
			// if wchar_t size is 2 bytes, go ahead and do memcpy
			::memcpy(this->ptr_, ptr, this->length_ * sizeof(char16_t));
		}
		else
		{
			// otherwise we have no option other than for-looping
			for (std::uint32_t i = 0u; i < this->length_; ++i)
			{
				this->ptr_[i] = static_cast<char16_t>(ptr[i]);
			}
		}
#else
		if (sizeof(wchar_t) == sizeof(char16_t))
		{
			// if wchar_t size is 2 bytes, go ahead and do memcpy
			::memcpy(this->ptr_, ptr, this->length_ * sizeof(char16_t));
		}
		else
		{
			// otherwise we have no option other than for-looping
			for (std::uint32_t i = 0u; i < this->length_; ++i)
			{
				this->ptr_[i] = static_cast<char16_t>(ptr[i]);
			}
		}
#endif
	}

	string::string(const char16_t* ptr, std::uint32_t start, std::uint32_t length)
	{
		this->length_ = length;

		if (this->length_ > string::kmax_buffer_size)
		{
			auto align = rax::math::align_pow_2(this->length_, 0x04u);
			this->ptr_ = new char16_t[align];
		}
		else
		{
			this->ptr_ = this->buffer_;
		}

		::memcpy(this->ptr_, ptr, this->length_ * sizeof(char16_t));
	}

	string::string(const char32_t* ptr, std::uint32_t start, std::uint32_t length)
	{
		this->length_ = length;

		if (this->length_ > string::kmax_buffer_size)
		{
			auto align = rax::math::align_pow_2(this->length_, 0x04u);
			this->ptr_ = new char16_t[align];
		}
		else
		{
			this->ptr_ = this->buffer_;
		}

		// cannot do memcpy, do for-loop iteration instead
		for (std::uint32_t i = 0u; i < this->length_; ++i)
		{
			this->ptr_[i] = static_cast<char16_t>(ptr[i]);
		}
	}
}
