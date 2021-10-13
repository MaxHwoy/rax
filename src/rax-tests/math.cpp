#include <gtest/gtest.h>
#include <rax/math.hpp>

namespace rax::tests
{
	TEST(rax_math, abs)
	{
		EXPECT_EQ(5.0f, rax::math::abs<float>(-5.0f));
		
		EXPECT_EQ(7.4, rax::math::abs<double>(7.4));
		
		EXPECT_NE(-100, rax::math::abs<std::int32_t>(-100));
	}

	TEST(rax_math, is_prime)
	{
		EXPECT_TRUE(rax::math::is_prime(2u));
		EXPECT_TRUE(rax::math::is_prime(3u));
		EXPECT_TRUE(rax::math::is_prime(7u));
		EXPECT_TRUE(rax::math::is_prime(23u));
		EXPECT_TRUE(rax::math::is_prime(1301u));
		EXPECT_TRUE(rax::math::is_prime(2009191u));
		EXPECT_TRUE(rax::math::is_prime(7199369u));
		EXPECT_TRUE(rax::math::is_prime(39916801u));

		EXPECT_FALSE(rax::math::is_prime(4u));
		EXPECT_FALSE(rax::math::is_prime(25u));
		EXPECT_FALSE(rax::math::is_prime(117u));
		EXPECT_FALSE(rax::math::is_prime(4997u));
		EXPECT_FALSE(rax::math::is_prime(20171u));
		EXPECT_FALSE(rax::math::is_prime(121023u));
		EXPECT_FALSE(rax::math::is_prime(3010347u));
		EXPECT_FALSE(rax::math::is_prime(20996013u));
	}

	TEST(rax_math, max)
	{
		EXPECT_EQ(4, rax::math::max(2, 4));
		EXPECT_EQ(4, rax::math::max(4, 2));

		EXPECT_EQ(30.5f, rax::math::max(3.7f, 30.5f, -40.5f));
		EXPECT_EQ(30.5f, rax::math::max(30.5f, -40.5f, 3.7f));
		EXPECT_EQ(30.5f, rax::math::max(-40.5f, 3.7f, 30.5f));

		char arr[7] { 2, 44, 63, 94, 33, -54, -21 };
		EXPECT_EQ(94, rax::math::max(arr, 7u));

		EXPECT_EQ(100, rax::math::max<std::int32_t>(4u, 38, 59, 20, 100));
	}

	TEST(rax_math, min)
	{
		EXPECT_EQ(2, rax::math::min(2, 4));
		EXPECT_EQ(2, rax::math::min(4, 2));

		EXPECT_EQ(-40.5f, rax::math::min(3.7f, 30.5f, -40.5f));
		EXPECT_EQ(-40.5f, rax::math::min(30.5f, -40.5f, 3.7f));
		EXPECT_EQ(-40.5f, rax::math::min(-40.5f, 3.7f, 30.5f));

		char arr[7] { 2, 44, 63, 94, 33, -54, -21 };
		EXPECT_EQ(-54, rax::math::min(arr, 7u));

		EXPECT_EQ(20, rax::math::min<std::int32_t>(4u, 38, 59, 20, 100));
	}

	TEST(rax_math, minmax)
	{
		auto minmax1 = rax::math::minmax(2.0f, 4.0f);
		auto minmax2 = rax::math::minmax(4.0f, 2.0f);

		EXPECT_EQ(2.0f, minmax1.item1());
		EXPECT_EQ(4.0f, minmax1.item2());
		EXPECT_EQ(2.0f, minmax2.item1());
		EXPECT_EQ(4.0f, minmax2.item2());

		auto minmax3 = rax::math::minmax(24, -21, 59);
		auto minmax4 = rax::math::minmax(59, 24, -21);
		auto minmax5 = rax::math::minmax(-21, 59, 24);

		EXPECT_EQ(-21, minmax3.item1());
		EXPECT_EQ(-21, minmax4.item1());
		EXPECT_EQ(-21, minmax5.item1());
		EXPECT_EQ(+59, minmax3.item2());
		EXPECT_EQ(+59, minmax4.item2());
		EXPECT_EQ(+59, minmax5.item2());

		char arr1[7] = { 2, 44, 63, 94, 33, -54, -21 };
		double arr2[5] = { -30.0, 294.0, -492.0, 0.0, 42.0 };
		auto minmax6 = rax::math::minmax(arr1, 7u);
		auto minmax7 = rax::math::minmax(arr2, 5u);

		EXPECT_EQ(-54, minmax6.item1());
		EXPECT_EQ(+94, minmax6.item2());
		EXPECT_EQ(-492.0, minmax7.item1());
		EXPECT_EQ(+294.0, minmax7.item2());

		auto minmax8 = rax::math::minmax<char>(6u, 'A', 'X', 'D', 'F', 'Q', 'P');
		auto minmax9 = rax::math::minmax<std::int32_t>(3u, 7, 3, 9);

		EXPECT_EQ('A', minmax8.item1());
		EXPECT_EQ('X', minmax8.item2());
		EXPECT_EQ(3, minmax9.item1());
		EXPECT_EQ(9, minmax9.item2());
	}

	TEST(rax_math, sqrt)
	{
		EXPECT_NEAR(std::sqrtf(25.0f), rax::math::sqrt(25.0f), 0.00001f);
		EXPECT_NEAR(std::sqrtf(39.0f), rax::math::sqrt(39.0f), 0.00001f);
		EXPECT_TRUE(std::isnan(rax::math::sqrt(-1.0f)));
	}
}
