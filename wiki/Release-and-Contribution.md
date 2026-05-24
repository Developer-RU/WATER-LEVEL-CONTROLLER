# Release and Contribution

## Development Workflow

1. Create a feature branch from `main`.
2. Implement focused changes.
3. Run local validation:
   - `pio run`
   - target-specific smoke checks if hardware is available.
4. Update documentation and changelog under `Unreleased`.
5. Open a pull request using the repository template.

## Pull Request Quality Gate

Expected in every PR:
- No unintended control logic changes.
- Safety behavior preserved and validated.
- Changelog updated.
- Docs updated for behavioral or interface changes.

## Release Preparation

Use the checklist in [docs/RELEASE_CHECKLIST.md](../docs/RELEASE_CHECKLIST.md).

Primary artifacts:
- Firmware build output from PlatformIO.
- Updated [CHANGELOG.md](../CHANGELOG.md).
- Updated [RELEASE_NOTES.md](../RELEASE_NOTES.md).

## Related Governance Files

- [CONTRIBUTING.md](../CONTRIBUTING.md)
- [CODE_OF_CONDUCT.md](../CODE_OF_CONDUCT.md)
- [SECURITY.md](../SECURITY.md)
- [SUPPORT.md](../SUPPORT.md)
- [.github/pull_request_template.md](../.github/pull_request_template.md)
