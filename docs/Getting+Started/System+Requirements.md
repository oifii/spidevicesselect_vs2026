# Getting Started

## System Requirements

This section outlines the core requirements to build and run the **spidevicesselect** utility. Ensure your development environment matches these specifications before proceeding with compilation or execution.

### 1. Platform & Toolchain 🖥️

| Component | Specification |
| --- | --- |
| Operating System | Windows 10 or later (Win32 or x64) |
| IDE / Build System | Microsoft Visual Studio (solution uses provided `.vcxproj`) |
| C++ Standard | C++17 (configured in project settings) |


### 2. PortAudio Integration 🎛️

> The Visual Studio project configurations target both **Win32** and **x64** platforms. Build using the supplied `.vcxproj`—no additional CMake or custom build scripts are required.

You must use the bundled PortAudio source tree for both headers and libraries. This ensures full compatibility with the project’s Visual Studio settings.

- **Source Tree**: `lib-src/portaudio-2021/portaudio_vs2026`
- **Include Directory**:

```xml
  <ClCompile>
    <AdditionalIncludeDirectories>
      .\lib-src\portaudio-2021\portaudio_vs2026\include;
    </AdditionalIncludeDirectories>
  </ClCompile>
```

- **Library Dependencies**:

```xml
  <Link>
    <AdditionalDependencies>
      .\lib-src\portaudio-2021\portaudio_vs2026\build\msvc\Win32\Debug\portaudio_x86.lib;
      %(AdditionalDependencies)
    </AdditionalDependencies>
  </Link>
```

### 3. Optional ASIO Support 🎧

> The project references Debug and Release builds of **portaudio_x86.lib** or **portaudio_x64.lib** depending on the target platform.

ASIO support is enabled at compile time and requires a compatible ASIO driver installed on your system.

- **Enable Macro**: `PA_USE_ASIO=1`
- **Project Definition**:

```xml
  <PreprocessorDefinitions>
    WIN32;PA_USE_ASIO=1;_DEBUG;_CONSOLE;%(PreprocessorDefinitions)
  </PreprocessorDefinitions>
```

- **Runtime Requirement**: A system ASIO driver (e.g., manufacturer-provided) must be present to enumerate ASIO devices.

### 4. Runtime Environment 📂

> When **PA_USE_ASIO** is defined, the code includes the ASIO header and populates ASIO-specific latency information, allowing selection of low-latency audio I/O.

To execute the utility and capture device listings:

- **Executable**: `spidevicesselect.exe`
- **Console Output**: Interactive listing of all audio devices
- **Log File**: Automatically generated `device.txt` in the working directory

```bash
> spidevicesselect.exe > device.txt
```

> The program writes a detailed report of each host API and device, including default latencies and channel counts, to both **stdout** and the `device.txt` log.

---

With these requirements met, you can load the Visual Studio solution, select your configuration (Debug/Release, Win32/x64), and build the **spidevicesselect** utility. Proceed to the **Building & Execution** section for step-by-step instructions.