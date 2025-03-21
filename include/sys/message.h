#include <types.h>

#define MESSAGE_SIZE 32

typedef struct MESSAGE 
{
  UINT SenderID;
  UINT ReceiverID;
  UINT8 Payload[MESSAGE_SIZE];
} MESSAGE;

