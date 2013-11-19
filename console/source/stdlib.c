// Copyright (C) 2013  Manuel Jelinek
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "stdlib.h"
#include "uart.h"
//#include "stdio.h"  // needed for puts (monitor output)

#define MEM_CORRUPT -1
#define DOUBLE_FREE -2
#define INVALLID_FREE -3

#define WRITE_ERROR(x) uartPuts(x)
//#define WRITE_ERROR(x) puts(x)  // use this define if you have initialized the
                                // framebuffer and want the error outputs there

static const char* MEM_CORRUPT_OUTPUT   = "ERROR: memory corruption!!!\n";
static const char* DOUBLE_FREE_OUTPUT   = "ERROR: double free!!!\n";
static const char* INVALLID_FREE_OUTPUT = "ERROR: invalid free of address!!!\n";
static const char* OUT_OF_MEM_OUTPUT    = "ERROR: no more memory available\n";

//          ELEMENT1 > ELEMENT2  |<--             Maximum available memory              -->|
#define SPACE(ELEMENT1,ELEMENT2) (((void*)(ELEMENT1)-(void*)(ELEMENT2+1))-ELEMENT2->length_)

static int32_t memory_integrity = 0;
const void* _heap_end = (void*)0x1C000000; // in standard configuration the GPU uses the last 64MB of the 512MB RAM


typedef struct header_ //manage_heap
{
  struct header_ *next_;
  struct header_ *prev_;
  uint32_t length_;  // length of allocated memory (without struct header_)
}__attribute__((__packed__)) header;

header* heap_start = NULL;


void *malloc(uint32_t size)
{
  // memory list corrupted
  if(memory_integrity != 0)
    return NULL;

  // smallest memory block size is 4byte
  if(size < 4)
    size = 4;

  // Only executed if HEAP is empty!!
  if (heap_start == NULL)
  {
    heap_start = (header*)(&_heap_start);

    heap_start->length_ = 0;
    heap_start->next_   = NULL;
    heap_start->prev_   = NULL;
  }

  // create first element
  header* element = heap_start;
  while(element->next_ != NULL)
  {
    // check whether Heap Consistency (not a complete checked)
    // is something TRUE the other won't be checked
    if(element->next_ <= heap_start || (void*)(element->next_) > _heap_end ||
       element->next_->prev_ < heap_start || (void*)(element->next_->prev_) >
                                _heap_end || element != element->next_->prev_)
    {
      WRITE_ERROR(MEM_CORRUPT_OUTPUT);
      memory_integrity = MEM_CORRUPT;
      return NULL;
    }

    // if enough memory is free after heapstart
    if(SPACE(element->next_, element) >= size && element->length_ == 0 &&
                                                   element->prev_ == NULL)
    {
      element->length_ = size;
      return (void*)(element+1);
    }

    // if enough memory is free between elements
    if(SPACE(element->next_, element) >= (sizeof(header)+size) ||
      (SPACE(element->next_, element) >= (sizeof(header)+size) &&
                                          element->prev_ == NULL))
    {
      element = element->next_;
      break;
    }

    element = element->next_;
  }

  // attach between items
  if(element->next_ != NULL)
  {
    header* temp = (header*)(((void*)(element->prev_+1))+element->prev_->length_);
    element->prev_->next_ = temp;
    temp->prev_           = element->prev_;
    temp->next_           = element;
    element->prev_        = temp;
    temp->length_         = size;

    return (void*)(temp+1);
  }

  // add new item at the end
  void *start_addr = (void*)(element+1);  //sbrk(size + sizeof(header));
  if(start_addr >= (start_addr + size + sizeof(header)))
  {
    WRITE_ERROR(OUT_OF_MEM_OUTPUT);
    return NULL;
  }

  element->next_ = start_addr + size;
  element->length_ = size;
  element->next_->next_   = NULL;
  element->next_->prev_   = element;
  element->next_->length_ = 0;

  return (void*) start_addr;
}

void free(void *ptr)
{
  // memory list corrupted
  if(memory_integrity != 0)
    return;

  // ptr is NULL-Pointer
  if(ptr == NULL)
    return;

  // check if pointer shows to heaps address-room
  if(heap_start == NULL || ptr >= _heap_end || ptr < (void*)(heap_start+1))
  {
    WRITE_ERROR(INVALLID_FREE_OUTPUT);
    return;
  }

  // check on double free (only works as long as the memory isn't overwritten)
  if(((header*)ptr-1)->length_ == 0)
  {
    WRITE_ERROR(DOUBLE_FREE_OUTPUT);
    return;
  }

  // element point to HEADER (ptr must be the first address of memory-block)
  header* element = ((header*)ptr)-1;

  // check whether Heap Consistency (first HEADER, not a complete checked)
  if(element == heap_start && (element->prev_ != NULL ||
    (void*)(element->next_) >= _heap_end || element->next_->prev_ < heap_start ||
    (void*)(element->next_->prev_) >= _heap_end || element != element->next_->prev_))
  {
    WRITE_ERROR(MEM_CORRUPT_OUTPUT);
    memory_integrity = MEM_CORRUPT;
    return;
  }

  // check whether Heap Consistency (not first HEADER, not a complete checked)
  if(element != heap_start)
  {
    if(element->prev_ < heap_start || element->next_ <= heap_start ||
      (void*)element->prev_ >= _heap_end || (void*)element->next_ >= _heap_end ||
      element->prev_->next_ <= heap_start || element->next_->prev_ < heap_start
      || (void*)element->prev_->next_ >= _heap_end || (void*)element->next_->prev_
      >= _heap_end || element->prev_->next_ != element ||
      element->next_->prev_ != element)
    {
      WRITE_ERROR(MEM_CORRUPT_OUTPUT);
      memory_integrity = MEM_CORRUPT;
      return;
    }
  }

  // first element and no other OR element is second and first and last are already empty
  if((element->prev_ == NULL && element->next_->next_ == NULL) ||
     (element->prev_ != NULL && element->next_ != NULL && element->prev_->prev_
      == NULL && element->next_->next_ == NULL && element->prev_->length_ == 0))
  {
    heap_start = NULL;
    return;
  }

  //first element free, but behind are more elements
  if(element->prev_ == NULL)
  {
    element->length_ = 0;
    return;
  }

  // element is not first
  element->prev_->next_ = element->next_;
  element->length_ = 0;

  // element is last
  if(element->next_->next_ == NULL)
  {
    header* temp = (header*)(((void*)(element->prev_+1))+element->prev_->length_);
    temp->next_   = NULL;
    temp->prev_   = element->prev_;
    element->prev_->next_ = temp;
    temp->length_ = 0;

    return;
  }

  element->next_->prev_ = element->prev_; // only necessary if not last
}

