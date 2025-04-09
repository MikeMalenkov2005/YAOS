#include "game.h"
#include "io.h"

#define MENU_FIELD_SIZE 0
#define MENU_MINE_COUNT 1
#define MENU_START_GAME 2
#define MENU_EXIT       3
#define MENU_LAST_ITEM  MENU_EXIT

static UINT MenuIndex = MENU_START_GAME;
static UINT FieldSize = 8;
static UINT MineCount = 8;

#define MIN_MINE_COUNT  ((FieldSize * FieldSize) >> 3)
#define MAX_MINE_COUNT  ((FieldSize * FieldSize) >> 2)

void DrawMenuItem(UINT Item)
{
  UINT8 Color = Item == MenuIndex ? 0xF0 : 0x0F;
  switch (Item)
  {
    case MENU_FIELD_SIZE:
      {
        char sBuffer[14] = "SIZE:   < XX >";
        sBuffer[10] = FieldSize > 9 ? (FieldSize / 10) + '0' : ' ';
        sBuffer[11] = (FieldSize % 10) + '0';
        PrintColoredString(33, 2, sizeof(sBuffer), sBuffer, Color);
      }
      break;
    case MENU_MINE_COUNT:
      {
        char sBuffer[14] = "MINES:  < XX >";
        sBuffer[10] = MineCount > 9 ? (MineCount / 10) + '0' : ' ';
        sBuffer[11] = (MineCount % 10) + '0';
        PrintColoredString(33, 4, sizeof(sBuffer), sBuffer, Color);
      }
      break;
    case MENU_START_GAME:
      PrintColoredString(33, 6, 14, "     PLAY     ", Color);
      break;
    case MENU_EXIT:
      PrintColoredString(33, 8, 14, "     EXIT     ", Color);
      break;
  }
}

void MainMenuProc(UINT Event)
{
  switch (Event & 0xFEFF)
  {
    case 'k':
      if (MenuIndex == MENU_FIELD_SIZE && FieldSize > 8)
      {
        --FieldSize;
        if (MineCount > MAX_MINE_COUNT)
        {
          MineCount = MAX_MINE_COUNT;
          DrawMenuItem(MENU_MINE_COUNT);
        }
      }
      if (MenuIndex == MENU_MINE_COUNT && MineCount > MIN_MINE_COUNT) --MineCount;
      DrawMenuItem(MenuIndex);
      break;
    case 't':
      if (MenuIndex == MENU_FIELD_SIZE && FieldSize < 19)
      {
        ++FieldSize;
        if (MineCount < MIN_MINE_COUNT)
        {
          MineCount = MIN_MINE_COUNT;
          DrawMenuItem(MENU_MINE_COUNT);
        }
      }
      if (MenuIndex == MENU_MINE_COUNT && MineCount < MAX_MINE_COUNT) ++MineCount;
      DrawMenuItem(MenuIndex);
      break;
    case 'u':
      if (MenuIndex) --MenuIndex;
      DrawMenuItem(MenuIndex + 1);
      DrawMenuItem(MenuIndex);
      break;
    case 'r':
      if (MenuIndex < MENU_LAST_ITEM) ++MenuIndex;
      DrawMenuItem(MenuIndex - 1);
      DrawMenuItem(MenuIndex);
      break;
    case 'Z':
      switch (MenuIndex)
      {
        case MENU_START_GAME:
          InitGame(FieldSize, MineCount);
          break;
        case MENU_EXIT:
          InitMainMenu();
          break;
      }
      break;
  }
}

BOOL InitMainMenu()
{
  if (!SendCommandBuffer(4, "\x07\x00\x0A\x0B")) return FALSE;
  MenuIndex = MENU_START_GAME;
  for (UINT Item = 0; Item <= MENU_LAST_ITEM; ++Item) DrawMenuItem(Item);
  SetSceneProc(MainMenuProc);
  return TRUE;
}

