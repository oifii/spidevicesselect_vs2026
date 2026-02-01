# Running the spidevicesselect Console Tool – Interpreting `device.txt`

The **spidevicesselect** console utility scans and logs all audio I/O devices visible to PortAudio on your system. By default, it writes a detailed snapshot to a file named `device.txt`. This log is invaluable for troubleshooting device matching, verifying host-API availability (MME, DirectSound, WASAPI, WDM-KS, ASIO), and examining device capabilities.

## Launching the Tool

Simply run the executable from a terminal. Optionally, pass a device name to select it immediately:

```bash
# Scan and log devices only
spidevicesselect

# Scan, log, and attempt to select "Realtek ASIO"
spidevicesselect "Realtek ASIO"
```

On startup, the tool:

1. Opens `device.txt` for writing.
2. Initializes PortAudio.
3. Enumerates all devices.
4. Marks defaults and prints detailed info to `device.txt`.

## Overview of `device.txt`

Each device entry in `device.txt` follows this structure:

- **Header line** marking the device index.
- **Default-device markers** for global and per-API defaults.
- **Identification line** (`id=…, hostapi=…, devicename=…`).
- **Channel counts** (`Max inputs`, `Max outputs`).
- **Latency values** (default low/high, input/output).
- **ASIO buffer info** (for ASIO-type devices).

### Sample Entry

```text
--------------------------------------- device #26
[ Default ASIO Input, Default ASIO Output ]
id=26, hostapi=ASIO, devicename=Realtek ASIO
Max inputs = 2, Max outputs = 2
Default low input latency   =   0.0232
Default low output latency  =   0.0232
Default high input latency  =   0.0232
Default high output latency =   0.0232
ASIO minimum buffer size    = 1024
ASIO maximum buffer size    = 1024
ASIO preferred buffer size  = 1024
ASIO buffer granularity     = 0
```

---

## Breakdown of Fields

| Section | Description |
| --- | --- |
| `**device #<index>**` | Identifies the device by its PortAudio index (zero-based). |
| **Default markers** 🎯 | Bracketed list indicating: <br/>- **Global default** input/output devices. <br/>- **Host-API default** input/output (e.g. `[ Default WASAPI Output ]`). |
| `**id=**` | Numeric device index (same as header). |
| `**hostapi=**` | Host API name (`MME`, `DirectSound`, `WASAPI`, `WDM-KS`, `ASIO`, etc.). |
| `**devicename=**` | Human-readable device name as returned by PortAudio. |
| `**Max inputs**`** / **`**outputs**` | Number of channels supported for capture/playback. |
| **Default low/high latencies** | Latency in seconds for **low** and **high** stream modes, for both input and output. |
| **ASIO buffer sizes** ⚙️ | For ASIO devices only: <br/>– **Minimum** buffer frames. <br/>– **Maximum** buffer frames. <br/>– **Preferred** buffer frames. <br/>– **Granularity** (step). |


---

## Default-Device Markers

- `[ Default Input ]` / `[ Default Output ]`

Marks the system-wide default for input or output.

- `[ Default <API> Input ]` / `[ Default <API> Output ]`

Marks the default device *for that specific host API*.

e.g. `[ Default Windows WASAPI Output ]`.

These markers help you confirm which device PortAudio will open by default under each API context.

---

## Latency Values

Latency values guide buffer sizing and performance tuning:

- **Low latency** mode is optimized for minimal delay.
- **High latency** may increase stability on congested systems.

Values are printed in seconds with four decimal places:

```text
Default low input latency   =   0.0900
Default high output latency =   0.1800
```

---

## ASIO-Specific Information

For devices under the **ASIO** host API, additional parameters appear:

| Parameter | Meaning |
| --- | --- |
| **Minimum buffer size** | Smallest allowed buffer (frames). |
| **Maximum buffer size** | Largest allowed buffer (frames). |
| **Preferred buffer size** | ASIO driver’s recommended size. |
| **Buffer granularity** | Step size for buffer adjustments; “power of 2” if driver enforces powers of two. |


This info is essential when targeting professional audio hardware.

---

## When to Use `device.txt`

- **Troubleshooting**

If a requested device name isn’t found, inspect `device.txt` to verify exact spelling, host API, and index.

- **Multi-API Verification**

Confirm availability across MME, DirectSound, WASAPI, WDM-KS, JACK, ALSA (on other platforms), and ASIO.

- **Performance Tuning**

Choose the API and device with the lowest latency that meets your channel-count needs.

---

## Best Practices

- **Always** open `device.txt` before running selection routines to capture the current state.
- Use the **exact** `hostapi:devicename` format (e.g. `ASIO:Realtek ASIO`) when specifying device arguments.
- Re-generate `device.txt` after hardware or driver changes to keep logs up to date.

---

By understanding each section of `device.txt`, you can quickly diagnose configuration issues, select the best audio device for your application, and optimize performance across multiple host APIs.