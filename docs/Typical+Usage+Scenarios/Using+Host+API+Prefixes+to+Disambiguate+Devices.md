## Typical Usage Scenarios – Using Host API Prefixes to Disambiguate Devices

When multiple audio interfaces share the same device name across different host APIs (e.g. MME vs. WASAPI), you can **explicitly specify** the host API in the device string. The format is:

```plaintext
"HostAPI:DeviceName"
```

This ensures the utility selects the **correct** driver instance and avoids ambiguity when scanning or matching devices.

---

### When to Use Host API Prefixes

- You have a microphone listed under both **MME** and **WASAPI**.
- Your system presents multiple virtual cables with identical names across **DirectSound** and **WDM-KS**.
- You need to force selection of an **ASIO** device, even if a WDM-KS version exists.

In these cases, prepending the host API label unambiguously targets the desired pipeline.

---

### Supported Host API Prefixes 📋

Use any of these labels (case-insensitive) before the colon. Internally, the prefix is uppercased and matched against the `global_hostapimap_hostapitype` map .

| Prefix Label | Mapped Enum (`PaHostApiType`) |
| --- | --- |
| Windows DirectSound | paDirectSound |
| DirectSound | paDirectSound |
| Windows MME | paMME |
| MME | paMME |
| ASIO | paASIO |
| Windows WDM-KS | paWDMKS |
| WDM-KS | paWDMKS |
| Windows WASAPI | paWASAPI |
| WASAPI | paWASAPI |
| JACK | paJACK |


---

### How the Code Parses and Matches

1. **Prefix Detection**

The utility searches for a colon (`:`) in the device string .

1. **Splitting**

```cpp
   auto found = global_audiooutputdevicename.find(':');
   if (found != std::string::npos) {
       global_audiooutputhostapi   = global_audiooutputdevicename.substr(0, found);
       global_audiooutputdevicename = global_audiooutputdevicename.substr(found + 1);
   }
```

1. **Host API Matching**
2. Uppercases the extracted prefix.
3. Iterates `global_hostapimap_hostapitype` to find a substring match .
4. Returns the corresponding `PaHostApiType` (`hostapitype`).
5. **Device Matching**
6. If `hostapitype` ≥ 0, selects from the API-specific map (`global_hostapimap_asio`, `global_hostapimap_wasapi`, etc.).
7. Otherwise uses the generic input/output device maps.
8. Matches the **device name** loosely (substring match) to obtain the device ID .

---

### Code Example

```cpp
#include "spiaudiodevice.h"

int main(int argc, char* argv[]) {
    SPIAudioDevice audioDev;

    // Example: force WASAPI Microphone selection
    audioDev.global_audioinputdevicename = "WASAPI:Microphone (Realtek(R) Audio)";
    audioDev.SelectAudioInputDevice();

    // Example: select ASIO output
    audioDev.global_audiooutputdevicename = "ASIO:Focusrite USB ASIO";
    audioDev.SelectAudioOutputDevice();

    // Now audioDev.global_inputParameters.device holds the chosen ID
    // and the stream can be opened with PortAudio APIs.

    return 0;
}
```

---

### Tips & Best Practices

- **Quote** your device string if it contains spaces or parentheses:

```bash
  myapp.exe "MME:Line (USB AUDIO CODEC)"
```

- If you omit the prefix, the utility matches across **all** host APIs, which may select an unintended device.
- Use the **exact** label from the supported list to avoid unexpected matches.

---

```card
{
    "title": "Key Takeaway",
    "content": "Prefix your device names with the desired Host API to guarantee correct audio device selection."
}
```