# Troubleshooting

## 1. Pump Does Not Start

Possible causes:
- Lower sensor logic mode mismatched.
- Pump relay channel mapped incorrectly.
- Relay polarity mismatch with hardware board.
- Setup mode freeze active.

Checks:
- Verify lower sensor state in dashboard.
- Verify pump relay setting and wiring.
- Verify RELAY_ACTIVE_LOW behavior for your relay board.
- Verify setup mode badge and test run status.

## 2. Pump Never Stops

Possible causes:
- Lower sensor wiring fault or incorrect logic.
- Upper sensor not connected or never triggers.
- Sensor contact type differs from configured mode.

Checks:
- Trigger sensors manually and observe dashboard states.
- Validate both sensor logic modes.
- Confirm upper sensor emergency stop path.

## 3. Alarm Triggers Unexpectedly

Possible causes:
- Electrical noise on upper sensor line.
- Incorrect upper sensor trigger mode.
- Sensor wiring instability.

Checks:
- Inspect sensor cable routing and grounding.
- Verify upper sensor logic configuration.
- Test with stable dry-contact simulation.

## 4. Metrics Look Incorrect

Possible causes:
- Pump flow value not calibrated.
- Test duration measured inaccurately.
- Expectations not adjusted for estimation model.

Checks:
- Calibrate pumpFlowLpm with measured output.
- Compare values with known timed test.
- Confirm counters persist correctly after reboot.

## 5. Dashboard Not Reachable

Possible causes:
- Setup AP not armed at boot.
- Client not connected to setup AP.
- Device rebooted or power unstable.

Checks:
- Boot with setup button hold.
- Connect to setup AP SSID.
- Verify serial logs for AP startup status.

## 6. OTA Upload Fails

Possible causes:
- Invalid binary file.
- Network interruption.
- Incomplete upload stream.

Checks:
- Rebuild firmware and retry upload.
- Keep client close to AP during upload.
- Check OTA error message in API/UI.

## 7. NVS Data Resets Unexpectedly

Possible causes:
- Corrupted persistence data.
- Manual factory reset.
- Flash/storage issue.

Checks:
- Inspect serial logs for fallback-to-defaults message.
- Verify power quality and brownout events.
- Reconfigure settings and monitor behavior over reboots.
