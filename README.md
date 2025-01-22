# fish-schooling

[![test](https://github.com/YTokumaru/fish-schooling/actions/workflows/test.yaml/badge.svg)](https://github.com/YTokumaru/fish-schooling/actions/workflows/test.yaml)
[![codecov](https://codecov.io/gh/YTokumaru/fish-schooling/graph/badge.svg?token=D9IDGI880Y)](https://codecov.io/gh/YTokumaru/fish-schooling)

A fish schooling simulator

## Perquisites

- cmake
- C++ compiler
- OpenMP

## Build

```bash
mkdir build
cd build
cmake ..
cmake --build .
cmake --install --prefix <dir>
```

## Running simulation

Go to the installed directory and you should find:

- fish_schooling: The simulation binary
- config.yaml: The simulation configuration file
- create_movie.sh: Creates movie form the output file

You can run the simulation with

```bash
./fish_schooling --config config.yaml
```

For optimal performance, you probably should set `OMP_NUM_THREADS` to the number of cores in your system:
```bash
OMP_NUM_THREADS=<YOUR_CORE_COUNT> ./fish_schooling --config config.yaml
```

Create a movie from the result by executing
```bash
./create_movie output.txt config.yaml
```

## Model

The model is *based* on the "Emergence of a Giant Rotating Cluster of Fish in Three Dimensions by Local Interactions"
Susumu Ito, Nariya Uchida
Journal of the Physical Society of Japan, 91, 064806 (2022) 10.7566/JPSJ.91.064806

However, some implementations of this repository code differ to the reference article and is not guaranteed to produce the same results.