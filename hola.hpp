#pragma once

#include <type_traits>
#include <tuple>
#include <array>

namespace hola
{
namespace internal
{
template <typename Vec, size_t... I>
constexpr auto dot_impl(Vec const& a, Vec const& b, std::index_sequence<I...>)
{
    return ((std::get<I>(a) * std::get<I>(b)) + ...);
}
}  // namespace internal

template <typename T, size_t Size>
class vec : public std::array<T, Size>
{
   public:
    constexpr static auto size = Size;

    constexpr bool operator==(vec<T, Size> const& o) const
    {
        return equals(o, std::make_index_sequence<size>{});
    }

    constexpr bool operator!=(vec<T, Size> const& o) const
    {
        return !equals(o, std::make_index_sequence<size>{});
    }

    template<typename MultT>
    constexpr auto operator*(MultT const mult) const
    {
        return std::apply(
            [&](auto const& ... a)
                { return vec<typename std::common_type<decltype(mult*a)...>::type, Size>{mult*a...};},
            get());
    }

   private:
    template <size_t... I>
    constexpr bool equals(vec<T, Size> const& o, std::index_sequence<I...>) const
    {
        return ((std::get<I>(*this) == std::get<I>(o)) && ...);
    }

    constexpr const std::array<T, Size>& get() const { return *this; }
};

template <typename... T>
vec(T&&...)->vec<typename std::common_type<T...>::type, sizeof...(T)>;

using vec2i = vec<int, 2>;
using vec3i = vec<int, 3>;
using vec2f = vec<float, 2>;
using vec3f = vec<float, 3>;

template <typename Vec>
constexpr auto get_x(Vec const& v)
{
    return std::get<0>(v);
}

template <typename Vec>
constexpr auto get_y(Vec const& v)
{
    return std::get<1>(v);
}

template <typename Vec>
constexpr auto get_z(Vec const& v)
{
    return std::get<2>(v);
}

template <typename Vec>
void set_x(Vec& v, decltype(get_x(v)) const& value)
{
    std::get<0>(v) = value;
}

template <typename Vec>
void set_y(Vec& v, decltype(get_y(v)) const& value)
{
    std::get<1>(v) = value;
}

template <typename Vec>
void set_z(Vec& v, decltype(get_z(v)) const& value)
{
    std::get<2>(v) = value;
}

template <typename Vec>
constexpr auto dot(Vec const& a, Vec const& b)
{
    return internal::dot_impl(a, b, std::make_index_sequence<Vec::size>{});
}

template <typename Vec>
constexpr auto cross(Vec const& a, Vec const& b)
{
    return Vec{
        get_y(a) * get_z(b) - get_z(a) * get_y(b),
        get_z(a) * get_x(b) - get_x(a) * get_z(b),
        get_x(a) * get_y(b) - get_y(a) * get_x(b)
    };
}
}  // namespace hola
