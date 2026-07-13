<div align="center">

# EggLock

**Internal Cheat for Deadlock (Source 2)**

[![Language](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows%20x64-lightgrey.svg)]()
[![License](https://img.shields.io/badge/License-MIT-green.svg)]()

</div>

---

## Features

### Visual (ESP)
- Box ESP (CoalBox, OutlineBox, Box, FillBox)
- Skeleton ESP
- Head Dot
- Health Bar
- Name ESP
- Distance ESP
- Snaplines
- NPC ESP
- Sound Step ESP

### AimBot
- Configurable FOV & Smooth
- Bone selection
- Team check
- Custom key bind

### Misc
- Sky Color Changer
- Night Mode
- Customizable menu (Dark Neon theme)
- Configurable menu key

### Auto-Update
- Pattern auto-update via HTTP
- Works across game updates without restart

---

## Build

### Requirements
- Visual Studio 2022+ (v143 toolset)
- Windows SDK 10.0
- C++17 or later

### Build Steps
1. Open `EggLock.sln` in Visual Studio
2. Select `Release | x64` configuration
3. Build solution (`Ctrl+Shift+B`)

### Loader
```bash
cd Loader
cl.exe /O2 /MT loader.cpp /Fe:Andromeda-Loader.exe /link user32.lib shell32.lib advapi32.lib
```

---

## Usage

1. Build the project
2. Copy `EggLock.dll` to the `Loader` folder
3. Run `Andromeda-Loader.exe` as Administrator
4. The loader will launch Deadlock and inject automatically
5. Press `Insert` in-game to open the menu

---

## Configuration

Edit `Settings.hpp` to enable/disable features and configure auto-update URL:

```cpp
namespace Settings
{
    namespace Visual { inline auto BoxEsp = true; }
    namespace AimBot { inline auto Active = false; }
    namespace Update {
        inline auto Enabled = true;
        inline const char* Url = "https://raw.githubusercontent.com/your-user/patterns.json";
    }
}
```

---

## Project Structure

```
EggLock/
├── AndromedaClient/       # Client-side code
│   ├── Features/          # ESP, AimBot
│   ├── Settings/          # Configuration
│   └── GUI/               # Menu system
├── Common/                # Utilities & hooks
├── DeadLock/              # SDK & patterns
│   ├── SDK/               # Function patterns
│   └── Hook/              # Engine hooks
└── GameClient/            # Game client logic
```

---

## Disclaimer

This project is for **educational purposes only**. Use at your own risk. The authors are not responsible for any consequences resulting from the use of this software.

---

<div align="center">

**Made with egg energy**

</div>
