#pragma once

#include <cstdint>
#include <string>
#include <rax/shared.hpp>
#include <rax/bitwise.hpp>
#include <rax/refarray.hpp>
#include <rax/text/strex.hpp>

#pragma warning (disable : 26495)

namespace rax
{
	enum class string_comparison : std::int32_t
	{
		current_culture,
		current_culture_ignore_case,
		invariant_culture,
		invariant_culture_ignore_case,
		ordinal,
		ordinal_ignore_case,
	};

	namespace globalization
	{
		enum class compare_options : std::int32_t;

		class culture_info;
	}

	namespace text
	{
		enum class encoding_page : std::int32_t;
	}

	class string final
	{
	private:
#ifdef TARGET_64BIT
		// on 64 bit platforms ptr_ will take 8 bytes of memory, so for
		// alignment purposes we limit the internal buffer to 0x14 char16s
		// to make the total size of 0x14 + 0x08 + 0x04 = 0x20 bytes
		static inline const std::uint32_t kmax_buffer_size = 0x14;
		
		// on 64 bit platforms it is preferrable to align allocations by 8
		static inline const std::uint32_t kalloc_alignment = 0x08;
#else
		// on 32 bit platforms ptr_ will take 4 bytes of memory, so with
		// alignment of 0x20 bytes we can take the internal buffer to be of
		// size 0x18, which is 4 chars bigger than on x64 platforms
		static inline const std::uint32_t kmax_buffer_size = 0x18;
		
		// on 32 bit platforms it is preferrable to align allocations by 4
		static inline const std::uint32_t kalloc_alignment = 0x04;
#endif // TARGET_64BIT

		// by default we store strings in unicode/utf8 encoding
		char* ptr_;
		std::uint32_t length_;
		char buffer_[kmax_buffer_size];

		string(std::uint32_t length)
		{
			this->length_ = length;
			this->allocate_space(length + 1u);
		}

		RAX_INLINE void allocate_space(std::uint32_t size)
		{
			if (size > string::kmax_buffer_size)
			{
				this->ptr_ = new char[rax::bitwise::align_pow_2(size, string::kalloc_alignment)];
			}
			else
			{
				this->ptr_ = this->buffer_;
			}
		}

	public:
		// ***********************************************************************************************
		// constructors
		// ***********************************************************************************************
		
		RAX_INLINE string() noexcept
		{
			this->length_ = 0u;
			this->ptr_ = this->buffer_;
			this->ptr_[0] = 0;
		}
		string(const char* ptr);
		string(const wchar_t* ptr);
		string(const char16_t* ptr);
		string(const char32_t* ptr);
		string(const char* ptr, std::uint32_t start, std::uint32_t length);
		string(const wchar_t* ptr, std::uint32_t start, std::uint32_t length);
		string(const char16_t* ptr, std::uint32_t start, std::uint32_t length);
		string(const char32_t* ptr, std::uint32_t start, std::uint32_t length);
		string(const char* ptr, std::uint32_t start, std::uint32_t length, text::encoding_page enc);
		string(const refarray<char>& arr);
		string(const refarray<wchar_t>& arr);
		string(const refarray<char16_t>& arr);
		string(const refarray<char32_t>& arr);
		string(const refarray<char>& arr, std::uint32_t start, std::uint32_t length);
		string(const refarray<wchar_t>& arr, std::uint32_t start, std::uint32_t length);
		string(const refarray<char16_t>& arr, std::uint32_t start, std::uint32_t length);
		string(const refarray<char32_t>& arr, std::uint32_t start, std::uint32_t length);
		string(const refarray<char>& arr, std::uint32_t start, std::uint32_t length, text::encoding_page enc);
		string(const std::string& str);
		string(const std::wstring& str);
		string(const std::u16string& str);
		string(const std::u32string& str);
		string(char c, std::uint32_t count);
		string(const string& other);
		string& operator=(const string& str);
		RAX_INLINE string(string&& other) noexcept
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
				this->ptr_ = other.ptr_;
			}

