#include "webserver.h"

#include <WiFi.h>

#include "config.h"

static const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover">
  <meta name="theme-color" content="#0c1118">
  <title>WATER-LEVEL-CONTROLLER</title>
  <style>
    :root {
      color-scheme: dark;
      --bg: #070b11;
      --panel: rgba(16, 22, 32, 0.92);
      --panel-2: rgba(20, 29, 42, 0.96);
      --line: rgba(131, 151, 174, 0.18);
      --text: #ebf1f8;
      --muted: #91a2b8;
      --accent: #53d6c2;
      --accent-2: #7c8cff;
      --danger: #ff6b73;
      --warn: #ffd166;
      --success: #3ddc97;
      --shadow: 0 24px 80px rgba(0, 0, 0, 0.5);
      --radius: 22px;
      --radius-sm: 16px;
      --sidebar: 290px;
      --gap: 18px;
      font-family: Inter, ui-sans-serif, system-ui, -apple-system, BlinkMacSystemFont, sans-serif;
    }
    * { box-sizing: border-box; }
    html, body { margin: 0; min-height: 100%; background:
      radial-gradient(circle at top left, rgba(83, 214, 194, 0.14), transparent 34%),
      radial-gradient(circle at bottom right, rgba(124, 140, 255, 0.12), transparent 28%),
      linear-gradient(180deg, #06090f, #0b121b 45%, #070b11); color: var(--text); }
    body { overflow-x: hidden; }
    .app { display: grid; grid-template-columns: var(--sidebar) 1fr; min-height: 100vh; }
    .sidebar {
      position: sticky; top: 0; height: 100vh; padding: 22px; border-right: 1px solid var(--line);
      background: linear-gradient(180deg, rgba(11, 18, 27, 0.96), rgba(8, 12, 18, 0.86)); backdrop-filter: blur(20px);
    }
    .brand { display: flex; align-items: center; gap: 14px; margin-bottom: 22px; }
    .logo { width: 48px; height: 48px; border-radius: 15px; display: grid; place-items: center;
      background: linear-gradient(135deg, rgba(83,214,194,0.24), rgba(124,140,255,0.28));
      border: 1px solid rgba(255,255,255,0.08); box-shadow: inset 0 1px 0 rgba(255,255,255,0.08); font-weight: 700; }
    .brand h1 { margin: 0; font-size: 17px; letter-spacing: 0.08em; text-transform: uppercase; }
    .brand p { margin: 3px 0 0; color: var(--muted); font-size: 12px; }
    .nav { display: grid; gap: 10px; margin-top: 24px; }
    .nav button {
      width: 100%; text-align: left; border: 1px solid transparent; border-radius: 16px; padding: 14px 16px;
      color: var(--text); background: rgba(255,255,255,0.02); cursor: pointer; transition: 180ms ease;
      font-size: 14px; letter-spacing: 0.02em;
    }
    .nav button.active, .nav button:hover { background: rgba(83,214,194,0.12); border-color: rgba(83,214,194,0.22); transform: translateX(2px); }
    .sidebar-footer { position: absolute; left: 22px; right: 22px; bottom: 22px; }
    .chip { display: inline-flex; gap: 8px; align-items: center; padding: 10px 12px; border-radius: 999px; background: rgba(255,255,255,0.04); color: var(--muted); font-size: 12px; }
    .setup-chip { background: rgba(255, 209, 102, 0.16); color: #ffd166; border: 1px solid rgba(255, 209, 102, 0.26); }
    .setup-banner {
      display: none; margin: 0 0 18px; padding: 16px 18px; border-radius: 18px; border: 1px solid rgba(255, 209, 102, 0.25);
      background: linear-gradient(180deg, rgba(255, 209, 102, 0.14), rgba(255, 209, 102, 0.07));
      color: #ffe7aa; box-shadow: var(--shadow);
    }
    .setup-banner h3 { margin: 0 0 8px; font-size: 14px; letter-spacing: 0.12em; text-transform: uppercase; }
    .setup-banner p { margin: 0; color: #f2ddb0; line-height: 1.6; }
    .setup-notice { display: none; margin-top: 16px; padding: 14px 16px; border-radius: 16px; background: rgba(255,255,255,0.03); border: 1px solid rgba(255,255,255,0.08); color: var(--muted); }
    .setup-notice strong { color: var(--text); }
    .content { padding: 20px; }
    .topbar {
      display: flex; align-items: center; justify-content: space-between; gap: 14px;
      margin-bottom: 18px; position: sticky; top: 0; z-index: 10; padding: 12px 0 18px;
      backdrop-filter: blur(16px);
    }
    .topbar-left { display: flex; align-items: center; gap: 14px; }
    .burger {
      display: none; width: 46px; height: 46px; border-radius: 14px; border: 1px solid var(--line); color: var(--text);
      background: rgba(255,255,255,0.04); font-size: 22px; cursor: pointer;
    }
    .hero { display: grid; gap: 8px; }
    .hero h2 { margin: 0; font-size: clamp(24px, 4vw, 38px); }
    .hero p { margin: 0; color: var(--muted); }
    .grid { display: grid; gap: var(--gap); }
    .cards { grid-template-columns: repeat(12, minmax(0, 1fr)); }
    .card {
      grid-column: span 3; background: linear-gradient(180deg, rgba(18,24,35,0.96), rgba(12,18,27,0.95));
      border: 1px solid var(--line); border-radius: var(--radius); padding: 18px; box-shadow: var(--shadow);
      animation: fadeUp 420ms ease both;
    }
    .card.wide { grid-column: span 6; }
    .card.full { grid-column: 1 / -1; }
    .label { color: var(--muted); font-size: 12px; text-transform: uppercase; letter-spacing: 0.12em; margin-bottom: 10px; }
    .value { font-size: 28px; font-weight: 700; line-height: 1.1; }
    .sub { color: var(--muted); font-size: 13px; margin-top: 8px; }
    .badge-row { display: flex; flex-wrap: wrap; gap: 10px; }
    .badge {
      display: inline-flex; align-items: center; gap: 8px; padding: 10px 12px; border-radius: 999px;
      background: rgba(255,255,255,0.04); border: 1px solid rgba(255,255,255,0.05); font-size: 12px;
    }
    .dot { width: 10px; height: 10px; border-radius: 50%; background: var(--muted); box-shadow: 0 0 0 4px rgba(255,255,255,0.03); }
    .dot.on { background: var(--success); }
    .dot.off { background: var(--danger); }
    .dot.warn { background: var(--warn); }
    .section { display: none; animation: fadeUp 300ms ease both; }
    .section.active { display: block; }
    .panel {
      background: linear-gradient(180deg, rgba(18,24,35,0.96), rgba(12,18,27,0.95)); border: 1px solid var(--line);
      border-radius: var(--radius); padding: 20px; box-shadow: var(--shadow);
    }
    .form-grid { display: grid; grid-template-columns: repeat(2, minmax(0, 1fr)); gap: 16px; }
    .field { display: grid; gap: 8px; }
    .field label { font-size: 13px; color: var(--muted); }
    .field input, .field select, .field button, .ota-file {
      width: 100%; border-radius: 14px; border: 1px solid rgba(255,255,255,0.09); background: rgba(255,255,255,0.03);
      color: var(--text); padding: 14px 14px; outline: none; font: inherit;
    }
    .field input::placeholder { color: #6f8199; }
    .field input:focus, .field select:focus { border-color: rgba(83,214,194,0.4); box-shadow: 0 0 0 4px rgba(83,214,194,0.08); }
    .actions { display: flex; flex-wrap: wrap; gap: 12px; margin-top: 18px; }
    .btn {
      border: 1px solid rgba(255,255,255,0.1); background: rgba(255,255,255,0.04); color: var(--text);
      padding: 13px 16px; border-radius: 14px; cursor: pointer; transition: 180ms ease;
    }
    .btn.primary { background: linear-gradient(135deg, rgba(83,214,194,0.18), rgba(124,140,255,0.18)); border-color: rgba(83,214,194,0.25); }
    .btn.danger { background: rgba(255,107,115,0.12); border-color: rgba(255,107,115,0.22); }
    .btn:hover { transform: translateY(-1px); }
    .split { display: grid; grid-template-columns: repeat(2, minmax(0,1fr)); gap: 16px; }
    .muted { color: var(--muted); }
    .progress {
      width: 100%; height: 12px; border-radius: 999px; background: rgba(255,255,255,0.06); overflow: hidden; border: 1px solid rgba(255,255,255,0.08);
    }
    .progress > span { display: block; height: 100%; width: 0%; background: linear-gradient(90deg, var(--accent), var(--accent-2)); transition: width 120ms linear; }
    .toast {
      position: fixed; right: 18px; bottom: 18px; max-width: min(420px, calc(100vw - 36px));
      padding: 14px 16px; border-radius: 16px; background: rgba(18,24,35,0.98); border: 1px solid var(--line); box-shadow: var(--shadow);
      color: var(--text); transform: translateY(18px); opacity: 0; pointer-events: none; transition: 220ms ease;
    }
    .toast.show { transform: translateY(0); opacity: 1; }
    @keyframes fadeUp { from { opacity: 0; transform: translateY(8px); } to { opacity: 1; transform: translateY(0); } }
    @media (max-width: 1024px) {
      .cards .card { grid-column: span 6; }
      .card.wide { grid-column: span 12; }
    }
    @media (max-width: 768px) {
      .app { grid-template-columns: 1fr; }
      .sidebar {
        position: fixed; left: 0; top: 0; width: min(100vw - 48px, 320px); transform: translateX(-105%); transition: transform 220ms ease;
        z-index: 50; box-shadow: var(--shadow);
      }
      body.nav-open .sidebar { transform: translateX(0); }
      .burger { display: grid; place-items: center; }
      .cards .card, .card.wide { grid-column: 1 / -1; }
      .form-grid, .split { grid-template-columns: 1fr; }
    }
  </style>
</head>
<body>
  <div class="app">
    <aside class="sidebar">
      <div class="brand">
        <div class="logo">WLC</div>
        <div>
          <h1>Water Level Controller</h1>
          <p>Standalone ESP32 dashboard</p>
        </div>
      </div>
      <div class="nav">
        <button data-view="dashboard" class="active">Dashboard</button>
        <button data-view="settings">Settings</button>
        <button data-view="ota">OTA Update</button>
        <button data-view="system">System</button>
      </div>
      <div class="sidebar-footer">
        <div class="chip">Offline local UI, no CDN, no internet</div>
      </div>
    </aside>
    <main class="content">
      <div class="setup-banner" id="setupModeBanner">
        <h3>SETUP MODE ACTIVE</h3>
        <p>Automatic control disabled. Relays frozen. Sensors paused.</p>
      </div>
      <div class="topbar">
        <div class="topbar-left">
          <button class="burger" id="burger">&#9776;</button>
          <div class="hero">
            <h2 id="pageTitle">Dashboard</h2>
            <p id="pageSubtitle">Live control and telemetry for the fill system.</p>
          </div>
        </div>
        <div class="chip" id="setupModeBadge">WORK MODE ACTIVE</div>
        <div class="chip" id="connectionChip">AP offline</div>
      </div>

      <section id="view-dashboard" class="section active">
        <div class="grid cards" id="dashboardCards">
          <div class="card"><div class="label">Pump</div><div class="value" id="pumpState">OFF</div><div class="sub" id="pumpRelayState">Relay 1</div></div>
          <div class="card"><div class="label">Lower Sensor</div><div class="value" id="lowerState">--</div><div class="sub" id="lowerLogic">Logic: Closed</div></div>
          <div class="card"><div class="label">Upper Sensor</div><div class="value" id="upperState">--</div><div class="sub" id="upperLogic">Logic: Closed</div></div>
          <div class="card"><div class="label">Timer</div><div class="value" id="timerState">--</div><div class="sub">Fill timeout status</div></div>
          <div class="card wide"><div class="label">Relays</div><div class="badge-row" id="relayBadges"></div></div>
          <div class="card"><div class="label">Total Water</div><div class="value" id="waterTotal">0.0 L</div><div class="sub">Accumulated volume</div></div>
          <div class="card"><div class="label">Pump Runtime</div><div class="value" id="runtimeTotal">0 s</div><div class="sub">Stored in NVS</div></div>
          <div class="card"><div class="label">Cycles</div><div class="value" id="cycleCount">0</div><div class="sub">Fill starts</div></div>
          <div class="card"><div class="label">Alarms</div><div class="value" id="alarmCount">0</div><div class="sub">Emergency shutdowns</div></div>
          <div class="card"><div class="label">Stop Reason</div><div class="value" id="stopReason">NONE</div><div class="sub">Last pump stop cause</div></div>
          <div class="card"><div class="label">Reset Diagnostics</div><div class="value" id="bootResetReason">UNKNOWN</div><div class="sub" id="prevResetReason">Prev: UNKNOWN | Boots: 0</div></div>
        </div>
        <div class="setup-notice" id="setupNotice">
          <strong>Automatic control disabled.</strong> Relays frozen. Sensors paused.
        </div>
      </section>

      <section id="view-settings" class="section">
        <div class="panel">
          <div class="split">
            <div class="field"><label for="fillTimeoutMinutes">Fill timeout minutes</label><input id="fillTimeoutMinutes" type="number" min="1" max="60" placeholder="1, 2, 5, 10, 15"></div>
            <div class="field"><label for="pumpFlowLpm">Pump flow L/min</label><input id="pumpFlowLpm" type="number" min="0.1" max="999.9" step="0.1" placeholder="10.0"></div>
          </div>
          <div class="form-grid" style="margin-top:16px;">
            <div class="field">
              <label for="lowerSensorLogic">Lower sensor logic</label>
              <select id="lowerSensorLogic">
                <option value="0">Triggered on closed contact</option>
                <option value="1">Triggered on open contact</option>
              </select>
            </div>
            <div class="field">
              <label for="upperSensorLogic">Upper sensor logic</label>
              <select id="upperSensorLogic">
                <option value="0">Triggered on closed contact</option>
                <option value="1">Triggered on open contact</option>
              </select>
            </div>
            <div class="field">
              <label for="pumpRelay">Pump relay</label>
              <select id="pumpRelay">
                <option value="1">Relay 1</option>
                <option value="2">Relay 2</option>
                <option value="3">Relay 3</option>
                <option value="4">Relay 4</option>
              </select>
            </div>
            <div class="field">
              <label for="alarmRelay">Alarm relay</label>
              <select id="alarmRelay">
                <option value="1">Relay 1</option>
                <option value="2">Relay 2</option>
                <option value="3">Relay 3</option>
                <option value="4">Relay 4</option>
              </select>
            </div>
          </div>
          <div class="actions">
            <button class="btn primary" id="saveSettingsBtn">Save Settings</button>
            <button class="btn" id="restoreDefaultsBtn">Restore Defaults</button>
          </div>
        </div>
      </section>

      <section id="view-ota" class="section">
        <div class="panel">
          <div class="split">
            <div class="field"><label for="otaFile">Firmware file</label><input class="ota-file" id="otaFile" type="file" accept=".bin,application/octet-stream"></div>
            <div class="field"><label>Upload progress</label><div class="progress"><span id="otaProgressBar"></span></div><div class="muted" id="otaProgressText" style="margin-top:8px;">0%</div></div>
          </div>
          <div class="actions">
            <button class="btn primary" id="otaUploadBtn">Upload Firmware</button>
          </div>
          <p class="muted">The device reboots automatically after a successful upload.</p>
        </div>
      </section>

      <section id="view-system" class="section">
        <div class="panel">
          <div class="actions">
            <button class="btn" id="testRunBtn">Start Test Run</button>
            <button class="btn primary" id="rebootBtn">Reboot Device</button>
            <button class="btn" id="resetStatsBtn">Reset Statistics</button>
            <button class="btn danger" id="factoryResetBtn">Restore Factory Settings</button>
          </div>
          <p class="muted" id="testRunHint">Available in Setup Mode only. Enables full automation for diagnostics.</p>
          <p class="muted">Factory reset restores defaults from config.h and clears stored statistics.</p>
        </div>
      </section>
    </main>
  </div>

  <div class="toast" id="toast"></div>

  <script>
    const state = { status: null, settings: null, defaults: null };
    const viewMeta = {
      dashboard: ['Dashboard', 'Live relay, sensor and water telemetry.'],
      settings: ['Settings', 'Tune sensor logic, timeout and relay mapping.'],
      ota: ['OTA Update', 'Upload a new firmware image directly over Wi-Fi.'],
      system: ['System', 'Maintenance and recovery actions.'],
    };

    const views = ['dashboard', 'settings', 'ota', 'system'];
    const burger = document.getElementById('burger');
    const toast = document.getElementById('toast');
    const connectionChip = document.getElementById('connectionChip');

    function showToast(message) {
      toast.textContent = message;
      toast.classList.add('show');
      clearTimeout(showToast.timer);
      showToast.timer = setTimeout(() => toast.classList.remove('show'), 2600);
    }

    function formatSeconds(totalSeconds) {
      const seconds = Math.max(0, Number(totalSeconds) || 0);
      const hours = Math.floor(seconds / 3600);
      const minutes = Math.floor((seconds % 3600) / 60);
      const rest = seconds % 60;
      if (hours > 0) return `${hours}h ${minutes}m ${rest}s`;
      if (minutes > 0) return `${minutes}m ${rest}s`;
      return `${rest}s`;
    }

    function formatLiters(value) {
      return `${(Number(value) || 0).toFixed(1)} L`;
    }

    function relayBadge(label, enabled) {
      const dotClass = enabled ? 'on' : 'off';
      return `<span class="badge"><span class="dot ${dotClass}"></span>${label}: ${enabled ? 'ON' : 'OFF'}</span>`;
    }

    function sensorBadge(label, active) {
      const dotClass = active ? 'warn' : 'off';
      return `<span class="badge"><span class="dot ${dotClass}"></span>${label}: ${active ? 'ACTIVE' : 'IDLE'}</span>`;
    }

    function showView(view) {
      views.forEach((name) => {
        document.getElementById(`view-${name}`).classList.toggle('active', name === view);
      });
      document.querySelectorAll('.nav button').forEach((button) => {
        button.classList.toggle('active', button.dataset.view === view);
      });
      const meta = viewMeta[view] || viewMeta.dashboard;
      document.getElementById('pageTitle').textContent = meta[0];
      document.getElementById('pageSubtitle').textContent = meta[1];
      history.replaceState({}, '', `/${view === 'dashboard' ? '' : view}`);
      document.body.classList.remove('nav-open');
    }

    function applySettings(settings) {
      document.getElementById('fillTimeoutMinutes').value = settings.fillTimeoutMinutes;
      document.getElementById('pumpFlowLpm').value = settings.pumpFlowLpm;
      document.getElementById('lowerSensorLogic').value = settings.lowerSensorLogic;
      document.getElementById('upperSensorLogic').value = settings.upperSensorLogic;
      document.getElementById('pumpRelay').value = settings.pumpRelay;
      document.getElementById('alarmRelay').value = settings.alarmRelay;
    }

    function updateDashboard(status) {
      const controlLabel = status.controlState === 1
        ? 'WAIT LOWER CLEAR'
        : status.controlState === 2
          ? 'TOP OFF'
          : status.controlState === 3
            ? 'ALARM'
            : 'IDLE';
      const setupActive = Boolean(status.setupModeActive);
      const testRunActive = Boolean(status.testRunActive);
      const automationFrozen = setupActive && !testRunActive;
      const testRunRemaining = Number(status.testRunRemainingSeconds || 0);
      document.getElementById('pumpState').textContent = automationFrozen ? 'OFF' : (status.pumpActive ? 'ON' : 'OFF');
      document.getElementById('pumpRelayState').textContent = `Pump relay ${status.settings.pumpRelay} | Alarm relay ${status.settings.alarmRelay}`;
      document.getElementById('lowerState').textContent = automationFrozen ? 'PAUSED' : (status.lowerSensorActive ? 'ACTIVE' : 'IDLE');
      document.getElementById('upperState').textContent = automationFrozen ? 'PAUSED' : (status.upperSensorActive ? 'ACTIVE' : 'IDLE');
      document.getElementById('lowerLogic').textContent = `Logic: ${status.settings.lowerSensorLogic === 0 ? 'Closed' : 'Open'}`;
      document.getElementById('upperLogic').textContent = `Logic: ${status.settings.upperSensorLogic === 0 ? 'Closed' : 'Open'}`;
      document.getElementById('timerState').textContent = status.alarmActive
        ? 'ALARM'
        : automationFrozen
          ? 'PAUSED'
        : status.controlState === 1
          ? 'WAITING FOR LOWER SENSOR'
          : status.controlState === 2
            ? formatSeconds(status.fillRemainingSeconds)
            : 'IDLE';
      document.getElementById('timerState').title = controlLabel;
      document.getElementById('waterTotal').textContent = formatLiters(status.totalWaterLiters);
      document.getElementById('runtimeTotal').textContent = formatSeconds(status.totalPumpRuntimeSeconds);
      document.getElementById('cycleCount').textContent = status.fillCycles;
      document.getElementById('alarmCount').textContent = status.alarmCount;
      document.getElementById('stopReason').textContent = status.pumpActive ? 'RUNNING' : (status.lastPumpStopReason || 'NONE');
      document.getElementById('bootResetReason').textContent = status.bootResetReason || 'UNKNOWN';
      document.getElementById('prevResetReason').textContent = `Prev: ${status.previousResetReason || 'UNKNOWN'} | Boots: ${status.bootCount || 0}`;
      document.getElementById('setupModeBanner').style.display = setupActive ? 'block' : 'none';
      document.getElementById('setupModeBadge').textContent = setupActive
        ? (testRunActive ? 'SETUP MODE TEST RUN' : 'SETUP MODE ACTIVE')
        : 'WORK MODE ACTIVE';
      document.getElementById('setupModeBadge').className = setupActive ? 'chip setup-chip' : 'chip';
      document.getElementById('setupNotice').style.display = automationFrozen ? 'block' : 'none';
      document.getElementById('setupModeBanner').querySelector('p').textContent = testRunActive
        ? `Test run is active. Automatic control temporarily enabled for diagnostics. Remaining: ${formatSeconds(testRunRemaining)}.`
        : 'Automatic control disabled. Relays frozen. Sensors paused.';
      const testRunBtn = document.getElementById('testRunBtn');
      testRunBtn.disabled = !setupActive;
      testRunBtn.textContent = testRunActive ? 'Stop Test Run' : 'Start Test Run';
      document.getElementById('testRunHint').textContent = setupActive
        ? (testRunActive
          ? `Test run is active. Auto-stop in ${formatSeconds(testRunRemaining)}.`
          : 'Setup Mode freeze is active. Start Test Run to temporarily enable automation.')
        : 'Available in Setup Mode only. Enables full automation for diagnostics.';
      document.getElementById('relayBadges').innerHTML = [
        relayBadge('Relay 1', status.relays[0]),
        relayBadge('Relay 2', status.relays[1]),
        relayBadge('Relay 3', status.relays[2]),
        relayBadge('Relay 4', status.relays[3]),
        sensorBadge('Lower sensor', status.lowerSensorActive),
        sensorBadge('Upper sensor', status.upperSensorActive),
      ].join('');
      connectionChip.textContent = status.wifiActive ? 'AP online' : 'AP offline';
    }

    async function api(path, options = {}) {
      const response = await fetch(path, { cache: 'no-store', ...options });
      if (!response.ok) {
        throw new Error(await response.text());
      }
      return response;
    }

    async function loadAll() {
      const [statusResponse, settingsResponse, defaultsResponse] = await Promise.all([
        api('/api/status'),
        api('/api/settings'),
        api('/api/defaults'),
      ]);
      state.status = await statusResponse.json();
      state.settings = await settingsResponse.json();
      state.defaults = await defaultsResponse.json();
      applySettings(state.settings);
      updateDashboard(state.status);
    }

    async function saveSettings() {
      const payload = new URLSearchParams({
        fillTimeoutMinutes: document.getElementById('fillTimeoutMinutes').value,
        pumpFlowLpm: document.getElementById('pumpFlowLpm').value,
        lowerSensorLogic: document.getElementById('lowerSensorLogic').value,
        upperSensorLogic: document.getElementById('upperSensorLogic').value,
        pumpRelay: document.getElementById('pumpRelay').value,
        alarmRelay: document.getElementById('alarmRelay').value,
      });
      await api('/api/settings', { method: 'POST', headers: { 'Content-Type': 'application/x-www-form-urlencoded' }, body: payload });
      showToast('Settings saved');
      await loadAll();
    }

    async function restoreDefaults() {
      await api('/api/settings/restore-defaults', { method: 'POST' });
      showToast('Factory defaults restored');
      await loadAll();
    }

    async function resetStatistics() {
      if (!confirm('Reset all statistics?')) return;
      await api('/api/system/reset-stats', { method: 'POST' });
      showToast('Statistics reset');
      await loadAll();
    }

    async function factoryReset() {
      if (!confirm('Restore factory settings and clear statistics?')) return;
      await api('/api/system/factory-reset', { method: 'POST' });
      showToast('Factory reset completed');
      await loadAll();
    }

    async function rebootDevice() {
      if (!confirm('Reboot the device now?')) return;
      await api('/api/system/reboot', { method: 'POST' });
      showToast('Device is rebooting');
    }

    async function toggleTestRun() {
      if (!state.status || !state.status.setupModeActive) {
        showToast('Test run is available only in Setup Mode');
        return;
      }

      const nextEnabled = !Boolean(state.status.testRunActive);
      const payload = new URLSearchParams({ enabled: nextEnabled ? '1' : '0' });
      await api('/api/system/test-run', {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: payload,
      });
      showToast(nextEnabled ? 'Test run started' : 'Test run stopped');
      await loadAll();
    }

    function uploadFirmware() {
      const fileInput = document.getElementById('otaFile');
      if (!fileInput.files.length) {
        showToast('Select a firmware .bin file first');
        return;
      }

      const file = fileInput.files[0];
      const formData = new FormData();
      formData.append('firmware', file, file.name);

      const xhr = new XMLHttpRequest();
      xhr.open('POST', '/api/ota/update', true);
      xhr.upload.onprogress = (event) => {
        if (!event.lengthComputable) return;
        const progress = Math.round((event.loaded / event.total) * 100);
        document.getElementById('otaProgressBar').style.width = `${progress}%`;
        document.getElementById('otaProgressText').textContent = `${progress}%`;
      };
      xhr.onload = () => {
        if (xhr.status >= 200 && xhr.status < 300) {
          showToast('Firmware uploaded successfully');
          document.getElementById('otaProgressText').textContent = '100% - rebooting';
        } else {
          showToast(`OTA failed: ${xhr.responseText || xhr.status}`);
          document.getElementById('otaProgressText').textContent = 'Upload failed';
          document.getElementById('otaProgressBar').style.width = '0%';
        }
      };
      xhr.onerror = () => {
        showToast('OTA upload network error');
      };
      xhr.send(formData);
    }

    burger.addEventListener('click', () => document.body.classList.toggle('nav-open'));
    document.querySelectorAll('.nav button').forEach((button) => button.addEventListener('click', () => showView(button.dataset.view)));
    document.getElementById('saveSettingsBtn').addEventListener('click', saveSettings);
    document.getElementById('restoreDefaultsBtn').addEventListener('click', restoreDefaults);
    document.getElementById('resetStatsBtn').addEventListener('click', resetStatistics);
    document.getElementById('factoryResetBtn').addEventListener('click', factoryReset);
    document.getElementById('rebootBtn').addEventListener('click', rebootDevice);
    document.getElementById('testRunBtn').addEventListener('click', toggleTestRun);
    document.getElementById('otaUploadBtn').addEventListener('click', uploadFirmware);

    const initialView = (location.pathname.split('/').filter(Boolean)[0] || 'dashboard').toLowerCase();
    showView(views.includes(initialView) ? initialView : 'dashboard');

    async function refreshStatus() {
      try {
        const response = await api('/api/status');
        state.status = await response.json();
        updateDashboard(state.status);
      } catch (error) {
        connectionChip.textContent = 'AP offline';
      }
    }

    loadAll().catch((error) => showToast(`Failed to load dashboard: ${error.message}`));
    setInterval(refreshStatus, 2000);
  </script>
</body>
</html>
)rawliteral";

void WebServerManager::configure(StorageManager *storage,
                                 RelayManager *relays,
                                 SensorManager *sensors,
                                 AppSettings *settings,
                                 AppStatistics *statistics,
                                 RuntimeState *runtime,
                                 OtaManager *ota) {
  storage_ = storage;
  relays_ = relays;
  sensors_ = sensors;
  settings_ = settings;
  statistics_ = statistics;
  runtime_ = runtime;
  ota_ = ota;
  configured_ = true;
  registerRoutes();
}

void WebServerManager::start() {
  if (!configured_ || active_) {
    return;
  }

  server_.begin();
  active_ = true;
}

void WebServerManager::stop() {
  if (!active_) {
    return;
  }

  server_.end();
  active_ = false;
}

bool WebServerManager::active() const {
  return active_;
}

void WebServerManager::registerRoutes() {
  server_.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", INDEX_HTML);
  });

  server_.on("/dashboard", HTTP_GET, [this](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", INDEX_HTML);
  });

  server_.on("/settings", HTTP_GET, [this](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", INDEX_HTML);
  });

  server_.on("/ota", HTTP_GET, [this](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", INDEX_HTML);
  });

  server_.on("/system", HTTP_GET, [this](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", INDEX_HTML);
  });

  server_.on("/api/status", HTTP_GET, [this](AsyncWebServerRequest *request) {
    request->send(200, "application/json", buildStatusJson());
  });

  server_.on("/api/settings", HTTP_GET, [this](AsyncWebServerRequest *request) {
    request->send(200, "application/json", buildSettingsJson());
  });

  server_.on("/api/settings", HTTP_POST, [this](AsyncWebServerRequest *request) {
    AppSettings updated = *settings_;
    if (!parseSettingsRequest(request, updated)) {
      request->send(400, "application/json", "{\"ok\":false,\"error\":\"Invalid settings payload\"}");
      return;
    }

    if (!storage_->saveSettings(updated)) {
      request->send(500, "application/json", "{\"ok\":false,\"error\":\"Failed to save settings\"}");
      return;
    }

    *settings_ = updated;
    sensors_->applySettings(*settings_);
    request->send(200, "application/json", buildSettingsJson());
  });

  server_.on("/api/settings/restore-defaults", HTTP_POST, [this](AsyncWebServerRequest *request) {
    *settings_ = StorageManager::factorySettings();
    sensors_->applySettings(*settings_);
    if (!storage_->saveSettings(*settings_)) {
      request->send(500, "application/json", "{\"ok\":false,\"error\":\"Failed to restore defaults\"}");
      return;
    }
    request->send(200, "application/json", buildSettingsJson());
  });

  server_.on("/api/defaults", HTTP_GET, [this](AsyncWebServerRequest *request) {
    request->send(200, "application/json", buildDefaultsJson());
  });

  server_.on("/api/stats", HTTP_GET, [this](AsyncWebServerRequest *request) {
    request->send(200, "application/json", buildStatisticsJson());
  });

  server_.on("/api/system/reboot", HTTP_POST, [this](AsyncWebServerRequest *request) {
    request->send(200, "application/json", "{\"ok\":true,\"message\":\"Reboot scheduled\"}");
    if (ota_) {
      ota_->scheduleRestart(1200);
    } else {
      ESP.restart();
    }
  });

  server_.on("/api/system/test-run", HTTP_POST, [this](AsyncWebServerRequest *request) {
    if (!runtime_->setupModeActive) {
      request->send(409, "application/json", "{\"ok\":false,\"error\":\"Test run is available only in setup mode\"}");
      return;
    }

    bool enabled = !runtime_->testRunActive;
    if (request->hasParam("enabled", true)) {
      enabled = request->getParam("enabled", true)->value().toInt() != 0;
    }

    runtime_->testRunActive = enabled;

    if (!enabled) {
      runtime_->testRunStartedAtMs = 0;
      runtime_->testRunTimeoutSeconds = 0;
      runtime_->testRunRemainingSeconds = 0;
      runtime_->controlState = ControlState::Standby;
      runtime_->pumpActive = false;
      runtime_->alarmActive = false;
      runtime_->lowerSensorActive = false;
      runtime_->upperSensorActive = false;
      runtime_->fillRemainingSeconds = 0;
      runtime_->fillDeadlineMs = 0;
      runtime_->topOffStartedAtMs = 0;
      relays_->allOff();
      Serial.println("Setup mode test run stopped. Automation frozen again.");
    } else {
      runtime_->testRunStartedAtMs = millis();
      runtime_->testRunTimeoutSeconds = SETUP_TEST_RUN_TIMEOUT_MINUTES * 60UL;
      runtime_->testRunRemainingSeconds = runtime_->testRunTimeoutSeconds;
      Serial.println("Setup mode test run started. Automation is enabled for diagnostics.");
    }

    request->send(200, "application/json", "{\"ok\":true}");
  });

  server_.on("/api/system/reset-stats", HTTP_POST, [this](AsyncWebServerRequest *request) {
    *statistics_ = StorageManager::factoryStatistics();
    if (!storage_->saveStatistics(*statistics_)) {
      request->send(500, "application/json", "{\"ok\":false,\"error\":\"Failed to reset statistics\"}");
      return;
    }
    request->send(200, "application/json", buildStatisticsJson());
  });

  server_.on("/api/system/factory-reset", HTTP_POST, [this](AsyncWebServerRequest *request) {
    *settings_ = StorageManager::factorySettings();
    *statistics_ = StorageManager::factoryStatistics();
    sensors_->applySettings(*settings_);
    if (!storage_->saveSettings(*settings_) || !storage_->saveStatistics(*statistics_)) {
      request->send(500, "application/json", "{\"ok\":false,\"error\":\"Factory reset failed\"}");
      return;
    }
    request->send(200, "application/json", "{\"ok\":true,\"message\":\"Factory reset completed\"}");
    if (ota_) {
      ota_->scheduleRestart(1200);
    } else {
      ESP.restart();
    }
  });

  server_.on("/api/ota/update", HTTP_POST,
    [this](AsyncWebServerRequest *request) {
      const bool success = !ota_->hasError();
      if (success) {
        request->send(200, "application/json", "{\"ok\":true,\"message\":\"Firmware uploaded\"}");
        ota_->scheduleRestart(1500);
      } else {
        String response = "{\"ok\":false,\"error\":\"" + jsonEscape(ota_->errorMessage()) + "\"}";
        request->send(500, "application/json", response);
      }
    },
    [this](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
      if (index == 0) {
        ota_->handleUploadStart(request->contentLength(), filename);
      }

      if (!ota_->handleUploadChunk(data, len)) {
        return;
      }

      if (final) {
        ota_->handleUploadEnd();
      }
    }
  );

  server_.onNotFound([this](AsyncWebServerRequest *request) {
    request->redirect("/");
  });
}

String WebServerManager::buildStatusJson() const {
  String json = "{";
  json += "\"device\":\"" + String(DEVICE_NAME) + "\",";
  json += "\"setupModeActive\":" + String(runtime_->setupModeActive ? "true" : "false") + ",";
  json += "\"testRunActive\":" + String(runtime_->testRunActive ? "true" : "false") + ",";
  json += "\"testRunRemainingSeconds\":" + String(runtime_->testRunRemainingSeconds) + ",";
  json += "\"bootResetReason\":\"" + jsonEscape(runtime_->bootResetReason) + "\",";
  json += "\"previousResetReason\":\"" + jsonEscape(runtime_->previousResetReason) + "\",";
  json += "\"bootCount\":" + String(runtime_->bootCount) + ",";
  json += "\"controlState\":" + String(static_cast<uint8_t>(runtime_->controlState)) + ",";
  json += "\"wifiActive\":" + String(runtime_->wifiActive ? "true" : "false") + ",";
  json += "\"webServerActive\":" + String(runtime_->webServerActive ? "true" : "false") + ",";
  json += "\"pumpActive\":" + String(runtime_->pumpActive ? "true" : "false") + ",";
  json += "\"lastPumpStopReason\":\"" + String(pumpStopReasonToText(runtime_->lastPumpStopReason)) + "\",";
  json += "\"alarmActive\":" + String(runtime_->alarmActive ? "true" : "false") + ",";
  json += "\"lowerSensorActive\":" + String(runtime_->lowerSensorActive ? "true" : "false") + ",";
  json += "\"upperSensorActive\":" + String(runtime_->upperSensorActive ? "true" : "false") + ",";
  json += "\"fillRemainingSeconds\":" + String(runtime_->fillRemainingSeconds) + ",";
  json += "\"fillDeadlineMs\":" + String(runtime_->fillDeadlineMs) + ",";
  json += "\"totalPumpRuntimeSeconds\":" + String(statistics_->totalPumpRuntimeSeconds) + ",";
  json += "\"totalWaterLiters\":" + String(statistics_->totalWaterLiters, 3) + ",";
  json += "\"fillCycles\":" + String(statistics_->fillCycles) + ",";
  json += "\"alarmCount\":" + String(statistics_->alarmCount) + ",";
  json += "\"settings\":{";
  json += "\"fillTimeoutMinutes\":" + String(settings_->fillTimeoutMinutes) + ",";
  json += "\"pumpFlowLpm\":" + String(settings_->pumpFlowLpm, 2) + ",";
  json += "\"pumpRelay\":" + String(settings_->pumpRelay) + ",";
  json += "\"alarmRelay\":" + String(settings_->alarmRelay) + ",";
  json += "\"lowerSensorLogic\":" + String(static_cast<uint8_t>(settings_->lowerSensorLogic)) + ",";
  json += "\"upperSensorLogic\":" + String(static_cast<uint8_t>(settings_->upperSensorLogic)) + "},";
  json += "\"relays\":[";
  for (uint8_t relay = 1; relay <= 4; ++relay) {
    json += String(relays_->relayState(relay) ? "true" : "false");
    if (relay < 4) {
      json += ",";
    }
  }
  json += "]}";
  return json;
}

String WebServerManager::buildSettingsJson() const {
  String json = "{";
  json += "\"fillTimeoutMinutes\":" + String(settings_->fillTimeoutMinutes) + ",";
  json += "\"pumpFlowLpm\":" + String(settings_->pumpFlowLpm, 2) + ",";
  json += "\"pumpRelay\":" + String(settings_->pumpRelay) + ",";
  json += "\"alarmRelay\":" + String(settings_->alarmRelay) + ",";
  json += "\"lowerSensorLogic\":" + String(static_cast<uint8_t>(settings_->lowerSensorLogic)) + ",";
  json += "\"upperSensorLogic\":" + String(static_cast<uint8_t>(settings_->upperSensorLogic));
  json += "}";
  return json;
}

String WebServerManager::buildStatisticsJson() const {
  String json = "{";
  json += "\"totalPumpRuntimeSeconds\":" + String(statistics_->totalPumpRuntimeSeconds) + ",";
  json += "\"totalWaterLiters\":" + String(statistics_->totalWaterLiters, 3) + ",";
  json += "\"fillCycles\":" + String(statistics_->fillCycles) + ",";
  json += "\"alarmCount\":" + String(statistics_->alarmCount);
  json += "}";
  return json;
}

String WebServerManager::buildDefaultsJson() const {
  const AppSettings defaults = StorageManager::factorySettings();
  String json = "{";
  json += "\"fillTimeoutMinutes\":" + String(defaults.fillTimeoutMinutes) + ",";
  json += "\"pumpFlowLpm\":" + String(defaults.pumpFlowLpm, 2) + ",";
  json += "\"pumpRelay\":" + String(defaults.pumpRelay) + ",";
  json += "\"alarmRelay\":" + String(defaults.alarmRelay) + ",";
  json += "\"lowerSensorLogic\":" + String(static_cast<uint8_t>(defaults.lowerSensorLogic)) + ",";
  json += "\"upperSensorLogic\":" + String(static_cast<uint8_t>(defaults.upperSensorLogic));
  json += "}";
  return json;
}

bool WebServerManager::parseSettingsRequest(AsyncWebServerRequest *request, AppSettings &settings) const {
  if (!request->hasParam("fillTimeoutMinutes", true) ||
      !request->hasParam("pumpFlowLpm", true) ||
      !request->hasParam("lowerSensorLogic", true) ||
      !request->hasParam("upperSensorLogic", true) ||
      !request->hasParam("pumpRelay", true) ||
      !request->hasParam("alarmRelay", true)) {
    return false;
  }

  settings.fillTimeoutMinutes = static_cast<uint16_t>(request->getParam("fillTimeoutMinutes", true)->value().toInt());
  settings.pumpFlowLpm = request->getParam("pumpFlowLpm", true)->value().toFloat();
  settings.lowerSensorLogic = static_cast<SensorLogic>(request->getParam("lowerSensorLogic", true)->value().toInt());
  settings.upperSensorLogic = static_cast<SensorLogic>(request->getParam("upperSensorLogic", true)->value().toInt());
  settings.pumpRelay = static_cast<uint8_t>(request->getParam("pumpRelay", true)->value().toInt());
  settings.alarmRelay = static_cast<uint8_t>(request->getParam("alarmRelay", true)->value().toInt());
  return true;
}

String WebServerManager::jsonEscape(const String &value) {
  String out;
  out.reserve(value.length() + 8);
  for (size_t index = 0; index < value.length(); ++index) {
    const char c = value[index];
    switch (c) {
      case '"': out += "\\\""; break;
      case '\\': out += "\\\\"; break;
      case '\b': out += "\\b"; break;
      case '\f': out += "\\f"; break;
      case '\n': out += "\\n"; break;
      case '\r': out += "\\r"; break;
      case '\t': out += "\\t"; break;
      default:
        if (static_cast<uint8_t>(c) < 0x20) {
          out += '?';
        } else {
          out += c;
        }
        break;
    }
  }
  return out;
}