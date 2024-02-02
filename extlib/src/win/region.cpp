#include "win/win.hpp"

#include "win/memapi.hpp"

namespace extlib::win
{
    std::vector< region_t >
    region_t::get_all_regions( std::unique_ptr< handle_t > handle, std::uintptr_t start, std::uintptr_t end )
    {
        std::vector< region_t > regions;

        auto start_address = start;

        while ( const auto info = memapi::virtual_query_ex( handle.get(), start_address ) )
        {
            if ( start_address > end || !info->RegionSize )
                break;

            const auto base_address = reinterpret_cast< std::uintptr_t >( info->BaseAddress );
            const auto end = base_address + info->RegionSize;

            const auto state = static_cast< region_state_t >( info->State );
            const auto type = static_cast< region_type_t >( info->Type );

            // std::cout << info->RegionSize / sinfo.dwregionSize << '\n';
            regions.push_back( { base_address, end, info->RegionSize, info->Protect, state, type } );

            start_address = end;
        }

        return regions;
    }
}  // namespace extlib::win