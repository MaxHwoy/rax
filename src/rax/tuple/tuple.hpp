#pragma once

namespace rax
{
	template <typename T1, typename T2> class tuple2
	{
	private:
		T1 item1_{};
		T2 item2_{};

	public:
		RAX_INLINE tuple2()
		{
		}
		RAX_INLINE tuple2(const T1& i1, const T2& i2) : item1_(i1), item2_(i2)
		{
		}
		RAX_INLINE tuple2(const tuple2& other) : item1_(other.item1_), item2_(other.item2_)
		{
		}
		RAX_INLINE tuple2& operator=(const tuple2& other)
		{
			if (this != &other)
			{
				this->item1_ = other.item1_;
				this->item2_ = other.item2_;
			}

			return *this;
		}

		RAX_INLINE auto item1() const -> const T1&
		{
			return this->item1_;
		}
		RAX_INLINE auto item2() const -> const T2&
		{
			return this->item2_;
		}
	};

	template <typename T1, typename T2, typename T3> class tuple3
	{
	private:
		T1 item1_{};
		T2 item2_{};
		T3 item3_{};

	public:
		RAX_INLINE tuple3()
		{
		}
		RAX_INLINE tuple3(const T1& i1, const T2& i2, const T3& i3) : item1_(i1), item2_(i2), item3_(i3)
		{
		}
		RAX_INLINE tuple3(const tuple3& other) : item1_(other.item1_), item2_(other.item2_), item3_(other.item3_)
		{
		}
		RAX_INLINE tuple3& operator=(const tuple3& other)
		{
			if (this != &other)
			{
				this->item1_ = other.item1_;
				this->item2_ = other.item2_;
				this->item3_ = other.item3_;
			}

			return *this;
		}

		RAX_INLINE auto item1() const -> const T1&
		{
			return this->item1_;
		}
		RAX_INLINE auto item2() const -> const T2&
		{
			return this->item2_;
		}
		RAX_INLINE auto item3() const -> const T3&
		{
			return this->item3_;
		}
	};
}
