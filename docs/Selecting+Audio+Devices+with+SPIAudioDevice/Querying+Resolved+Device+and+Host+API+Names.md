# Selecting Audio Devices with SPIAudioDevice – Querying Resolved Device and Host API Names

This section describes how SPIAudioDevice exposes human-readable **device** and **host API** labels for a given PortAudio device ID. These helpers let you confirm which physical or logical audio device is selected before opening a stream or displaying information to the user.

## SPIAudioDevice Internal Maps

SPIAudioDevice maintains several maps to store device and host API names indexed by PortAudio device IDs . The two key maps for name resolution are:

- **global_alldevicemap**

Stores every discovered device’s name (string) keyed by its integer device ID.

- **global_hostapimap**

Holds each device’s host API name (string) keyed by the same device ID.

### Map Descriptions

| Map Name | Purpose |
| --- | --- |
| global_alldevicemap | Device ID → Device name 🎧 |
| global_hostapimap | Device ID → Host API name 🔌 |


## GetDeviceName Method 🎧

Retrieves the human-readable **device name** for a given PortAudio device ID.

```cpp
string SPIAudioDevice::GetDeviceName(int deviceid);
```

- **Input:**
- `deviceid` – PortAudio device ID (≥ 0).
- **Returns:**
- The device name if found; otherwise, an empty string.

#### Implementation

```cpp
string SPIAudioDevice::GetDeviceName(int deviceid){
    string devicename = "";
    if ((!global_alldevicemap.empty()) && (deviceid > -1)) {
        for (auto it = global_alldevicemap.begin(); it != global_alldevicemap.end(); ++it) {
            if (it->first == deviceid) {
                devicename = it->second;
                break;
            }
        }
    }
    return devicename;
}
```

This method iterates through **global_alldevicemap** to find a matching key and returns its associated name .

## GetHostAPIName Method 🔌

Fetches the human-readable **host API name** for a given PortAudio device ID.

```cpp
string SPIAudioDevice::GetHostAPIName(int deviceid);
```

- **Input:**
- `deviceid` – PortAudio device ID (≥ 0).
- **Returns:**
- The host API name if found; otherwise, an empty string.

#### Implementation

```cpp
string SPIAudioDevice::GetHostAPIName(int deviceid){
    string hostapiname = "";
    if ((!global_hostapimap.empty()) && (deviceid > -1)) {
        for (auto it = global_hostapimap.begin(); it != global_hostapimap.end(); ++it) {
            if (it->first == deviceid) {
                hostapiname = it->second;
                break;
            }
        }
    }
    return hostapiname;
}
```

This method searches **global_hostapimap** by key and returns the corresponding API name .

## How It Works

1. **Populate Maps**

Call `ScanAudioDevices()` to fill `global_alldevicemap` and `global_hostapimap`.

1. **Select a Device**

Use `MatchDevice()` or `SelectAudioInputDevice()`/`SelectAudioOutputDevice()` to choose a device ID.

1. **Query Names**
2. `GetDeviceName(deviceid)`
3. `GetHostAPIName(deviceid)`

## Usage Example

```cpp
#include "spiaudiodevice.h"
#include <iostream>

int main() {
    SPIAudioDevice audioSel;
    audioSel.ScanAudioDevices();       // Populate maps
    int deviceId = audioSel.MatchDevice("loosely", spiaudiodeviceOUTPUT);
    std::cout << "Device Name: " << audioSel.GetDeviceName(deviceId) << std::endl;
    std::cout << "Host API:    " << audioSel.GetHostAPIName(deviceId) << std::endl;
    return 0;
}
```

## Dependencies and Initialization

- **PortAudio Initialization**

Ensure `Pa_Initialize()` is called before `ScanAudioDevices()`.

- **Map Population**

`ScanAudioDevices()` builds all internal maps, including **global_alldevicemap** and **global_hostapimap**, by querying PortAudio’s device list.

## Key Takeaways

- Always **scan devices** before querying names.
- Both methods return an empty string on failure, allowing you to detect missing entries.
- Use these helpers to display friendly labels or debug chosen devices at runtime.

```card
{
    "title": "Pro Tip",
    "content": "Call ScanAudioDevices before invoking GetDeviceName or GetHostAPIName."
}
```