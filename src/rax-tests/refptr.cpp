#include <gtest/gtest.h>
#include <rax/refptr.hpp>

namespace rax::tests
{
	TEST(rax_refptr, ref_count)
	{
		auto intref = __makeref(std::int32_t);

		auto moreref = rax::refptr<std::int32_t>(intref);
		auto sameref = moreref;

		EXPECT_EQ(rax::refptr<std::int32_t>::get_refcount(sameref), 3u);
	}
}
