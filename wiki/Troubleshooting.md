# Troubleshooting

## Pump Does Not Start

Checks:
- Verify lower sensor trigger state in dashboard status.
- Confirm automation mode is `Work` or setup test run is active.
- Confirm pump relay assignment is valid and unique.
- Confirm relay polarity (`RELAY_ACTIVE_LOW`) matches board hardware.

## Pump Does Not Stop

Checks:
- Verify upper sensor emergency trigger is detected.
- Confirm upper sensor logic mode (Closed/Open) is correct.
- Validate relay output wiring and external contactor behavior.

## False Sensor Events

Checks:
- Inspect wiring quality and grounding.
- Verify sensor contact type and selected logic mode.
- Confirm debounce assumptions are valid for installed sensors.

## Dashboard Not Reachable in Setup Mode

Checks:
- Ensure setup mode was armed at boot.
- Confirm AP is started and has expected SSID.
- Verify client is connected to setup AP.
- Check serial logs for AP startup failures.

## OTA Upload Fails

Checks:
- Confirm firmware binary target matches ESP32 environment.
- Ensure stable power during upload.
- Retry with a smaller network load and stable signal.
- Check OTA error message returned by endpoint/UI.

## Persistence Issues

Symptoms:
- Settings/statistics reset unexpectedly.

Checks:
- Look for NVS initialization errors in serial output.
- Validate no repeated brownout resets.
- Confirm factory-restore path is not triggered unintentionally.
