
#include <iostream>
#include "resumable.hpp"


return_type<custom_suspender> coroutine()
{
    for (int i{ 0 }; i < 10; ++i)
    {
        std::cout << i << std::endl;
        co_await std::suspend_always{};
    }
}
int main()
{
    auto handle = coroutine();

    handle.resume(); //print 0
    handle.resume(); //print 1
    std::cout << "-------ABORTING POINT-------\n";
    handle.abort(); // print 2 3 4 5 6 7 8 9 without suspention
	return 0;
}
