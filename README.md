<h1>EE120B – Embedded Systems (UCR)</h1>

<p>
This repository contains laboratory assignments and a final project for EE120B: Embedded Systems
at the University of California, Riverside. The course emphasizes bare-metal embedded programming,
real-time behavior, and hardware–software integration using AVR microcontrollers.
</p>

<p>
All projects are written in C, using direct register manipulation, timer-driven execution,
and synchronous state machines.
</p>

<h2>Course Topics</h2>
<ul>
  <li>AVR microcontroller programming</li>
  <li>GPIO, buttons, and LEDs</li>
  <li>Timers, interrupts, and PWM</li>
  <li>ADC (analog-to-digital conversion)</li>
  <li>Concurrent synchronous state machines (SynchSMs)</li>
  <li>Task scheduling using a GCD-based timer</li>
  <li>7-segment displays, LED matrices, and TFT screens</li>
  <li>Sensors (sonar, light sensors)</li>
  <li>Audio generation using timers</li>
  <li>SPI and serial communication</li>
</ul>

<h2>Laboratory Assignments</h2>

<h3>Lab 2 – Digital I/O &amp; Button Logic</h3>
<p>
Introduces basic GPIO control using push buttons and LEDs. This lab focuses on reading button inputs,
implementing bidirectional LED movement, and displaying hexadecimal values on a 7-segment display.
The goal is to understand low-level port manipulation and software debouncing without using timers.
</p>

<h3>Lab 3 – ADC &amp; Mode Switching</h3>
<p>
Introduces the Analog-to-Digital Converter (ADC) and structured state machines. Analog sensor values
are read and displayed on a 7-segment display, while button presses switch between different display
modes such as numeric and hexadecimal ranges. ADC values are also transmitted over UART for debugging
and verification.
</p>

<h3>Lab 4 – Timers, Thresholds &amp; Password Logic</h3>
<p>
Builds on ADC and state machines using timers and counters. The system maps sensor input to output
ranges, controls RGB LEDs based on thresholds, and implements timed behaviors such as snooze and delay
states. This lab also includes password-style access logic, where specific conditions unlock protected
states, and a danger mode with flashing output when unsafe conditions occur.
</p>

<h3>Lab 5 – Sonar Distance Measurement</h3>
<p>
Implements an ultrasonic distance sensing system. Distance is measured using a sonar sensor and
displayed on a 4-digit 7-segment display. LED indicators reflect distance thresholds, and button
inputs control display behavior. The system is implemented using multiple concurrent tasks with
different execution periods.
</p>

<h3>Lab 6 – Multi-Task Embedded Systems</h3>
<p>
Focuses on concurrent task scheduling. Multiple independent state machines run simultaneously to
handle inputs, timing, and output behavior. This lab reinforces how real embedded systems manage
multiple responsibilities without blocking delays.
</p>

<h3>Lab 7 – Combination Lock / Password System</h3>
<p>
Implements a combination lock (password) system using ordered user inputs. The system validates input
sequences, handles incorrect attempts, supports reset and lockout behavior, and provides visual and
audio feedback. This lab emphasizes security logic, sequencing, and robust state-machine design.
</p>

<h2>Final Project – Stacker Blocks Game</h2>

<p>
A complete embedded game inspired by Stacker Blocks, integrating all major concepts from the course.
</p>

<h3>Features</h3>
<ul>
  <li>4× MAX7219 LED matrix displays forming a 32×8 game board</li>
  <li>TFT screen (ST7735) displaying WIN and LOSE messages</li>
  <li>Audio effects generated using Timer0 PWM</li>
  <li>Two-button control (start and action)</li>
  <li>Dynamic difficulty with increasing speed</li>
  <li>Green and red LEDs for win/loss feedback</li>
  <li>Fully non-blocking, task-scheduled architecture</li>
  <li>Custom-drawn graphics (no bitmap fonts)</li>
</ul>

<h2>Hardware Used</h2>
<ul>
  <li>AVR microcontroller</li>
  <li>MAX7219 LED matrix modules</li>
  <li>ST7735 TFT display</li>
  <li>Ultrasonic (sonar) sensor</li>
  <li>Push buttons</li>
  <li>RGB and single-color LEDs</li>
  <li>Buzzer / speaker</li>
</ul>

<h2>Tools &amp; Software</h2>
<ul>
  <li>C (AVR-GCC)</li>
  <li>Atmel Studio / Microchip Studio</li>
  <li>Timer-based task scheduler</li>
  <li>SPI, ADC, PWM</li>
  <li>Bare-metal register programming</li>
</ul>
