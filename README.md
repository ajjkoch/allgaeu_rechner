# AllgäuRechner

A cross-platform calculator application built with C++ and Qt.

## Features

- Graphical calculator interface

- Mathematical expression parser

- Support for mathematical functions and angle modes

- Keyboard input and expression editing

- Unit tests for calculator logic and GUI interactions

## Platforms

- **Desktop:** Linux and other supported Qt platforms

- **Android:** ARM64 APK

## Technologies

- C++

- Qt 6

- CMake

- GoogleTest

- GitHub Actions

## Build and Testing

See `instructor/Calculator_cmake/TESTING.md` for build and testing instructions.

## CI/CD

GitHub Actions workflows are located in `.github/workflows/`:

- `calculator.yml` – Desktop build and tests

- `calculator-android.yml` – Android ARM64 APK build

## License

See the repository license information.

## Version and Releases

The current application version is maintained in `VERSION`. Releases are created
by pushing a matching version tag in the form `vMAJOR.MINOR.PATCH` (for example,
`v1.0.0`). The release workflow validates the tag against `VERSION`, builds the
Android ARM64 APK and attaches it to the GitHub Release.

## Copyright and License

Copyright © 2026 **Koch, Achim Johannes Josef**.

AllgäuRechner is distributed under the [MIT License](LICENSE). See
[COPYRIGHT.md](COPYRIGHT.md) for the project copyright notice. Third-party
components and bundled assets remain subject to their respective licenses.
