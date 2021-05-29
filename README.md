# 🧭 Speedometer for BakkesMod

An unobtrusive speedometer for Rocket League to remind you to go fast. 🌠

![Preview](https://raw.githubusercontent.com/mtimkovich/SpeedometerPlugin/main/preview.png)

Partially inspired by [this JohnnyBoi tweet][tweet], I wanted to see how my average car speed compared to the pros.

## ✨ Features

- Current car speed.
- Per-game average car speed.

## 💽 Installation

1. Download the [latest release][release].
3. Copy `Speedometer.dll` to `/plugins` your BakkesMod folder.
4. _(Optional)_ You may need to copy the `speedometer.set` file to `/plugins/settings` if settings
   aren't showing up in the `Plugins` tab.
5. Open RL and BakkesMod.
6. Open the BM console (`~`) and run:
    - `plugin load speedometer`
    - `cl_settings_refreshplugins`

## 🖥️ Compiling

### Prereqs

- [Visual Studio 2019][vs]

### Steps

1. Open `x64 Native Tools Command Prompt for VS 2019`.
2. Use the path to the BakkesModSDK in the command below. This will either be in your BakkesMod
folder or cloned from the [repo][bakkessdk].

```powershell
cl /LD /EHsc -I bakkesmodsdk/include/ bakkesmodsdk/lib/pluginsdk.lib Speedometer.cpp
```

## 🚧 Roadmap

- [x] Show in-game car speed.
- [x] Adjustable position.
- [x] Metric option.
- [ ] Calculate per game metrics. ([#1][1])
  - [x] Mean
  - [ ] Stdev
- [x] Add average pro car speeds as examples to the `README`.

[release]: https://github.com/mtimkovich/SpeedometerPlugin/releases
[compile]: https://github.com/mtimkovich/SpeedometerPlugin#compiling
[vs]: https://visualstudio.microsoft.com/
[bakkessdk]: https://github.com/bakkesmodorg/BakkesModSDK
[1]: https://github.com/mtimkovich/SpeedometerPlugin/issues/1
[tweet]: https://twitter.com/JohnnyBoi_i/status/1385989157642805248
