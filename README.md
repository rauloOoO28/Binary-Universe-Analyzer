# Binary Universe Analyzer

![C](https://img.shields.io/badge/C-Bitwise%20generation-informational)
![Python](https://img.shields.io/badge/Python-Data%20analysis-informational)
![NumPy](https://img.shields.io/badge/NumPy-Numerical%20computing-informational)
![Matplotlib](https://img.shields.io/badge/Matplotlib-Visualization-informational)
![License](https://img.shields.io/badge/License-MIT-success)

A hybrid **C and Python** project for generating binary-string universes and analyzing bit-density statistics.

The C module exhaustively generates the strings in \(\Sigma^n\), where \(\Sigma = \{0,1\}\), using bitwise operations. The Python module calculates analytical statistics and visualizes density, logarithmic density, mean, and variance across values of `n`.

## Preview

![Density heatmap](assets/density-heatmap.png)

<p align="center">
  <img src="assets/mean-density.png" width="48%" alt="Mean density chart">
  <img src="assets/density-variance.png" width="48%" alt="Density variance chart">
</p>

## Why this project matters

This project demonstrates:

- Algorithm design and complexity analysis.
- Bit manipulation in C.
- Dynamic memory management and file handling.
- Exponential-growth awareness.
- Analytical optimization to avoid processing multi-gigabyte files.
- Numerical computing with NumPy.
- Data visualization with Matplotlib.
- Integration of low-level generation and high-level analysis.

## Core features

### C generator

- Manual mode for a specific value of `n`.
- Automatic mode from `n = 0` to a selected upper bound.
- Generation of all \(2^n\) fixed-width binary strings.
- Bitwise construction using right shifts and bit masks.
- Text-file output in set notation.
- Time complexity of approximately \(O(n \cdot 2^n)\).

### Python analyzer

- Analytical calculation without reading the complete generated universe.
- Bit-density analysis by position.
- Natural-logarithm density heatmap.
- Mean and variance calculation.
- Visualization for values from `n = 0` to `n = 33`.

## Project structure

```text
binary-universe-analyzer/
├── assets/
│   ├── density-heatmap.png
│   ├── density-variance.png
│   ├── log-density-heatmap.png
│   └── mean-density.png
├── docs/
│   └── README.md
├── src/
│   ├── creacionDeCadenas.c
│   └── graficador.py
├── .gitignore
├── LICENSE
├── README.md
└── requirements.txt
```

## Requirements

### C

- GCC or MinGW.
- A terminal.
- Sufficient free disk space for generated files.

### Python

- Python 3.
- NumPy.
- Matplotlib.

Install the Python dependencies:

```bash
python -m pip install -r requirements.txt
```

## Build and run

### Compile the C generator

From the repository root:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic src/creacionDeCadenas.c -o binary-universe
```

On Windows with MinGW, the output may be:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic src/creacionDeCadenas.c -o binary-universe.exe
```

Run it on Windows:

```bash
binary-universe.exe
```

Run it on Linux or macOS:

```bash
./binary-universe
```

> The current source combines `system("cls")`, which is Windows-specific, with `unistd.h` and `sleep()`, which are POSIX-oriented. A portability refactor is listed in the roadmap.

### Run the Python analyzer

```bash
python src/graficador.py
```

## Example

For `n = 3`, the C module generates:

```text
Σ³ = {000, 001, 010, 011, 100, 101, 110, 111}
```

The number of strings is:

```text
2³ = 8
```

For a general `n`, the generation algorithm iterates over every integer from `0` to `2^n - 1` and extracts each bit using:

```c
(i >> j) & 1
```

## Scalability

The universe grows exponentially:

| n | Number of strings | Approximate manual text-file size |
|---:|------------------:|----------------------------------:|
| 10 | 1,024          | 12 KB     |
| 20 | 1,048,576      | 23 MB     |
| 24 | 16,777,216     | 436 MB    |
| 25 | 33,554,432     | 906 MB    |
| 26 | 67,108,864     | 1.88 GB   |
| 27 | 134,217,728    | 3.89 GB   |
| 28 | 268,435,456    | 8.05 GB   |
| 30 | 1,073,741,824  | 34.36 GB  |
| 33 | 8,589,934,592  | 300.65 GB |

> [!WARNING]
> Resource consumption increases exponentially with `n`. The disk estimates above are based on the current manual output format, where each entry contains `n` binary characters plus an approximate two-character separator. Headers and line breaks add a small additional amount. The automatic mode, which stores every universe from `0` through `n`, may require almost twice the space of generating only the largest value.

### Disk-space considerations

The approximate size of a manual output file is:

```text
disk_bytes(n) ≈ 2^n × (n + 2)
```

For the automatic mode from `0` through `n`, the total size is approximately:

```text
automatic_disk_bytes(n) ≈ (n + 1) × 2^(n + 1)
```

Examples:

- Manual `n = 25`: approximately **906 MB**.
- Automatic `0...25`: approximately **1.74 GB**.
- Manual `n = 30`: approximately **34.36 GB**.
- Manual `n = 33`: approximately **300.65 GB**.
- Automatic `0...33`: approximately **584 GB**.

Always keep additional free space available for filesystem overhead, temporary files, and interrupted runs.

### RAM considerations

The C generator writes each string directly to disk and keeps only one small string buffer in memory. Its working-memory requirement is therefore approximately \(O(n)\), excluding operating-system file buffers. Generating a very large file does **not** require holding the entire file in RAM.

However, loading a generated file for direct analysis requires at least as much RAM as the raw file size. Parsing it into a Python list of individual strings can require roughly **two to four times more memory**, or even more, because every Python string and list entry has additional object overhead.

Examples:

- A raw `n = 25` file is about **906 MB**, but a parsed in-memory representation may require several gigabytes.
- A raw `n = 27` file is about **3.89 GB** and may approach or exceed the usable memory of a computer with 16 GB of RAM after parsing.
- A raw `n = 30` file is already too large for direct in-memory analysis on most personal computers.

The current Python analyzer avoids this problem by using analytical formulas. Its numerical arrays require only a small amount of memory; plotting may use tens of megabytes depending on the operating system and graphical backend.

### Practical computational limit

There is no single universal limit because execution time depends on CPU speed, disk performance, filesystem, output format, and available storage. For a typical personal computer with **8–16 GB of RAM and an SSD**, the following ranges are reasonable:

- `n <= 20`: generally easy to generate and store.
- `n = 21...24`: manageable, but generation time and file size become noticeable.
- `n = 25`: a practical upper limit for routine exhaustive text generation on many common computers.
- `n = 26...28`: technically possible with sufficient disk space, but increasingly slow and inconvenient.
- `n >= 30`: generally impractical for exhaustive text generation on a common personal computer.
- `n = 33`: not recommended; it requires processing more than **8.5 billion strings** and produces approximately **300 GB** in manual mode.
- `n = 34`: would produce more than **17 billion strings** and approximately **618 GB** in manual mode.

The use of a signed 64-bit counter also imposes an implementation-level numerical limit below `n = 63`, but the practical time and storage limits are reached much earlier.

Generated text files can quickly become extremely large. For this reason:

- Generated `.txt` files are excluded by `.gitignore`.
- The compiled `.exe` is not stored in the repository.
- The Python module uses analytical formulas rather than loading the full dataset.
- Large values of `n` should be analyzed analytically instead of being exhaustively stored.

## Current implementation note

The C program generates **fixed-width strings with leading zeroes**, while the current Python module analyzes **unpadded binary representations** for integers from `1` to `2^n - 1`.

Unifying both representations is the main technical improvement planned for the next version. Documenting this distinction avoids presenting two different models as the same dataset.

## Roadmap

- Align the C and Python representations.
- Add command-line arguments.
- Save plots automatically from the Python program.
- Replace platform-specific screen-clearing and sleep calls.
- Estimate output size before starting generation.
- Add automated tests.
- Validate non-numeric user input.
- Add a safe cancellation mechanism.
- Benchmark file-writing performance.
- Provide a small sample dataset for demonstration.

## Academic context

Developed as a project for **Theory of Computation** at the Escuela Superior de Cómputo, Instituto Politécnico Nacional.

## Author

**Raúl Enrique Martínez Cruz**

## License

Distributed under the MIT License. See [LICENSE](LICENSE).
