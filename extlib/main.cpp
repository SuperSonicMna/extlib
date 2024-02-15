#include <iostream>

#include "win/snapshot.hpp"

using namespace extlib;

std::int32_t main()
{
    try
    {
        auto snapshot = win::snapshot< win::snapshot_kind::thread_t >::get();

        for ( const auto& entry : snapshot )
        {
            std::cout << "pID: " << entry.th32OwnerProcessID << " tID: " << entry.th32ThreadID << '\n';
        }
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}