			other.ptr_ = other.buffer_;
			other.length_ = 0u;
		}
		RAX_INLINE ~string()
		{
			if (this->ptr_ != this->buffer_)
			{
				delete[] this->ptr_;
				this->ptr_ = this->buffer_;
			}

			this->length_ = 0u;
			this->ptr_[0] = 0;
		}

		// ***********************************************************************************************
		// properties
		// ***********************************************************************************************
		
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

		// ***********************************************************************************************
		// casts
		// ***********************************************************************************************
		
		operator const char*();
		operator std::string();
		operator std::wstring();
		operator std::u16string();
		operator std::u32string();

		// ***********************************************************************************************
		// operators == and !=
		// ***********************************************************************************************
		
		friend bool operator==(const string& lhs, const string& rhs);
		friend bool operator!=(const string& lhs, const string& rhs);

		// ***********************************************************************************************
		// operators + and +=
		// ***********************************************************************************************
		
		friend auto operator+(const string& lhs, const string& rhs) -> string;
		friend auto operator+(const string& lhs, const char* rhs) -> string;
		friend auto operator+(const char* lhs, const string& rhs) -> string;
		friend auto operator+(const string& lhs, const wchar_t* rhs) -> string;
		friend auto operator+(const wchar_t* lhs, const string& rhs) -> string;
		friend auto operator+(const string& lhs, const char16_t* rhs) -> string;
		friend auto operator+(const char16_t* lhs, const string& rhs) -> string;
		friend auto operator+(const string& lhs, const char32_t* rhs) -> string;
		friend auto operator+(const char32_t* lhs, const string& rhs) -> string;
		friend auto operator+(const string& lhs, const std::string& rhs) -> string;
		friend auto operator+(const std::string& lhs, const string& rhs) -> string;
		friend auto operator+(const string& lhs, const std::wstring& rhs) -> string;
		friend auto operator+(const std::wstring& lhs, const string& rhs) -> string;
		friend auto operator+(const string& lhs, const std::u16string& rhs) -> string;
		friend auto operator+(const std::u16string& lhs, const string& rhs) -> string;
		friend auto operator+(const string& lhs, const std::u32string& rhs) -> string;
		friend auto operator+(const std::u32string& lhs, const string& rhs) -> string;

		// ***********************************************************************************************
		// instance
		// ***********************************************************************************************
		
		bool contains(const string& value);

		void copy_to(std::uint32_t src_index, char* dest, std::uint32_t dest_index, std::uint32_t length);

		bool ends_with(char value);
		bool ends_with(const char* value);
		bool ends_with(const string& value);
		bool ends_with(const string& value, string_comparison comparison);
		bool ends_with(const string& value, bool ignore_case, globalization::culture_info culture);

		bool equals(const string& value);
		bool equals(const string* value);
		bool equals(const string& value, string_comparison comparison);
		bool equals(const string* value, string_comparison comparison);

		// #TODO: index_of
		// #TODO: index_of_any
		// #TODO: last_index_of
		// #TODO: last_index_of_any

		auto pad_left(std::uint32_t width) -> string;
		auto pad_left(std::uint32_t width, std::int32_t value) -> string;
		auto pad_right(std::uint32_t width) -> string;
		auto pad_right(std::uint32_t width, std::int32_t value) -> string;

		auto remove(std::uint32_t start) -> string;
		auto remove(std::uint32_t start, std::uint32_t length) -> string;

		auto replace(char oldc, char newc) -> string;
		auto replace(const string& olds, const string& news) -> string;

		// #TODO: split

		bool starts_with(char value);
		bool starts_with(const char* value);
		bool starts_with(const string& value);
		bool starts_with(const string& value, string_comparison comparison);
		bool starts_with(const string& value, bool ignore_case, globalization::culture_info culture);

		auto substring(std::uint32_t start) -> string;
		auto substring(std::uint32_t start, std::uint32_t length) -> string;

		auto to_refarray() -> refarray<char>;
		auto to_refarray(std::uint32_t start, std::uint32_t length) -> refarray<char>;

		// #TODO: to_lower
		// #TODO: to_upper
		// #TODO: trim

		// ***********************************************************************************************
		// static
		// ***********************************************************************************************
		
		static auto compare(const string& a, const string& b) -> std::int32_t;
		static auto compare(const string& a, const string& b, bool ignore_case) -> std::int32_t;
		static auto compare(const string& a, const string& b, string_comparison comparison) -> std::int32_t;
		static auto compare(const string& a, const string& b, bool ignore_case, globalization::culture_info culture) -> std::int32_t;
		static auto compare(const string& a, const string& b, globalization::culture_info culture, globalization::compare_options options) -> std::int32_t;
		static auto compare(const string& a, std::uint32_t index_a, const string& b, std::uint32_t index_b, std::uint32_t length) -> std::int32_t;
		static auto compare(const string& a, std::uint32_t index_a, const string& b, std::uint32_t index_b, std::uint32_t length, bool ignore_case) -> std::int32_t;
		static auto compare(const string& a, std::uint32_t index_a, const string& b, std::uint32_t index_b, std::uint32_t length, string_comparison comparison) -> std::int32_t;
		static auto compare(const string& a, std::uint32_t index_a, const string& b, std::uint32_t index_b, std::uint32_t length, bool ignore_case, globalization::culture_info culture) -> std::int32_t;
		static auto compare(const string& a, std::uint32_t index_a, const string& b, std::uint32_t index_b, std::uint32_t length, globalization::culture_info culture, globalization::compare_options options) -> std::int32_t;

		static auto compare_ordinal(const string& a, const string& b) -> std::int32_t;
		static auto compare_ordinal(const string* a, const string* b) -> std::int32_t;
		static auto compare_ordinal(const string& a, std::uint32_t index_a, const string& b, std::uint32_t index_b, std::uint32_t length) -> std::int32_t;
		static auto compare_ordinal(const string* a, std::uint32_t index_a, const string* b, std::uint32_t index_b, std::uint32_t length) -> std::int32_t;

		static auto concat(const string& a, const string& b) -> string;
		static auto concat(const string& a, const char* b) -> string;
		static auto concat(const char* a, const string& b) -> string;
		static auto concat(const string& a, const wchar_t* b) -> string;
		static auto concat(const wchar_t* a, const string& b) -> string;
		static auto concat(const string& a, const char16_t* b) -> string;
		static auto concat(const char16_t* a, const string& b) -> string;
		static auto concat(const string& a, const char32_t* b) -> string;
		static auto concat(const char32_t* a, const string& b) -> string;
		RAX_INLINE static auto concat(const string& a, const std::string& b) -> string
		{
			return string::concat(a, b.c_str());
		}
		RAX_INLINE static auto concat(const std::string& a, const string& b) -> string
		{
			return string::concat(a.c_str(), b);
		}
		RAX_INLINE static auto concat(const string& a, const std::wstring& b) -> string
		{
			return string::concat(a, b.c_str());
		}
		RAX_INLINE static auto concat(const std::wstring& a, const string& b) -> string
		{
			return string::concat(a.c_str(), b);
		}
		RAX_INLINE static auto concat(const string& a, const std::u16string& b) -> string
		{
			return string::concat(a, b.c_str());
		}
		RAX_INLINE static auto concat(const std::u16string& a, const string& b) -> string
		{
			return string::concat(a.c_str(), b);
		}
		RAX_INLINE static auto concat(const string& a, const std::u32string& b) -> string
		{
			return string::concat(a, b.c_str());
		}
		RAX_INLINE static auto concat(const std::u32string& a, const string& b) -> string
		{
			return string::concat(a.c_str(), b);
		}
		static auto concat(const string& a, const string& b, const string& c) -> string;
		static auto concat(const string* ptr, std::uint32_t length) -> string;

		static bool equals(const string& a, const string& b);
		static bool equals(const string* a, const string* b);
		static bool equals(const string& a, const string& b, string_comparison comparison);
		static bool equals(const string* a, const string* b, string_comparison comparison);

		// #TODO: format

		RAX_INLINE static bool is_null_or_empty(const string& str)
		{
			return str.length_ == 0u;
		}
		RAX_INLINE static bool is_null_or_empty(const string* str)
		{
			return str == nullptr || str->length_ == 0u;
		}
		RAX_INLINE static bool is_null_or_whitespace(const string& str)
		{
			for (std::uint32_t i = 0u; i < str.length_; ++i)
			{
				if (!rax::text::strex::is_whitespace(str[i]))
				{
					return false;
				}
			}

			return true;
		}

		// #TODO: join
	};

	RAX_ASSERT_SIZE(string, 0x20);
}
