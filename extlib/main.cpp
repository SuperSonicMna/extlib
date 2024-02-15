#include <iostream>

#include "win/snapshot.hpp"

using namespace extlib;

std::int32_t main()
{
    try
    {
        auto snapshot = win::snapshot< win::snapshot_kind::process_t >::get();

        for ( const auto& entry : snapshot )
        {
            std::cout << entry.szExeFile << std::endl;
        }
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}