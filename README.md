# Advanced Embedded Systems: MCU Low-Power Architecture

![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)
![Proteus](https://img.shields.io/badge/Proteus-8.12-blue?style=for-the-badge)
![AVR](https://img.shields.io/badge/Microchip-AVR-red?style=for-the-badge)
![STM32](https://img.shields.io/badge/STM32-ARM_Cortex--M4-03234B?style=for-the-badge)

## Overview
This repository contains the deliverables for the Advanced Embedded Systems assignment focusing on microcontroller low-power modes, sleep-state registers, and hardware interrupt wake-up mechanisms. 

The project is divided into two main architectures:
1. **ATmega328P (AVR):** Bare-metal Embedded C firmware and Proteus hardware simulation demonstrating Power-down sleep mode and `INT0` wake-up.
2. **STM32L476RG (ARM Cortex-M4):** Architectural research and presentation on ultra-low-power modes, Wake-up sources, and `WFI`/`WFE` execution.

---

## Part 1: ATmega328P Sleep Mode & Simulation

### Hardware Configuration
The circuit is designed and simulated in Proteus with the following peripherals:
* **ATmega328P** running on a 16 MHz external crystal oscillator.
* **LM016L 16x2 LCD** (4-bit mode) for system state visualization.
* **Push Button** on `PD2` (`INT0`) acting as the external wake-up trigger.
* **LED (`PB5`) & Buzzer (`PB0`)** for visual and audio operation feedback.

### Firmware & Register Level Implementation
The firmware is written in bare-metal C (compiled via `avr-gcc`) and directly manipulates the MCU's control registers to achieve ultra-low power consumption.

**Key Registers Configured:**
* `SMCR` (Sleep Mode Control Register): Configured to `010` to select **Power-down mode**. The `SE` (Sleep Enable) bit is set immediately before executing the `sleep` assembly instruction.
* `EICRA` (External Interrupt Control Register A): Configured to trigger on a falling edge.
* `EIMSK` (External Interrupt Mask Register): Enables the `INT0` hardware interrupt.
* `SREG` (Status Register): Global interrupts enabled via the `I-bit` (`sei()`) to allow the wake-up vector to execute.

### State Flow
1. **Normal Operation:** System runs, LED is active, LCD displays "System: NORMAL".
2. **Sleep Mode:** MCU halts execution, enters Power-down mode. LCD displays "Zzz... Sleeping".
3. **Wake-up:** A falling edge on `PD2` triggers the `INT0` vector, waking the CPU. System resumes, triggers the buzzer, and returns to normal operation.

### Simulation Previews 
![Wake Up](docs/screenshots/wake_up.jpeg) 

---

## Part 2: STM32 Low-Power Architecture Research

The second half of this project analyzes the STM32L476RG microcontroller, an ultra-low-power ARM Cortex-M4 MCU. 

**Research Highlights (See attached PDF presentation):**
* Comparison of **Sleep, Stop (0, 1, 2), Standby, and Shutdown** modes.
* Core power control via `PWR_CR1` and Cortex-M4 `SCR` (`SLEEPDEEP` bit).
* Extended Interrupt Controller (`EXTI`) and `NVIC` wake-up routing.
* Architectural differences between `WFI` (Wait For Interrupt) and `WFE` (Wait For Event) mechanisms.

---

## Final Architecture Comparison

| Feature | ATmega328P | STM32L476RG (Cortex-M4) |
| :--- | :--- | :--- |
| **Sleep Modes** | 6 (Idle, ADC Noise Reduction, Power-down, Power-save, Standby, Ext. Standby) | 7 (Sleep, LP Sleep, Stop 0/1/2, Standby, Shutdown) |
| **Deepest Sleep** | Power-down (~0.1 µA) | Shutdown (~30 nA) |
| **Sleep Control** | `SMCR`, `MCUCR` | `PWR_CR1`, `PWR_CR2`, `Cortex-M SCR` |
| **Wake-up Sources**| External Interrupts (INT0/1), Pin Change, Watchdog | EXTI, RTC, WKUP Pins, LPTIM, I2C/UART Address Match |
| **RAM Retention** | Fully retained in Power-down | Retained in Stop; Optionally partial in Standby; Lost in Shutdown |

---

## Build Instructions (ATmega328P Firmware)

Ensure you have the AVR toolchain (`avr-gcc`, `avr-libc`, `make`) installed.

```bash
# Clone the repository
git clone https://github.com/hunde32/sleep_mode.git
cd atmega328p-stm32-low-power

# Compile the firmware
make

# The resulting main.hex file can be loaded directly into the Proteus ATmega328P properties.
