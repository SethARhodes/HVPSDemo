/*
 * This file is part of the HVPS demonstration project.
 *
 * Developed for the National Aeronautics and Space Administration (NASA)
 * by High Voltage Research and Development (HVRND) under NASA contract.
 * 
 * Copyright (c) 2024 HVRND LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <SPI.h>
#include <RA8875.h>
#include "ui_elements.h"
#include "Images/battery.h"
#include "Images/HVRND.h"
#include "Images/NASA.h"
#include "Images/CLPS.h"
#include "Fonts/nasa_14.c"
#include "Fonts/nasa_36.c"

#define RA8875_INT 3 // Interrupt pin for touch
#define RA8875_CS 2  // Chip select pin for SPI RA8875
#define RA8875_RESET 14 // Reset pin for RA8875

#define meter_radius 50 // Minimum radius is 50

RA8875 tft = RA8875(RA8875_CS, RA8875_RESET); // Declare RA8875 object
UI ui = UI(); // Declare UI object

uint16_t tx, ty; // Touch coordinates
uint8_t currentTab = 0; // Current tab index
unsigned long lastTouchTime = 0; // Timestamp of the last touch event
const unsigned long touchDebounceDelay = 200; // Debounce delay in milliseconds
uint16_t ringMeterValue = 0; // Value of the ring meter
unsigned long lastUpdateTime = 0; // Timestamp for ring meter update
const char* tabs[] = {"CONTROL PANEL", "ABOUT"}; // Tab names
uint8_t waveform = 0; // Waveform selection

// Test variables
bool testState = false;
uint16_t testValue = 0;
bool testStart = false;

// Define the TestEnum
enum TestEnum { Option1, Option2, Option3 };
const char* testEnumOptions[] = {"Option1", "Option2", "Option3"};
const uint8_t num_options = 3;
uint8_t sel_option = 0;
bool expanded = false;

// Function prototypes
void drawInterface();
void drawTabBar();
void handleTouch();
void showTab1Content();
void showTab2Content();
void showTab3Content();

void setup() {
  Serial.begin(9600);
  Serial.println("RA8875 start");

  tft.begin(Adafruit_800x480); // Initialize RA8875
  tft.useINT(RA8875_INT); // Use generic interrupt helper for internal resistive touch
  tft.touchBegin(); // Enable touch support for RA8875
  if (!tft.touchCalibrated()) { // If not calibrated
    Serial.println("Maybe you better calibrate first!"); // Print message
  }
  tft.enableISR(true); // Enable interrupt
  drawTabBar(); // Draw the tab bar
  drawInterface(); // Draw initial interface
  ui.drawOffWaveform();
}

void loop() {
  // Handle touch input with debounce
  if (tft.touched()) {
    unsigned long currentTime = millis();
    if (currentTime - lastTouchTime > touchDebounceDelay) {
      tft.touchReadPixel(&tx, &ty); // Get touch coordinates
      handleTouch(); // Handle touch input
      lastTouchTime = currentTime; // Update the timestamp of the last touch event
    }
  }

  // Update ring meter if Tab 1 is active
  if (currentTab == 0) {
    unsigned long currentTime = millis();
    if (currentTime - lastUpdateTime > 100) { // Update every 100ms
      ringMeterValue = (ringMeterValue + 50) % 4001; // Increment value and wrap around at 4000
      // Draw a ring meter
      tft.ringMeter(ringMeterValue, 0, 4000, 400, 250, meter_radius, "none", 0, tft.Color565(169, 168, 240), 150, 10);
      // Draw the ring meter value in the center of the ring meter blanking out the previous value each time
      // The ring meter coordinates are specified by the top right corner of the bounding box
      tft.setFont(INT);
      tft.setTextColor(RA8875_BLACK, tft.Color565(169, 168, 240));
      tft.setCursor(436, 280);
      tft.setFontScale(0);
      tft.print(ringMeterValue);
      tft.setCursor(433, 300);
      tft.setFont(&nasa_36);
      tft.setTextColor(RA8875_RED);
      tft.print("V");
      tft.setFont(INT);
      // reset the active window
      tft.setActiveWindow();
      lastUpdateTime = currentTime;
    }
  }
}

void drawInterface() {
  switch (currentTab) {
    case 0:
      showTab1Content();
      break;
    case 1:
      showTab2Content();
      break;
  }
}

void drawTabBar() {
  uint16_t xPos = 0;
  uint16_t tabPadding = 20; // Padding around the text

  tft.setActiveWindow(0, tft.width(), 0, 51); // Set active window for tab bar
  for (int i = 0; i < 25; i++) {
    tft.drawFastHLine(0, i, 800, tft.colorInterpolation(138, 136, 235, 235, 208, 136, i, 25));
  }
  for (int i = 0; i < 25; i++) {
    tft.drawFastHLine(0, 25+i, 800, tft.colorInterpolation(235, 208, 136, 138, 136, 235, i, 25));
  }
  tft.drawFastHLine(0, 51, 800, RA8875_BLACK);

  for (uint8_t i = 0; i < 2; i++) {
    tft.setCursor(xPos + tabPadding / 2, 15);
    tft.setFont(&nasa_14);
    tft.setTextColor(currentTab == i ? RA8875_RED : RA8875_BLACK);
    tft.print(tabs[i]);

    uint16_t textWidth = tft.getCursorX() - xPos + tabPadding / 2;
    xPos += textWidth + tabPadding;

    if (i < 2) { // Draw divider line between tabs
      tft.drawFastVLine(xPos - tabPadding/2, 0, 50, RA8875_BLACK);
    }
  }
  ui.drawArray(image_data_NASA, 7800, 156, tft.width()-156-132, 0);
  tft.fillRect(tft.width()-132, 0, 132, 50, RA8875_WHITE);
  ui.drawArray(image_data_HVRND, 5940, 132, tft.width()-132, 3);
}

void handleTouch() {
  uint16_t xPos = 0;
  uint16_t tabPadding = 20; // Padding around the text

  // Calculate the touch regions for each tab
  for (uint8_t i = 0; i < 2; i++) {
    tft.setCursor(xPos + tabPadding / 2, 15);
    tft.setFont(&nasa_14);
    tft.setTextColor(currentTab == i ? RA8875_RED : RA8875_BLACK);
    tft.print(tabs[i]);

    uint16_t textWidth = tft.getCursorX() - xPos + tabPadding / 2;
    xPos += textWidth + tabPadding;

    if (ty <= 50 && tx <= xPos) {
      currentTab = i;
      tft.setActiveWindow(0, tft.width(), 50, tft.height()); // Set active window for content
      tft.fillWindow(RA8875_BLACK); // Fill content area with black
      drawTabBar(); // Redraw tab bar
      drawInterface(); // Redraw interface when tab changes
      return;
    }
  }
  // Add more touch regions for other controls here
  if (currentTab == 0) {
    // Handle touch for buttons
    if (ui.handleButtonTouch(tx, ty, 20, tft.height() - 60 - 15 - 5, 100, 60, testStart ? "Stop" : "Start", testStart)) {
      ui.drawButton(20, tft.height() - 60 - 15 - 5, 100, 60, testStart ? "Stop" : "Start", testStart);
      testStart ? ui.drawSwitchingWaveform() : ui.drawOffWaveform();
      return;
    }
    // Handle touch for Toggle
    if (ui.handleToggleTouch(tx, ty, 550, 100, testState)) {
      return;
    }
    // Handle touch for Enum Dropdown
    if (ui.handleEnumDropdownTouch(tx, ty, 550, 280, 200, 40, testEnumOptions, num_options, sel_option, expanded)) {
      return;
    }
    // Handle touch for Slider
    if (ui.handleSliderTouch(tx, ty, 562, 370, tft.width()/4 - 17, testValue, 0, 4000)) {
      return;
    }
    // Handle touch for Stepper
    if (ui.handleStepperTouch(tx, ty, 590, 400, testValue)) {
      return;
    }
  }
}

void showTab1Content() {
  // Content for Tab 1
  tft.setFont(INT);
  tft.useLayers(false);
  tft.setActiveWindow(0, tft.width(), 51, tft.height());
  for (int i = 0; i < tft.width(); i++) {
    tft.drawFastVLine(i, 51, 430, tft.colorInterpolation(138, 136, 235, 235, 208, 136, i, tft.width()));
  }
  // Draw a rounded rectangle as the background for the buttons
  tft.drawRoundRect(tft.width() - tft.width()/3 -1, 66, tft.width()/3 - 15, tft.height() - 51 - 15*2, 10, RA8875_BLACK);
  tft.fillRoundRect(tft.width() - tft.width()/3, 67, tft.width()/3 - 17, tft.height() - 51 - 16*2, 10, tft.Color565(169, 168, 240));
  // Draw a toggle button
  ui.drawToggle(550, 100, testState);
  tft.setTextColor(RA8875_BLACK);
  tft.setCursor(630, 105);
  tft.print("Arm Output");
  // Draw a stepper control
  ui.drawStepper(590, 400, testValue);
  // Draw a slider control
  ui.drawSlider(562, 370, tft.width()/4 - 17, 40, testValue, 0, 4000);
  // Draw a enum dropdown control
  ui.drawEnumDropdown(550, 280, 200, 40, testEnumOptions[sel_option], testEnumOptions, num_options, sel_option, expanded);

  tft.drawRoundRect(15, tft.height() - 85, tft.width() - tft.width()/3 - 28, 100 - 15*2, 10, RA8875_BLACK);
  tft.fillRoundRect(16, tft.height() - 85 + 1, tft.width() - tft.width()/3 - 30, 100 - 16*2, 10, tft.Color565(169, 168, 240));
  // Draw a start button and a stop button
  ui.drawButton(20, tft.height() - 60 - 15 - 5, 100, 60, testStart ? "Stop" : "Start", testStart);

  tft.drawRoundRect(15, 66, tft.width() - tft.width()/3 - 28, 345 - 15*2, 10, RA8875_BLACK);
  tft.fillRoundRect(16, 67, tft.width() - tft.width()/3 - 30, 345 - 16*2, 10, tft.Color565(169, 168, 240));
  testStart ? ui.drawSwitchingWaveform() : ui.drawOffWaveform();


  // Add more controls for Tab 1 here
}

void showTab2Content() {
  // Content for Tab 2
  tft.setFont(INT);
  tft.useLayers(false);
  tft.setActiveWindow(0, tft.width(), 50, tft.height());
  for (int i = 0; i < tft.width(); i++) {
    tft.drawFastVLine(i, 51, 430, tft.colorInterpolation(138, 136, 235, 235, 208, 136, i, tft.width()));
  }
  tft.setActiveWindow(20, tft.width()-20, 50, tft.height()-20);
  tft.setCursor(20, 70);
  tft.setTextColor(RA8875_BLACK);
  tft.print("The Electrostatics and Surface Physics Lab (ESPL) at NASA focuses on understanding and controlling the interactions between spacecraft surfaces and the space environment. This includes studying how electrostatic charges accumulate on spacecraft and their surfaces and how these charges can impact spacecraft performance and safety. The lab also examines how surface properties influence the adhesion of particles and contaminants, which is critical for the operation of equipment and instruments in space.");
  tft.setCursor(20, 200);
  tft.print("One significant development from the ESPL is the Electrodynamic Dust Shield (EDS). The EDS technology was created to address the issue of dust accumulation on spacecraft surfaces, particularly on solar panels and other critical components. Dust accumulation can significantly reduce the efficiency of solar panels and obscure sensors and cameras, impacting the overall mission performance. The EDS uses electrostatic forces to repel dust particles, keeping surfaces clean without the need for mechanical cleaning methods. This technology enhances the longevity and reliability of spacecraft and rovers, especially in dusty environments like the Moon and Mars.");

  ui.drawArray(image_data_battery, 26600, 200, 20, tft.height()-153);
  ui.drawArray(image_data_CLPS, 26467, 199, 220, tft.height()-153);

  // Add more controls for Tab 2 here
}
