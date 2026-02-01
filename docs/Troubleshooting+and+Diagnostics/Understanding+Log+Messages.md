# Troubleshooting and Diagnostics – Understanding Log Messages

When `**m_pFILE**` is non-null, `SPIAudioDevice` emits detailed diagnostics to help you understand how host APIs and devices are discovered and selected. These messages guide you in tuning your device name strings and host API prefixes until the expected device is reliably chosen.

## Enabling Diagnostic Logging

To start capturing diagnostics, you must:

- Enable detailed reporting.
- Assign a valid `FILE*` to `**m_pFILE**` before invoking any device‐scanning or selection methods.

```cpp
#include "spiaudiodevice.h"
#include <cstdio>

int main() {
    SPIAudioDevice device;
    // Turn on detailed reports
    device.bdetailedreport = true;
    // Open a log file for diagnostics
    device.m_pFILE = std::fopen("audio_diag.log", "w");
    if (!device.m_pFILE) {
        std::perror("Failed to open log file");
        return 1;
    }
    // Perform scanning and selection
    device.ScanAudioDevices();
    device.SelectAudioInputDevice();
    device.SelectAudioOutputDevice();

    std::fclose(device.m_pFILE);
    return 0;
}
```

This snippet initializes `**m_pFILE**` to a writable file stream, enabling all subsequent `**fprintf**` diagnostics .

## Log Message Categories

- **✅ Success**: Confirmation that a matching host API or device was found and retained.
- **⚠️ Warning**: Indicates potential ambiguities or missing specifications.
- **❌ Error**: An unsupported or mismatched host API was inferred, defaulting to **MME** for safety.

## ✅ Success Messages

These messages confirm the first valid match:

| Message Pattern | Purpose |
| --- | --- |
| `success: matching hostapi found, %s will be retained.` | A host API string matched uniquely |
| `success: matching device found, %s will be retained.` | A device name string matched uniquely |


Example:

```text
success: matching hostapi found, WASAPI will be retained.
success: matching device found, Speakers (USB AUDIO CODEC) will be retained.
```

## ⚠️ Warning Messages

Warnings alert you to ambiguous or missing specifications:

| Message Pattern | Scenario |
| --- | --- |
| `warning: more than one matching hostapi found, %s will be ignored.` | Multiple host APIs match; only the first is used |
| `warning: more than one matching device found, %s will be ignored.` | Multiple devices match; only the first is used |
| `warning: input hostapi not specified.` | No host API prefix provided for input, might lead to wrong match |
| `warning: output hostapi not specified.` | No host API prefix provided for output, might lead to wrong match |


Tips:

- **Specify** the host API (e.g., `WASAPI:Headphones`) to reduce ambiguity.
- **Review** multiple‐match warnings to adjust your search strings.

## ❌ Error Messages

Errors indicate serious mismatches:

| Message Pattern | Action Taken |
| --- | --- |
| `error: hostapitype not specified correctly in SPIAudioDevice::MatchDevice(), will choose MME` | An unrecognized host API forced fallback to MME |


If you see this error, verify that your host API prefix (before the colon) is one of the supported values:

```plaintext
"DirectSound", "MME", "ASIO", "WDM-KS", "WASAPI", "JACK", ...
```

## Using Log Messages to Tune Configuration

1. **Read** the log to identify **which** host API and device substrings matched.
2. **Refine** `global_audio[input|output]hostapi` and `global_audio[input|output]devicename`:
3. Add or remove characters to avoid partial matches.
4. Use the colon syntax (`"WASAPI:Speakers"`) to force a specific API.
5. **Rerun** the utility and monitor new log entries until only the desired match appears.

```text
hostapi has been specified:
global_audiooutputhostapi=WASAPI, global_audiooutputdevicename=Speakers
success: matching hostapi found, WASAPI will be retained.
success: matching device found, Speakers (USB AUDIO CODEC) will be retained.
```

## Sample Log Output

```text
-------------------- Scanning Devices --------------------
hostapi has been specified:
global_audioinputhostapi=MME, global_audioinputdevicename=Microphone
warning: more than one matching device found, Microphone (Realtek Audio) will be ignored.
success: matching device found, Microphone (USB Condenser Microphone) will be retained.
error: hostapitype not specified correctly in SPIAudioDevice::MatchDevice(), will choose MME

------------------ Device Selection Complete ------------------
```

```card
{
    "title": "Best Practice",
    "content": "Always include the host API prefix (e.g., \"ASIO:Realtek ASIO\") to ensure accurate device selection."
}
```

By interpreting these log messages, you can iteratively adjust your configuration until the utility consistently selects the correct audio device.