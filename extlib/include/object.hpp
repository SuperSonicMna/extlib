#include <memory>
#include <string>

#include "win/win.hpp"

namespace extlib
{
    struct object_pattern_t;

    struct type_descriptor_t;
    struct complete_object_locator_t;
    struct class_hierarchy_descriptor_t;
    struct class_hierarchy_descriptor_t;

    /// <summary>
    /// Represents an object in the target process (classes, structs, etc.)
    /// </summary>
    class object final
    {
       public:
        /// <summary>
        /// Finds an object within the provided module, using the given object pattern.
        /// </summary>
        /// <param name="module">The module to search.</param>
        /// <param name="pattern">The object pattern.</param>
        /// <returns>A unique object.</returns>
        static std::unique_ptr< object > find_object( const win::module_t& main_module, const object_pattern_t& pattern );
    };

    /// <summary>
    /// The pattern used when finding runtime type information.
    /// </summary>
    struct object_pattern_t
    {
        /// <summary>
        /// Creates a new object pattern from a class name.
        /// </summary>
        /// <param name="class_name">The name of the class (e.g. `extlib::scan`).</param>
        /// <returns>A new pattern.</returns>
        static object_pattern_t from_class_name( const std::string_view class_name );

        /// <summary>
        /// The raw RTTI string.
        /// </summary>
        std::string string;
    };

    /// <summary>
    /// Describes a type within the remote process.
    /// </summary>
    struct type_descriptor_t
    {
        /// <summary>
        /// A pointer to the `type_info` virtual table.
        /// </summary>
        std::uintptr_t vtable;

        /// <summary>
        /// No idea what this is.
        /// </summary>
        std::uintptr_t spare;
    };

    /// <summary>
    /// The complete object locator for a class in a process.
    /// </summary>
    struct complete_object_locator_t
    {
        /// <summary>
        /// Architecture signature (1 for 64-bit, 0 for 32-bit).
        /// </summary>
        std::uint32_t signature;

        /// <summary>
        /// The offset of this vtable in the complete class.
        /// </summary>
        std::uint32_t vtable_offset;

        /// <summary>
        /// The constructor displacement offset.
        /// </summary>
        std::uint32_t constructor_offset;

        /// <summary>
        /// The RVA to the type descriptor of the complete class.
        /// </summary>
        std::int32_t type_descriptor_rva;

        /// <summary>
        /// The RVA to inheritance hierarchy description.
        /// </summary>
        std::int32_t class_hierarchy_descriptor_rva;

        /// <summary>
        /// The RVA to the objects base (used to get the base address of the module).
        /// </summary>
        std::int32_t complete_object_locator_rva;
    };

    /// <summary>
    /// The class hierarchy descriptor for a class in a process.
    /// </summary>
    struct class_hierarchy_descriptor_t
    {
        /// <summary>
        /// Architecture signature (1 for 64-bit, 0 for 32-bit).
        /// </summary>
        std::uint32_t signature;

        /// <summary>
        /// Attributes
        /// </summary>
        std::uint32_t attributes;

        /// <summary>
        /// The number of base classes that the main class has.
        /// </summary>
        std::uint32_t base_class_count;

        /// <summary>
        /// The RVA to the base class array
        /// </summary>
        std::int32_t base_class_array_rva;
    };

    struct base_class_descriptor_t
    {
        std::int32_t type_descriptor_rva;         // Rva to type descriptor of the class complete class.
        std::uint32_t num_contained_bases;   // Number of nested classes in base_class_rva_array_rva.
        std::int32_t member_displacement;         // Member displacement.
        std::int32_t vtable_displacement;    // VTable displacement.
        std::int32_t displacement_within_vtable;  // Displacement within vtable.
        std::uint32_t attributes;
    };

}  // namespace extlib