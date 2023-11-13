#include <stdbool.h>
#include <string.h>

#include "VirtualMemory.h"
#include "DRAM_Cache.h"

bool vmEnabled = false;

struct PageTableEntry PageTable[PAGE_TABLE_ENTRIES];

struct PageTableEntry resolvePageData(Address addr) {
    int virtualPageNumber = (addr >> 6) & 0x3FFFFFF;
    struct PageTableEntry entry = PageTable[virtualPageNumber];
    return entry;
}

int physcialAdress(struct PageTableEntry entry, Address addr) {
    int physicalPageNumber = entry.physicalPageNumber;
    int offset = addr & 0x3F;
    int physicalAddress = (physicalPageNumber << 6) | offset;
    return physicalAddress;
}

int vmRead(Address addr) {
    if (vmEnabled) {
        struct PageTableEntry entry = resolvePageData(addr);
        if (entry.valid) {
            return readWithCache(physcialAdress(entry, addr));
        } else {
            // Load 
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