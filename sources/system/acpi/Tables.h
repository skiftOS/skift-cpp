#pragma once

#include <libruntime/Macros.h>
#include <libruntime/Types.h>

namespace system::acpi
{

struct __packed RSDP
{
    char signature[8];
    uint8_t checksum;
    char OEM_id[6];
    uint8_t revision;
    uint32_t rsdt_address;
};

struct __packed SDT
{
    char Signature[4];
    uint32_t Length;
    uint8_t Revision;
    uint8_t Checksum;
    char OEMID[6];
    char OEMTableID[8];
    uint32_t OEMRevision;
    uint32_t CreatorID;
    uint32_t CreatorRevision;
};

struct __packed RSDT
{
    SDT header;
    uint32_t childs[];

    SDT *child(size_t index)
    {
        return reinterpret_cast<SDT *>(childs[index]);
    }

    size_t child_count()
    {
        return (header.Length - sizeof(header)) / 4;
    }
};

} // namespace system::acpi
