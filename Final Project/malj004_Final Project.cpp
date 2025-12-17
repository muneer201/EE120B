
/*        Your Name & E-mail: Muneer Al Jufout malj004@ucr.edu
*          Discussion Section: 021
 *         Assignment: Final Project
 *         I acknowledge all content contained herein, excluding template or example code, is my own original work.
 *         Demo Link: https://youtu.be/ijldJOSP1Qc
 */



#include "timerISR.h"
#include <avr/io.h>
#include <util/delay.h>

#define NUM_TASKS 2
#define NUM_SCREENS 4

const unsigned long GAME_PERIOD = 100;
const unsigned long AUDIO_PERIOD = 50;
const unsigned long GCD_PERIOD = 50;

typedef struct _task {
signed char state;
unsigned long period;
unsigned long elapsedTime;
int (*TickFct)(int);
} task;

task tasks[NUM_TASKS];

void TimerISR() {
for (unsigned int i = 0; i < NUM_TASKS; i++) {
if (tasks[i].elapsedTime == tasks[i].period) {
tasks[i].state = tasks[i].TickFct(tasks[i].state);
tasks[i].elapsedTime = 0;
}
tasks[i].elapsedTime += GCD_PERIOD;
}
}

#define MAX7219_DIN_PIN  (1 << 2)
#define MAX7219_CLK_PIN  (1 << 3)
#define MAX7219_CS_PIN   (1 << 4)

#define TFT_CS_PIN    (1 << 2)
#define TFT_RESET_PIN (1 << 1)
#define TFT_DC_PIN    (1 << 0)
#define TFT_MOSI_PIN  (1 << 3)
#define TFT_SCK_PIN   (1 << 5)

#define GREEN_LED1_PIN (1 << 2)
#define GREEN_LED2_PIN (1 << 3)
#define RED_LED1_PIN   (1 << 4)
#define RED_LED2_PIN   (1 << 5)

#define MAX7219_REG_NOOP         0x00
#define MAX7219_REG_DIGIT0       0x01
#define MAX7219_REG_DIGIT1       0x02
#define MAX7219_REG_DIGIT2       0x03
#define MAX7219_REG_DIGIT3       0x04
#define MAX7219_REG_DIGIT4       0x05
#define MAX7219_REG_DIGIT5       0x06
#define MAX7219_REG_DIGIT6       0x07
#define MAX7219_REG_DIGIT7       0x08
#define MAX7219_REG_DECODEMODE   0x09
#define MAX7219_REG_INTENSITY    0x0A
#define MAX7219_REG_SCANLIMIT    0x0B
#define MAX7219_REG_SHUTDOWN     0x0C
#define MAX7219_REG_DISPLAYTEST  0x0F

#define ST7735_SWRESET 0x01
#define ST7735_SLPOUT  0x11
#define ST7735_COLMOD  0x3A
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C

#define COLOR_BLACK   0x0000
#define COLOR_WHITE   0xFFFF
#define COLOR_RED     0x001F
#define COLOR_GREEN   0x07E0
#define COLOR_BLUE    0x001F
#define COLOR_YELLOW  0xFFE0

unsigned int stackFreq1 = 800, stackFreq2 = 800, stackFreq3 = 800;
unsigned char stackBeat1 = 2, stackBeat2 = 2, stackBeat3 = 2;

unsigned int winFreq1 = 523, winFreq2 = 659, winFreq3 = 784, winFreq4 = 1047;
unsigned int winFreq5 = 784, winFreq6 = 1047, winFreq7 = 1047, winFreq8 = 784;
unsigned int winFreq9 = 1047, winFreq10 = 1047;
unsigned char winBeat1 = 3, winBeat2 = 3, winBeat3 = 3, winBeat4 = 4;
unsigned char winBeat5 = 3, winBeat6 = 3, winBeat7 = 4, winBeat8 = 3;
unsigned char winBeat9 = 4, winBeat10 = 8;

unsigned int loseFreq1 = 330, loseFreq2 = 294, loseFreq3 = 262, loseFreq4 = 220;
unsigned int loseFreq5 = 196, loseFreq6 = 175, loseFreq7 = 147, loseFreq8 = 131;
unsigned char loseBeat1 = 5, loseBeat2 = 5, loseBeat3 = 6, loseBeat4 = 7;
unsigned char loseBeat5 = 8, loseBeat6 = 10, loseBeat7 = 12, loseBeat8 = 16;

