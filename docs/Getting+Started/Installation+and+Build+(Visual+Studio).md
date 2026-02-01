# Getting Started – Installation and Build (Visual Studio)

This section walks you through opening, configuring, and building the **spidevicesselect** console utility with Visual Studio. By following these steps, you’ll produce a standalone executable linked against the bundled PortAudio 2021 tree.

## Prerequisites

Before you begin, ensure you have:

- A Windows development machine (Win10+).
- A compatible Visual Studio version (Platform Toolset v145).
- The repository checked out, including the bundled PortAudio tree under `lib-src/portaudio-2021/portaudio_vs2026`.

## 1. Open the Project

1. Launch Visual Studio.
2. Choose **File → Open → Project/Solution**.
3. Select **spidevicesselect.vcxproj** in the root of the repository.

## 2. Verify PortAudio Include & Library Paths

The project expects PortAudio headers and built libraries in specific relative locations:

| Type | Path |
| --- | --- |
| Headers | `.\lib-src\portaudio-2021\portaudio_vs2026\include` |
| Win32 Debug lib | `.\lib-src\portaudio-2021\portaudio_vs2026\build\msvc\Win32\Debug\portaudio_x86.lib` |
| x64 Debug lib | `.\lib-src\portaudio-2021\portaudio_vs2026\build\msvc\x64\Debug\portaudio_x64.lib` |
| Win32 Release lib | `.\lib-src\portaudio-2021\portaudio_vs2026\build\msvc\Win32\Release\portaudio_x86.lib` |
| x64 Release lib | `.\lib-src\portaudio-2021\portaudio_vs2026\build\msvc\x64\Release\portaudio_x64.lib` |


## 3. Select Configuration & Platform

> **Tip:** If you rebuild PortAudio yourself, match these paths or adjust the project’s Additional Include/Library Directories accordingly.

The solution defines four build targets. Each config sets important preprocessor symbols and ties to the correct PortAudio library.

| Configuration | Platform | Preprocessor Definitions | PortAudio Library Path |
| --- | --- | --- | --- |
| **Debug** | Win32 | `WIN32;PA_USE_ASIO=1;_DEBUG;_CONSOLE` | `…\build\msvc\Win32\Debug\portaudio_x86.lib` |
| **Debug** | x64 | `_WIN64;PA_USE_ASIO=1;_DEBUG;_CONSOLE` | `…\build\msvc\x64\Debug\portaudio_x64.lib` |
| **Release** | Win32 | `WIN32;PA_USE_ASIO=1;NDEBUG;_CONSOLE` | `…\build\msvc\Win32\Release\portaudio_x86.lib` |
| **Release** | x64 | `_WIN64;PA_USE_ASIO=1;NDEBUG;_CONSOLE` | `…\build\msvc\x64\Release\portaudio_x64.lib` |


- **PA_USE_ASIO=1** enables ASIO support.
- **_CONSOLE** builds a console subsystem application.
- **WIN32/_WIN64** flags select 32-bit or 64-bit compilation.
- **_DEBUG/NDEBUG** control runtime checks and optimizations.

## 4. Build the Solution 🛠️

You can build the project in Visual Studio via:

```bash
# In Visual Studio: 
Build → Build Solution (or press F7)
```

After a successful build, you’ll find the console executable:

```plaintext
<repo_root>\$(Configuration)\spidevicesselect.exe
```

It will link against the correct `portaudio_x86.lib` or `portaudio_x64.lib` as per your chosen build.

## 5. Run the Tool

No installer or registration is needed. Simply open a Command Prompt, navigate to the output folder, and launch:

```bash
spidevicesselect.exe
```

You’ll see a list of available PortAudio host APIs and devices, along with their parameters.

---

**Enjoy scanning and selecting audio devices!**