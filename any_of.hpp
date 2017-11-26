#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

template <typename... Args> class any_of
{
public:
    explicit any_of(const Args&... args)
        : args_(std::forward_as_tuple(args...))
    {
    }
    template <typename... Ts,
              typename = std::enable_if_t<sizeof...(Ts) == sizeof...(Args)>>
    explicit any_of(Ts&&... args)
        : args_(std::forward_as_tuple(std::forward<Ts>(args)...))
    {
    }

    template <typename T, typename BinaryPredicate>
    bool impl(const T& value, BinaryPredicate pred) const
    {
        return std::apply(
            [&value, &pred](auto&&... args) {
                return ((pred(value, args)) || ...);
            },
            args_);
    };

private:
    const std::tuple<const Args&...> args_;
};

template <typename T, typename... Args>
bool operator==(const T& lhs, const any_of<Args...>& rhs)
{
    return rhs.impl(lhs, std::equal_to<>());
}

template <typename T, typename... Args>
bool operator!=(const T& lhs, const any_of<Args...>& rhs)
{
    return !(lhs == rhs);
}

template <typename T, typename... Args>
bool operator<(const T& lhs, const any_of<Args...>& rhs)
{
    return rhs.impl(lhs, std::less<>());
}

template <typename T, typename... Args>
bool operator>(const T& lhs, const any_of<Args...>& rhs)
{
    return rhs.impl(lhs, std::greater<>());
}

template <typename T, typename... Args>
bool operator<=(const T& lhs, const any_of<Args...>& rhs)
{
    return rhs.impl(lhs, std::less_equal<>());
}

template <typename T, typename... Args>
bool operator>=(const T& lhs, const any_of<Args...>& rhs)
{
    return rhs.impl(lhs, std::greater_equal<>());
}
