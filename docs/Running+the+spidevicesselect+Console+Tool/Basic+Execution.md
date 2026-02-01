# Running the spidevicesselect Console Tool – Basic Execution

After successfully building the `spidevicesselect` project (see build instructions in the Visual Studio solution), you can launch the console tool either directly from a **Developer Command Prompt** or by running it under the Visual Studio debugger (press **F5**). On startup, the program performs a minimal initialization sequence before listing audio devices.

Below is an overview of the steps executed on launch, followed by detailed explanations.

```cpp
// In spidevicesselect.cpp, main():
Pa_Initialize();
system("cls");
printf(
    "PortAudio version number = %d\n"
    "PortAudio version text   = '%s'\n",
    Pa_GetVersion(),
    Pa_GetVersionText()
);
// … continue to scan and list devices …
Pa_Terminate();
return 0;
```

---

## Startup Sequence

| Step | Call / Operation | Purpose |
| --- | --- | --- |
| 1 | `Pa_Initialize()` | **Initializes the PortAudio library**, setting up host APIs.^1 |
| 2 | `system("cls")` | **Clears the Windows console** for a clean output window. |
| 3 | `printf( … Pa_GetVersion(), Pa_GetVersionText() )` | **Displays the PortAudio version number and textual build info.** |


---

### 1. Initialize PortAudio 🎛️

- **Function:** `Pa_Initialize()`
- **Location:** Called at the very start of `main` (`spidevicesselect.cpp`).
- **Role:**
- Probes and initializes all enabled host APIs (MME, WASAPI, ASIO, WDM-KS, etc.).
- Allocates internal resources required for subsequent audio-device queries and streams.
- **Failure Handling:** On error, the program jumps to cleanup and reports via `Pa_Terminate()` and `Pa_GetErrorText(err)`.

### 2. Clear the Console 🧹

- **Operation:** `system("cls")`
- **Behavior:**
- Invokes the Windows shell to clear the command window buffer and screen.
- Ensures that only fresh tool output is visible, improving readability.
- **Portability Note:** This uses the Windows-specific `cls`. On other platforms, adjust accordingly (e.g., `clear` on UNIX).

### 3. Print PortAudio Version ℹ️

- **Calls:**
- `Pa_GetVersion()` – returns an integer version (e.g., `PA_VERSION_MAJOR << 16 | PA_VERSION_MINOR << 8 | PA_VERSION_SUBMINOR`).
- `Pa_GetVersionText()` – returns a human-readable string (e.g., `"PortAudio V19.7.0-devel"`).
- **Console Output Example:**

```plaintext
  PortAudio version number = 16777331
  PortAudio version text   = 'PortAudio V19.7.0-devel'
```

- **Purpose:** Confirms which PortAudio build is in use; aids troubleshooting if mismatched libraries are present.

---

## Quick Reference

- **Run from Developer Command Prompt:**

```bash
  spidevicesselect.exe
```

- **Run under Visual Studio:**
- Open `spidevicesselect_vs2026.sln`.
- Set `spidevicesselect` as the startup project.
- Press **F5** or **Ctrl + F5**.

---

> **Tip:** Ensure your built binary links against the correct PortAudio library (`portaudio_x86.lib` or `portaudio_x64.lib`) matching your active Visual Studio platform (Win32/x64).

---

^1 PortAudio initialization and version retrieval are defined in the PortAudio core; see `portaudio.h` for full API details.