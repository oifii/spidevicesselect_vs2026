# Running the spidevicesselect Console Tool – Using Command-Line Arguments to Select a Device

This section explains how to invoke the `spidevicesselect` console utility with a command-line argument to preselect an audio device. You can pass a device name or a host API–qualified name as the first parameter; the tool will then attempt to match it against the available PortAudio devices.

## Specifying a Device Name

If you invoke the tool with one argument, the program reads it into the **selecteddevicename** string and uses that to guide device selection .

```bash
# Select by simple device name
spidevicesselect "E-MU ASIO"

# Select by host API and device name
spidevicesselect "ASIO:E-MU ASIO"
```

- **Simple name**: Matches a device whose PortAudio name exactly equals the string you provide.
- **Host API prefix**: Before the first colon (`:`), specify the PortAudio host API. After the colon, give the device name. This ensures the selector prefers that API when multiple devices share similar names .

## How the Argument Is Processed

1. **Reading the argument**

The `main` function checks `argc > 1` and assigns:

```cpp
   string selecteddevicename = "";
   if(argc > 1){
       selecteddevicename = argv[1];
   }
```

1. **Building the global device map**

During device enumeration, every device name and its ID are inserted into a `std::map<string,int> global_devicemap`:

```cpp
   global_devicemap.insert(pair<string,int>(deviceInfo->name, i));
```

1. **Direct lookup**

After listing all devices, the tool attempts a direct map lookup:

```cpp
   auto it = global_devicemap.find(selecteddevicename);
   if(it != global_devicemap.end()){
       printf("%s maps to %d\n", selecteddevicename.c_str(), it->second);
       deviceID = it->second;
   }
```

If found, the exact device ID is printed and used; no further matching is performed .

## Host API–Qualified Matching

When the direct lookup fails, the utility falls back to a more flexible matching strategy implemented in `SPIAudioDevice`, which handles:

- **Host API extraction**

If the argument contains a colon, it splits into `global_audiooutputhostapi` and `global_audiooutputdevicename`:

```cpp
  auto pos = global_audiooutputdevicename.find(":");
  if(pos != string::npos){
      global_audiooutputhostapi   = global_audiooutputdevicename.substr(0, pos);
      global_audiooutputdevicename = global_audiooutputdevicename.substr(pos + 1);
  }
```

- **Loose matching**
- Call `MatchHostAPI("loosely", spiaudiodeviceOUTPUT)` to pick the desired host API.
- Call `MatchDevice("loosely", spiaudiodeviceOUTPUT, hostapitype)` to find the best device match.

This two-stage matching allows case-insensitive, substring matching of both API names (e.g., “asio”) and device names (e.g., “emu asio”).

## Usage Examples

| Invocation | Effect |
| --- | --- |
| `spidevicesselect` | Lists all devices; no preselection |
| `spidevicesselect "Microphone (Realtek(R) Audio)"` | Directly selects the Realtek microphone if exact name matches |
| `spidevicesselect "WASAPI:Speakers (USB AUDIO CODEC)"` | Prefers WASAPI host API when matching the USB AUDIO CODEC speaker |


## Summary

- Pass **one argument** to preselect a device by name.
- You may use a **host API prefix** (`API:DeviceName`) to disambiguate when multiple devices share a name.
- The tool first tries a **direct lookup** in its global map; if that fails, it falls back to **flexible matching** in `SPIAudioDevice`  .