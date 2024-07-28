# HVPSDemo

This project contains the source code for a High Voltage Power Supply (HVPS) demo application. The application is designed to manage and control high voltage outputs, providing an interface for interaction via a touchscreen display.

## Features

- **Stepper Control**: Increment or decrement values using stepper controls.
- **Slider Control**: Adjust values using sliders for precise control.
- **Dropdown Menu**: Select options from a dropdown menu.
- **Tab Interface**: Navigate between different tabs to view and interact with different controls and information.

## File Descriptions

### `ui_elements.h`

This header file defines the `UI` class, which encapsulates various UI elements and their functionalities. It includes methods for drawing and handling stepper controls, sliders, and dropdown menus.

### `ui_elements.cpp`

This source file implements the `UI` class methods defined in `ui_elements.h`. It includes the actual drawing and touch handling logic for the UI elements.

### `HVPS.ino`

This file contains the main Arduino sketch for the HVPS demo application. It initializes the display, sets up the UI, and manages the main loop for handling user interactions.

## Getting Started

### Prerequisites

- Arduino IDE
- RA8875 TFT Display Library
- Adafruit GFX Library

### Installation

1. Clone this repository:
   ```sh
   git clone git@github.com:SethARhodes/HVPSDemo.git
   cd HVPSDemo
   ```

2. Open HVPS.ino in the Arduino IDE.

3. Connect your hardware (TFT display, microcontroller, etc.) according to your setup.

4. Upload the sketch to your microcontroller.

### Usage

Once the sketch is uploaded, the application will start running on your microcontroller. You can interact with the UI via the touchscreen display to control the high voltage outputs and view different tabs.

### Contributing

Contributions are welcome! Please submit a pull request or open an issue to discuss any changes or improvements.

### License

This project is licensed under the MIT License. See the LICENSE file for details.
