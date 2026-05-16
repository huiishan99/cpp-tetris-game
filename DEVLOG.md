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

### 2026-05-16 - Add maintenance devlog

- Changed: Created `DEVLOG.md` and linked it from `README.md`.
- Why: Establish a traceable change history before future fixes and refactors.
- Risk: Documentation-only change; no runtime impact.
- Verified: Not run; documentation-only change.
- Follow-ups: Add a new devlog entry for every future repository change.
