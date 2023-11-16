#include <stdbool.h>
#include <string.h>

#include "DRAM_Cache.h"
#include "Performance.h"
#include "VirtualMemory.h"

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
        if (TLBTable[i].virtualPageNumber == (virtualAddr >> 10) && TLBTable[i].valid) {
            physicalPageNumber = TLBTable[i].physicalPageNumber;
            perfTlbHit(physicalPageNumber);
            break;
        }
    }
    if (physicalPageNumber == -1) {
        physicalPageNumber = readWithCache(PageTableAddress + ((virtualAddr >> 10) * BYTES_PER_PTE)) & 0x3F;
        TLBTable[roundRobin].physicalPageNumber = physicalPageNumber;
        TLBTable[roundRobin].virtualPageNumber = virtualAddr >> 10;
        TLBTable[roundRobin].valid = true;
        roundRobin = !roundRobin;
        perfTlbMiss(physicalPageNumber);
    }
    int phys = (physicalPageNumber << 10) | (virtualAddr & 0x3FF);
    perfEndAddressTranslation(phys);
    return phys;
}

int vmRead(Address addr) {
    if (vmEnabled) {
        return readWithCache(physcialAdress(addr));
    } else {
        return readWithCache(addr);
    }
}

void vmWrite(Address addr, int value) {
    if (vmEnabled) {
        writeWithCache(physcialAdress(addr), value);
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