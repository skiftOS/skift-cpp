/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libruntime/LinkedList.h>
#include <libsystem/Assert.h>
#include <libsystem/Logger.h>

#include "arch/Arch.h"
#include "system/System.h"
#include "system/memory/Memory.h"
#include "system/memory/Region.h"

using namespace system;
using namespace libruntime;

namespace system::memory
{

static bool bootstraped = false;
static Region bootstarp;

static LinkedList<Region> *free_list;

bool is_bootstraped()
{
    return bootstraped;
}

static Region take_from_free_list(size_t how_many_pages)
{
    Region region = Region::empty();

    free_list->iterate([&](auto &free_region) {
        if (free_region.page_count() >= how_many_pages)
        {
            region = free_region.take(how_many_pages);

            if (free_region.is_empty())
            {
                free_list->remove_all(free_region);
            }

            return Iteration::STOP;
        }
        else
        {
            return Iteration::CONTINUE;
        }
    });

    return region;
}

Region alloc_region(size_t how_many_pages)
{
    assert(how_many_pages > 0);

    if (!bootstraped)
    {
        PANIC("MemoryManager not bootstraped!");
    }

    Region region = Region::empty();

    if (bootstarp.page_count() >= how_many_pages)
    {
        // Look like we have some space in the bootstrap :)
        region = bootstarp.take(how_many_pages);

        if (bootstarp.is_empty())
        {
            logger_info("The bootstrap is now empty.");
        }
    }
    else
    {
        region = take_from_free_list(how_many_pages);
    }

    if (region.is_empty())
    {
        PANIC("Out of memory!");
    }

    return region;
}

void free_region(Region region)
{
    auto kernel_region = arch::get_kernel_region();

    if (region.is_overlaping_with(kernel_region))
    {
        // An half of the region is under the kernel.
        if (region.base_page() < kernel_region.base_page())
        {
            Region lower_half = Region::from_page(
                region.base_page(),
                kernel_region.base_page() - region.base_page());

            free_region(lower_half);
        }

        // An another half of the region is over the kernel.
        if (region.end_page() > kernel_region.end_page())
        {
            Region upper_half = Region::from_page(
                kernel_region.end_page(),
                region.end_page() - kernel_region.end_page());

            free_region(upper_half);
        }
    }
    else if (!bootstraped)
    {
        logger_info("Bootstraping with {}", region);

        bootstarp = region;
        bootstraped = true;

        free_list = new LinkedList<Region>();
    }
    else
    {
        // FIXME: we should also merge if we are filling an hole.
        bool has_been_merge_with_other_region = false;

        free_list->iterate([&](auto &other) {
            if (other.is_contiguous_with(region))
            {
                other.merge(region);

                has_been_merge_with_other_region = true;

                return Iteration::STOP;
            }

            return Iteration::CONTINUE;
        });

        if (!has_been_merge_with_other_region)
        {
            free_list->push_back(region);
        }
    }
}

} // namespace system::memory
