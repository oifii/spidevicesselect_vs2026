# Selecting Audio Devices with SPIAudioDevice – Scanning Devices and Building Maps 🔍

The `ScanAudioDevices()` method in **SPIAudioDevice** clears and repopulates internal maps of all PortAudio devices and host APIs. This routine provides a unified view of available audio endpoints (inputs, outputs, and host-API groupings), enabling later selection and matching by name or API type.

## Overview

The primary responsibilities of `ScanAudioDevices()` are to:

- **Reset** all device and host-API maps when invoked with an empty `matchmode`.
- **Populate**:
- A global list of all devices.
- Separate maps for input-only and output-only devices.
- A map of host-API names per device.
- A host-API-type lookup for matching by API label.
- Host-API-specific maps (DirectSound, MME, ASIO, WASAPI, etc.).
- **Iterate** over the current PortAudio device list via `Pa_GetDeviceCount()` and `Pa_GetDeviceInfo()`.
- **Insert** entries into each map based on channel capabilities and host-API type.

This setup underpins `MatchHostAPI()`, `MatchDevice()`, and the device selection routines (`SelectAudioInputDevice()`, `SelectAudioOutputDevice()`).

## Clearing and Initializing Maps

When called with the default `matchmode` (an empty string), `ScanAudioDevices()` first **clears** all existing maps and **initializes** the host-API-type labels:

| Map Name | Purpose |
| --- | --- |
| `global_alldevicemap` | All device IDs → device names |
| `global_inputdevicemap` | Input-capable device IDs → device names |
| `global_outputdevicemap` | Output-capable device IDs → device names |
| `global_hostapimap` | Device IDs → host-API names |
| `**global_hostapimap_hostapitype**` | Host-API type constants → uppercase labels |
| `global_hostapimap_<api>` | Device IDs → device names for each specific API |


```cpp
if (matchmode.empty()) {
    global_alldevicemap.clear();
    global_inputdevicemap.clear();
    global_outputdevicemap.clear();
    global_hostapimap.clear();
    
    global_hostapimap_hostapitype.clear();
    global_hostapimap_hostapitype.insert({ paDirectSound, "DIRECTSOUND" });
    global_hostapimap_hostapitype.insert({ paMME,         "MME"         });
    global_hostapimap_hostapitype.insert({ paASIO,        "ASIO"        });
    // Additional APIs...
    global_hostapimap_hostapitype.insert({ paWDMKS,       "WDMKS"       });
    global_hostapimap_hostapitype.insert({ paWDMKS,       "WDM-KS"      });
    global_hostapimap_hostapitype.insert({ paJACK,        "JACK"        });
    global_hostapimap_hostapitype.insert({ paWASAPI,      "WASAPI"      });
    // Extended mappings: SoundManager, CoreAudio, OSS, ALSA, AL, BeOS, AudioScienceHPI
    // Clear all host-API-specific maps...
}
```

*(Excerpt from SPIAudioDevice::ScanAudioDevices) *

## Host-API Label Mappings 🗺️

`global_hostapimap_hostapitype` allows matching by common labels rather than raw PortAudio constants:

| PortAudio Type Constant | Label |
| --- | --- |
| `paDirectSound` | DIRECTSOUND |
| `paMME` | MME |
| `paASIO` | ASIO |
| `paWDMKS` | WDMKS / WDM-KS |
| `paJACK` | JACK |
| `paWASAPI` | WASAPI |
| `paSoundManager` | SOUNDMANAGER |
| `paCoreAudio` | COREAUDIO |
| `paOSS` | OSS |
| `paALSA` | ALSA |
| `paAL` | AL |
| `paBeOS` | BEOS |
| `paAudioScienceHPI` | AUDIOSCIENCEHPI |


## Enumerating Devices and Building Maps

After initialization, the method queries PortAudio for the current device list and **inserts** each device into the appropriate maps:

```cpp
const PaDeviceInfo* deviceInfo;
int numDevices = Pa_GetDeviceCount();

for (int i = 0; i < numDevices; ++i) {
    deviceInfo = Pa_GetDeviceInfo(i);
    string name = deviceInfo->name;

    // All devices
    global_alldevicemap.insert({ i, name });

    // Input devices
    if (deviceInfo->maxInputChannels > 0)
        global_inputdevicemap.insert({ i, name });

    // Output devices
    if (deviceInfo->maxOutputChannels > 0)
        global_outputdevicemap.insert({ i, name });

    // Host-API name mapping
    const PaHostApiInfo* hostInfo = Pa_GetHostApiInfo(deviceInfo->hostApi);
    global_hostapimap.insert({ i, hostInfo->name });

    // API-specific device maps
    switch (hostInfo->type) {
        case paDirectSound:
            global_hostapimap_directsound.insert({ i, name });
            break;
        case paMME:
            global_hostapimap_mme.insert({ i, name });
            break;
        case paASIO:
            global_hostapimap_asio.insert({ i, name });
            break;
        // ... other API cases ...
        default:
            break;
    }
}
```

*(Excerpt from SPIAudioDevice::ScanAudioDevices) *

## Usage in Device Selection

Once the maps are built, `**MatchHostAPI()**` and `**MatchDevice()**` leverage them to locate the requested endpoint:

1. **MatchHostAPI** scans `global_hostapimap_hostapitype` for the best‐fit host-API label.
2. **MatchDevice** filters a host-API-specific map (e.g., `global_hostapimap_asio`) or the input/output map, then performs a loose string match on device names.
3. **SelectAudioInputDevice** and **SelectAudioOutputDevice** call `ScanAudioDevices()` before selection to ensure maps are current.

```cpp
// Populate all maps
SPIAudioDevice audioDev;
audioDev.ScanAudioDevices();

// Later, select an output device loosely matching "ASIO"
int deviceID = audioDev.ScanAudioDevices("loosely", spiaudiodeviceOUTPUT);
```

## Integration and Dependencies

- **Files**:
- `spiaudiodevice.h` / `spiaudiodevice.cpp` define the class and its methods.
- `spidevicesselect.cpp` demonstrates usage in console-based selection and reporting.
- **PortAudio**: Requires initialization (`Pa_Initialize()`) before scanning and termination (`Pa_Terminate()`) after use.
- **Host-API Support**: ASIO‐specific structures (`PaAsioStreamInfo`) are managed in selection methods, but scanning itself remains API-agnostic.
- **Configuration**: Built as a Visual Studio project against a bundled PortAudio 2021 tree; conditional macros (`PA_USE_ASIO`, `_WIN32`) enable platform-specific code.

---

This detailed documentation explains how `**ScanAudioDevices()**` orchestrates the discovery and classification of audio devices, forming the backbone for flexible, label-based device selection across multiple host APIs.