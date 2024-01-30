#include "object.hpp"

#include <iostream>

#include "scan.hpp"
#include "win/memapi.hpp"

namespace extlib
{
    object_pattern_t object_pattern_t::from_class_name( const std::string_view class_name )
    {
        std::string string = ".?AV";

        std::size_t pos, last = class_name.length() - 1;
        while ( ( pos = class_name.rfind( "::", last ) ) != std::string_view::npos )
        {
            string.append( class_name.substr( pos + 2, last - pos - 1 ) );
            string.push_back( '@' );

            last = pos - 1;
        }

        string.append( class_name.substr( 0, last + 1 ) );
        string.append( "@@" );

        return { string };
    }

    std::unique_ptr< object > object::find_object( const win::module_t& main_module, const object_pattern_t& pattern )
    {
        const auto rdata = main_module[ ".rdata" ];

        std::cout << "Searching for \"" << pattern.string << "\"" << '\n';

        scanner scan{ main_module };

        const auto& matches = scan.find_all( pattern.string );

        if ( matches.empty() )
            return nullptr;

        std::cout << "Found " << matches.size() << " matches for pattern\n";

        const auto type_descriptor_ptr = matches.front() - sizeof( std::uintptr_t ) * 2;
        const auto type_descriptor_rva = static_cast< std::int32_t >( type_descriptor_ptr - main_module.start );

        const auto& xrefs = rdata.find_all( type_descriptor_rva );

        std::cout << "Found " << xrefs.size() << " instances of possible type descriptor\n";

        for ( const auto& xref : xrefs )
        {
            const auto object_locator_ptr = xref - sizeof( std::uint32_t ) * 3;

            if ( !object_locator_ptr || !main_module.contains( object_locator_ptr ) )
                continue;

            const auto object_locator = main_module.read< complete_object_locator_t >( object_locator_ptr );

            if ( object_locator.signature != 1 )
                continue;

            const auto class_hierarchy_ptr = main_module.start + object_locator.class_hierarchy_descriptor_rva;

            if ( !class_hierarchy_ptr || !main_module.contains( class_hierarchy_ptr ) )
                continue;

            const auto class_hierarchy = main_module.read< class_hierarchy_descriptor_t >( class_hierarchy_ptr );

            if ( class_hierarchy.signature != 0 )
                continue;

            const auto base_class_array_ptr = main_module.start + class_hierarchy.base_class_array_rva;

            if ( !base_class_array_ptr || !main_module.contains( base_class_array_ptr ) )
                continue;

            std::cout << "Class consists of " << class_hierarchy.base_class_count << " classes\n";

            for ( std::size_t i = 0; i < class_hierarchy.base_class_count; ++i )
            {
                const auto& rva = main_module.read< std::int32_t >( base_class_array_ptr + sizeof( std::int32_t ) * i );

                const auto base_class_ptr = main_module.start + rva;

                if ( !base_class_ptr || !main_module.contains( base_class_ptr ) )
                    continue;

                const auto base_class = main_module.read< base_class_descriptor_t >( base_class_ptr );

                const auto base_type_descriptor_ptr = main_module.start + base_class.type_descriptor_rva;

                if ( !base_type_descriptor_ptr || !main_module.contains( base_type_descriptor_ptr ) )
                    continue;

                const auto base_type_descriptor = main_module.read< type_descriptor_t >( base_type_descriptor_ptr );
                const auto type_name = main_module.read_string( base_type_descriptor_ptr + sizeof( type_descriptor_t ) );

                std::cout << "\t" << type_name << std::endl;
            }

            std::cout << "found object_locator: 0x" << std::hex << object_locator_ptr << '\n';

            const auto& object_locator_xrefs =
                scanner{ { 0, main_module.end, main_module.handle } }.find_all( object_locator_ptr );

            std::cout << "found " << object_locator_xrefs.size() << " xrefs to object locator\n" << std::endl;
        }

        std::cout << '\n' << '\n';

        // for ( auto address = rdata.start; address < rdata.end; address += sizeof( std::uint8_t ) )
        //{
        //     // Virtual tables (VTables) start with an address to their CompleteObjectLocator and are followed by their
        //     // function addresses.
        //     const auto object_locator_ptr = main_module.read< std::uintptr_t >( address - sizeof( std::uintptr_t ) );
        //     const auto vtable_ptr = main_module.read< std::uintptr_t >( address );

        //    if ( !object_locator_ptr || !main_module.contains( object_locator_ptr ) )
        //        continue;

        //    const auto object_locator = main_module.read< complete_object_locator_t >( object_locator_ptr );

        //    if ( object_locator.signature != 1 )
        //        continue;

        //    const auto type_descriptor_ptr = main_module.start + object_locator.type_descriptor_rva;

        //    if ( !main_module.contains( type_descriptor_ptr ) )
        //        continue;

        //    const auto type_descriptor = main_module.read< type_descriptor_t >( type_descriptor_ptr );
        //    const auto type_name = main_module.read_string( type_descriptor_ptr + sizeof( type_descriptor_t ) );

        //    std::cout << "found obj locator 0x" << std::hex << object_locator_ptr << "  " << type_name << '\n';
        //}

        // const auto strings = rdata.find_all( pattern_t::from_string( pattern.string ) );
        // std::cout << strings.size() << '\n';
        // for ( const auto& string : strings )
        //     std::cout << std::hex << "  :" << string << '\n';

        return nullptr;
    }
}  // namespace extlib