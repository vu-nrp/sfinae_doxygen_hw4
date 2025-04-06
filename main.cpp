#include <list>
#include <tuple>
#include <vector>
#include <cstdint>
#include <iostream>
#include <type_traits>

//!
//! \brief print_d - helper function for byte printing
//!

int print_d(const uint8_t *pointer)
{
    return *pointer;
}

//!
//! \brief print_p - helper function for point (.) printing
//!

std::ostream &print_p(bool yes = true)
{
    if (yes)
        std::cout << '.';
    return std::cout;
}

//!
//! \brief is_c_str - helper template for definition is type has c_str() method
//!

template <typename T>
using is_c_str = decltype(std::declval<T>().c_str());

//!
//! \brief has_c_str - helper template struct for checking type to having string method c_str()
//!

template<typename T>
struct has_c_str
{
private:
    static int check(...);
    template<typename U> static is_c_str<U> check(const U&);
public:
    static constexpr bool value = std::is_same_v<const char *, decltype(check(std::declval<T>()))>;
};


//!
//! \brief has_c_str_v - helper alias for has_c_str value
//!

template<typename T>
constexpr bool has_c_str_v = has_c_str<T>::value;

//!
//! \brief print_t - helper template function for print tuple item
//!

template<typename T, size_t... I>
void print_t(const T &tuple, std::index_sequence<I...>)
{
    (..., (print_p(I != 0) << std::get<I>(tuple)));
}

//!
//! \brief check_t - helper template function for checking same types of item
//!

template<typename T, typename... N>
constexpr bool check_t()
{
    return (std::is_same_v<T, N> && ...);
}

//!
//! \brief print_ip - printing function for numeric types
//!

template<typename T, typename F = typename std::enable_if_t<std::is_arithmetic_v<T>, T>>
void print_ip(const T &numeric)
{
    const auto last = static_cast<int>(sizeof(T)) - 1;
    const auto pointer = reinterpret_cast<const uint8_t *>(&numeric);

    print_p(false) << print_d(pointer + last);
    for (int i = (last - 1);i > -1;--i)
        print_p() << print_d(pointer + i);
    std::cout << std::endl;
}

//!
//! \brief print_ip - printing function for those who have string c_str() method
//!

template<typename T, typename F = is_c_str<T>>
auto print_ip(const T &str)
{
    std::cout << str << std::endl;
}

//!
//! \brief print_ip - printing function for container types & not string
//!

template<typename T, typename I = typename T::const_iterator, typename F = typename std::enable_if_t<!has_c_str_v<T>, T>>
void print_ip(const T &container)
{
    const auto cbeg = container.cbegin();
    for (auto it = cbeg;it != container.cend();++it)
        print_p(it != cbeg) << *it;
    std::cout << std::endl;
}

//!
//! \brief print_ip - printing function for tuple
//!

template<typename... N>
constexpr void print_ip(const std::tuple<N...> &tuple)
{
    static_assert(check_t<N...>());
    print_t(tuple, std::make_index_sequence<sizeof...(N)>());
    std::cout << std::endl;
}

//!
//! \brief print_ip - printing function for variable items count list
//!

template <typename... N>
constexpr void print_ip(N... args)
{
    print_ip(std::make_tuple(args...));
}

//!
//! \brief main - main printing all function
//!

int main()
{
    std::cout << "Home work #4" << std::endl;

    print_ip(int8_t {-1});                               // 255
    print_ip(int16_t {0});                               // 0.0
    print_ip(int32_t {2130706433});                      // 127.0.0.1
    print_ip(int64_t {8875824491850138409});             // 123.45.67.89.101.112.131.41
    print_ip(std::string {"Hello, World!"});             // Hello, World!
    print_ip(std::vector<int> {100, 200, 300, 400});     // 100.200.300.400
    print_ip(std::list<short> {400, 300, 200, 100});     // 400.300.200.100
    print_ip(std::make_tuple(123, 456, 789, 0));         // 123.456.789.0
    print_ip(std::make_tuple("123", "456", "789", "0")); // 123.456.789.0
    print_ip(1123, 1456, 1789, 10);                      // 1123.1456.1789.10
    print_ip("00", "11");                                // 00.11
//    // compile error!!!
//    print_ip(std::make_tuple(123, 456, "456", true));  // can't print, type is different
//    print_ip("00", 11);                                // can't print, type is different
}
