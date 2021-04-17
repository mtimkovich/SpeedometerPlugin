# Speedometer for BakkesMod

An unobtrusive speedometer for Rocket League to remind you to go fast.

## Installation

1. Download the [latest release][release].
2. Copy `Speedometer.dll` to `/plugins` your BakkesMod folder.
3. (Optional) You may need to copy the `speedometer.set` file to `/plugins/settings`.
4. Open RL and BakkesMod.
5. Open the BM console (`~`) and run:
    - `plugin load speedometer`
    - `cl_settings_refreshplugins`

## Compiling

### Prereqs

- [Visual Studio 2019][vs]

### Steps

1. Open "x64 Native Tools Command Prompt for VS 2019".
2. Use the path to the BakkesModSDK in the command below. This will either be in your BakkesMod
folder or cloned from the [repo][bakkessdk].

```powershell
cl /LD -I bakkesmodsdk/include/ bakkesmodsdk/lib/pluginsdk.lib Speedometer.cpp
```

## Roadmap

- [x] Show in-game car speed.
- [x] Adjustable position.
- [x] Metric option.
- [ ] Fix crash when leaving a game.
- [ ] Calculate per game metrics (mean and stdev).

[release]: https://github.com/mtimkovich/SpeedometerPlugin/releases
[vs]: https://visualstudio.microsoft.com/
[bakkessdk]: https://github.com/bakkesmodorg/BakkesModSDK
