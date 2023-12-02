//
// Created by Peter Westerstrom on 2020-12-18.
//

#ifndef TUPLE_MATH_H
#define TUPLE_MATH_H

#include <cstddef>
#include <tuple>
#include <utility>
#include <vector>

namespace westerstrom
{
	template <size_t i = 0, typename F, typename... Ts>
	constexpr void tuple_map(std::tuple<Ts...>& a, F f)
	{
		constexpr auto N = std::tuple_size_v<std::tuple<Ts...>>;
		if constexpr(i < N)
		{
			f(std::get<i>(a));
			tuple_map<i + 1>(a, f);
		}
	}

	namespace detail
	{
		template <size_t i, typename... Ts>
		constexpr void add_impl(std::tuple<Ts...>& a, const std::tuple<Ts...>& b)
		{
			constexpr auto N = std::tuple_size_v<std::tuple<Ts...>>;
			if constexpr(i < N)
			{
				std::get<i>(a) += std::get<i>(b);
				add_impl<i + 1>(a, b);
			}
		}

	} // namespace detail

	template <typename... Ts> constexpr auto add(std::tuple<Ts...> a, const std::tuple<Ts...>& b)
	{
		detail::add_impl<0, Ts...>(a, b);
		return a;
	}

	template <typename... Ts>
	constexpr auto operator+(std::tuple<Ts...> a, const std::tuple<Ts...>& b)
	{
		return add(a, b);
	}

	template <typename... Ts>
	constexpr auto& operator+=(std::tuple<Ts...>& a, const std::tuple<Ts...>& b)
	{
		detail::add_impl<0, Ts...>(a, b);
		return a;
	}

	template <typename S, typename... Ts>
	constexpr auto& operator*=(std::tuple<Ts...>& a, const S& scalar)
	{
		tuple_map(a, [scalar](auto& t) { t *= scalar; });
		return a;
	}

	template <typename S, typename... Ts>
	constexpr auto operator*(const S& scalar, std::tuple<Ts...> a)
	{
		a *= scalar;
		return a;
	}

	template <typename S, typename... Ts>
	constexpr auto operator*(std::tuple<Ts...> a, const S& scalar)
	{
		a *= scalar;
		return a;
	}

	template <typename... Ts> constexpr auto& operator-=(std::tuple<Ts...>& a, std::tuple<Ts...> b)
	{
		b *= -1;
		a += b;
		return a;
	}

	template <typename... Ts>
	constexpr auto operator-(std::tuple<Ts...> a, const std::tuple<Ts...>& b)
	{
		a -= b;
		return a;
	}

	// product
	template <typename... Ts> auto tuple_manhattan(std::tuple<Ts...> a, std::tuple<Ts...> b)
	{
		a -= b;
		std::tuple_element_t<0, std::tuple<Ts...>> sum{};
		tuple_map(a, [&sum](auto x) { sum += abs(x); });
		return sum;
	}

	template <class T> using vec2 = std::tuple<T, T>;
	template <class T> using vec3 = std::tuple<T, T, T>;
	using vec2f = vec2<float>;
	using vec3f = vec3<float>;
	using vec2i = vec2<int>;
	using vec3i = vec3<int>;

	template <class VecT> std::tuple_element_t<0, VecT> get_x(const VecT& v) noexcept
	{
		return std::get<0>(v);
	}

	template <class VecT> std::tuple_element_t<1, VecT> get_y(const VecT& v) noexcept
	{
		return std::get<1>(v);
	}

	template <class VecT> std::tuple_element_t<2, VecT> get_z(const VecT& v) noexcept
	{
		return std::get<2>(v);
	}

	template <class T> std::vector<vec2<T>> neighbors3x3(const vec2<T>& v)
	{
		std::vector<vec2<T>> positions;
		for(int y = -1; y <= 1; ++y)
		{
			for(int x = -1; x <= 1; ++x)
			{
				if(!(x == 0 && y == 0))
				{
					positions.emplace_back(get_x(v) + x, get_y(v) + y);
				}
			}
		}
		return positions;
	}

} // namespace westerstrom
#endif // TUPLE_MATH_H
