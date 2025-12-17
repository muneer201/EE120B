EE120B – Embedded Systems (UCR)

This repository contains my laboratory assignments and final project for EE120B: Embedded Systems at the University of California, Riverside. The course emphasizes bare-metal embedded programming, real-time behavior, and hardware–software integration using AVR microcontrollers.

All projects are written in C, using direct register manipulation, timer-driven execution, and synchronous state machines.



Course Topics

AVR microcontroller programming

GPIO, buttons, and LEDs

Timers, interrupts, and PWM

ADC (analog-to-digital conversion)

Concurrent synchronous state machines (SynchSMs)

Task scheduling using a GCD-based timer

7-segment displays, LED matrices, and TFT screens

Sensors (sonar, light sensors)

Audio generation using timers

SPI and serial communication



Laboratory Assignments


Lab 2 – Digital I/O & Button Logic

Introduces basic GPIO control using push buttons and LEDs. This lab focuses on reading button inputs, implementing bidirectional LED movement, and displaying hexadecimal values on a 7-segment display. The goal is to understand low-level port manipulation and software debouncing without using timers.



Lab 3 – ADC & Mode Switching

Introduces the Analog-to-Digital Converter (ADC) and structured state machines. Analog sensor values are read and displayed on a 7-segment display, while button presses switch between different display modes (such as numeric and hexadecimal ranges). ADC values are also transmitted over UART for debugging and verification.



Lab 4 – Timers, Thresholds & Password Logic

Builds on ADC and state machines using timers and counters. The system maps sensor input to output ranges, controls RGB LEDs based on thresholds, and implements timed behaviors such as snooze and delay states. This lab also includes password-style access logic, where specific conditions unlock protected states, and a danger mode with flashing output when unsafe conditions occur.

Lab 5 – Sonar Distance Measurement

Implements an ultrasonic distance sensing system. Distance is measured using a sonar sensor and displayed on a 4-digit 7-segment display. LED indicators reflect distance thresholds, and button inputs control display behavior. The system is implemented using multiple concurrent tasks with different execution periods.

Lab 6 – Multi-Task Embedded Systems

Focuses on concurrent task scheduling. Multiple independent state machines run simultaneously to handle inputs, timing, and output behavior. This lab reinforces how real embedded systems manage multiple responsibilities without blocking delays.

Lab 7 – Combination Lock / Password System

Implements a combination lock (password) system using ordered user inputs. The system validates input sequences, handles incorrect attempts, supports reset and lockout behavior, and provides visual and audio feedback. This lab emphasizes security logic, sequencing, and robust state-machine design.



Final Project – Stacker Blocks Game

A completely embedded game inspired by Stacker Blocks, integrating all major concepts from the course.

Features:

4× MAX7219 LED matrix displays forming a 32×8 game board

TFT screen (ST7735) displaying WIN and LOSE messages

Audio effects generated using Timer0 PWM

Two-button control (start and action)

Dynamic difficulty with increasing speed

Green and red LEDs for win/loss feedback

Fully non-blocking, task-scheduled architecture

Custom-drawn graphics (no bitmap fonts)


Hardware Used


AVR microcontroller

MAX7219 LED matrix modules

ST7735 TFT display

Ultrasonic (sonar) sensor

Push buttons

RGB and single-color LEDs

Buzzer/speaker



Tools & Software

C (AVR-GCC)

Atmel Studio / Microchip Studio

Timer-based task scheduler

SPI, ADC, PWM

Bare-metal register programming
