#include <iostream>

#include "../extlib/include/process.hpp"

std::int32_t main()
{
    try
    {
        const auto& process = extlib::process::get_all_by_name( "RobloxPlayerBeta.exe" );
    }
    catch ( const std::exception& e )
    {
        std::cerr << "example: " << e.what() << std::endl;
    }
    return 1;
}