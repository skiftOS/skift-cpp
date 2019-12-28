#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libruntime/Macros.h>
#include <libruntime/Types.h>

namespace x86
{

#define GDT_ENTRY_COUNT 6

#define GDT_PRESENT 0b10010000
#define GDT_USER 0b01100000
#define GDT_EXECUTABLE 0b00001000
#define GDT_READWRITE 0b00000010
#define GDT_ACCESSED 0b00000001

#define GDT_FLAGS 0b1100
#define TSS_FLAGS 0

struct __packed Tss
{
    uint32_t prev_tss;
    uint32_t esp0;
    uint32_t ss0;
    uint32_t esp1;
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t es;
    uint32_t cs;
    uint32_t ss;
    uint32_t ds;
    uint32_t fs;
    uint32_t gs;
    uint32_t ldt;
    uint16_t trap;
    uint16_t iomap_base;
};

struct __packed GdtDescriptor
{
    uint16_t size;
    uint32_t offset;
};

struct __packed GdtEntry
{
    uint16_t limit0_15;
    uint16_t base0_15;
    uint8_t base16_23;
    uint8_t acces;
    uint8_t limit16_19 : 4;
    uint8_t flags : 4;
    uint8_t base24_31;

    static GdtEntry create(uint32_t base, uint32_t limit, uint8_t acces, uint8_t flags)
    {
        GdtEntry entry;

        entry.base0_15 = base & 0xffff;
        entry.base16_23 = (base >> 16) & 0xff;
        entry.base24_31 = (base >> 24) & 0xff;
        entry.limit0_15 = limit & 0xffff;
        entry.limit16_19 = (limit >> 16) & 0x0f;
        entry.acces = (acces);
        entry.flags = (flags);

        return entry;
    }
};

void segmentation_initialize();

} // namespace x86
