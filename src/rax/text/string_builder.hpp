#pragma once

#include <rax/shared.hpp>
#include <rax/environment.hpp>

#include <rax/string.hpp>

namespace rax::text
{
	class string_builder
	{
	public:
		string_builder() = default;
		string_builder(const string_builder&) = default;
		string_builder(string_builder&&) = default;

		RAX_INLINE string_builder(const char* string) : string_(string)
		{

		}

		RAX_INLINE string_builder(const wchar_t* string) : string_(string)
		{

		}

		RAX_INLINE string_builder(const char16_t* string) : string_(string)
		{

		}

		RAX_INLINE string_builder(const char32_t* string) : string_(string)
		{

		}

		RAX_INLINE string_builder(const std::string& string) : string_(string)
		{

		}

		RAX_INLINE string_builder(const std::wstring& string) : string_(string)
		{

		}

		RAX_INLINE string_builder(const std::u16string& string) : string_(string)
		{

		}

		RAX_INLINE string_builder(const std::u32string& string) : string_(string)
		{

		}

		RAX_INLINE auto string() const -> const rax::string&
		{
			return this->string_;
		}

		RAX_INLINE void append(const char* string)
		{
			this->string_ = rax::string::concat(this->string_, string);
		}

		RAX_INLINE void append(const wchar_t* string)
		{
			this->string_ = rax::string::concat(this->string_, string);
		}

		RAX_INLINE void append(const char16_t* string)
		{
			this->string_ = rax::string::concat(this->string_, string);
		}

		RAX_INLINE void append(const char32_t* string)
		{
			this->string_ = rax::string::concat(this->string_, string);
		}

		RAX_INLINE void append(const std::string& string)
		{
			this->string_ = rax::string::concat(this->string_, string);
		}

		RAX_INLINE void append(const std::wstring& string)
		{
			this->string_ = rax::string::concat(this->string_, string);
		}

		RAX_INLINE void append(const std::u16string& string)
		{
			this->string_ = rax::string::concat(this->string_, string);
		}

		RAX_INLINE void append(const std::u32string& string)
		{
			this->string_ = rax::string::concat(this->string_, string);
		}
		
		RAX_INLINE void append_line()
		{
			this->string_ = string::concat(this->string_, environment::new_line());
		}

		RAX_INLINE void replace(char oldc, char newc)
		{
			// to be done when rax::string::replace!
			// this->string_.replace(oldc, newc);
		}

		RAX_INLINE void replace(const rax::string& olds, const rax::string& news)
		{
			// to be done when rax::string::replace!
			// this->string_.replace(olds, news);
		}

		RAX_INLINE void substring(std::uint32_t begin)
		{
			this->string_ = this->string_.substring(begin);
		}

		RAX_INLINE void substring(std::uint32_t begin, std::uint32_t end)
		{
			this->string_ = this->string_.substring(begin, end);
		}

		auto operator=(string_builder&) -> string_builder& = default;
		auto operator=(string_builder&&) -> string_builder& = default;

	private:
		rax::string string_;
	};
}
