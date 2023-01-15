#include <type_traits>

template <class T, class... Ts>
struct is_any : std::disjunction<std::is_same<T, Ts>...>
{
};
