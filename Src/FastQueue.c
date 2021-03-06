#include "FastQueue.h"

static uint8_t* InternalMemCpy(uint8_t *dest, uint8_t *src, size_t len) {
  while (len--)
    *dest++ = *src++;
  return dest;
}
#if FAST_QUEUE_LOW_LEVEL_CLEAR == 1
static void InternalMemClear(uint8_t *dest, size_t len, uint8_t clear_value) {
  while (len--)
    *dest++ = clear_value;
}
#endif
bool FQ_Init(Type_FastQueue* ptrFQControlblock, uint8_t* queueBuffer, uint32_t bufferLength, uint32_t itemSize) {
  if(ptrFQControlblock) {
    ptrFQControlblock->Lock = true; // Lock
    if(queueBuffer)
      if(itemSize > 0)
        if(bufferLength > itemSize) {
          ptrFQControlblock->Buffer = queueBuffer;
          ptrFQControlblock->BufferLength = bufferLength;
          ptrFQControlblock->ItemSize = itemSize;
          ptrFQControlblock->BufferItemLength = ptrFQControlblock->BufferLength / ptrFQControlblock->ItemSize;
          ptrFQControlblock->Cursor = 0;
          ptrFQControlblock->Position = 0;
          ptrFQControlblock->CircleCounter = 0;
          ptrFQControlblock->TotalInsertedItem = 0;
          
          ptrFQControlblock->Lock = false; // Unlcok
          
          return true;
        }
    ptrFQControlblock->Lock = false; // Unlcok
  }
  return false;
}
size_t FQ_GetFreeSpace(Type_FastQueue* ptrFQControlblock) {
  if (ptrFQControlblock)
    return ptrFQControlblock->BufferItemLength - ptrFQControlblock->Position;
  return 0;
}
size_t FQ_GetOccupiedSpace(Type_FastQueue* ptrFQControlblock) {
  if (ptrFQControlblock)
      return ptrFQControlblock->TotalInsertedItem;
  return 0;
}
size_t FQ_GetAvailableItems(Type_FastQueue* ptrFQControlblock) {
  if (ptrFQControlblock)
    return ptrFQControlblock->Position - ptrFQControlblock->Cursor;
  return 0;
}
bool static IsEmpty(Type_FastQueue* ptrFQControlblock) {
  if (ptrFQControlblock->Position >= (ptrFQControlblock->BufferItemLength - 1))
    return false;
  else {
    if (ptrFQControlblock->Position - ptrFQControlblock->Cursor < 1)
      return true;
    else
      return false;
  }
}
bool FQ_IsEmpty(Type_FastQueue* ptrFQControlblock) {
  if (ptrFQControlblock)
    return IsEmpty(ptrFQControlblock);
  return false;
}
bool FQ_IsFull(Type_FastQueue* ptrFQControlblock) {
  if (ptrFQControlblock)
    if (FQ_GetFreeSpace(ptrFQControlblock) == 0)
      return true;
  return false;
}
bool FQ_Insert(Type_FastQueue* ptrFQControlblock, uint8_t* insertItem) {
	if(ptrFQControlblock) {
    ptrFQControlblock->Lock = true; // Lock
    if (FQ_IsFull(ptrFQControlblock) == false) {
      InternalMemCpy(
        ptrFQControlblock->Buffer + (ptrFQControlblock->ItemSize * ptrFQControlblock->Position),
        insertItem,
        ptrFQControlblock->ItemSize);
      ptrFQControlblock->Position++;
      ptrFQControlblock->TotalInsertedItem++;
      
      ptrFQControlblock->Lock = false; // Unlcok
      
      return true;
    }
    ptrFQControlblock->Lock = false; // Unlcok
	}
	return false;
}
uint8_t* FQ_Read1(Type_FastQueue* ptrFQControlblock) {
  if (ptrFQControlblock) {
    ptrFQControlblock->Lock = true; // Lock
    if (FQ_GetAvailableItems(ptrFQControlblock) != 0) {
      ptrFQControlblock->Lock = false; // Unlcok
      return ptrFQControlblock->Buffer + (ptrFQControlblock->ItemSize * ptrFQControlblock->Cursor++);
    }
    ptrFQControlblock->Lock = false; // Unlcok
  }
  return NULL;
}
void FQ_Read2(Type_FastQueue* ptrFQControlblock, uint8_t** readItem) {
	if(ptrFQControlblock) {
    ptrFQControlblock->Lock = true; // Lock
    
		*readItem = FQ_Read1(ptrFQControlblock);
    
    ptrFQControlblock->Lock = false; // Unlcok
  }
}
void FQ_Read3(Type_FastQueue* ptrFQControlblock, uint8_t* readItem) {
  if(ptrFQControlblock) {
    ptrFQControlblock->Lock = true; // Lock
    
    InternalMemCpy(readItem, FQ_Read1(ptrFQControlblock), ptrFQControlblock->ItemSize);
    
    ptrFQControlblock->Lock = false; // Unlcok
  }
}
uint8_t* FQ_Peek1(Type_FastQueue* ptrFQControlblock) {
  if (ptrFQControlblock) {
    ptrFQControlblock->Lock = true; // Lock
    if (FQ_GetAvailableItems(ptrFQControlblock) != 0) {
      ptrFQControlblock->Lock = false; // Unlcok
      return ptrFQControlblock->Buffer + (ptrFQControlblock->ItemSize * ptrFQControlblock->Cursor);
    }
    ptrFQControlblock->Lock = false; // Unlcok
  }
  return NULL;
}
void FQ_Peek2(Type_FastQueue* ptrFQControlblock, uint8_t** readItem) {
	if(ptrFQControlblock) {
    ptrFQControlblock->Lock = true; // Lock
    
		*readItem = FQ_Peek1(ptrFQControlblock);
    
    ptrFQControlblock->Lock = false; // Unlcok
  }
}
void FQ_Peek3(Type_FastQueue* ptrFQControlblock, uint8_t* readItem) {
  if(ptrFQControlblock) {
    ptrFQControlblock->Lock = true; // Lock
    
    InternalMemCpy(readItem, FQ_Peek1(ptrFQControlblock), ptrFQControlblock->ItemSize);
    
    ptrFQControlblock->Lock = false; // Unlcok
  }
}
void FQ_Clear(Type_FastQueue* ptrFQControlblock) {
  if(ptrFQControlblock) {
    ptrFQControlblock->Lock = true; // Lock
    
#if FAST_QUEUE_LOW_LEVEL_CLEAR == 1
    InternalMemClear(ptrFQControlblock->Buffer, ptrFQControlblock->BufferLength, 0);
#endif
    
    ptrFQControlblock->Cursor = 0;
    ptrFQControlblock->Position = 0;
    ptrFQControlblock->CircleCounter = 0;
    ptrFQControlblock->TotalInsertedItem = 0;
    
    ptrFQControlblock->Lock = false; // Unlcok
  }
}
bool FQ_IsLock(Type_FastQueue* ptrFQControlblock) {
  if(ptrFQControlblock)
    return ptrFQControlblock->Lock;
  return false;
}
