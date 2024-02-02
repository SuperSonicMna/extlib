#include <cstdint>
#include <memory>
#include <vector>

namespace extlib::win
{
    struct handle_t;

    /// <summary>
    /// The state of a region.
    /// </summary>
    enum class region_state_t : std::uint64_t
    {
        /// <summary>
        /// Indicates committed regions for which physical storage has been allocated, either in memory or in the paging file
        /// on disk.
        /// </summary>
        commit_t = 0x1000,

        /// <summary>
        /// Indicates free regions not accessible to the calling process and available to be allocated.
        /// </summary>
        free_t = 0x10000,

        /// <summary>
        /// ndicates reserved regions where a range of the process's virtual address space is reserved without any physical
        /// storage being allocated.
        /// </summary>
        reserve_t = 0x2000,
    };

    /// <summary>
    /// The type of regions in the region.
    /// </summary>
    enum class region_type_t : std::uint64_t
    {
        /// <summary>
        /// Indicates that the memory regions within the region are mapped into the view of an image section.
        /// </summary>
        image_t = 0x1000000,

        /// <summary>
        /// Indicates that the memory regions within the region are mapped into the view of a section.
        /// </summary>
        mapped_t = 0x40000,

        /// <summary>
        /// Indicates that the memory regions within the region are private (that is, not shared by other processes).
        /// </summary>
        private_t = 0x20000
    };

    /// <summary>
    /// Represents a region of memory within a remote process.
    /// </summary>
    struct region_t
    {
        /// <summary>
        /// The start and end addresses of the region.
        /// </summary>
        std::uintptr_t start, end;

        /// <summary>
        /// The size of the current region.
        /// </summary>
        std::size_t size;

        /// <summary>
        /// The access protection of the regions in the region.
        /// </summary>
        std::uint64_t protect;

        /// <summary>
        /// The state of the current region.
        /// </summary>
        region_state_t state;

        /// <summary>
        /// The type of region we currently have.
        /// </summary>
        region_type_t type;

        /// <summary>
        /// Gets all regions within an address range.
        /// </summary>
        /// <param name="handle">The handle of the target process.</param>
        /// <param name="start">The start address.</param>
        /// <param name="end">The end address.</param>
        /// <returns>A list of regions.</returns>
        static std::vector< region_t >
        get_all_regions( std::unique_ptr< handle_t > handle, std::uintptr_t start, std::uintptr_t end );
    };

}  // namespace extlib::win