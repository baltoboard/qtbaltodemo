#include <QDebug>
//-------------------------------------------------------------------------------------------------------------------------------------
#include "allocator.h"
//-------------------------------------------------------------------------------------------------------------------------------------
Allocator* pMyAllocator = NULL;
//-------------------------------------------------------------------------------------------------------------------------------------
Allocator::Allocator(int max, size_t size) {

    _size            = size;
    _used_memory     = 0;
    _num_allocations = 0;
    _max_used_memory = 0;

    maxAllocation = max;
    p_memoryMap = NULL;
    p_memoryMap = new TAllocElement[max];


    for(int i=0;i<maxAllocation;i++){
        p_memoryMap[i].p     = 0;
        p_memoryMap[i].size  = 0;
    }

}
//-------------------------------------------------------------------------------------------------------------------------------------
Allocator::~Allocator()
{

}
//-------------------------------------------------------------------------------------------------------------------------------------
void Allocator::remove(void* p){

    for(int i=0;i<maxAllocation;i++){
        if(p_memoryMap[i].p == p){
            p_memoryMap[i].size = 0;
            p_memoryMap[i].p    = 0;
            return;
        }
    }
    printf("Allocator insert fails\n");
}
//-------------------------------------------------------------------------------------------------------------------------------------
void Allocator::insert(void* p, size_t size){

    for(int i=0;i<maxAllocation;i++){
        if(p_memoryMap[i].p == 0){
            p_memoryMap[i].size = size;
            p_memoryMap[i].p    = p;
            return;
        }
    }
   printf("Allocator insert fails\n");
}
//-------------------------------------------------------------------------------------------------------------------------------------
size_t Allocator::getSize(void* p){

    for(int i=0;i<maxAllocation;i++){
        if(p_memoryMap[i].p == p){
            return(p_memoryMap[i].size);
        }
    }
    return(0);
}
//-------------------------------------------------------------------------------------------------------------------------------------
void Allocator::newMem (void* p, size_t size){

    if ( p_memoryMap != NULL){

        insert(p,size);
        _used_memory += size;
        _num_allocations++;

        if(_used_memory > _max_used_memory){
            _max_used_memory = _used_memory;
        }

        //printf("New : %x size:%d used:%d\n", p, size, _used_memory);
    }

}
//-------------------------------------------------------------------------------------------------------------------------------------
void Allocator::freeMem(void* p){

    if ( p_memoryMap != NULL){

        size_t size = getSize(p);

       _used_memory -= size;
       remove(p);
       _num_allocations--;

       printf("Free: %x size:%d used:%d\n", p, size, _used_memory);
    }

}

//-------------------------------------------------------------------------------------------------------------------------------------
void Allocator::printMem(){

    for(int i=0;i<maxAllocation;i++){
        if(p_memoryMap[i].p != NULL){
            qDebug() << p_memoryMap[i].p << " size:" << p_memoryMap[i].size;
        }
    }

    qDebug() << "_used_memory_:" << _used_memory << " " << _used_memory/1024 << " Kb " << _used_memory/0x100000 << " Mb";
    qDebug() << "_num_allocations:" << _num_allocations;

}
//-------------------------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------------------------
void* operator new(size_t  size) {

  void * p = malloc(size);


  if (p == NULL) {
      qDebug() << "OUT OF MEMORY";
      return(NULL);
  }

 if(pMyAllocator != NULL)
    pMyAllocator->newMem (p,size);

  return p;
}
//-------------------------------------------------------------------------------------------------------------------------------------
void operator delete(void *p) {


    if (p == NULL) {
        return;
    }

    if(pMyAllocator != NULL)
        pMyAllocator->freeMem (p);

    free(p);

    p = NULL;

}
