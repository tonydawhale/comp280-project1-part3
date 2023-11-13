#include <stdbool.h>
#include <string.h>

#include "VirtualMemory.h"
#include "DRAM_Cache.h"

bool vmEnabled = false;

struct PageTableEntry PageTable[PAGE_TABLE_ENTRIES];

struct PageTableEntry resolvePageData(Address addr) {
    // struct PageTableEntry PageTable[64];
    int virtualPageNumber = (addr >> 6) & 0x3FFFFFF;
    struct PageTableEntry entry = PageTable[virtualPageNumber];
    return entry;
}


int vmRead(Address addr) {
    if (vmEnabled) {
        struct PageTableEntry entry = resolvePageData(addr);
        if (entry.valid) {
            PageTable[entry.physicalPageNumber];
        } else {

        }
    }
    return -1;
}

void vmWrite(Address addr, int value) {
    if (vmEnabled) {

    }
}

void vmDisable() {
    vmEnabled = false;
}

void vmEnable(Address pageTable) {

}