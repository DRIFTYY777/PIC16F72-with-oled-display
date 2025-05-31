# PIC16F72 OLED Display Driver (0.92" SSD1306 - I2C Bit-Bang)

This lightweight library enables interfacing a **0.92" SSD1306 OLED display** with a **PIC16F72** microcontroller using **bit-banged I²C protocol**. The library supports printing **numbers**, **characters**, and **strings**, and is optimized for performance and simplicity on low-resource systems.

Additionally, the project includes a **blinking LED** feature on **RA0/AN0**, making it useful for testing and visual confirmation of operation.

---

## ⚙️ Project Features

- ✅ Lightweight OLED library (no external dependencies)
- ✅ Bit-bang I²C (software implementation)
- ✅ Supports `printChar()`, `printString()`, `printNumber()`
- ✅ 0.92" SSD1306 128x64 OLED support
- ✅ LED blinking on `RA0/AN0`
- ✅ Optimized for 12 MHz system clock
- ✅ Clean and readable C code

---

## 🧠 Microcontroller Info

- **MCU**: PIC16F72
- **Clock Speed**: 12 MHz
- **Language**: C (MPLAB X / XC8 Compatible)

---

## 🔌 Pin Configuration

| OLED Pin | PIC16F72 Pin         | Description             |
|----------|----------------------|-------------------------|
| VCC      | 3.3V or 5V           | Power Supply            |
| GND      | GND                  | Ground                  |
| SDA      | RC4 / SDI / SDA      | Bit-bang I2C Data Line  |
| SCL      | RC3 / SCK / SCL      | Bit-bang I2C Clock Line |

### LED Pin

| LED      | RA0 / AN0            | Output Pin for Blinking |

---

## 🧪 Usage

1. Clone the repository.
2. Include `oled.c` and `oled.h` in your project.
3. Set up the project in MPLAB X with XC8 compiler.
4. Call `oled_init()` in your `main()` function.
5. Use the provided API functions:
   - `oled_printChar('A', x, y);`
   - `oled_printString("Hello", x, y);`
   - `oled_printNumber(123, x, y);`

Example:
```c
oled_printString("Temp:", 0, 0);
oled_printNumber(25, 40, 0);
