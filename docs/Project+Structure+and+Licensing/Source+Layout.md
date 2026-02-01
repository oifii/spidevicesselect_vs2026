# Project Structure and Licensing

This section outlines the organization of source files, build artifacts, and licensing for the **spidevicesselect_vs2026** project. It helps contributors locate key components and understand how they fit together.

## Source Layout

The root directory contains the core C++ sources, headers, configuration, build files, bundled PortAudio tree, and the project license.

```bash
├── spidevicesselect.cpp
├── spiaudiodevice.cpp
├── spiaudiodevice.h
├── defs.h
├── spidevicesselect.vcxproj
├── spidevicesselect.vcxproj.filters
├── lib-src/
│   └── portaudio-2021/
│       └── portaudio_vs2026/
└── LICENSE.txt
```

| File / Directory | Type | Purpose |
| --- | --- | --- |
| **spidevicesselect.cpp** 🖥️ | C++ Source | Main console application; lists, scans, and selects audio devices via PortAudio. |
| **spiaudiodevice.cpp** 🛠️ | C++ Source | Implements the `SPIAudioDevice` helper class for scanning and matching audio devices. |
| **spiaudiodevice.h** 🔧 | Header | Declares the `SPIAudioDevice` class interface and related enums. |
| **defs.h** ⚙️ | Header | Configuration macros (sample format, rate, buffer size, channels) used throughout the code. |
| **spidevicesselect.vcxproj** 📝 | Visual Studio Project | Defines build configurations (Debug/Release, Win32/x64), include paths, and dependencies. |
| **spidevicesselect.vcxproj.filters** 📑 | Visual Studio Filters | Organizes source and header files into IDE filters for better navigation. |
| **lib-src/portaudio-2021/portaudio_vs2026/** 📂 | Bundled PortAudio subtree | PortAudio 2021 source, headers, and prebuilt libraries tailored for VS2026 builds. |
| **LICENSE.txt** 📜 | License | Full text of the GNU General Public License, version 3. |


### C++ Sources

- **spidevicesselect.cpp**
- Contains the `main` function, parses command-line options, and invokes device selection routines.
- **spiaudiodevice.cpp / spiaudiodevice.h**
- Encapsulate PortAudio interactions in `SPIAudioDevice`, providing:
- `ScanAudioDevices()`
- `MatchHostAPI()`
- `MatchDevice()`
- `SelectAudioInputDevice()`, `SelectAudioOutputDevice()`

### Configuration Header

- **defs.h**
- Defines constants such as `SAMPLE_RATE`, `FRAMES_PER_BUFFER`, `NUM_CHANNELS`, and `PA_SAMPLE_TYPE`.
- Adjust these values to change sample format or performance characteristics.

### Build Files

- **spidevicesselect.vcxproj**
- MSBuild project: sets `<AdditionalIncludeDirectories>` to `.\lib-src\portaudio-2021\portaudio_vs2026\include`
- Links against PortAudio libraries under `build\msvc\…\portaudio_x86.lib` / `portaudio_x64.lib`.
- **spidevicesselect.vcxproj.filters**
- Maps `*.cpp` into **Source Files** filter and `*.h` into **Header Files** filter for Visual Studio.

### Bundled PortAudio

The `lib-src/portaudio-2021/portaudio_vs2026` directory contains:

- **include/**: PortAudio public headers
- **build/msvc/**: Prebuilt `.lib` files for Win32 and x64, Debug and Release

The project’s `.vcxproj` references these for compilation and linking.

## Licensing

This project is distributed under the **GNU General Public License v3.0**. All derived works must comply with GPLv3 terms.

- **License File**: `LICENSE.txt` contains the full GPLv3 text.
- **Source Headers**: Each `.cpp` begins with a GPLv3 header comment granting users the rights to use, modify, and redistribute.
- **Redistribution**: Permitted provided modified files retain license notices and recipients receive a copy of GPLv3.

```card
{
    "title": "License Overview",
    "content": "This project is released under GNU GPL v3. See LICENSE.txt for the full terms."
}
```