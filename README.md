# AllgäuRechner

A cross-platform calculator application built with C++ and Qt.

## Features

- Graphical calculator interface

- Mathematical expression parser

- Support for mathematical functions and angle modes

- Keyboard input and expression editing

- Unit tests for calculator logic and GUI interactions

## Support the project

AllgäuRechner is and will remain free of charge and free of advertising.

Its continued development nevertheless requires time and financial resources – for
development hardware, software, and AI tools used to build, maintain, and improve
the application. Voluntary contributions help make this work possible and allow
the developer to dedicate time to the project instead of having to devote that
time entirely to other paid work.

If you find AllgäuRechner useful and would like to support its future development,
you can make a voluntary contribution via PayPal:

**[Support AllgäuRechner via PayPal](https://www.paypal.me/AchimKoch430)**

Every contribution helps. Thank you!

### Projekt unterstützen

Der AllgäuRechner bleibt kostenlos und werbefrei. Seine Weiterentwicklung
erfordert jedoch Zeit und finanzielle Mittel – beispielsweise für
Entwicklungs-Hardware, Software und KI-Werkzeuge.

Freiwillige Beiträge helfen dabei, diese Arbeit überhaupt erst zu ermöglichen.
Sie unterstützen mich dabei, Zeit in die Weiterentwicklung zu investieren,
anstatt diese Zeit vollständig für eine andere Erwerbstätigkeit aufbringen zu
müssen.

Wenn dir der AllgäuRechner gefällt und du seine Zukunft unterstützen möchtest,
freue ich mich über einen freiwilligen Beitrag. Jeder Beitrag zählt. Vielen Dank!

**[AllgäuRechner über PayPal unterstützen](https://www.paypal.me/AchimKoch430)**

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

### Android release signing

Before creating a version tag, configure these GitHub Actions repository secrets
with a persistent Android release keystore:

- `ANDROID_SIGNING_KEYSTORE_BASE64` – Base64-encoded PKCS#12 keystore
- `ANDROID_SIGNING_KEYSTORE_PASSWORD` – keystore password
- `ANDROID_SIGNING_KEY_ALIAS` – key alias
- `ANDROID_SIGNING_KEY_PASSWORD` – key password

Tagged releases fail safely if these secrets are missing. Ordinary branch builds
continue to use a temporary debug key.
