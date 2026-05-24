# Control Logic

## High-Level FSM

The automation finite-state machine lives in [src/main.cpp](../src/main.cpp).

States:
- `Standby`: pump OFF, waiting for lower sensor trigger.
- `FillingUntilLowerClears`: pump ON, waiting for lower sensor to clear.
- `TopOffCountdown`: pump ON for configured top-off timeout.
- `Alarm`: pump OFF, alarm ON until both sensors are normal.

## Safety Priority

Upper sensor emergency is always dominant:
- Triggered upper sensor causes immediate pump stop.
- Alarm relay turns ON.
- Alarm counter increments on new alarm event.

## Fill Cycle Semantics

A new fill cycle starts when lower sensor triggers from standby.

Top-off behavior:
- Countdown starts only after lower sensor clears.
- If lower sensor triggers again during countdown, the countdown resets.
- When countdown expires, the pump stops with reason `TOP_OFF_DONE`.

## Setup Mode and Test Run

In setup mode:
- Automation is frozen by default.
- `POST /api/system/test-run` starts a timed diagnostic run.
- Remaining test time is exposed in runtime status.
- Expiration auto-stops test run and returns to frozen setup state.

## Stop Reasons

Pump stop reasons are normalized in `PumpStopReason`:
- `NONE`
- `TOP_OFF_DONE`
- `UPPER_ALARM`
- `SETUP_FREEZE`
- `TEST_RUN_STOPPED`
- `TEST_RUN_TIMEOUT`
