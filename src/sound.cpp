#include "sound.h"

#ifdef _WIN32
#include <windows.h>
#endif

void PlayMoveSound()
{
#ifdef _WIN32
    Beep(440, 12);
#endif
}

void PlaySoftDropSound()
{
#ifdef _WIN32
    Beep(330, 10);
#endif
}

void PlayHardDropSound()
{
#ifdef _WIN32
    Beep(220, 35);
#endif
}

void PlayHoldSound()
{
#ifdef _WIN32
    Beep(520, 25);
#endif
}

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

void PlayLevelUpSound()
{
#ifdef _WIN32
    Beep(740, 45);
    Beep(980, 65);
#endif
}

void PlayPauseSound()
{
#ifdef _WIN32
    Beep(500, 30);
#endif
}

void PlayGameOverSound()
{
#ifdef _WIN32
    Beep(220, 70);
    Beep(165, 110);
#endif
}
