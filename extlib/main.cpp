#include <iostream>

#include "process.hpp"

using namespace extlib;

std::int32_t main()
{
    try
    {
        const auto process = process::get( "extlib.exe" );
    }
    catch ( const win::error& e )
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}