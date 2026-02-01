# Selecting Audio Devices with SPIAudioDevice

This section explains how `SPIAudioDevice` configures PortAudio input and output devices. It covers typical usage flows of `SelectAudioInputDevice` and `SelectAudioOutputDevice`, showing how device maps are populated, how device IDs are chosen, and how `PaStreamParameters` are set up for stream opening.

## 🎯 SelectAudioInputDevice

The `SelectAudioInputDevice` method prepares the **input** stream parameters. It performs the following steps:

- Ensures the input device map is populated by calling `ScanAudioDevices()` if empty.
- Finds a candidate device ID via `ScanAudioDevices("loosely", spiaudiodeviceINPUT)`.
- Falls back to PortAudio’s default input device if no match is found (`paNoDevice`).
- Populates `global_inputParameters`:
- `device`: matched or default device ID
- `channelCount`: `global_numchannels` (from `NUM_CHANNELS`)
- `sampleFormat`: `PA_SAMPLE_TYPE`
- `suggestedLatency`: device’s `defaultLowInputLatency`
- On Windows with ASIO enabled, initializes `PaAsioStreamInfo` (`global_asioInputInfo`), setting:
- `flags = paAsioUseChannelSelectors`
- `channelSelectors = global_inputAudioChannelSelectors`
- Assigns `hostApiSpecificStreamInfo` based on host API:
- `NULL` for default or WDM-KS devices
- Pointer to `global_asioInputInfo` for ASIO devices
- Returns `true` on success, leaving `global_inputParameters` ready for `Pa_OpenStream`.

```cpp
bool SPIAudioDevice::SelectAudioInputDevice() {
    if (global_inputdevicemap.empty()) {
        ScanAudioDevices();
    }
    int deviceid = ScanAudioDevices("loosely", spiaudiodeviceINPUT);
    if (deviceid == paNoDevice)
        deviceid = Pa_GetDefaultInputDevice();
    // Populate parameters
    global_inputParameters.device = deviceid;
    global_inputParameters.channelCount = global_numchannels;
    global_inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    global_inputParameters.suggestedLatency =
        Pa_GetDeviceInfo(deviceid)->defaultLowInputLatency;
#if (defined(_WIN64) || defined(WIN32))
  #if PA_USE_ASIO
    // ASIO-specific config
    global_asioInputInfo.size = sizeof(PaAsioStreamInfo);
    global_asioInputInfo.hostApiType = paASIO;
    global_asioInputInfo.version = 1;
    global_asioInputInfo.flags = paAsioUseChannelSelectors;
    global_asioInputInfo.channelSelectors = global_inputAudioChannelSelectors;
  #endif
#endif
    // Assign hostApiSpecificStreamInfo
    if (deviceid == Pa_GetDefaultInputDevice()) {
        global_inputParameters.hostApiSpecificStreamInfo = NULL;
    }
#if (defined(_WIN64) || defined(WIN32))
  #if PA_USE_ASIO
    else if (Pa_GetHostApiInfo(
               Pa_GetDeviceInfo(deviceid)->hostApi)->type == paASIO) {
        global_inputParameters.hostApiSpecificStreamInfo = &global_asioInputInfo;
    }
  #endif
    else if (Pa_GetHostApiInfo(
               Pa_GetDeviceInfo(deviceid)->hostApi)->type == paWDMKS) {
        global_inputParameters.hostApiSpecificStreamInfo = NULL;
    }
#endif
    return true;
}
```

Excerpt from implementation .

## 🎯 SelectAudioOutputDevice

The `SelectAudioOutputDevice` method prepares the **output** stream parameters. It follows a similar flow:

- Populates the output device map if empty (`ScanAudioDevices()`).
- Uses `ScanAudioDevices("loosely", spiaudiodeviceOUTPUT)` to match a device.
- If no match (`paNoDevice`), returns `false` (no fallback).
- Fills `global_outputParameters`:
- `device`: matched device ID
- `channelCount`: `global_numchannels`
- `sampleFormat`: `PA_SAMPLE_TYPE`
- `suggestedLatency`: device’s `defaultLowOutputLatency`
- On Windows with ASIO:
- Configures `global_asioOutputInfo` similarly to input.
- Sets `hostApiSpecificStreamInfo`:
- `NULL` for default or WDM-KS devices
- Pointer to `global_asioOutputInfo` for ASIO devices
- Returns `true` on success; after this, `global_outputParameters` is stream-ready.

