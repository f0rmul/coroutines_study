#include "resumable.hpp"
#include <iostream>
std::optional<int> one() noexcept
{
    return  1;
}
std::optional<int> two()noexcept
{
    return 2;
}


std::optional<int> empty()
{
    return std::nullopt;
}
resume_type<int> sum()
{
    auto a = co_await one();
    auto b = co_await two();

    co_return a + b;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::optional<T>& opt)
{
    if (opt)
        std::cout << " sum() = " << opt.value() << std::endl;
    else
        std::cout << "empty state\n";
    return os;
}
int main()
{
    try
    {
        auto res = sum();
        std::cout << res.current_value();
    }
    catch (std::exception& e)
    {
        std::cout << e.what();
    }
}