void *realloc(void *ptr, uint32_t size)
{
  // memory list corrupted
  if(memory_integrity != 0)
    return NULL;

  // if pointer is NULL a new element will be created on HEAP
  if(ptr == NULL)
  {
    return malloc(size);
  }

  // check if pointer shows to heaps address-room
  if(ptr > (_heap_end - sizeof(header)) || heap_start == NULL ||
                                   ptr < (void*)(heap_start+1))
  {
    WRITE_ERROR(MEM_CORRUPT_OUTPUT);
    memory_integrity = MEM_CORRUPT;
    return NULL;
  }

  // if size is 0 memory will be freed
  if(size == 0)
  {
    free(ptr);
    return NULL;
  }

  // smallest memory block size is 4byte
  if(size < 4)
    size = 4;

  // element point to HEADER (ptr must be the first address of memory-block)
  header* element = ((header*)ptr)-1;

  // check whether Heap Consistency (first HEADER, length_ don't be 0,
  //                                 not a complete check)
  if(element == heap_start && (element->prev_ != NULL ||
     (void*)(element->next_) >= _heap_end || element->next_->prev_ < heap_start ||
     (void*)(element->next_->prev_) >= _heap_end ||
     element != element->next_->prev_ || element->length_ == 0))
  {
    WRITE_ERROR(MEM_CORRUPT_OUTPUT);
    memory_integrity = MEM_CORRUPT;
    return NULL;
  }

  // check whether Heap Consistency (not first HEADER, not a complete check)
  if(element != heap_start)
  {
    if(element->prev_ < heap_start || element->next_ <= heap_start ||
      (void*)element->prev_ >= _heap_end || (void*)element->next_ >= _heap_end ||
      element->prev_->next_ <= heap_start || element->next_->prev_ < heap_start
      || (void*)element->prev_->next_ >= _heap_end || (void*)element->next_->prev_
      >= _heap_end || element->prev_->next_ != element ||
      element->next_->prev_ != element)
    {
      WRITE_ERROR(MEM_CORRUPT_OUTPUT);
      memory_integrity = MEM_CORRUPT;
      return NULL;
    }
  }

  // new block size smaller as old
  if(element->length_ >= size)
  {
    element->length_ = size;
    return (void*)(element+1);
  }

  // last element, therefore enough space to enlarge
  if(element->length_ < size && element->next_->next_ == NULL)
  {
    element->length_ = size;
    header* temp = (header*)(((void*)(element+1)) + size);
    if((void*)(temp+1) >= _heap_end)
    {
      WRITE_ERROR(OUT_OF_MEM_OUTPUT);
      return NULL;
    }

    temp->length_  = 0;
    temp->next_    = NULL;
    temp->prev_    = element;
    element->next_ = temp;
    return (void*)(element+1);
  }

  // enough space after block to enlarge
  if(((void*)(element->next_)-(void*)(element+1)) >= size)
  {
    element->length_ = size;
    return (void*)(element+1);
  }

  // not enough space behind the block to enlarge
  if(((void*)(element->next_)-(void*)(element+1)) < size)
  {
    // if in front of the block is enough memory free -> move
    // previous block don't be heap_start, if length_ is 0
    if(element->prev_ != NULL && element->prev_->length_ != 0 &&
       SPACE(element->next_,element->prev_) >= (sizeof(header)+size))
    {
      header* temp = (header*)((void*)(element->prev_+1)+element->prev_->length_);
      uint32_t length = element->length_ + sizeof(header);
      uint32_t i = 0;
      for(i=0; i<length; i++)
      {
        ((uint8_t*)temp)[i] = ((uint8_t*)element)[i];
      }
      temp->length_ = size;
      temp->prev_->next_ = temp;
      temp->next_->prev_ = temp;

      return (void*)(temp+1);
    }

    // if in front of the block is enough memory free and its heap_start with length_ 0
    if(element->prev_ != NULL && element->prev_->length_ == 0 &&
       SPACE(element->next_,element->prev_) >= size)
    {
      element->prev_->next_   = element->next_;
      element->next_->prev_   = element->prev_;
      element->prev_->length_ = size;

      uint32_t length = element->length_;
      uint32_t i = 0;
      for(i=0; i<length; i++)
      {
        ((uint8_t*)(heap_start+1))[i] = ((uint8_t*)(element+1))[i];
      }

      return (void*)(heap_start+1);
    }

    // get new block
    uint8_t* new_array = (uint8_t*)malloc(size);

    // copy memory from old to new block
    uint8_t* old_array = (uint8_t*)(element+1);
    uint32_t i=0;
    for(i=0; i<element->length_; i++)
      new_array[i] = old_array[i];

    // free old block
    free((void*)(element+1));

    return (void*)new_array;
  }

  return NULL;
}