```cpp
bool SPIAudioDevice::SelectAudioOutputDevice() {
    if (global_outputdevicemap.empty()) {
        ScanAudioDevices();
    }
    int deviceid = ScanAudioDevices("loosely", spiaudiodeviceOUTPUT);
    if (deviceid == paNoDevice) {
        return false;
    }
    // Populate parameters
    global_outputParameters.device = deviceid;
    global_outputParameters.channelCount = global_numchannels;
    global_outputParameters.sampleFormat = PA_SAMPLE_TYPE;
    global_outputParameters.suggestedLatency =
        Pa_GetDeviceInfo(deviceid)->defaultLowOutputLatency;
#if (defined(_WIN64) || defined(WIN32))
  #if PA_USE_ASIO
    global_asioOutputInfo.size = sizeof(PaAsioStreamInfo);
    global_asioOutputInfo.hostApiType = paASIO;
    global_asioOutputInfo.version = 1;
    global_asioOutputInfo.flags = paAsioUseChannelSelectors;
    global_asioOutputInfo.channelSelectors = global_outputAudioChannelSelectors;
  #endif
#endif
    if (deviceid == Pa_GetDefaultOutputDevice()) {
        global_outputParameters.hostApiSpecificStreamInfo = NULL;
    }
#if (defined(_WIN64) || defined(WIN32))
  #if PA_USE_ASIO
    else if (Pa_GetHostApiInfo(
               Pa_GetDeviceInfo(deviceid)->hostApi)->type == paASIO) {
        global_outputParameters.hostApiSpecificStreamInfo = &global_asioOutputInfo;
    }
  #endif
    else if (Pa_GetHostApiInfo(
               Pa_GetDeviceInfo(deviceid)->hostApi)->type == paWDMKS) {
        global_outputParameters.hostApiSpecificStreamInfo = NULL;
    }
#endif
    return true;
}
```

Excerpt from implementation .

## Comparison at a Glance

| Feature | Input Device | Output Device |
| --- | --- | --- |
| Device map check | `global_inputdevicemap.empty()` | `global_outputdevicemap.empty()` |
| Device matching call | `ScanAudioDevices("loosely", INPUT)` | `ScanAudioDevices("loosely", OUTPUT)` |
| Fallback on no match | Default input via `Pa_GetDefaultInputDevice()` | None; returns `false` |
| Suggested latency source | `defaultLowInputLatency` | `defaultLowOutputLatency` |
| ASIO stream info variable | `global_asioInputInfo` | `global_asioOutputInfo` |
| Return value on success | `true` | `true` |
| Return value on failure | *never* (`true` always) | `false` |


## Selection Flowchart

```mermaid
flowchart TD
  A[Start Selection] --> B{Device map empty?}
  B -->|Yes| C[ScanAudioDevices()]
  B -->|No| D[Proceed]
  C --> D
  D --> E[Match device ID]
  E --> F{Match result}
  F -->|paNoDevice on Input| G[Use default input]
  F -->|paNoDevice on Output| H[Return false]
  F -->|Found| I[Use matched ID]
  G --> J[Populate StreamParameters]
  I --> J
  J --> K{Windows + ASIO?}
  K -->|Yes| L[Configure PaAsioStreamInfo]
  K -->|No| M[Set hostApiSpecificStreamInfo=NULL]
  L --> M
  M --> N[Return true]
  H --> O[Selection Failed]
```

This diagram outlines the logical steps for both input and output selection, highlighting special cases and ASIO configuration.

---

By invoking these methods, the application ensures that the chosen audio device parameters are correctly set for subsequent calls to `Pa_OpenStream`, streamlining device selection in a cross-platform, multi-API environment.