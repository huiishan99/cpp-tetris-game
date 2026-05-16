# Devlog

This file records project changes, the reason for each change, verification
notes, and follow-up work. Keep it current so future bugs can be traced back to
specific edits.

## Change Policy

- Add one entry for every code, asset, build, or documentation change.
- Keep newest entries at the top.
- Include the files touched, why the change was made, what risk it carries, and
  how it was verified.
- If a change cannot be built or tested locally, say that explicitly.

## Entry Template

```md
### YYYY-MM-DD - Short summary

- Changed: ...
- Why: ...
- Risk: ...
- Verified: ...
- Follow-ups: ...
```

## Entries

### 2026-05-16 - Add CMake Windows build

- Changed: Added `CMakeLists.txt` and documented MinGW/Visual Studio CMake
  build commands in `README.md`.
- Why: Support more Windows compiler and IDE setups with one build definition.
- Risk: CMake configuration is new and currently limited to Win32 builds.
- Verified: Tried `cmake -S . -B /private/tmp/cpp-tetris-cmake-check`;
  `cmake` is not installed in this environment. Win32 GUI build not run.
- Follow-ups: Split game logic from Win32 rendering so the core can build and
  be tested on non-Windows platforms.

### 2026-05-16 - Add maintenance devlog

- Changed: Created `DEVLOG.md` and linked it from `README.md`.
- Why: Establish a traceable change history before future fixes and refactors.
- Risk: Documentation-only change; no runtime impact.
- Verified: Not run; documentation-only change.
- Follow-ups: Add a new devlog entry for every future repository change.
