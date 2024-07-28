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

#include "ui_elements.h"
#include <SPI.h>
#include <RA8875.h>

extern RA8875 tft; // Declare RA8875 object

UI::UI() {
  // Constructor
}

extern void drawInterface();

void UI::drawTab(uint16_t x, uint16_t y, uint16_t width, uint16_t height, String text, bool selected) {
  // Draw a tab
  tft.drawRoundRect(x, y, width, height, 10, RA8875_BLACK);
  tft.fillRoundRect(x, y, width, height, 10, selected ? RA8875_WHITE : RA8875_LIGHT_GREY);
  tft.setTextColor(selected ? RA8875_BLACK : RA8875_WHITE);
  tft.setCursor(x + width/2, y + height/2, true);
  tft.print(text);
}

void UI::drawButton(uint16_t x, uint16_t y, uint16_t width, uint16_t height, String text, bool selected) {
  // Draw a button
  tft.drawRoundRect(x, y, width, height, 10, RA8875_BLACK);
  tft.fillRoundRect(x+1, y+1, width-2, height-2, 10, selected ? RA8875_RED : RA8875_GREEN);
  tft.setFont(INT);
  tft.setTextColor(RA8875_BLACK);
  tft.setCursor(x + width/2 - text.length()*5, y + height/2 - 10);
  tft.print(text);
}

bool UI::handleButtonTouch(uint16_t tx, uint16_t ty, uint16_t x, uint16_t y, uint16_t width, uint16_t height, String text, bool &selected) {
  if (tx >= x && tx <= x + width && ty >= y && ty <= y + height) {
    selected = !selected;
    delay(100);
    drawButton(x, y, width, height, text, selected);
    return true;
  }
  return false;
}

void UI::drawToggle(uint16_t x, uint16_t y, bool state) {
  // Draw a toggle switch
  if (state) {
    tft.drawRoundRect(x, y-1, 70, 40, 20, RA8875_BLACK);
    tft.fillRoundRect(x, y, 70, 40, 20, RA8875_GREEN);
    tft.drawCircle(x + 50, y + 21, 17, RA8875_BLACK);
    tft.fillCircle(x + 50, y + 20, 17, RA8875_WHITE);
  } else {
    tft.drawRoundRect(x, y-1, 70, 40, 20, RA8875_BLACK);
    tft.fillRoundRect(x, y, 70, 40, 20, RA8875_LIGHT_GREY);
    tft.drawCircle(x + 19, y + 21, 17, RA8875_BLACK);
    tft.fillCircle(x + 19, y + 20, 17, RA8875_WHITE);
  }
}

bool UI::handleToggleTouch(uint16_t tx, uint16_t ty, uint16_t x, uint16_t y, bool &state) {
  if (tx >= x && tx <= x + 70 && ty >= y && ty <= y + 40) {
    state = !state;
    drawToggle(x, y, state);
    return true;
  }
  return false;
}

void UI::drawStepper(uint16_t x, uint16_t y, int val) {
  // Draw a stepper control
  tft.drawRoundRect(x, y-1, 120, 40, 20, RA8875_BLACK);
  tft.fillRoundRect(x, y, 120, 40, 20, RA8875_WHITE);
  tft.drawCircle(x + 19, y + 20, 17, RA8875_BLACK);
  tft.fillCircle(x + 19, y + 19, 17, RA8875_LIGHT_GREY);
  tft.drawCircle(x + 100, y + 20, 17, RA8875_BLACK);
  tft.fillCircle(x + 100, y + 19, 17, RA8875_LIGHT_GREY);
  tft.setFont(INT);
  tft.setTextColor(RA8875_BLACK);
  tft.setCursor(x + 16, y + 10);
  tft.print("-");
  tft.setCursor(x + 97, y + 10);
  tft.print("+");
  tft.setCursor(x + 60, y + 19, true);
  tft.print(val);
}

bool UI::handleStepperTouch(uint16_t tx, uint16_t ty, uint16_t x, uint16_t y, uint16_t &val) {
  if (tx >= x && tx <= x + 120 && ty >= y && ty <= y + 40) {
    if (tx >= x && tx <= x + 40) {
      val--;
    } else if (tx >= x + 80 && tx <= x + 120) {
      val++;
    }
    drawStepper(x, y, val);
    return true;
  }
  return false;
}