unsigned char stackAudioMode = 0;
unsigned char winAudioMode = 0;
unsigned char loseAudioMode = 0;
unsigned char currentStep = 0;
unsigned int beatCounter = 0;

unsigned char BTN1_PRESSED = 0;
unsigned char prevBTN1 = 0;
unsigned char START_BTN_PRESSED = 0;
unsigned char prevSTART_BTN = 0;

unsigned char gameBoard[32];
unsigned char currentColumn = 0;
unsigned char blockPosition = 0;
unsigned char blockWidth = 2;
unsigned char blockHeight = 4;
unsigned char movingDirection = 1;
unsigned char gameSpeed = 15;
unsigned char speedCounter = 0;
unsigned char gameActive = 1;
unsigned char gameWon = 0;
unsigned char gameLost = 0;
unsigned char winStateTimer = 0;
unsigned char hasFoundation = 0;
unsigned char gameStarted = 0;

void turnOffAllLEDs() {
PORTC &= ~(GREEN_LED1_PIN | GREEN_LED2_PIN | RED_LED1_PIN | RED_LED2_PIN);
}

void turnOnGreenLEDs() {
PORTC |= (GREEN_LED1_PIN | GREEN_LED2_PIN);
PORTC &= ~(RED_LED1_PIN | RED_LED2_PIN);
}

void turnOnRedLEDs() {
PORTC |= (RED_LED1_PIN | RED_LED2_PIN);
PORTC &= ~(GREEN_LED1_PIN | GREEN_LED2_PIN);
}

void TFT_SPI_Init(void) {
DDRB |= TFT_CS_PIN | TFT_RESET_PIN | TFT_DC_PIN | TFT_MOSI_PIN | TFT_SCK_PIN;
SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
PORTB |= TFT_CS_PIN;
}

void TFT_SPI_Send(unsigned char data) {
SPDR = data;
while (!(SPSR & (1 << SPIF)));
}

void TFT_Send_Command(unsigned char cmd) {
PORTB &= ~TFT_CS_PIN;
PORTB &= ~TFT_DC_PIN;
TFT_SPI_Send(cmd);
PORTB |= TFT_CS_PIN;
}

void TFT_Send_Data(unsigned char data) {
PORTB &= ~TFT_CS_PIN;
PORTB |= TFT_DC_PIN;
TFT_SPI_Send(data);
PORTB |= TFT_CS_PIN;
}

void TFT_Hardware_Reset(void) {
PORTB &= ~TFT_RESET_PIN;
_delay_ms(200);
PORTB |= TFT_RESET_PIN;
_delay_ms(200);
}

void TFT_Init(void) {
TFT_SPI_Init();
TFT_Hardware_Reset();
TFT_Send_Command(ST7735_SWRESET);
_delay_ms(150);
TFT_Send_Command(ST7735_SLPOUT);
_delay_ms(200);
TFT_Send_Command(ST7735_COLMOD);
TFT_Send_Data(0x05);
_delay_ms(10);
TFT_Send_Command(ST7735_DISPON);
_delay_ms(200);
}

void TFT_Set_Address_Window(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1) {
TFT_Send_Command(ST7735_CASET);
TFT_Send_Data(0x00);
TFT_Send_Data(x0);
TFT_Send_Data(0x00);
TFT_Send_Data(x1);
TFT_Send_Command(ST7735_RASET);
TFT_Send_Data(0x00);
TFT_Send_Data(y0);
TFT_Send_Data(0x00);
TFT_Send_Data(y1);
TFT_Send_Command(ST7735_RAMWR);
}

void TFT_Fill_Screen(unsigned int color) {
TFT_Set_Address_Window(0, 0, 127, 159);
PORTB &= ~TFT_CS_PIN;
PORTB |= TFT_DC_PIN;
for (unsigned int i = 0; i < 20480; i++) {
TFT_SPI_Send(color >> 8);
TFT_SPI_Send(color & 0xFF);
}
PORTB |= TFT_CS_PIN;
}

