#include "game.h"
#include "io.h"

static UINT RandomSeed;

#define Random() (RandomSeed = 23497 * RandomSeed + 12345)

static UINT GameSize;
static UINT SafeCount;

static UINT8 CursorX;
static UINT8 CursorY;

static BOOLEAN bGameIsNew;
static BOOLEAN bGameEnded;
static BOOLEAN bAltIsPressed;

static UINT8 GameField[18][18];

#define GAME_FIELD_POSITION (40 - (GameSize >> 1))

inline static UINT16 GetSymbol(UINT8 State)
{
  if (State & 0x20) return 0x200 | 'F';
  if (State & 0x10)
  {
    if (State & 0x40) return 0x400 | 'X';
    if ((State & 15) < 9) return 0xE00 | ((State & 15) + '0');
    return 0x500 | '?';
  }
  return 0x700 | '.';
}

inline static void UpdatePosition(UINT8 X, UINT8 Y)
{
  SetSymbol(X + GAME_FIELD_POSITION, Y, GetSymbol(GameField[X][Y]));
  FlushSymbols();
}

void GameProc(UINT Event)
{
  switch (Event & 0xFEFF)
  {
    case 0x11:
      bAltIsPressed = TRUE;
      break;
    case 0x411:
      bAltIsPressed = FALSE;
      break;
    case 'k':
      if (CursorX) CursorX--;
      break;
    case 't':
      if (CursorX < GameSize - 1) CursorX++;
      break;
    case 'u':
      if (CursorY) CursorY--;
      break;
    case 'r':
      if (CursorY < GameSize - 1) CursorY++;
      break;
    case 'Z':
      if (!(GameField[CursorX][CursorY] & 0x10))
      {
        if (!bAltIsPressed && !(GameField[CursorX][CursorY] & 0x20))
        {
          if ((GameField[CursorX][CursorY] |= 0x10) & 0x40)
          {
            UpdatePosition(CursorX, CursorY);
            PrintColoredString(GAME_FIELD_POSITION, GameSize + 1, 9, "GAME OVER", 4);
            bGameEnded = TRUE;
          }
          else SafeCount--;
        }
        else GameField[CursorX][CursorY] ^= 0x20;
      }
      break;
    case 'v':
      InitMainMenu();
      break;
  }
  if (bGameEnded) UpdatePosition(CursorX, CursorY);
}

BOOL InitGame(UINT FieldSize, UINT MineCount)
{
  if (MineCount >= FieldSize * FieldSize || !SendCommandBuffer(6, "\x07\x00\x0B\x09\x0B\x0D")) return FALSE;
  RandomSeed = FieldSize * MineCount;
  GameSize = FieldSize;
  SafeCount = FieldSize - MineCount;
  CursorX = 0;
  CursorY = 0;
  bGameIsNew = TRUE;
  bGameEnded = FALSE;
  bAltIsPressed = FALSE;
  for (UINT Y = 0; Y < GameSize; ++Y)
  {
    for (UINT X = 0; X < GameSize; ++X) GameField[X][Y] = 0;
    PrintColoredString(GAME_FIELD_POSITION, Y, GameSize, "..................", 7);
  }
  for (UINT i = 0; i < MineCount; ++i)
  {
    UINT X = Random() % GameSize;
    UINT Y = Random() % GameSize;
    while (GameField[X][Y] & 0x40)
    {
      if (Random() & 1) X = Random() % GameSize;
      else Y = Random() % GameSize;
    }
    
    if (Y > 0)
    {
      if (X > 0) GameField[X-1][Y-1]++;
      GameField[X][Y-1]++;
      if (X < GameSize - 1) GameField[X+1][Y-1]++;
    }

    if (X > 0) GameField[X-1][Y]++;
    GameField[X][Y] |= 0x40;
    if (X < GameSize - 1) GameField[X+1][Y]++;
    
    if (Y < GameSize - 1)
    {
      if (X > 0) GameField[X-1][Y+1]++;
      GameField[X][Y+1]++;
      if (X < GameSize - 1) GameField[X+1][Y+1]++;
    }
  }
  UpdatePosition(CursorX, CursorY);
  SetSceneProc(GameProc);
  return TRUE;
}

