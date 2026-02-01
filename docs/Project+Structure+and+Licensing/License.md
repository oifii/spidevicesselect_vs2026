# Project Structure and Licensing

## License Overview

This project is distributed under the **GNU General Public License version 3 (GPLv3)**. The GPLv3 ensures that the source remains free to use, modify, and redistribute, while protecting end users’ freedoms and requiring that derivative works also be licensed under GPLv3.

---

## 🛡️ License Artifacts

Below is a summary of the license-related files and headers in the repository:

| File | Description | License |
| --- | --- | --- |
| **LICENSE.txt** | Full text of the GPLv3 | GNU GPL v3 |
| **spidevicesselect.cpp** | Console utility source; includes GPLv3 header | GPL v3 (header in file) |
| **spiaudiodevice.cpp** | Helper class implementation; GPLv3 header | GPL v3 (header in file) |
| **spiaudiodevice.h** | Class interface; GPLv3 header | GPL v3 |


---

## GPLv3 Source Headers

Each primary source file begins with a GPLv3 notice, for example in `spidevicesselect.cpp`:

```cpp
/*
 * Copyright (c) 2012-2026 Stephane Poirier
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
```

Similarly, `spiaudiodevice.cpp` and its header `spiaudiodevice.h` open with an identical GPLv3 notice .

---

## Key GPLv3 Terms

- **Freedom to Use**: Run the program for any purpose.
- **Freedom to Study**: Access and inspect the source.
- **Freedom to Share**: Redistribute verbatim copies under the same license.
- **Freedom to Modify**: Create and distribute derivative works under GPLv3.
- **No Warranty**: Provided “as is” without any warranty of merchantability or fitness for a particular purpose.

---

## Compliance and Integration

To integrate this helper class into other applications, ensure your project’s license is **compatible with GPLv3**. Key points:

- Any combined work must be **licensed in its entirety under GPLv3**.
- You must **preserve all copyright notices** and the GPLv3 headers.
- Distribute a copy of `LICENSE.txt` alongside compiled binaries.
- Acknowledge that **no warranty** is provided.

> **Note:** If your project cannot adopt GPLv3, consider seeking alternatives or obtaining explicit permission from the copyright holder.

---

## Redistribution Checklist

- [ ] Include the full `LICENSE.txt` in your distribution.
- [ ] Retain the GPLv3 header in every source file.
- [ ] Re-license any derivative works under GPLv3 (or later).
- [ ] Clearly document any modifications and dates.
- [ ] Ensure recipients can access or receive the corresponding source code.

---

> “The GNU General Public License does not permit incorporating your program into proprietary programs. If you want to allow linking from proprietary code, consider using the GNU Lesser General Public License instead.” — GPLv3 FAQ