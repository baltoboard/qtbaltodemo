#ifndef ALLOCATOR_H
#define ALLOCATOR_H
//------------------------------------------------------------------------------------------------------------------
extern bool gMonitor;
//------------------------------------------------------------------------------------------------------------------
#include <stddef.h>
//------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------
typedef struct{
    void*  p;
    size_t size;
}TAllocElement;
//------------------------------------------------------------------------------------------------------------------
class Allocator
{

private:
    void remove(void* p);
    void insert(void* p, size_t size);
    size_t getSize(void* p);

    unsigned char* pMemmoryBuffer;

public:
    Allocator(int max = 8000, size_t size = 0x200000);
    ~Allocator();

protected:
    void*         _start;
    size_t        _size;

    size_t        _max_used_memory;
    size_t        _used_memory;
    size_t        _num_allocations;

    int maxAllocation;
    TAllocElement* p_memoryMap;

public:  
    size_t getSize() const
    {
        return _size;
    }

    size_t getUsedMemory() const
    {
        return _used_memory;
    }

    size_t getNumAllocations() const
    {
        return _num_allocations;
    }

    void newMem (void* p, size_t size);
    void freeMem(void* p);

    void printMem();


    size_t getMaxUsedSize() const
    {
        return _max_used_memory;
    }


};

//------------------------------------------------------------------------------------------------------------------
extern Allocator* pMyAllocator;
//------------------------------------------------------------------------------------------------------------------

#endif // ALLOCATOR_H
