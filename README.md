# Tokimeki Idol Score Ranking OCR Engine

[![CircleCI](https://dl.circleci.com/status-badge/img/gh/Kamioda/TokidolRankingOCREngine/tree/main.svg?style=svg)](https://dl.circleci.com/status-badge/redirect/gh/Kamioda/TokidolRankingOCREngine/tree/main)

## Build

### Windows

```shell
cmake -B <Path to build> -G <generator name>
cmake --build <Path to build>
```

### Linux

```shell
cmake -B <Path to build>
cmake --build <Path to build>
```

## Required module

- OpenCV 4.0.0 or later
- [cpp-httplib](https://github.com/yhirose/cpp-httplib)
- [nlohmann/json](https://github.com/nlohmann/json)
- [Tesseract](https://github.com/tesseract-ocr/tesseract)

Tesseract is used via OpenCV

## Required OpenCV modules

- text
- highgui
- modules that depend on them above modules