void UI::drawSlider(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t value, uint16_t min, uint16_t max) {
  // Draw a slider control
  tft.drawRoundRect(x-height/3, y-12, width+(3*height/4), 2*height/3+3, height/3, RA8875_BLACK);
  tft.fillRoundRect(x-height/3, y-11, width+(3*height/4), 2*height/3+3, height/3, RA8875_WHITE);
  tft.drawCircle(x + map(value, min, max, 0, width), y+2, height/3, RA8875_WHITE);
  tft.drawCircle(x + map(value, min, max, 0, width), y+4, height/3, RA8875_BLACK);
  tft.fillCircle(x + map(value, min, max, 0, width), y+3, height/3, RA8875_LIGHT_GREY);
  for (int i = 0; i < 12; i++) {
    tft.drawCircle(x + map(value, min, max, 0, width), y+3, height/3-2-i, tft.Color565(255, 255, 255-i*20));
  }
}

bool UI::handleSliderTouch(uint16_t tx, uint16_t ty, uint16_t x, uint16_t y, uint16_t width, uint16_t &value, uint16_t min, uint16_t max) {
  if (tx >= x && tx <= x + width && ty >= y && ty <= y + 40) {
    value = map(tx, x, x + width, min, max);
    drawSlider(x, y, width, 40, value, min, max);
    return true;
  }
  return false;
}

void UI::drawEnumDropdown(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const char* text, const char* options[], uint8_t numOptions, uint8_t selectedOption, bool expanded) {
  // Draw a dropdown control
  tft.drawRoundRect(x, y, width, height, 10, RA8875_BLACK);
  tft.fillRoundRect(x, y+1, width, height, 10, RA8875_WHITE);
  tft.setFont(INT);
  tft.setTextColor(RA8875_BLACK);
  tft.setCursor(x + 10, y + 10);
  tft.print(text);
  tft.setCursor(x + width - 30, y + 10);
  //tft.print(options[selectedOption]);
  tft.fillTriangle(x + width - 20, y + height/2-5, x + width - 10, y + height/2-5, x + width - 15, y + height/2+5, RA8875_BLACK);
  if (expanded) {
    for (int i = 0; i < numOptions; i++) {
      tft.drawRect(x, y + 10 + (i+1)*height, width, height, RA8875_BLACK);
      tft.fillRect(x, y + 10 + (i+1)*height, width, height, RA8875_WHITE);
      tft.setFont(INT);
      tft.setTextColor(RA8875_BLACK);
      tft.setCursor(x + 10, y + 10 + (i+1)*height + 10);
      tft.print(options[i]);
    }
  }
}

bool UI::handleEnumDropdownTouch(uint16_t tx, uint16_t ty, uint16_t x, uint16_t y, uint16_t width, uint16_t height, const char* options[], uint8_t numOptions, uint8_t &selectedOption, bool &expanded) {
  if (tx >= x && tx <= x + width && ty >= y && ty <= y + height) {
    expanded = !expanded;
    return true;
  }
  if (expanded) {
    for (int i = 0; i < numOptions; i++) {
      if (tx >= x && tx <= x + width && ty >= y + 10 + (i+1)*height && ty <= y + 10 + (i+2)*height) {
        selectedOption = i;
        expanded = false;
        drawInterface();
        return true;
      }
    }
  }
  return false;
}

void UI::drawSwitchingWaveform(){
  // Draw a graph background covering the left half of this section
  tft.drawRoundRect(21, 72, tft.width() - tft.width()/2 - 38, 345 - 20*2, 10, RA8875_WHITE);
  tft.fillRoundRect(20, 71, tft.width() - tft.width()/2 - 38, 345 - 20*2, 10, RA8875_BLACK);
  // Draw the dark green grid lines
  for (int i = 1; i < 10; i++) {
    tft.drawFastHLine(21, 72 + i * 30, tft.width() - tft.width()/2 - 39, tft.Color565(0, 50, 0));
  }
  for (int i = 1; i < 12; i++) {
    tft.drawFastVLine(21 + i * 30, 72, 345 - 20*2-1, tft.Color565(0, 50, 0));
  }
  // Set active window for the graph area
  tft.setActiveWindow(21, tft.width()/2 - 19, 72, 345 - 20*2);
  // Draw a thick yellow trace
  for (int i = 0; i < 7; i++) {
    tft.drawLineAngle(21+i*120, 282, 1, 120, tft.Color565(255, 255, 0));
    tft.drawLineAngle(21+i*120+60, 282, 0, 120, tft.Color565(255, 255, 0));
  }
  for (int i = 0; i < 7; i++) {
    tft.drawLineAngle(21+i*120+60, 282, 90, 60, tft.Color565(255, 255, 0));
    tft.drawLineAngle(21+i*120+2, 162, 90, 58, tft.Color565(255, 255, 0));
  }
  // Draw a thick blue trace 180 degrees out of phase with the yellow trace
  for (int i = 0; i < 7; i++) {
    tft.drawLineAngle(21+i*120+60, 282, 1, 120, tft.Color565(50, 50, 255));
    tft.drawLineAngle(21+i*120, 282, 0, 120,    tft.Color565(50, 50, 255));
  }
  for (int i = 0; i < 7; i++) {
    tft.drawLineAngle(21+i*120+2, 282, 90, 60,  tft.Color565(50, 50, 255));
    tft.drawLineAngle(21+i*120+60, 162, 90, 58, tft.Color565(50, 50, 255));
  }
  // Reset the active window
  tft.setActiveWindow();
};

