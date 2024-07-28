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

#ifndef UI_ELEMENTS_H
#define UI_ELEMENTS_H

#include <Arduino.h>

class UI {
  public:
    UI();
    void drawArray(const uint32_t * image,uint32_t isize,uint16_t iwidth,uint16_t x,uint16_t y);
    void drawButton(uint16_t x, uint16_t y, uint16_t width, uint16_t height, String text, bool selected);
    void drawTab(uint16_t x, uint16_t y, uint16_t width, uint16_t height, String text, bool selected);
    void drawToggle(uint16_t x, uint16_t y, bool state);
    void drawStepper(uint16_t x, uint16_t y, int val);
    void drawSlider(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t value, uint16_t min, uint16_t max);
    void drawEnumDropdown(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const char* text, const char* options[], uint8_t numOptions, uint8_t selectedOption, bool expanded);

    // Touch handling methods
    bool handleButtonTouch(uint16_t tx, uint16_t ty, uint16_t x, uint16_t y, uint16_t width, uint16_t height, String text, bool &selected);
    bool handleToggleTouch(uint16_t tx, uint16_t ty, uint16_t x, uint16_t y, bool &state);
    bool handleStepperTouch(uint16_t tx, uint16_t ty, uint16_t x, uint16_t y, uint16_t &val);
    bool handleSliderTouch(uint16_t tx, uint16_t ty, uint16_t x, uint16_t y, uint16_t width, uint16_t &value, uint16_t min, uint16_t max);
    bool handleEnumDropdownTouch(uint16_t tx, uint16_t ty, uint16_t x, uint16_t y, uint16_t width, uint16_t height, const char* options[], uint8_t numOptions, uint8_t &selectedOption, bool &expanded);

    // Waveform drawing methods
    void drawSwitchingWaveform();
    void drawPolarization1();
    void drawPolarization2();
    void drawOffWaveform();
};

#endif