void TFT_Draw_Rectangle(unsigned char x, unsigned char y, unsigned char width, unsigned char height, unsigned int color) {
TFT_Set_Address_Window(x, y, x + width - 1, y + height - 1);
PORTB &= ~TFT_CS_PIN;
PORTB |= TFT_DC_PIN;
for (unsigned int i = 0; i < width * height; i++) {
TFT_SPI_Send(color >> 8);
TFT_SPI_Send(color & 0xFF);
}
PORTB |= TFT_CS_PIN;
}

void TFT_Draw_Y(unsigned char x, unsigned char y, unsigned int color) {
TFT_Draw_Rectangle(x, y, 12, 16, color);
TFT_Draw_Rectangle(x + 2, y, 8, 6, COLOR_BLACK);
TFT_Draw_Rectangle(x, y + 10, 4, 6, COLOR_BLACK);
TFT_Draw_Rectangle(x + 8, y + 10, 4, 6, COLOR_BLACK);
}

void TFT_Draw_O(unsigned char x, unsigned char y, unsigned int color) {
TFT_Draw_Rectangle(x, y, 12, 16, color);
TFT_Draw_Rectangle(x + 3, y + 3, 6, 10, COLOR_BLACK);
}

void TFT_Draw_U(unsigned char x, unsigned char y, unsigned int color) {
TFT_Draw_Rectangle(x, y, 12, 16, color);
TFT_Draw_Rectangle(x + 3, y, 6, 12, COLOR_BLACK);
}

void TFT_Draw_W(unsigned char x, unsigned char y, unsigned int color) {
TFT_Draw_Rectangle(x, y, 2, 16, color);
TFT_Draw_Rectangle(x + 2, y + 10, 2, 2, color);
TFT_Draw_Rectangle(x + 3, y + 8, 2, 2, color);
TFT_Draw_Rectangle(x + 4, y + 6, 2, 2, color);
TFT_Draw_Rectangle(x + 5, y, 2, 16, color);
TFT_Draw_Rectangle(x + 6, y + 6, 2, 2, color);
TFT_Draw_Rectangle(x + 7, y + 8, 2, 2, color);
TFT_Draw_Rectangle(x + 8, y + 10, 2, 2, color);
TFT_Draw_Rectangle(x + 10, y, 2, 16, color);
}

void TFT_Draw_I(unsigned char x, unsigned char y, unsigned int color) {
TFT_Draw_Rectangle(x, y, 12, 2, color);
TFT_Draw_Rectangle(x + 5, y + 2, 2, 12, color);
TFT_Draw_Rectangle(x, y + 14, 12, 2, color);
}

void TFT_Draw_N(unsigned char x, unsigned char y, unsigned int color) {
TFT_Draw_Rectangle(x, y, 2, 16, color);
TFT_Draw_Rectangle(x + 2, y + 2, 2, 2, color);
TFT_Draw_Rectangle(x + 3, y + 4, 2, 2, color);
TFT_Draw_Rectangle(x + 4, y + 6, 2, 2, color);
TFT_Draw_Rectangle(x + 5, y + 8, 2, 2, color);
TFT_Draw_Rectangle(x + 6, y + 10, 2, 2, color);
TFT_Draw_Rectangle(x + 7, y + 12, 2, 2, color);
TFT_Draw_Rectangle(x + 10, y, 2, 16, color);
}

void TFT_Draw_L(unsigned char x, unsigned char y, unsigned int color) {
TFT_Draw_Rectangle(x, y, 2, 16, color);
TFT_Draw_Rectangle(x, y + 14, 12, 2, color);
}

void TFT_Draw_S(unsigned char x, unsigned char y, unsigned int color) {
TFT_Draw_Rectangle(x, y, 12, 2, color);
TFT_Draw_Rectangle(x, y + 2, 2, 6, color);
TFT_Draw_Rectangle(x, y + 7, 12, 2, color);
TFT_Draw_Rectangle(x + 10, y + 9, 2, 5, color);
TFT_Draw_Rectangle(x, y + 14, 12, 2, color);
}

void TFT_Draw_E(unsigned char x, unsigned char y, unsigned int color) {
TFT_Draw_Rectangle(x, y, 2, 16, color);
TFT_Draw_Rectangle(x, y, 12, 2, color);
TFT_Draw_Rectangle(x, y + 7, 8, 2, color);
TFT_Draw_Rectangle(x, y + 14, 12, 2, color);
}

