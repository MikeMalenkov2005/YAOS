#ifndef GAME_H
#define GAME_H

#include <types.h>

typedef void (*PFN_SCENE_PROC)(UINT Event);

void SetSceneProc(PFN_SCENE_PROC pfnNewSceneProc);

BOOL InitMainMenu();

BOOL InitGame(UINT FieldSize, UINT MineCount);

#endif

