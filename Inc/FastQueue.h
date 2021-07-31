#ifndef _FAST_QUEUE_H
#define	_FAST_QUEUE_H

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
} Type_FastQueue;

bool FQ_Init(Type_FastQueue* ptrFQControlblock, uint8_t* queueBuffer, uint32_t bufferLength, uint32_t itemSize);
size_t FQ_GetFreeSpace(Type_FastQueue* ptrFQControlblock);
size_t FQ_GetOccupiedSpace(Type_FastQueue* ptrFQControlblock);
size_t FQ_GetAvailableItems(Type_FastQueue* ptrFQControlblock);
bool FQ_IsEmpty(Type_FastQueue* ptrFQControlblock);
bool FQ_IsFull(Type_FastQueue* ptrFQControlblock);
bool FQ_Insert(Type_FastQueue* ptrFQControlblock, uint8_t* insertItem);
uint8_t* FQ_Read1(Type_FastQueue* ptrFQControlblock);
void FQ_Read2(Type_FastQueue* ptrFQControlblock, uint8_t** readItem);
void FQ_Clear(Type_FastQueue* ptrFQControlblock);

#endif /* _FAST_QUEUE_H */