void TFT_Display_Win_Text(void) {
TFT_Fill_Screen(COLOR_BLACK);
TFT_Draw_Y(30, 50, COLOR_GREEN);
TFT_Draw_O(45, 50, COLOR_GREEN);
TFT_Draw_U(60, 50, COLOR_GREEN);
TFT_Draw_W(30, 80, COLOR_GREEN);
TFT_Draw_I(48, 80, COLOR_GREEN);
TFT_Draw_N(60, 80, COLOR_GREEN);
}

void TFT_Display_Lose_Text(void) {
TFT_Fill_Screen(COLOR_BLACK);
TFT_Draw_Y(20, 50, COLOR_RED);
TFT_Draw_O(35, 50, COLOR_RED);
TFT_Draw_U(50, 50, COLOR_RED);
TFT_Draw_L(15, 80, COLOR_RED);
TFT_Draw_O(30, 80, COLOR_RED);
TFT_Draw_S(45, 80, COLOR_RED);
TFT_Draw_E(60, 80, COLOR_RED);
}

void TFT_Display_Ready(void) {
TFT_Fill_Screen(COLOR_BLACK);
}

void TFT_Display_Win(void) {
TFT_Display_Win_Text();
}

void TFT_Display_Lose(void) {
TFT_Display_Lose_Text();
}

void TFT_Display_Playing(void) {
}

void setupTFT(void) {
TFT_Init();
TFT_Display_Ready();
}

void sendToMAX7219(unsigned char address, unsigned char data, unsigned char chip) {
PORTD &= ~MAX7219_CS_PIN;

for (int i = 0; i < NUM_SCREENS; i++) {
unsigned char currentAddress;
unsigned char currentData;

if (i == chip) {
currentAddress = address;
currentData = data;
}
else {
currentAddress = MAX7219_REG_NOOP;
currentData = 0x00;
}

for (int j = 7; j >= 0; j--) {
if (currentAddress & (1 << j)) {
PORTD |= MAX7219_DIN_PIN;
}
else {
PORTD &= ~MAX7219_DIN_PIN;
}

PORTD |= MAX7219_CLK_PIN;
PORTD &= ~MAX7219_CLK_PIN;
}

for (int j = 7; j >= 0; j--) {
if (currentData & (1 << j)) {
PORTD |= MAX7219_DIN_PIN;
}
else {
PORTD &= ~MAX7219_DIN_PIN;
}

PORTD |= MAX7219_CLK_PIN;
PORTD &= ~MAX7219_CLK_PIN;
}
}

PORTD |= MAX7219_CS_PIN;
}

void sendToAllMAX7219(unsigned char address, unsigned char data) {
PORTD &= ~MAX7219_CS_PIN;

for (int chip = 0; chip < NUM_SCREENS; chip++) {
for (int i = 7; i >= 0; i--) {
if (address & (1 << i)) {
PORTD |= MAX7219_DIN_PIN;
}
else {
PORTD &= ~MAX7219_DIN_PIN;
}

PORTD |= MAX7219_CLK_PIN;
PORTD &= ~MAX7219_CLK_PIN;
}

for (int i = 7; i >= 0; i--) {
if (data & (1 << i)) {
PORTD |= MAX7219_DIN_PIN;
}
else {
PORTD &= ~MAX7219_DIN_PIN;
}

PORTD |= MAX7219_CLK_PIN;
PORTD &= ~MAX7219_CLK_PIN;
}
}

PORTD |= MAX7219_CS_PIN;
}

void clearAllDisplays() {
for (unsigned char i = 1; i <= 8; i++) {
sendToAllMAX7219(i, 0x00);
}
}

void initMAX7219() {
DDRD |= MAX7219_DIN_PIN | MAX7219_CLK_PIN | MAX7219_CS_PIN;
PORTD |= MAX7219_CS_PIN;
sendToAllMAX7219(MAX7219_REG_SHUTDOWN, 0x01);
sendToAllMAX7219(MAX7219_REG_DECODEMODE, 0x00);
sendToAllMAX7219(MAX7219_REG_SCANLIMIT, 0x07);
sendToAllMAX7219(MAX7219_REG_INTENSITY, 0x08);
sendToAllMAX7219(MAX7219_REG_DISPLAYTEST, 0x00);
clearAllDisplays();
}

