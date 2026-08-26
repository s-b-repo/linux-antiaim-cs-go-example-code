# linux-antiaim-cs-go-example-code

An educational excerpt of a Source-1 CS:GO client-cheat framework's
anti-aim implementation. The `code` file (no extension, ~90 lines) is
the body of a single method — `Features::AntiAim::createMove(CUserCmd*
cmd)` — that shows how six anti-aim modes were driven from the
game's `createmove` hook.

> **Historical reference only.**
>
> - CS:GO on Source 1 was retired in September 2023 when Valve shipped
>   Counter-Strike 2. CS2 uses Source 2, has fundamentally different
>   netcode, no LBY, no `CUserCmd` in the same shape, and completely
>   different anti-cheat surface (VAC-Net, VAC on Source 2, plus
>   Overwatch). Nothing in this file runs against CS2.
> - Even against Source-1 servers, running third-party client software
>   is VAC-bannable and against the Steam Subscriber Agreement.
> - This repo exists to make the *mechanics* of anti-aim readable —
>   not to be built and deployed.

## What each mode does

The `case` values come from a config-tree lookup
(`CONFIGINT("Rage>AntiAim>Real>Method")` and the corresponding Fake).

| Real mode | Fake mode | Effect |
|-----------|-----------|--------|
| 0: Static | — | Set `viewangles.y` to a fixed value; server-side yaw stops updating between commands. |
| 1: Jitter | — | Alternate `viewangles.y` by ±N degrees every `jitterDelay` commands — the real angle is a moving target. |
| 2: Real Jitter | — | Same as Jitter but jitters the *real* angle sent to the server (not just the visible one). |
| 3: Static + Fake Jitter | 1: Fake Jitter | The real angle is fixed; the fake (shown to spectators/other players) jitters. Splits the aim-punish surface. |
| 4: Spin | — | Increment `viewangles.y` by `spinDelay` every command — the real angle rotates continuously. Interacts badly with hitboxes. |
| — | — (default) | LBY (Lower Body Yaw) desync — flip the fake 180° so shots at the head-yaw don't line up with the body's stand-still LBY. |

## Files

- `code` — the original method body. Preserved unchanged.
- `stubs/framework.h` — new. Opaque forward declarations for the
  framework types the snippet references (`CUserCmd`, `Weapon`,
  `Globals`, `Interfaces::…`, `ItemIndex::*`, `CONFIGINT`,
  `updatingLby()`), so a compiler at least gets past the `#include`
  stage when analysing the file. Doesn't link.

## Known issues in the snippet

- Non-breaking-space (`U+00A0`) characters mixed into the indentation
  on some lines — cosmetic in most editors, will confuse some
  formatters and older MSVC settings.
- Missing divide-by-zero guards on `jitterDelay`, `fakeJitterDelay`,
  `realJitterDelay`, `spinDelay`.
- No `default:` case in the switch.
- Integer angle math (`int real, fake`) truncates fractions.

None of these are being "fixed" — this repo documents the snippet, it
does not maintain it.

## For readers interested in the technique

- [OtterCTF / Cheatsheet: Anti-Aim mechanics on Source 1](https://www.unknowncheats.me/forum/counterstrike-global-offensive/) — most quality write-ups live on UC's CS:GO subforum; search "resolver" and "LBY breakers" for the defender-side perspective.
- Source-1 SDK ([`CUserCmd` header](https://github.com/ValveSoftware/source-sdk-2013)) for what the struct actually looks like on the wire.
- For a defender's angle: read the `client.dll` netchan message
  handling on Source 1, then read Overwatch case reviews to see what
  spins look like to human reviewers.

## License

No commercial license — research/reference material. Do not ship.
