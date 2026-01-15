---
description: Build and Upload using PlatformIO in Cursor
---

This workflow helps you manage the Bass Hole project using the PlatformIO IDE extension in Cursor.

### Building the Project

1. Look for the **Checkmark** icon (✓) in the bottom status bar or the PlatformIO sidebar.
2. Click it to compile the code. PlatformIO will automatically download `TFT_eSPI` and `XPT2046_Touchscreen`.

### Uploading to ESP32 CYD

1. Connect your CYD to **COM3** (as configured in `platformio.ini`).
2. Click the **Right Arrow** icon (→) in the status bar or PlatformIO sidebar.
3. **Important:** If the upload fails to connect, hold the **BOOT** button on the CYD while the upload starts, then release it once flashing begins.
4. The code will be flashed to your device.

### Monitoring Output

1. Click the **Plug** icon in the status bar to open the Serial Monitor.
2. The speed is set to **115200** baud.

> [!TIP]
> If you need to change the COM port, update the `upload_port` line in [platformio.ini](file:///c:/Users/wests/OneDrive/Documents/GitHub/Bass-Hole/platformio.ini).

### Troubleshooting

- **Library Errors**: If you get "Missing header" errors, click the **PlatformIO: Core** -> **Rebuild IntelliSense Index** option in the sidebar.
- **Upload Failures**: Ensure no other programs (like Arduino IDE) are using COM3.