void setupAudioHardware() {
DDRD |= (1 << PD6);
TCCR0A = (1 << WGM01) | (1 << WGM00);
TCCR0B = (TCCR0B & 0xF8) | 0x03;
}

void createAudioTone(unsigned int freq) {
unsigned char ocrValue;
ocrValue = 128;

switch (freq) {
case 131: 
ocrValue = 240; 
break;

case 147: 
ocrValue = 215; 
break;

case 175: 
ocrValue = 180; 
break;

case 196: 
ocrValue = 200; 
break;

case 220: 
ocrValue = 180; 
break;

case 262: 
ocrValue = 150; 
break;

case 294: 
ocrValue = 135; 
break;

case 330: 
ocrValue = 120; 
break;

case 523: 
ocrValue = 75; 
break;

case 659: 
ocrValue = 60; 
break;

case 784: 
ocrValue = 50; 
break;

case 800: 
ocrValue = 49; 
break;

case 1047: 
ocrValue = 38; 
break;

default: 
ocrValue = 100; 
break;
}

OCR0A = ocrValue;
TCCR0A |= (1 << COM0A1);
}

void stopAudio() {
TCCR0A &= ~(1 << COM0A1);
PORTD &= ~(1 << PD6);
}

unsigned int getCurrentFrequency() {
if (stackAudioMode) {
if (currentStep == 0) {
return stackFreq1;
}
else if (currentStep == 1) {
return stackFreq2;
}
else if (currentStep == 2) {
return stackFreq3;
}
}
else if (winAudioMode) {
if (currentStep == 0) {
return winFreq1;
}
else if (currentStep == 1) {
return winFreq2;
}
else if (currentStep == 2) {
return winFreq3;
}
else if (currentStep == 3) {
return winFreq4;
}
else if (currentStep == 4) {
return winFreq5;
}
else if (currentStep == 5) {
return winFreq6;
}
else if (currentStep == 6) {
return winFreq7;
}
else if (currentStep == 7) {
return winFreq8;
}
else if (currentStep == 8) {
return winFreq9;
}
else if (currentStep == 9) {
return winFreq10;
}
}
else if (loseAudioMode) {
if (currentStep == 0) {
return loseFreq1;
}
else if (currentStep == 1) {
return loseFreq2;
}
else if (currentStep == 2) {
return loseFreq3;
}
else if (currentStep == 3) {
return loseFreq4;
}
else if (currentStep == 4) {
return loseFreq5;
}
else if (currentStep == 5) {
return loseFreq6;
}
else if (currentStep == 6) {
return loseFreq7;
}
else if (currentStep == 7) {
return loseFreq8;
}
}
return 0;
}

unsigned char getCurrentBeatDuration() {
if (stackAudioMode) {
if (currentStep == 0) {
return stackBeat1;
}
else if (currentStep == 1) {
return stackBeat2;
}
else if (currentStep == 2) {
return stackBeat3;
}
}
else if (winAudioMode) {
if (currentStep == 0) {
return winBeat1;
}
else if (currentStep == 1) {
return winBeat2;
}
else if (currentStep == 2) {
return winBeat3;
}
else if (currentStep == 3) {
return winBeat4;
}
else if (currentStep == 4) {
return winBeat5;
}
else if (currentStep == 5) {
return winBeat6;
}
else if (currentStep == 6) {
return winBeat7;
}
else if (currentStep == 7) {
return winBeat8;
}
else if (currentStep == 8) {
return winBeat9;
}
else if (currentStep == 9) {
return winBeat10;
}
}
else if (loseAudioMode) {
if (currentStep == 0) {
return loseBeat1;
}
else if (currentStep == 1) {
return loseBeat2;
}
else if (currentStep == 2) {
return loseBeat3;
}
else if (currentStep == 3) {
return loseBeat4;
}
else if (currentStep == 4) {
return loseBeat5;
}
else if (currentStep == 5) {
return loseBeat6;
}
else if (currentStep == 6) {
return loseBeat7;
}
else if (currentStep == 7) {
return loseBeat8;
}
}
return 1;
}

