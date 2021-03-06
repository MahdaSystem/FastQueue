#ifndef _FAST_QUEUE_H
#define	_FAST_QUEUE_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define FAST_QUEUE_LOW_LEVEL_CLEAR 0

typedef struct Struct_FastQueue {
  uint8_t* Buffer;
  uint32_t BufferLength;
  uint32_t BufferItemLength;
  uint32_t ItemSize;
  uint32_t Cursor;
  uint32_t Position;
  uint32_t CircleCounter;
  uint32_t TotalInsertedItem;
  bool     Lock;
} Type_FastQueue;

#define FQ_WaitUnlock(ptrFQControlblock,delayFunc) \
  while(FQ_IsLock(ptrFQControlblock)) { \
    delayFunc; \
  }

#define FQ_WaitTickUnlock(ptrFQControlblock,delayFunc,delayTickTimeout) \
  do { \
    uint32_t fq_WaitTickUnlock_delayTickTimeout = delayTickTimeout; \
    while(FQ_IsLock(ptrFQControlblock) && ((fq_WaitTickUnlock_delayTickTimeout--) > 0)) { \
      delayFunc; \
    } \
  } while(0)

bool FQ_Init(Type_FastQueue* ptrFQControlblock, uint8_t* queueBuffer, uint32_t bufferLength, uint32_t itemSize);
size_t FQ_GetFreeSpace(Type_FastQueue* ptrFQControlblock);
size_t FQ_GetOccupiedSpace(Type_FastQueue* ptrFQControlblock);
size_t FQ_GetAvailableItems(Type_FastQueue* ptrFQControlblock);
bool FQ_IsEmpty(Type_FastQueue* ptrFQControlblock);
bool FQ_IsFull(Type_FastQueue* ptrFQControlblock);
bool FQ_Insert(Type_FastQueue* ptrFQControlblock, uint8_t* insertItem);
uint8_t* FQ_Read1(Type_FastQueue* ptrFQControlblock);
void FQ_Read2(Type_FastQueue* ptrFQControlblock, uint8_t** readItem);
void FQ_Read3(Type_FastQueue* ptrFQControlblock, uint8_t* readItem);
uint8_t* FQ_Peek1(Type_FastQueue* ptrFQControlblock);
void FQ_Peek2(Type_FastQueue* ptrFQControlblock, uint8_t** readItem);
void FQ_Peek3(Type_FastQueue* ptrFQControlblock, uint8_t* readItem);
void FQ_Clear(Type_FastQueue* ptrFQControlblock);
bool FQ_IsLock(Type_FastQueue* ptrFQControlblock);

#endif /* _FAST_QUEUE_H */
