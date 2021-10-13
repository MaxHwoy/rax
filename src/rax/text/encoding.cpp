#include <rax/text/encoding.hpp>

namespace rax::text
{
	collections::templates::map<std::int32_t, encoding> encoding::converters_{}; // #TODO

	auto encoding::convert(encoding_page from, encoding_page to, const void* src, void* dest) -> std::int32_t
	{
		return -1;
	}

	auto encoding::convert(encoding_page from, encoding_page to, const void* src, void* dest, std::uint32_t start, std::uint32_t length) -> std::int32_t
	{
		return -1;
	}
}
