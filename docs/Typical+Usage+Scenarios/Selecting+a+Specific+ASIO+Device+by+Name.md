## Typical Usage Scenarios – Selecting a Specific ASIO Device by Name

This scenario demonstrates how to pick an ASIO device by name using the **spidevicesselect** console tool or the underlying **SPIAudioDevice** helper class. Follow these steps to identify your device, invoke the tool, and integrate the resulting PortAudio parameters into your own application.

---

### 1. Identify Your ASIO Device Name

Before selection, run the device-listing mode of **spidevicesselect** to view all available audio devices:

```bash
spidevicesselect list
```

Look for lines marked `[ Default ASIO Input, Default ASIO Output ]` or where **Host API = ASIO**.

Example excerpt:

| Device # | Host API | Device Name |
| --- | --- | --- |
| ---------: | ---------- | ----------------- |
| 26 | ASIO | Realtek ASIO |
| 9 | ASIO | E-MU ASIO |


> 🎯 **Tip:** You can match by full or partial name (e.g. `"E-MU ASIO"` or `"Realtek"`).

---

### 2. Invoke the Console Tool

Pass the ASIO device name (exact or partial) as the first argument:

```bash
# Loose match on device name
spidevicesselect "E-MU ASIO"

# Explicitly specify host API and device name
spidevicesselect "ASIO:E-MU ASIO"
```

| Command Syntax | Description |
| --- | --- |
| `spidevicesselect "<name>"` | Performs a loose match on `<name>`. |
| `spidevicesselect "ASIO:<name>"` | Forces matching within the ASIO host API. |


---

### 3. Internal Processing Flow

1. **Populate Device Maps**

`ScanAudioDevices("loosely", spiaudiodeviceOUTPUT)` builds maps of all devices and their host APIs.

1. **Parse Host API Prefix**

If the input string contains a colon (`:`), the part before it becomes `global_audiooutputhostapi`, and the rest becomes `global_audiooutputdevicename` .

1. **Resolve Host API**

`MatchHostAPI("loosely", spiaudiodeviceOUTPUT)` returns the numeric host API type for ASIO.

1. **Perform Loose Name Match**

`MatchDevice("loosely", spiaudiodeviceOUTPUT, paASIO)` searches the ASIO device map for a partial or exact match.

1. **Configure Stream Parameters**

On finding exactly one match, `SelectAudioOutputDevice()` sets up `global_outputParameters` and, for ASIO devices, fills a `PaAsioStreamInfo` structure:

- **device** → matched device ID
- **channelCount** → `global_numchannels`
- **sampleFormat** → `PA_SAMPLE_TYPE`
- **suggestedLatency** → the device’s default low output latency
- **hostApiSpecificStreamInfo** → pointer to `global_asioOutputInfo` when API = ASIO

```cpp
   // ASIO-specific stream info
   global_asioOutputInfo.size             = sizeof(PaAsioStreamInfo);
   global_asioOutputInfo.hostApiType      = paASIO;
   global_asioOutputInfo.version          = 1;
   global_asioOutputInfo.flags            = paAsioUseChannelSelectors;
   global_asioOutputInfo.channelSelectors = global_outputAudioChannelSelectors;
   global_outputParameters.hostApiSpecificStreamInfo = &global_asioOutputInfo;
```

---

### 4. Consuming `PaStreamParameters` in Your Application

If you link against **SPIAudioDevice** rather than using the console tool, you can directly retrieve the configured stream parameters:

```cpp
#include "spiaudiodevice.h"

SPIAudioDevice audioDev;
audioDev.ScanAudioDevices();                                   // Build device maps
audioDev.global_audiooutputdevicename = "ASIO:E-MU ASIO";      // Set desired device
audioDev.SelectAudioOutputDevice();                            // Configures global_outputParameters

// Now use in PortAudio
Pa_OpenStream(
    &audioDev.global_stream,
     nullptr,                              // no input
    &audioDev.global_outputParameters,     // your ASIO output
    global_samplerate,
    global_frames_per_buffer,
    paNoFlag,
    YourCallback,
    yourUserData
);
```

> 📋 **Note:** Identical steps apply to **input** devices by using `global_audioinputdevicename` and `SelectAudioInputDevice()`.

---

### 5. When to Use This Scenario

- You have multiple ASIO devices installed (e.g. hardware + ASIO4ALL).
- You need to script or automate audio I/O selection without a GUI.
- Your application uses low-latency ASIO streams and requires explicit channel selectors.
- You prefer declarative device specification (by name) instead of numerical IDs.

By following these steps, you can reliably select and configure any ASIO device by name, leveraging the **SPIAudioDevice** helper to abstract PortAudio’s host-API specifics.