unsigned char getMaxSteps() {
if (stackAudioMode) {
return 3;
}
else if (winAudioMode) {
return 10;
}
else if (loseAudioMode) {
return 8;
}
return 0;
}

unsigned char block2x4(unsigned char position, unsigned char width, unsigned char height) {
unsigned char pattern = 0;
for (unsigned char i = 0; i < height; i++) {
if (position + i < 8) {
pattern |= (1 << (position + i));
}
}
return pattern;
}

unsigned char checkblock2x4(unsigned char column, unsigned char position, unsigned char width, unsigned char height) {
if (column + width > 32) {
return 0;
}
if (position + height > 8) {
return 0;
}
return 1;
}

unsigned char get2x4overlap(unsigned char column, unsigned char position, unsigned char width, unsigned char height) {
if (column >= 32) {
return 0;
}

unsigned char movingPattern = block2x4(position, width, height);
unsigned char existingPattern = gameBoard[column];
return movingPattern & existingPattern;
}

void updateDisplay() {
clearAllDisplays();

if (!gameStarted) {
return;
}

for (unsigned char screen = 0; screen < NUM_SCREENS; screen++) {
unsigned char physicalScreen = (NUM_SCREENS - 1) - screen;

for (unsigned char row = 0; row < 8; row++) {
unsigned char columnData = 0;

for (unsigned char col = 0; col < 8; col++) {
unsigned char globalColumn = screen * 8 + col;

if (globalColumn < currentColumn) {
if (gameBoard[globalColumn] & (1 << row)) {
columnData |= (1 << col);
}
}

if (gameActive && (globalColumn == currentColumn || globalColumn == currentColumn + 1)) {
unsigned char movingPattern = block2x4(blockPosition, blockWidth, blockHeight);
if (movingPattern & (1 << row)) {
columnData |= (1 << col);
}
}
}

sendToMAX7219(row + 1, columnData, physicalScreen);
}
}
}

void initGame() {
currentColumn = 0;
blockPosition = 0;
blockWidth = 2;
blockHeight = 4;
movingDirection = 1;
gameSpeed = 15;
speedCounter = 0;
gameActive = 1;
gameWon = 0;
gameLost = 0;
winStateTimer = 0;
gameStarted = 0;

stackAudioMode = 0;
winAudioMode = 0;
loseAudioMode = 0;
currentStep = 0;
beatCounter = 0;

for (unsigned char i = 0; i < 32; i++) {
gameBoard[i] = 0;
}

turnOffAllLEDs();
clearAllDisplays();
}

enum GameStates { GAME_INIT, GAME_WAITING_START, GAME_PLAYING, GAME_WIN, GAME_LOSE };