void UI::drawPolarization1(){
  // Draw a graph background covering the left half of this section
  tft.drawRoundRect(21, 72, tft.width() - tft.width()/2 - 38, 345 - 20*2, 10, RA8875_WHITE);
  tft.fillRoundRect(20, 71, tft.width() - tft.width()/2 - 38, 345 - 20*2, 10, RA8875_BLACK);
  // Draw the dark green grid lines
  for (int i = 1; i < 10; i++) {
    tft.drawFastHLine(21, 72 + i * 30, tft.width() - tft.width()/2 - 39, tft.Color565(0, 50, 0));
  }
  for (int i = 1; i < 12; i++) {
    tft.drawFastVLine(21 + i * 30, 72, 345 - 20*2-1, tft.Color565(0, 50, 0));
  }
  // Set active window for the graph area
  tft.setActiveWindow(21, tft.width()/2 - 19, 72, 345 - 20*2);
  // Draw a thick yellow trace
  tft.drawFastHLine(21, 162, 7*120, tft.Color565(255, 255, 0));
  // Draw a thick blue trace
  tft.drawFastHLine(21, 282, 7*120, tft.Color565(50, 50, 255));
  // Reset the active window
  tft.setActiveWindow();
};

void UI::drawPolarization2(){
  // Draw a graph background covering the left half of this section
  tft.drawRoundRect(21, 72, tft.width() - tft.width()/2 - 38, 345 - 20*2, 10, RA8875_WHITE);
  tft.fillRoundRect(20, 71, tft.width() - tft.width()/2 - 38, 345 - 20*2, 10, RA8875_BLACK);
  // Draw the dark green grid lines
  for (int i = 1; i < 10; i++) {
    tft.drawFastHLine(21, 72 + i * 30, tft.width() - tft.width()/2 - 39, tft.Color565(0, 50, 0));
  }
  for (int i = 1; i < 12; i++) {
    tft.drawFastVLine(21 + i * 30, 72, 345 - 20*2-1, tft.Color565(0, 50, 0));
  }
  // Set active window for the graph area
  tft.setActiveWindow(21, tft.width()/2 - 19, 72, 345 - 20*2);
  // Draw a thick yellow trace
  tft.drawFastHLine(21, 282, 7*120, tft.Color565(255, 255, 0));
  // Draw a thick blue trace
  tft.drawFastHLine(21, 162, 7*120, tft.Color565(50, 50, 255));
  // Reset the active window
  tft.setActiveWindow();
};

void UI::drawOffWaveform(){
  // Draw a graph background covering the left half of this section
  tft.drawRoundRect(21, 72, tft.width() - tft.width()/2 - 38, 345 - 20*2, 10, RA8875_WHITE);
  tft.fillRoundRect(20, 71, tft.width() - tft.width()/2 - 38, 345 - 20*2, 10, RA8875_BLACK);
  // Draw the dark green grid lines
  for (int i = 1; i < 10; i++) {
    tft.drawFastHLine(21, 72 + i * 30, tft.width() - tft.width()/2 - 39, tft.Color565(0, 50, 0));
  }
  for (int i = 1; i < 12; i++) {
    tft.drawFastVLine(21 + i * 30, 72, 345 - 20*2-1, tft.Color565(0, 50, 0));
  }
  // Set active window for the graph area
  tft.setActiveWindow(21, tft.width()/2 - 19, 72, 345 - 20*2);
  // Draw a thick yellow trace
  tft.drawFastHLine(21, 223, 7*120, tft.Color565(255, 255, 0));
  // Draw a thick blue trace
  tft.drawFastHLine(21, 222, 7*120, tft.Color565(50, 50, 255));
  // Reset the active window
  tft.setActiveWindow();
};

void UI::drawArray(const uint32_t * image,uint32_t isize,uint16_t iwidth,uint16_t x,uint16_t y){
  uint16_t pixels[iwidth];//container
  uint32_t i,idx;
  for (idx=0;idx<isize/iwidth;idx++){
    for (i = (iwidth*idx); i < iwidth*(idx+1);i++){
      pixels[i - (iwidth*idx)] = tft.Color24To565(image[i]);
    }
    tft.drawPixels(pixels,iwidth,x,idx+y);
  }
}
