# Bass-Hole Setup Guide

This guide lists the tools, extensions, and libraries required to set up the development environment for the Bass-Hole project on the ESP32 Cheap Yellow Display (CYD).

## Prerequisites

- **VS Code**: [Download and install Visual Studio Code](https://code.visualstudio.com/).
- **Git**: Ensure Git is installed and accessible from the command line.

## VS Code Extensions

The following extension is required:

- **PlatformIO IDE** (`platformio.platformio-ide`): This is the primary build system and dependency manager used for the project.

*Note: The project is configured to recommend this extension automatically when you open the folder in VS Code.*

## Libraries & Dependencies

This project uses **PlatformIO** to manage libraries. All dependencies are defined in `platformio.ini` and will be automatically downloaded when you run a build.

### Core Dependencies (Automatic)
The following libraries are specified in `platformio.ini`:
- **TFT_eSPI** by Bodmer (v2.5.43) - For display driver support.
- **XPT2046_Touchscreen** by Paul Stoffregen (Git version) - For touch screen support.

### Hardware Configuration
The project is configured for the **ESP32 CYD (Cheap Yellow Display)**. The `platformio.ini` file contains specific build flags to configure the display driver (`ILI9341_DRIVER`) and pin mappings.

## Getting Started

1. **Clone the Repository**:
   ```bash
   git clone <repository-url>
   code Bass-Hole
   ```

2. **Install Extensions**: 
   When you open the project in VS Code, you should see a prompt to install the recommended extensions. If not, search for "PlatformIO IDE" in the Extensions Marketplace and install it.

3. **Build the Project**:
   Click the PlatformIO icon in the sidebar, open the "Project Tasks", and select **Build**. PlatformIO will automatically download the toolchains and libraries listed above.

4. **Upload**:
   Connect your ESP32 CYD device via USB and select **Upload**.