int TickGameController(int state) {
prevBTN1 = BTN1_PRESSED;
prevSTART_BTN = START_BTN_PRESSED;
unsigned char pinC = PINC;
BTN1_PRESSED = !(pinC & (1 << 0));
START_BTN_PRESSED = !(pinC & (1 << 1));

unsigned char buttonPressed = BTN1_PRESSED && !prevBTN1;
unsigned char startButtonPressed = START_BTN_PRESSED && !prevSTART_BTN;

switch (state) {
case GAME_INIT:
initGame();
TFT_Display_Ready();
state = GAME_WAITING_START;
break;

case GAME_WAITING_START:
clearAllDisplays();
if (startButtonPressed) {
gameStarted = 1;
state = GAME_PLAYING;
}
break;

case GAME_PLAYING:
if (buttonPressed && gameActive && gameStarted) {
unsigned char currentPattern = block2x4(blockPosition, blockWidth, blockHeight);

if (currentColumn == 0) {
gameBoard[currentColumn] = currentPattern;
gameBoard[currentColumn + 1] = currentPattern;
stackAudioMode = 1;
currentStep = 0;
beatCounter = 0;
currentColumn += 2;
blockPosition = 0;
}
else {
unsigned char overlap1 = currentPattern & gameBoard[currentColumn - 2];
unsigned char overlap2 = currentPattern & gameBoard[currentColumn - 1];

if (overlap1 == 0 || overlap2 == 0) {
gameLost = 1;
gameActive = 0;
loseAudioMode = 1;
currentStep = 0;
beatCounter = 0;
TFT_Display_Lose();
turnOnRedLEDs();
state = GAME_LOSE;
}
else {
gameBoard[currentColumn] = overlap1;
gameBoard[currentColumn + 1] = overlap2;
stackAudioMode = 1;
currentStep = 0;
beatCounter = 0;

unsigned char height1 = 0, height2 = 0;
for (unsigned char i = 0; i < 8; i++) {
if (overlap1 & (1 << i)) {
height1++;
}
if (overlap2 & (1 << i)) {
height2++;
}
}

if (height1 < height2) {
blockHeight = height1;
}
else {
blockHeight = height2;
}

if (blockHeight < 1) {
blockHeight = 1;
}

currentColumn += 2;
blockPosition = 0;

if (gameSpeed > 2) {
gameSpeed -= 2;
if (gameSpeed < 2) {
gameSpeed = 2;
}
}
}
}
}

if (gameActive && gameStarted && currentColumn >= 32) {
gameWon = 1;
gameActive = 0;
winAudioMode = 1;
stackAudioMode = 0;
loseAudioMode = 0;
currentStep = 0;
beatCounter = 0;
winStateTimer = 0;
TFT_Display_Win();
turnOnGreenLEDs();
state = GAME_WIN;
}

if (gameActive && gameStarted) {
speedCounter++;
if (speedCounter >= gameSpeed) {
speedCounter = 0;

if (movingDirection) {
if (blockPosition + blockHeight >= 8) {
movingDirection = 0;
}
else {
blockPosition++;
}
}
else {
if (blockPosition == 0) {
movingDirection = 1;
}
else {
blockPosition--;
}
}
}
}

updateDisplay();
break;

case GAME_WIN:
winStateTimer++;
if (startButtonPressed && winStateTimer > 15) {
winAudioMode = 0;
stackAudioMode = 0;
loseAudioMode = 0;
stopAudio();
turnOffAllLEDs();
state = GAME_INIT;
}
break;

case GAME_LOSE:
if (startButtonPressed) {
loseAudioMode = 0;
stackAudioMode = 0;
winAudioMode = 0;
stopAudio();
turnOffAllLEDs();
state = GAME_INIT;
}
break;

default:
state = GAME_INIT;
break;
}

return state;
}

enum AudioStates { AUDIO_SILENT, AUDIO_TONE_ON, AUDIO_TIMING };

int TickAudioController(int state) {
switch (state) {
case AUDIO_SILENT:
if (stackAudioMode || winAudioMode || loseAudioMode) {
state = AUDIO_TONE_ON;
}
break;

case AUDIO_TONE_ON:
if (currentStep < getMaxSteps()) {
createAudioTone(getCurrentFrequency());
beatCounter = 0;
state = AUDIO_TIMING;
}
else {
stopAudio();
currentStep = 0;
stackAudioMode = 0;
winAudioMode = 0;
loseAudioMode = 0;
state = AUDIO_SILENT;
}
break;

case AUDIO_TIMING:
if (beatCounter >= getCurrentBeatDuration() * 2) {
currentStep++;
state = AUDIO_TONE_ON;
}
else {
beatCounter++;
}
break;

default:
state = AUDIO_SILENT;
break;
}

switch (state) {
case AUDIO_SILENT:
stopAudio();
break;
case AUDIO_TONE_ON:
break;
case AUDIO_TIMING:
break;
}

return state;
}

int main(void) {
DDRB = 0x2F;
PORTB = 0x00;
DDRC = 0x3C;
PORTC = 0x03;
DDRD = 0x40;
PORTD = 0x00;

setupAudioHardware();
initMAX7219();
setupTFT();
turnOffAllLEDs();

tasks[0].period = GAME_PERIOD;
tasks[0].state = GAME_INIT;
tasks[0].elapsedTime = 0;
tasks[0].TickFct = &TickGameController;

tasks[1].period = AUDIO_PERIOD;
tasks[1].state = AUDIO_SILENT;
tasks[1].elapsedTime = 0;
tasks[1].TickFct = &TickAudioController;

TimerSet(GCD_PERIOD);

TimerOn();

while (1) {
}
return 0;
}