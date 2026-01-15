# Development History & Lessons Learned

## PlatformIO Migration (Jan 2026)
**Goal**: Move from Arduino IDE/ESP-IDF to PlatformIO for better dependency management and build speed.

### Challenge: Python 3.13 Incompatibility
PlatformIO's build tool (SCons) relies on the `pipes` module, which was removed in Python 3.13. This caused `UnknownPackageError` during initialization.
- **Failed Attempt**: Shimming `pipes.py` manually.
- **Failed Attempt**: Renaming Python 3.13 folders temporarily (`BUILD_WORKAROUND.md`).
- **Solution**: Installed Python 3.11 side-by-side and executed PlatformIO via `python -m platformio run`.

### Challenge: Dependency Registry Issues
The `XPT2046_Touchscreen` library failed to install from the registry on Windows/AMD64.
- **Solution**: Switched to Git dependency in `platformio.ini`: `https://github.com/PaulStoffregen/XPT2046_Touchscreen.git`.

### Challenge: Compiler Flags
Initial `platformio.ini` had malformed flags (e.g., `-SPI_FREQUENCY=...` without `-D`).
- **Solution**: Corrected to `-DSPI_FREQUENCY=...`.

### Working CYD Configuration
See `SETUP_CYD.md` for the reliable `platformio.ini` template.

### Challenge: Antigravity IDE & Open VSX Registry
Antigravity IDE uses the Open VSX registry, which does not host the `PlatformIO IDE` or `ms-vscode.cpptools` extensions due to Microsoft licensing restrictions.
- **Symptom**: "Extension deprecated" or "Missing dependencies" errors.
- **Solution**: Manually downloaded official `.vsix` files from the Visual Studio Marketplace and installed them via the Antigravity CLI:
  ```powershell
  # PlatformIO IDE
  & ".../Antigravity/bin/antigravity.cmd" --install-extension platformio.vsix
  # C/C++ Tools (win32-x64)
  & ".../Antigravity/bin/antigravity.cmd" --install-extension cpptools.vsix
  ```
