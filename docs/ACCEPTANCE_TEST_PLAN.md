# Acceptance Test Plan

This plan defines practical field acceptance criteria for commissioning.

## 1. Preconditions

- Correct wiring according to project documentation.
- Pump and tank available in controlled test conditions.
- Two level sensors connected and physically testable.
- Serial monitor available.
- Dashboard accessible via setup AP.

## 2. Functional Acceptance Cases

### AT-01 Standby Baseline
- Initial condition: both sensors normal.
- Expected: pump OFF, alarm OFF, state Standby.

### AT-02 Fill Start Trigger
- Action: activate lower sensor in standby.
- Expected: pump ON, fill cycle counter increments by one.

### AT-03 Top-Off Start
- Action: while filling, clear lower sensor.
- Expected: top-off countdown starts and remaining time is visible.

### AT-04 Top-Off Reset
- Action: during countdown, reactivate lower sensor.
- Expected: countdown resets and state returns to filling phase.

### AT-05 Emergency Stop
- Action: activate upper sensor from any active pump state.
- Expected: immediate pump OFF, alarm ON, alarm counter increments.

### AT-06 Alarm Recovery
- Action: return both sensors to normal.
- Expected: alarm clears, system returns to standby.

### AT-07 Setup Freeze
- Action: boot into setup mode with button hold.
- Expected: automation frozen, relays OFF.

### AT-08 Setup Test Run
- Action: start test run from dashboard.
- Expected: automation enabled temporarily, then auto-stops by timeout.

## 3. Metrics Acceptance Cases

### AT-09 Runtime Counter
- Action: run pump for known interval.
- Expected: totalPumpRuntimeSeconds increases by measured duration.

### AT-10 Liter Estimation
- Action: set known flow value and run pump for known time.
- Expected: liters value follows model flow multiplied by time.

### AT-11 Cycle Counter
- Action: perform full refill start event twice.
- Expected: fillCycles increments by exactly two.

### AT-12 Alarm Counter
- Action: trigger emergency upper sensor once.
- Expected: alarmCount increments by exactly one.

## 4. Persistence Acceptance Cases

### AT-13 Settings Persistence
- Action: change settings, reboot.
- Expected: settings restored correctly after reboot.

### AT-14 Statistics Persistence
- Action: generate metrics, reboot.
- Expected: counters preserved.

### AT-15 Factory Reset
- Action: invoke factory reset from dashboard.
- Expected: defaults restored, statistics cleared, controlled reboot.

## 5. Pass Criteria

System is accepted when all applicable tests pass without safety violations or unstable behavior.
