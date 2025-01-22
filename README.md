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

## Model

The model is *based* on the "Emergence of a Giant Rotating Cluster of Fish in Three Dimensions by Local Interactions"
Susumu Ito, Nariya Uchida
Journal of the Physical Society of Japan, 91, 064806 (2022) 10.7566/JPSJ.91.064806

However, some implementations of this repository code differ to the reference article and is not guaranteed to produce the same results.