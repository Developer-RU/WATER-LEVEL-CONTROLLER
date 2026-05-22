# Contributing Guide

Thank you for your interest in improving this project.

## Scope

This repository contains a production-oriented embedded solution for well pump and tank level control. Contributions must preserve runtime stability and safety behavior.

## Ground Rules

- Do not change control logic semantics without explicit rationale and test evidence.
- Keep hardware safety behavior intact.
- Prefer small, focused pull requests.
- Update documentation when behavior or configuration changes.

## Development Setup

1. Install PlatformIO.
2. Open the repository in VS Code or terminal.
3. Build firmware:
   - `pio run`
4. Flash firmware:
   - `pio run -t upload`
5. Open monitor:
   - `pio device monitor -b 115200`

## Pull Request Checklist

- Build passes locally.
- No unintended behavior changes in control FSM.
- README and technical docs are updated if needed.
- New constants and settings are documented.
- Changelog entry is added under Unreleased.

## Commit Message Style

Recommended prefixes:
- `feat:` new user-visible capability.
- `fix:` behavior correction.
- `docs:` documentation-only update.
- `chore:` maintenance update.

## Safety-Critical Changes

For modifications affecting pump start/stop behavior, sensor interpretation, or alarm handling, include:
- Problem statement.
- Before/after behavior description.
- Validation steps.
- Rollback plan.
