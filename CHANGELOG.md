# Changelog

All notable changes to this project are documented in this file.

The format is based on Keep a Changelog and this project follows Semantic Versioning.

## [Unreleased]

### Added
- Release process and repository governance documentation.
- Expanded technical documentation set for architecture and operations.
- Field acceptance test plan and troubleshooting runbook.
- Code of conduct and GitHub issue/PR templates.
- GitHub Actions CI workflow for PlatformIO build verification.
- CODEOWNERS and Dependabot configuration for repository maintenance.
- Full project wiki with architecture, configuration, API, operations, and release guidance.

### Changed
- README summary and GitHub-facing short description guidance were refined.

## [1.0.0] - 2026-05-22

### Added
- Stable water level control firmware for ESP32 relay boards.
- Two-sensor control strategy with emergency upper-level shutdown.
- Setup mode with local AP and responsive web dashboard.
- OTA firmware update endpoint.
- Persistent settings and runtime statistics in NVS.
- Extended project documentation, wiring guide, and screenshots.

### Tracked Metrics
- Total pump runtime in seconds.
- Estimated pumped water volume in liters.
- Fill cycle count.
- Emergency alarm shutdown count.

### Notes
- This release is intended as a production baseline for well pump and Eurocube level automation.
