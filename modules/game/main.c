#include "game.h"
#include "io.h"

#include <sys/yaos.h>

static PFN_SCENE_PROC pfnSceneProc;

void SetSceneProc(PFN_SCENE_PROC pfnNewSceneProc)
{
  pfnSceneProc = pfnNewSceneProc;
}

void EventLoop()
{
  int Event;
  while (pfnSceneProc && (Event = NextKeyEvent()) != -1)
  {
    pfnSceneProc(Event);
  }
}

void Init()
{
  if (InitMainMenu())
  {
    EventLoop();
  }
  Terminate();
}

