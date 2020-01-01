#include "system/memory/RegionPool.h"

namespace system::memory
{

Region RegionPool::take(size_t how_many_pages)
{
    for (size_t i = 0; i < _regions.count(); i++)
    {
        if (_regions[i].page_count() >= how_many_pages)
        {
            Region region = _regions[i].take(how_many_pages);

            if (_regions[i].is_empty())
            {
                _regions.remove(i);
            }

            _quantity -= how_many_pages;

            return region;
        }
    }

    return Region::empty();
}

void RegionPool::take(Region region)
{
    for (size_t i = 0; i < _regions.count(); i++)
    {
        Region &current_region = _regions[i];

        if (current_region.is_overlaping_with(region))
        {
            Region lower_half = current_region.half_under(region);
            Region upper_half = current_region.half_over(region);

            _regions.remove(current_region);

            put(lower_half);
            put(upper_half);
        }
    }
}

void RegionPool::put(Region region)
{
    if (region.is_empty())
        return;

    for (size_t i = 0; i < _regions.count(); i++)
    {
        Region &current_region = _regions[i];

        assert(!current_region.is_overlaping_with(region));

        if (!current_region.is_contiguous_with(region))
        {
            current_region.merge(region);

            if (i + 1 < _regions.count())
            {
                Region &next_region = _regions[i + 1];

                if (current_region.is_contiguous_with(next_region))
                {
                    current_region.merge(next_region);
                    _regions.remove(next_region);
                }
            }

            return;
        }
    }

    _regions.insert_sorted(region, [](auto &left, auto &right) -> bool {
        return left.base_page() < right.base_page();
    });

    _quantity += region.page_count();
}

} // namespace system::memory
