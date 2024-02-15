#include <iostream>

#include "win/snapshot.hpp"

using namespace extlib;

std::int32_t main()
{
    auto snapshot = win::snapshot< win::snapshot_kind::process_t >::get();
    
    for (const auto& entry : snapshot.value())
    {
        std::cout << entry.th32ProcessID << '\n';
    }

    std::cout << "Hello, world!" << std::endl;
    return 0;
}