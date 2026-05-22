# Operations Runbook

This runbook describes routine operational procedures.

## 1. Initial Commissioning

1. Validate wiring and relay channel assignment.
2. Boot in setup mode.
3. Open dashboard and configure:
- fill timeout
- pump flow
- sensor logic modes
- pump and alarm relays
4. Save settings.
5. Run acceptance tests.

## 2. Normal Operation

- Controller runs in work mode automatically.
- Monitor dashboard metrics periodically.
- Investigate alarm count growth and unexpected cycle patterns.

## 3. Monthly Maintenance

- Inspect sensor mechanical condition.
- Inspect relay terminals and cable tightness.
- Verify calibrated flow value remains realistic.
- Record key telemetry snapshot.

## 4. Firmware Update Procedure

1. Build validated firmware.
2. Enter setup mode and open dashboard.
3. Upload firmware via OTA page.
4. Wait for controlled reboot.
5. Re-check settings and key metrics.

## 5. Incident Response

### Overflow Risk

- Immediately disable pump power path.
- Verify upper sensor operation and wiring.
- Validate emergency stop behavior before re-enable.

### Unexpected Pump Start

- Check lower sensor state and logic mode.
- Verify setup mode state and test run status.
- Inspect relay mapping and wiring.

### Repeated Alarms

- Review sensor signal stability.
- Inspect cabling and grounding.
- Validate mechanical float movement.

## 6. Backup and Recovery

- Export settings manually from dashboard values.
- After factory reset, restore critical settings first.
- Run quick functional safety checks before returning to service.
