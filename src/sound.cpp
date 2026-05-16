#include "sound.h"

#ifdef _WIN32
#include <windows.h>
#endif

void PlayRotateSound()
{
#ifdef _WIN32
    Beep(880, 25);
#endif
}

void PlayLineClearSound()
{
#ifdef _WIN32
    Beep(660, 60);
#endif
}
