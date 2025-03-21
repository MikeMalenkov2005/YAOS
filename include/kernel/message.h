#ifndef KERNEL_MESSAGE_H
#define KERNEL_MESSAGE_H

#include <sys/message.h>

#ifndef MESSAGE_BUFFER_SIZE
#define MESSAGE_BUFFER_SIZE 307
#endif

typedef struct MESSAGE_QUEUE
{
  SIZE_T Head, Tail;
  MESSAGE Buffer[MESSAGE_BUFFER_SIZE];
} MESSAGE_QUEUE;

MESSAGE_QUEUE *CreateMessageQueue();

void DeleteMessageQueue(MESSAGE_QUEUE *pQueue);

BOOL PushMessage(MESSAGE_QUEUE *pQueue, const MESSAGE *pMessage);

BOOL PollMessage(MESSAGE_QUEUE *pQueue, MESSAGE *pMessage);

BOOL PeekMessage(MESSAGE_QUEUE *pQueue, MESSAGE *pMessage);

#endif

