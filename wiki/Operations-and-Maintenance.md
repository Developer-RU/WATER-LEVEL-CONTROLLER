# Operations and Maintenance

## Startup Checklist

1. Verify wiring and relay polarity.
2. Confirm sensor logic mode matches physical sensor behavior.
3. Validate emergency upper sensor reaction before unattended operation.
4. Confirm AP setup mode can be entered when needed.

## Routine Operation

Recommended periodic checks:
- Sensor trigger correctness and mechanical movement.
- Pump on/off transitions in expected FSM states.
- Runtime counters increasing consistently.
- Alarm channel and indicator functionality.

## OTA Update Procedure

1. Enter setup mode.
2. Open dashboard OTA page.
3. Upload firmware binary.
4. Wait for finalization and automatic reboot.
5. Verify boot diagnostics and API health.

## Data Persistence

NVS namespaces:
- Settings: `wlc-settings`
- Statistics: `wlc-stats`
- Diagnostics: `wlc-diag`

If data is missing or invalid at boot, firmware self-heals by restoring factory defaults.

## Safety Practices

- Keep mains wiring isolated from logic wiring.
- Use external switching hardware if pump load exceeds relay limits.
- Test upper sensor emergency path regularly.
- Keep relay assignments unique for pump and alarm channels.
