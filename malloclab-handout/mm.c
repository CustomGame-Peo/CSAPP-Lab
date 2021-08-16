/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"



/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/


/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define bin_idx(size) (size-0x10)>>4
#define get_size(p) *(p+4)
#define get_inuse(p) (get_size(p)&1)
static char *mem_start_brk;  /* points to first byte of heap */
static char *mem_brk;        /* points to last byte of heap */
static char *mem_max_addr;   /* largest legal heap address */ 
size_t heap_max_size=0x110;
char* bin[0x20];
char* large_bin;
/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    mem_init();
    mem_start_brk = mem_heap_lo();
    mem_sbrk(0);

    mem_brk=mem_heap_hi();
    mem_max_addr=mem_start_brk;
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    int newsize = ALIGN(size + SIZE_T_SIZE)+0x8;

    if(size<=heap_max_size){
        int index=bin_idx(newsize);
        if(bin[index]+8==NULL)
        {
            void *p = mem_sbrk(newsize);
            if (p == (void *)-1)
                return NULL;
            else {
                *(size_t *)(p+4) = size;
                return (void *)((char *)p + SIZE_T_SIZE);
            }
        }
        else
        {
            void *p;
            p=bin[index];
            bin[index]=*(bin[index]+8);
            return p+8;
        }
    }else{
        int* p;
        int max_size=0x7fffffff;
        int* tmp;
        for(p=large_bin;;p=*(p+2))
        {
            if(p==NULL)
            {
                void *p = mem_sbrk(newsize);
                if (p == (void *)-1)
                    return NULL;
                else {
                    *(size_t *)(p+4) = size;
                    return (void *)((char *)p + SIZE_T_SIZE);
                }
            }
            else
            {
                if(*(p+4)-newsize>=0&&*(p+4)-newsize<max_size){
                    tmp=p;
                    max_size=*(p+4)-newsize;
                }
            }
            if(*(p+2)==NULL){
                return (void*)tmp+8;
            }
        }
    }
    
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    if(*((unsigned*)ptr+1)<0x110){
        int idx=bin_idx(*((unsigned*)ptr+1));
        *((char*)ptr+8)=bin[idx];
        bin[idx]=ptr;
    }else{
        void* p;
        void* tmp=large_bin;
        if(large_bin==NULL){
            large_bin=ptr;
        }
        else if(*((int*)large_bin+1)>=*((int*)ptr+1))
        {
            *((char*)ptr+8)=large_bin;
            large_bin=ptr;
        }
        else{
            for(p=large_bin;;p=*((char*)p+8)){
                if(*((int*)p+1)>=*((int*)ptr+1))
                {
                    *((char*)ptr+8)=p;
                    *((char*)tmp+8)=ptr;
                }
                tmp=p;
            }
        }
    }
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}














