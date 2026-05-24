# WATER-LEVEL-CONTROLLER Wiki

## Overview

WATER-LEVEL-CONTROLLER is an ESP32 firmware project for automated well pump control with dual float sensors and strict emergency shutdown behavior.

Core goals:
- Keep tank fill level stable with deterministic logic.
- Stop the pump immediately on upper-level emergency trigger.
- Provide local setup, diagnostics, and OTA update capability.

## Quick Navigation

- [Architecture](Architecture)
- [Configuration](Configuration)
- [Control Logic](Control-Logic)
- [API Reference](API-Reference)
- [Operations and Maintenance](Operations-and-Maintenance)
- [Troubleshooting](Troubleshooting)
- [Release and Contribution](Release-and-Contribution)

## Repository Pointers

- Main overview: [README.md](../README.md)
- Source code: [src](../src)
- Public headers: [include](../include)
- Deep technical docs: [docs](../docs)
