#include "game.h"
#include "io.h"

static UINT32 RandomSeed;

#define Random() ((UINT)((RandomSeed = RandomSeed * 1103515245 + 12345) >> 16) & 0x7FFF)

static UINT GameSize;
static UINT SafeCount;

static UINT8 CursorX;
static UINT8 CursorY;

static BOOLEAN bGameIsNew;
static BOOLEAN bGameEnded;
static BOOLEAN bAltIsPressed;

static UINT8 GameField[19][19];

#define GAME_FIELD_POSITION (40 - GameSize)

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
  SetSymbol((X << 1) + GAME_FIELD_POSITION, Y + 1, GetSymbol(GameField[X][Y]));
  FlushSymbols();
}

void PlaceMine()
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

void RemoveMine(UINT8 X, UINT8 Y)
{
  if (GameField[X][Y] & 0x40)
  {
    if (Y > 0)
    {
      if (X > 0) GameField[X-1][Y-1]--;
      GameField[X][Y-1]--;
      if (X < GameSize - 1) GameField[X+1][Y-1]--;
    }

    if (X > 0) GameField[X-1][Y]--;
    GameField[X][Y] &= 0xBF;
    if (X < GameSize - 1) GameField[X+1][Y]--;
    
    if (Y < GameSize - 1)
    {
      if (X > 0) GameField[X-1][Y+1]--;
      GameField[X][Y+1]--;
      if (X < GameSize - 1) GameField[X+1][Y+1]--;
    }
  }
}

void OpenCell(UINT8 X, UINT8 Y)
{
  if (GameField[X][Y] & 0x10) return;
  UINT8 State = (GameField[X][Y] |= 0x10);
  if ((State & 0x40) && !bGameIsNew)
  {
    PrintColoredString(GAME_FIELD_POSITION, GameSize + 2, 8, "YOU LOST", 4);
    UpdatePosition(X, Y);
    bGameEnded = TRUE;
  }
  else
  {
    if (State & 0x40)
    {
      PlaceMine();
      RemoveMine(X, Y);
    }
    if (!(State & 15))
    {
      if (Y > 0)
      {
        if (X > 0) OpenCell(X-1, Y-1);
        OpenCell(X, Y-1);
        if (X < GameSize - 1) OpenCell(X+1, Y-1);
      }
      if (X > 0) OpenCell(X-1, Y);
      if (X < GameSize - 1) OpenCell(X+1, Y);
      if (Y < GameSize - 1)
      {
        if (X > 0) OpenCell(X-1, Y+1);
        OpenCell(X, Y+1);
        if (X < GameSize - 1) OpenCell(X+1, Y+1);
      }
    }
    if (!--SafeCount)
    {
      PrintColoredString(GAME_FIELD_POSITION, GameSize + 2, 8, "YOU WON!", 2);
      UpdatePosition(X, Y);
      bGameEnded = TRUE;
    }
  }
  UpdatePosition(X, Y);
  bGameIsNew = FALSE;
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
      if (!bGameEnded && CursorX) UpdatePosition(--CursorX, CursorY);
      break;
    case 't':
      if (!bGameEnded && CursorX < GameSize - 1) UpdatePosition(++CursorX, CursorY);
      break;
    case 'u':
      if (!bGameEnded && CursorY) UpdatePosition(CursorX, --CursorY);
      break;
    case 'r':
      if (!bGameEnded && CursorY < GameSize - 1) UpdatePosition(CursorX, ++CursorY);
      break;
    case 'Z':
      if (!bGameEnded && !(GameField[CursorX][CursorY] & 0x10))
      {
        if (bAltIsPressed || (GameField[CursorX][CursorY] & 0x20))
        {
          GameField[CursorX][CursorY] ^= 0x20;
          UpdatePosition(CursorX, CursorY);
        }
        else OpenCell(CursorX, CursorY);
      }
      break;
    case 'v':
      InitMainMenu();
      break;
  }
}

BOOL InitGame(UINT FieldSize, UINT MineCount)
{
  if (MineCount >= FieldSize * FieldSize || !SendCommandBuffer(6, "\x07\x00\x0B\x09\x0B\x0D")) return FALSE;
  PrintColoredString( 0, 24, 20, " Use ESC to exit.   ", 14);
  PrintColoredString(20, 24, 20, " Move with arrows.  ", 14);
  PrintColoredString(40, 24, 20, " CR opens a cell.   ", 14);
  PrintColoredString(60, 24, 19, " ALT+CR sets a flag.", 14);
  SetSymbol(79, 24, 0xE00 | '.');
  RandomSeed = FieldSize * MineCount;
  GameSize = FieldSize;
  SafeCount = FieldSize * FieldSize - MineCount;
  CursorX = 0;
  CursorY = 0;
  bGameIsNew = TRUE;
  bGameEnded = FALSE;
  bAltIsPressed = FALSE;
  for (UINT Y = 0; Y < GameSize; ++Y)
  {
    for (UINT X = 0; X < GameSize; ++X) GameField[X][Y] = 0;
    PrintColoredString(GAME_FIELD_POSITION, Y + 1, GameSize << 1, ". . . . . . . . . . . . . . . . . . . ", 7);
  }
  for (UINT i = 0; i < MineCount; ++i) PlaceMine();
  UpdatePosition(CursorX, CursorY);
  SetSceneProc(GameProc);
  return TRUE;
}

