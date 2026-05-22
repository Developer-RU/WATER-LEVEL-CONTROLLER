# Release Checklist

Use this checklist before publishing a production release.

## 1. Versioning

- Version number defined for release.
- CHANGELOG updated.
- RELEASE_NOTES updated.

## 2. Build and Binary Validation

- Clean build completes with PlatformIO.
- Firmware binary generated successfully.
- Memory usage reviewed and acceptable.

## 3. Functional Validation

- Standby to fill transition works.
- Top-off countdown starts and completes correctly.
- Upper sensor emergency stop verified.
- Alarm clear condition verified.
- Setup mode freeze behavior verified.
- Setup mode test run timeout verified.

## 4. Metrics Validation

- Runtime seconds increment only when pump active.
- Liter estimation responds to configured flow value.
- fillCycles increments once per cycle start.
- alarmCount increments on emergency trigger events.

## 5. Persistence Validation

- Settings save/load across reboot verified.
- Statistics save/load across reboot verified.
- Factory reset restores defaults.

## 6. API and UI Validation

- All documented API endpoints return expected payloads.
- Dashboard values match runtime behavior.
- Mobile and tablet responsive layouts verified.
- OTA upload and controlled reboot verified.

## 7. Safety Validation

- Relay polarity validated for target hardware.
- Pump and alarm relays mapped to different channels.
- Emergency path tested with real or simulated upper sensor event.
- Electrical safety notes reviewed for deployment.

## 8. Documentation Validation

- README is up to date.
- CODE_REFERENCE and ARCHITECTURE documents are up to date.
- CONTRIBUTING, SECURITY, SUPPORT are up to date.
- Issue and PR templates present.

## 9. Release Publication

- Commit and tag created.
- Main branch push completed.
- Release notes published in GitHub release.
