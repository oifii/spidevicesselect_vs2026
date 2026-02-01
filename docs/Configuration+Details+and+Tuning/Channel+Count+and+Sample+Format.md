# Configuration Details and Tuning – Channel Count and Sample Format

This section explains how the project uses compile-time constants from **defs.h** to configure PortAudio streams. You’ll learn how to adjust the default number of channels (mono vs. stereo) and the underlying sample format (e.g., 32-bit float or 16-bit integer).

## Compile-Time Constants in defs.h

defs.h defines key macros that determine the **sample format** and **channel count** for all audio streams. Changing these values requires recompiling the project.

- **PA_SAMPLE_TYPE**: PortAudio sample format macro
- **SAMPLE**: C/C++ data type for audio samples
- **SAMPLE_SILENCE**: Value representing silence
- **PRINTF_S_FORMAT**: printf format string for sample values
- **NUM_CHANNELS**: Number of audio channels per stream

```c
// Select sample format.
#if 1
  #define PA_SAMPLE_TYPE      paFloat32
  typedef float SAMPLE;
  #define SAMPLE_SILENCE      (0.0f)
  #define PRINTF_S_FORMAT     "%.8f"
#elif 1
  #define PA_SAMPLE_TYPE      paInt16
  typedef short SAMPLE;
  #define SAMPLE_SILENCE      (0)
  #define PRINTF_S_FORMAT     "%d"
#elif 0
  #define PA_SAMPLE_TYPE      paInt8
  typedef char SAMPLE;
  #define SAMPLE_SILENCE      (0)
  #define PRINTF_S_FORMAT     "%d"
#else
  #define PA_SAMPLE_TYPE      paUInt8
  typedef unsigned char SAMPLE;
  #define SAMPLE_SILENCE      (128)
  #define PRINTF_S_FORMAT     "%d"
#endif

#define SAMPLE_RATE           (44100)
#define FRAMES_PER_BUFFER     (2048)

// Mono vs. Stereo
#define NUM_CHANNELS          (1)
#define NUM_CHANNELS          (2)
```

## Sample Format Options 🎚️

| Macro | C Type | Silence Value | Printf Format |
| --- | --- | --- | --- |
| **paFloat32** | float | 0.0f | `"%.8f"` |
| **paInt16** | short | 0 | `"%d"` |
| **paInt8** | char | 0 | `"%d"` |
| **paUInt8** | unsigned char | 128 | `"%d"` |


Use **paFloat32** for high-precision audio. Choose **paInt16** or lower for compatibility or reduced memory.

## Channel Count Tuning

- **NUM_CHANNELS = 1** sets **mono** streams
- **NUM_CHANNELS = 2** sets **stereo** streams
- For surround setups, define **NUM_CHANNELS** to match the required layout (e.g., 6 for 5.1).

Adjusting **NUM_CHANNELS** in defs.h changes both input and output streams.

## Global Variables and Initialization

The macros in defs.h feed into global variables used at runtime. In **spiaudiodevice.h**, these are initialized as:

```cpp
#include "defs.h"
…
int global_numchannels = NUM_CHANNELS;
```

## Applying Configuration to PaStreamParameters

When selecting an audio device, the helper class assigns these globals to the PortAudio stream parameters:

```cpp
bool SPIAudioDevice::SelectAudioInputDevice(){
    …
    global_inputParameters.device       = deviceid;
    global_inputParameters.channelCount = global_numchannels;    // NUM_CHANNELS
    global_inputParameters.sampleFormat = PA_SAMPLE_TYPE;         // paFloat32, paInt16, etc.
    …
    return true;
}

bool SPIAudioDevice::SelectAudioOutputDevice(){
    …
    global_outputParameters.device       = deviceid;
    global_outputParameters.channelCount = global_numchannels;    // NUM_CHANNELS
    global_outputParameters.sampleFormat = PA_SAMPLE_TYPE;         // paFloat32, paInt16, etc.
    …
    return true;
}
```

## Practical Tuning Guide

- **Mono vs. Stereo**
- Mono reduces CPU and bandwidth; stereo is standard for music.
- **Sample Format Trade-Offs**
- **paFloat32**: best dynamic range, slightly higher CPU.
- **paInt16**: native support on many devices, lower memory footprint.
- **Custom Channel Layouts**
- For multichannel audio (e.g., 5.1), set **NUM_CHANNELS** accordingly and verify device capability.

## Recommendations

```card
{
    "title": "Recompile After Changes",
    "content": "After updating NUM_CHANNELS or PA_SAMPLE_TYPE, rebuild the project for changes to take effect."
}
```

- Verify your target device supports the chosen format and channel count.
- Use **PrintSupportedStandardSampleRates** to confirm available sample rates for the configured format and channels.
- Keep **FRAMES_PER_BUFFER** consistent with your latency and CPU usage requirements.

By tuning **NUM_CHANNELS** and **PA_SAMPLE_TYPE**, you control both the audio layout and precision of all streams opened by this utility. Proper configuration ensures compatibility with your hardware and optimizes performance.