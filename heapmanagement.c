#include <stdio.h>
#include <stdlib.h>

#define HEAP_SIZE 1000

typedef struct Block {
    size_t size;
    struct Block* next;
} Block;

// Global pointer to the start of the heap
Block* heap = NULL;

// Function to initialize the heap
void initializeHeap() {
    heap = (Block*)malloc(HEAP_SIZE);
    if (heap == NULL) {
        fprintf(stderr, "Heap initialization failed.\n");
        exit(EXIT_FAILURE);
    }

    heap->size = HEAP_SIZE - sizeof(Block);
    heap->next = NULL;
}

// Function to allocate memory using the first-fit method
void* myMalloc(size_t size) {
    Block* current = heap;
    Block* prev = NULL;

    while (current != NULL) {
        if (current->size >= size) {
            // Allocate memory from this block
            if (current->size > size + sizeof(Block)) {
                // Split the block if there is extra space
                Block* newBlock = (Block*)((char*)current + sizeof(Block) + size);
                newBlock->size = current->size - size - sizeof(Block);
                newBlock->next = current->next;
                current->size = size;
                current->next = newBlock;
            }

            // Remove the allocated block from the free list
            if (prev == NULL) {
                heap = current->next;
            } else {
                prev->next = current->next;
            }

            return (void*)((char*)current + sizeof(Block));
        }

        prev = current;
        current = current->next;
    }

    fprintf(stderr, "Memory allocation failed. Insufficient space.\n");
    return NULL;
}

// Function to deallocate memory
void myFree(void* ptr) {
    if (ptr == NULL) {
        return;
    }

    Block* block = (Block*)((char*)ptr - sizeof(Block));
    block->next = heap;
    heap = block;
}

// Function to display the current state of the heap
void printHeap() {
    Block* current = heap;

    while (current != NULL) {
        printf("Block: %p, Size: %lu\n", current, current->size);
        current = current->next;
    }
}

// Main function for testing the heap management
int main() {
    initializeHeap();

    // Allocate memory blocks
    void* ptr1 = myMalloc(100);
    void* ptr2 = myMalloc(200);
    void* ptr3 = myMalloc(150);

    // Display the current state of the heap
    printf("Heap after allocations:\n");
    printHeap();

    // Free memory blocks
    myFree(ptr2);

    // Display the current state of the heap after deallocation
    printf("\nHeap after deallocation:\n");
    printHeap();

    // Allocate more memory
    void* ptr4 = myMalloc(50);

    // Display the final state of the heap
    printf("\nHeap after additional allocation:\n");
    printHeap();

    // Free all memory
    myFree(ptr1);
    myFree(ptr3);
    myFree(ptr4);

    // Display the empty heap
    printf("\nHeap after freeing all memory:\n");
    printHeap();

    return 0;
}