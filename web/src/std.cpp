#include "std.h"

// A very simple malloc and free implementation for WebAssembly create by Bastiaan van der Plaat
// Before every alloc block there is a uint32_t which contains the size of the following block
// When allocated the top bit is set when freed the top bit is cleared so the block can be reused
// When the end of memory is reach the memory is grow by x amount of pages until the next block can be allocated
// This system is very slow O(n) I think because it loops over all blocks to find a new one but it is simple
uint32_t *nextBlockHeader = (uint32_t *)&__heap_base;
__attribute__((export_name("malloc"))) void *malloc(size_t size) {
    // Align all allocations to pointer size bytes
    size_t alignedSize = size + (sizeof(size_t) - (size % sizeof(size_t)));

    // Loop over all blocks and check if they are freed and the right size
    uint32_t *blockHeader = (uint32_t *)&__heap_base;
    while (blockHeader != nextBlockHeader) {
        uint32_t blockSize = *blockHeader & 0x7fffffff;
        if ((*blockHeader & 0x80000000) == 0 && blockSize >= alignedSize) {
            // When there is a free block of at least the right size allocate it
            *blockHeader |= 0x80000000;
            return blockHeader + sizeof(uint32_t);
        }
        blockHeader += sizeof(uint32_t) + blockSize;
    }

    // When the next block header falls outside of the current memory grow it X amount of pages
    nextBlockHeader += sizeof(uint32_t) + alignedSize;
    size_t memorySize = __builtin_wasm_memory_size(0) * WASM_PAGE_SIZE;
    if ((uintptr_t)nextBlockHeader > memorySize) {
        size_t neededExtraSize = (uintptr_t)nextBlockHeader - memorySize;
        __builtin_wasm_memory_grow(0, (neededExtraSize + (WASM_PAGE_SIZE - 1)) / WASM_PAGE_SIZE);
    }

    // When we can't find a freed block to reallocate allocate a new block at the end
    *blockHeader = 0x80000000 | alignedSize;
    return blockHeader + sizeof(uint32_t);
}

__attribute__((export_name("free"))) void free(void *ptr) {
    // Clear the top bit of this block header to free it
    uint32_t *blockHeader = (uint32_t *)ptr - sizeof(uint32_t);
    *blockHeader &= 0x7fffffff;
}

void *operator new(size_t size) {
    return malloc(size);
}

void operator delete(void *ptr) noexcept {
    free(ptr);
}
