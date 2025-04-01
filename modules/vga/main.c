#include <kernel/arch/x86/pio.h>
#include <sys/yaos.h>

static volatile UINT16 *pTextScreen;
static UINT16 Cursor;
static BOOLEAN bCursorEnabled;
static UINT8 Color;

inline static void UpdateCursor()
{
  if (bCursorEnabled)
  {
    WritePort8(0x3D4, 0x0F);
    WritePort8(0x3D5, Cursor);
    WritePort8(0x3D4, 0x0E);
    WritePort8(0x3D5, Cursor >> 8);
  }
}

inline static void EnableCursor(UINT8 Start, UINT8 End)
{
  WritePort8(0x3D4, 0x0A);
  WritePort8(0x3D5, Start & 0x1F);
  WritePort8(0x3D4, 0x0B);
  WritePort8(0x3D5, End & 0x1F);
  bCursorEnabled = TRUE;
}

inline static void DisableCursor()
{
  WritePort8(0x3D4, 0x0A);
  WritePort8(0x3D5, 0x20);
  bCursorEnabled = FALSE;
}

inline static void ClearScreen()
{
  Cursor = 0;
  for (UINT i = 0; i < 2000; ++i) pTextScreen[i] = ((UINT16)Color << 8);
  UpdateCursor();
}

inline static void Scroll(UINT8 Lines)
{
  if (Lines < 25)
  {
    UINT16 Index = 0;
    UINT16 Chars = Lines * 80;
    UINT16 Length = 2000 - Chars;
    for (;Index < Length; ++Index) pTextScreen[Index] = pTextScreen[Index + Chars];
    for (;Index < 2000; ++Index) pTextScreen[Index] = (UINT16)Color << 8;
    if (Cursor > Chars) Cursor -= Chars;
    else Cursor = Cursor % 80;
    UpdateCursor();
  }
  else ClearScreen();
}

void MessageLoop()
{
  MESSAGE Message = { 0 };
  while (WaitMessage(&Message) == SYSRET_OK)
  {
    for (UINT i = 0; i < MESSAGE_SIZE && Message.Payload[i]; ++i)
    {
      switch (Message.Payload[i])
      {
        case 1: /* Print String */
          if (i + 1 < MESSAGE_SIZE)
          {
            for (UINT8 Length = Message.Payload[++i]; Length && i < MESSAGE_SIZE; --Length)
            {
              pTextScreen[Cursor++] = Message.Payload[++i] | ((UINT16)Color << 8);
              if (Cursor >= 2000) Scroll(1);
            }
            UpdateCursor();
          }
          break;
        case 2:   /* Print Colored String */
          if (i + 2 < MESSAGE_SIZE)
          {
            UINT8 Color = Message.Payload[++i];
            for (UINT8 Length = Message.Payload[++i]; Length && i < MESSAGE_SIZE; --Length)
            {
              pTextScreen[Cursor++] = Message.Payload[++i] | ((UINT16)Color << 8);
              if (Cursor >= 2000) Scroll(1);
            }
            UpdateCursor();
          }
          break;
        case 3:   /* Print Symbol */
          if (i + 1 < MESSAGE_SIZE)
          {
            pTextScreen[Cursor++] = Message.Payload[++i] | ((UINT16)Color << 8);
            if (Cursor >= 2000) Scroll(1);
            else UpdateCursor();
          }
          break;
        case 4:   /* Print Colored Symbol */
          if (i + 2 < MESSAGE_SIZE)
          {
            UINT8 Color = Message.Payload[++i];
            pTextScreen[Cursor++] = Message.Payload[++i] | ((UINT16)Color << 8);
            if (Cursor >= 2000) Scroll(1);
            else UpdateCursor();
          }
          break;
        case 5:   /* Set Symbol */
          if (i + 1 < MESSAGE_SIZE)
          {
            pTextScreen[Cursor] = Message.Payload[++i] | ((UINT16)Color << 8);
          }
          break;
        case 6:   /* Set Colored Symbol */
          if (i + 2 < MESSAGE_SIZE)
          {
            UINT8 Color = Message.Payload[++i];
            pTextScreen[Cursor] = Message.Payload[++i] | ((UINT16)Color << 8);
          }
          break;
        case 7:   /* Set Color */
          if (i + 1 < MESSAGE_SIZE) Color = Message.Payload[++i];
          break;
        case 8:   /* Set Cursor */
          if (i + 2 < MESSAGE_SIZE)
          {
            UINT8 X = Message.Payload[++i];
            UINT8 Y = Message.Payload[++i];
            if (X >= 80) X = 79;
            Cursor = X + Y * 80;
            if (Y >= 25) Scroll(Y - 24);
            else UpdateCursor();
          }
          break;
        case 9:   /* Enable Cursor */
          if (i + 2 < MESSAGE_SIZE)
          {
            UINT8 Start = Message.Payload[++i];
            UINT8 End = Message.Payload[++i];
            EnableCursor(Start, End);
          }
          break;
        case 10:  /* Disable Cursor */
          DisableCursor();
          break;
        case 11:  /* Print Custom String */
          break;
      }
    }
  }
}

void Init()
{
  Color = 7;
  pTextScreen = MapDevice(NULL, 0x1000, 0xB8000 | MAP_MEMORY_WRITABLE | MAP_MEMORY_READABLE);
  EnableCursor(14, 15);
  ClearScreen();
  MessageLoop();
  Terminate();
}

