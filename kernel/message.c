#include <kernel/message.h>
#include <kernel/memory.h>

MESSAGE_QUEUE *CreateMessageQueue()
{
  MESSAGE_QUEUE *pQueue = MapLastFreePages(PAGE_ROUND_UP(sizeof(MESSAGE_QUEUE)) / PAGE_SIZE, MAPPING_WRITABLE_BIT | MAPPING_READABLE_BIT);
  if (pQueue)
  {
    pQueue->Head = 0;
    pQueue->Tail = 0;
  }
  return pQueue;
}

void DeleteMessageQueue(MESSAGE_QUEUE *pQueue)
{
  (void)FreeMappedPages((UINTPTR)pQueue, PAGE_ROUND_UP(sizeof(MESSAGE_QUEUE)) / PAGE_SIZE);
}

BOOL PushMessage(MESSAGE_QUEUE *pQueue, const MESSAGE *pMessage)
{
  if (pQueue->Head >= MESSAGE_BUFFER_SIZE) return FALSE;
  pQueue->Buffer[pQueue->Head++] = *pMessage;
  if (pQueue->Head >= MESSAGE_BUFFER_SIZE) pQueue->Head = 0;
  if (pQueue->Head == pQueue->Tail) pQueue->Head = MESSAGE_BUFFER_SIZE;
  return TRUE;
}

BOOL PollMessage(MESSAGE_QUEUE *pQueue, MESSAGE *pMessage)
{
  if (pQueue->Head == pQueue->Tail) return FALSE;
  if (pQueue->Head >= MESSAGE_BUFFER_SIZE) pQueue->Head = pQueue->Tail;
  *pMessage = pQueue->Buffer[pQueue->Tail++];
  if (pQueue->Tail >= MESSAGE_BUFFER_SIZE) pQueue->Tail = 0;
  return TRUE;
}

BOOL PeekMessage(MESSAGE_QUEUE *pQueue, MESSAGE *pMessage)
{
  if (pQueue->Head == pQueue->Tail) return FALSE;
  *pMessage = pQueue->Buffer[pQueue->Tail];
  return TRUE;
}

