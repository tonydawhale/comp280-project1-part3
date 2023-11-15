#include <stdbool.h>
#include <string.h>

#include "VirtualMemory.h"
#include "Performance.h"
#include "DRAM_Cache.h"


struct TLBEntry {
    int virtualPageNumber;
    int physicalPageNumber;
    bool valid;
};

struct TLBEntry TLBTable[2];
bool vmEnabled = false;
Address PageTableAddress;
int roundRobin = 0;

int physcialAdress(Address virtualAddr) {
    perfStartAddressTranslation(virtualAddr);
    int physicalPageNumber = -1;
    for (int i = 0; i < 2; i++) {
        if (TLBTable[i].virtualPageNumber == virtualAddr >> 6) {
            physicalPageNumber = TLBTable[i].physicalPageNumber;
            perfTlbHit(physicalPageNumber & 0x3F);
            roundRobin = (i + 1) % 2;
            break;
        }
    }
    if (physicalPageNumber == -1) {
        physicalPageNumber = readWithCache((PageTableAddress + ((virtualAddr >> 6) * BYTES_PER_PTE)) & 0x3F);
        TLBTable[!roundRobin ? 0 : 1].physicalPageNumber = physicalPageNumber;
        perfTlbMiss(physicalPageNumber & 0x3F);
    }
    int phys = (physicalPageNumber << 6) | (virtualAddr & 0x3F);
    perfEndAddressTranslation(phys);
    return phys;
}

int vmRead(Address addr) {
    if (vmEnabled) {
        int physicalAddr = physcialAdress(addr);
        return readWithCache(physicalAddr);
    } else {
        return readWithCache(addr);
    }
}

void vmWrite(Address addr, int value) {
    if (vmEnabled) {
        int physicalAddr = physcialAdress(addr);
        writeWithCache(physicalAddr, value);
    } else {
        writeWithCache(addr, value);
    }
}

void vmDisable() {
    vmEnabled = false;
}

void vmEnable(Address pageTable) {
    vmEnabled = true;
    PageTableAddress = pageTable;
}