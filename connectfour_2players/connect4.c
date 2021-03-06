// ******* Required Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PE2/AIN1
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Blue Nokia 5110
// ---------------
// Signal        (Nokia 5110) LaunchPad pin
// Reset          RST connected to PA7
// SSI0Fss        CE  connected to PA3
// Data/Command   DC  connected to PA6
// SSI0Tx         Din connected to PA5
// SSI0Clk        Clk connected to PA2
// 3.3V           Vcc power 3.3V
// back light     BL  not connected, consists of 4 white LEDs which draw ~80mA total
// Ground         Gnd ground

#include "..//tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"
#include "Sound.h"
#include "ADC.h"

void PortF_Init(void);
void DisableInterrupts(void); 
void EnableInterrupts(void);  
void SysTick_Init(void);
void Timer2_Init(unsigned long period);
void Delay100ms(unsigned long count); // time delay in 0.1 seconds
unsigned long TimerCount;
unsigned long Semaphore;

int ADC0_rescale(int val);

void SysTick_Wait10ms(unsigned long delay); 
void SysTick_Wait(unsigned long delay);

void play(int pos , int player); 
void screen_win(int p); 

void startingScreen(void) ;
void Animations(int max);
void displayandwait(int time);
int  select_mode(void); 
int AI_decision(void);

void check_vertical(void); 
void check_horizontal(void); 
void check_diagonal(void); 



const unsigned char black[] ={
 0x42, 0x4D, 0x8E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x0F, 0xFF,
 0xF0, 0x00, 0x0F, 0xFF, 0xF0, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,

};

const unsigned char blank[] ={
 0x42, 0x4D, 0x8E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xF0, 0x00,
 0x0F, 0x00, 0xF0, 0x00, 0x0F, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF,

};


const unsigned char cursor[] ={
 0x42, 0x4D, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x0F, 0xFF, 0xF0, 0x00, 0xFF, 0xFF,
 0xFF, 0x00, 0xFF,

};


const unsigned char gridpiece[] ={
 0x42, 0x4D, 0x96, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x0F, 0xF0, 0x00,
 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,

};

const unsigned char nullImage[] ={0};
const unsigned char Logo[] ={	// Connect 4 Symbol
 0x42, 0x4D, 0x4A, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0xD4, 0x03, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x80, 0x00, 0x00, 0x07, 0x77, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88, 0x80, 0x00, 0x00, 0x08, 0xF8, 0xF7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x08, 0x88, 0x80, 0x00, 0x00, 0x88, 0x88, 0x88, 0x00, 0x03, 0x00, 0x00, 0x08, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88, 0x88,
 0x88, 0x00, 0x88, 0x88, 0x8F, 0x00, 0x8F, 0xF7, 0x00, 0x0F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88, 0x88, 0x88, 0x80, 0x78, 0x88,
 0xFF, 0x07, 0xFF, 0xFF, 0x00, 0x08, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88, 0x88, 0x88, 0x80, 0x07, 0x8F, 0xF0, 0x07, 0xF8, 0x7F,
 0x00, 0x88, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xF7, 0x0F, 0x00, 0x88, 0x30, 0x88,
 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xF7, 0x08, 0x00, 0x88, 0x88, 0x8F, 0x70, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x88, 0x00, 0x00, 0x00, 0x07, 0x88, 0x70, 0x0F, 0xF0, 0x08, 0x08, 0x80, 0x0F, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x88, 0x80, 0x00, 0x00, 0x78, 0x88, 0x88, 0x0F, 0xF0, 0x0F, 0x08, 0x80, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x88, 0x88, 0x88, 0x88, 0x00, 0xFF, 0x88, 0x8F, 0x00, 0x00, 0x08, 0x08, 0x80, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88, 0x88,
 0x88, 0x00, 0x88, 0x88, 0xF8, 0x00, 0x00, 0x00, 0x08, 0x88, 0x7F, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88,
 0xF7, 0x00, 0x00, 0x00, 0x00, 0x88, 0x88, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x70, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x28, 0x87, 0x70, 0x00, 0x00, 0x00, 0x00, 0x08, 0x80, 0x07, 0x07, 0x70, 0x08, 0x70,
 0x7F, 0x8F, 0x70, 0x07, 0x78, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x88, 0x88, 0xF8, 0x70, 0x00, 0x00, 0x00, 0x08, 0x80, 0x08, 0x07, 0x80, 0x88, 0x70, 0x7F, 0x87, 0x70, 0x88,
 0x88, 0x80, 0x00, 0xF7, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xFF, 0x77, 0x78, 0x70, 0x00, 0x88, 0x73, 0x0F, 0x80, 0x88, 0x07, 0x70, 0x88, 0x70, 0x78, 0x00, 0x08, 0xF8, 0x77, 0x80, 0x00, 0xF7,
 0x00, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x87, 0x00, 0x07, 0x70, 0x08, 0x88, 0x88, 0x08, 0x88, 0x88, 0x07, 0x78, 0x88, 0x70, 0x7F, 0x88, 0x08, 0xF7, 0x00, 0x00, 0x00, 0xF7, 0x00, 0x00, 0x00, 0x00,
 0x77, 0x88, 0x87, 0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0x08, 0x88, 0x88, 0x07, 0x88, 0x87, 0x70, 0x7F, 0xF8, 0x08, 0xF7, 0x00, 0x00, 0x00, 0xF7, 0x00, 0x00, 0x00, 0x00, 0x68, 0x88, 0x70, 0x00,
 0x00, 0x00, 0x88, 0x88, 0x88, 0x08, 0x88, 0x88, 0x07, 0x88, 0x00, 0x70, 0x88, 0x70, 0x08, 0xF7, 0x00, 0x20, 0x00, 0xF7, 0x00, 0x00, 0x00, 0x00, 0x68, 0x88, 0x70, 0x00, 0x00, 0x00, 0x08, 0x88,
 0x87, 0x08, 0x88, 0x08, 0x08, 0x87, 0x00, 0x80, 0xF8, 0x77, 0x08, 0xF7, 0x33, 0x00, 0x08, 0xF8, 0x70, 0x00, 0x00, 0x00, 0x68, 0x88, 0x70, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x08, 0x80, 0x08,
 0x07, 0xF7, 0x03, 0x80, 0x7F, 0x88, 0x00, 0xFF, 0x78, 0x00, 0x8F, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x78, 0x88, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x80, 0x00, 0x00,
 0x8F, 0xFF, 0x70, 0x8F, 0xFF, 0x00, 0xFF, 0x88, 0xF7, 0x00, 0x00, 0x00, 0x08, 0x8F, 0xF7, 0x00, 0x06, 0x70, 0x00, 0x08, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xF7, 0x00, 0x07, 0x70, 0x78, 0x8F, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0x8E, 0x70, 0x68, 0x00, 0x78, 0x88, 0x88, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x0F, 0x8E, 0x88, 0xF8, 0x00, 0x78, 0x88, 0x88, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x78, 0x87,
 0x70, 0x00, 0x7F, 0x88, 0x8F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xFF,
 0x84, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,

};


unsigned long SW1,SW2;  // input from PF4,PF0
int cursorPlace = 0 ; 
int ADCdata ; 
int sliderPosition ; 
int button1pressed = 1 ; //1 means no , 0 means yes 
int button2pressed = 1 ; 
int col = 0 , row = 0  ; 
int game[6][7] ;
int turn = 0 ; 
int nextcycle = 0 ; //this is used to force the ai to wait to the net cycle to play 
int game_mode = 0 ;  //this case doesn't happen when 2 players plays as we are not as fast as the code!
int x = 0 ;

//volatile unsigned long ticks = 0 ; 

int p_ver[3] , p_hor[3] , p_dia[3];  
int p_win[3] ; 
 

int roww , roww2 , temp = 0 ; 	


struct gameObject {
	  const unsigned char* spirit ;
};

typedef struct gameObject GameObject ; 

/*
game[i][j] == 0 => empty place 
game[i][j] == 1 => player1 has a chip in this position
game[i][j] == 2 => player2 has a chip in this position
*/

GameObject Chips[3] = {{nullImage},{black},{blank}}; 

//int i , j ; 
int main(void){
	SysTick_Init() ; 
	PortF_Init(); 
	TExaS_Init(SSI0_Real_Nokia5110_Scope); 
	ADC0_Init();    // initialize ADC0, channel 1, sequencer 3
//Random_Init(1);
  Nokia5110_Init();
	EnableInterrupts();           
  Nokia5110_ClearBuffer();

	startingScreen();
	game_mode = select_mode(); //1 means p1vp2 , 0 p1vai
	Animations(15);
	
		//this crazy 2 for loops destroys my array game[][] 
			//it changes values to 0x101010
/*	
		for(i=0; i < 7 ; i++){
		for(j=0 ; j < 6 ; j++)
		{	Nokia5110_PrintBMP(12 + (i * 8) , 8 + (j * 8 ), gridpiece , 0);}}
	*/
	
	
  while(1){ 	
		
	 for(col=0; col < 7 ; col++){
		Nokia5110_PrintBMP(12 + (col * 8) , 8 , gridpiece , 0);
		for(row=0 ; row < 6 ; row++)
		{
			if (game[row][col] > 0 )
			Nokia5110_PrintBMP(13 +(8*col) ,7 +(8*row), Chips[game[row][col]].spirit, 0); 
			}}
			
	
		//read slider data 
			sliderPosition = ADC0_rescale( ADC0_In());
		
		//move the slider 
			Nokia5110_PrintBMP( 13 + 8* sliderPosition , 4, cursor, 0);
		

		//read switches and play after moving the slider 	 
  		SW1 = (GPIO_PORTF_DATA_R&0x10);     // read PF4 into SW1
      SW2 = (GPIO_PORTF_DATA_R&0x01);     // read PF0 into SW2
		
		 nextcycle = !nextcycle ;
			
		 if(!SW1 && SW2 && !turn ){                // just SW1 pressed & player 1 turn
         if(button1pressed)
				 {
					 play(sliderPosition , 1); 
					 button1pressed = 0 ; 
					 turn = 1 ;
					 nextcycle = 0 ; 
					 
				 }
      }else{ 
					button1pressed = 1 ; 
			}		 
     if(SW1 && !SW2 && turn && game_mode ){      // just SW2 pressed & player 2 turn & P1VsP1 
         if(button2pressed)
				 {
					 play(sliderPosition , 2); 
					 button2pressed = 0 ; 
					 turn = 0 ; 
				 }
      }else{ 
					button2pressed = 1 ; 
			}		
		
			if (turn && !game_mode && nextcycle)
			{
					play(AI_decision() , 2);
					turn = 0 ; 
			}
			
			//display the game 
			displayandwait(7);			
			
			check_vertical();
			check_horizontal();
			check_diagonal() ; 
			 
			for (x = 1 ; x < 3 ; x++)
			{		
			if(p_win[x] == 1 ) 
			{ 
				screen_win(x);
			}
			}

}
}

int AI_decision(void){
	//	int decision ; 
	
	return 0 ; 
}
int select_mode(){//there is no modes yet
	int mode ; 
	while (1) {
	sliderPosition = ADC0_rescale( ADC0_In());
	Nokia5110_SetCursor(0,0);
	Nokia5110_OutString("select");
	Nokia5110_SetCursor(7,0);
	Nokia5110_OutString("mode");
	
	if (sliderPosition < 3)
	{Nokia5110_SetCursor(0,2);
	Nokia5110_OutString(">> P1 VS P2");
	Nokia5110_SetCursor(0,3);
	Nokia5110_OutString("   P1 VS AI");
	mode = 1 ; 
	}
	else 
	{
	Nokia5110_SetCursor(0,2);
	Nokia5110_OutString("   P1 VS P2");
	Nokia5110_SetCursor(0,3);
	Nokia5110_OutString(">> P1 VS AI");
	mode = 0 ; 
	}
	
	SysTick_Wait10ms(7);
	
	SW1 = (GPIO_PORTF_DATA_R&0x10);     // read PF4 into SW1
  SW2 = (GPIO_PORTF_DATA_R&0x01);     // read PF0 into SW2
	
		 if(!SW1 || !SW2)                // if any of the buttons is pressed
				break; 

	}
	return mode ; 
}
void startingScreen(void){
	Nokia5110_SetCursor(1,1);
	Nokia5110_ClearBuffer();
	Nokia5110_PrintBMP(15, 35, Logo, 0);	// On the screen : Col no. 10 ---- Row no. 35
	//Last parameter 0 means OFF if color is 0 and ON if in range 1 to 15 as 16 color pixels
	Nokia5110_DisplayBuffer();
	Nokia5110_SetCursor(2,5);
	Nokia5110_OutString("Welcome !");
	SysTick_Wait10ms(200);//200
	for(x=0 ; x<6 ; x++){
		Nokia5110_SetCursor(1,x);
		Nokia5110_OutString("           ");
		SysTick_Wait10ms(30);//30
	}
	
	Nokia5110_ClearBuffer();
	
}


void Animations(int max) {
for (x = 0 ; x < max ; x++)		
	{row = roww ;
		for(col=0; col < 7 ; col++){
			Nokia5110_PrintBMP(13 +(8*col) ,7 +(8*row), Chips[2].spirit, 0); 
			row--; 
		}
		
		
	row = roww; 
	for(col= 6; col >= 1 ; col--){
			Nokia5110_PrintBMP(13 +(8*col) ,7 +(8*row), Chips[2].spirit, 0); 
			row++; 
		}
			
				row = roww2 ; 
		for(col=0; col < 7 ; col++){
			Nokia5110_PrintBMP(13 +(8*col) ,7 +(8*row), Chips[1].spirit, 0); 
			row++; 
		}
		
		
	row = roww2; 
	for(col= 6; col >= 1 ; col--){
			Nokia5110_PrintBMP(13 +(8*col) ,7 +(8*row), Chips[1].spirit, 0); 
			row--; 
		}
					
		roww = (roww+1) % 6; // 0 to 5 
		roww2 = (5 - roww) ; // 5 to 0
		
			displayandwait(20);
	}

}
void displayandwait(int time){
			Nokia5110_DisplayBuffer();
			SysTick_Wait10ms(time); //I wanted to use systic periodic timer but didn't work ! 
			Nokia5110_ClearBuffer();
			//drawgrid();
}
void screen_win(int p){
	while(1){
				int x ; 
		for (x =0 ; x <2 ; x++)
			{	Nokia5110_Clear();		
				Nokia5110_SetCursor(1, 1);
				Nokia5110_OutString("Player");
				Nokia5110_SetCursor(1, 2);		
				Nokia5110_OutUDec(p);
				Nokia5110_SetCursor(1, 3);
				Nokia5110_OutString("wins !");
		
				SysTick_Wait10ms(40);
				Nokia5110_Clear();
				SysTick_Wait10ms(40);
			}
		for (x = 0 ; x < 2 ; x++)			
			{	Nokia5110_SetCursor(1, 1);
				Nokia5110_OutString("press RESET");		
				Nokia5110_SetCursor(1, 2);
				Nokia5110_OutString("to play");
				Nokia5110_SetCursor(1, 3);		
				Nokia5110_OutString("new game");
				
				SysTick_Wait10ms(40);
				Nokia5110_Clear();
				SysTick_Wait10ms(40);
			}
}

}

void check_horizontal(void){

		for(row=0; row < 6 ; row++){
		for(col=0 ; col < 7 ; col++)
		{
		if(col > 0 && game[row][col-1] != game[row][col] ) //on change reset counter
		{p_hor[game[row][col-1]] = 0 ; }
		 p_hor[game[row][col]]++ ; 
		
	
		if(	p_hor[game[row][col]] >= 4 )
				p_win[game[row][col]] = 1 ; 
		
		
		}
		p_hor[0] = 0 ;
		p_hor[1] = 0 ;
		p_hor[2] = 0 ;
		
	if(p_win[1] || p_win[2] )
					break ; 	
		}
}

void check_vertical(void){
					
		for(col=0; col < 7 ; col++){
		for(row=0 ; row < 6 ; row++)
		{
		if(row > 0 && game[row-1][col] != game[row][col] ) //on change reset counter
		{p_ver[game[row-1][col]] = 0 ; }
		 p_ver[game[row][col]]++ ; 
		
			if (p_ver[game[row][col]] >= 4 )
			{p_win[game[row][col]] = 1 ; }
		}		
		p_ver[0] = 0 ;
		p_ver[1] = 0 ;
		p_ver[2] = 0 ;
		
		if(p_win[1] || p_win[2] )
					break ; 
		}
}

void check_diagonal(void){
	temp = 0; 
	
	for (row = 0 ; row <  6 ; row++)
	{ roww = row ;
		for(col=0; col < 7 ; col++){ 
			
			if (roww > 5  || roww <0) 
				continue ; 
			
			
			if ( temp != game[roww][col])
			p_dia[temp] = 0 ; 
			
			
			p_dia[game[roww][col]]++ ; 

			if (p_dia[game[roww][col]] >= 4 )
			{p_win[game[roww][col]] = 1 ;}		
			temp = game[roww][col] ; 
			roww--;
		}
		p_dia[0] = 0 ;
		p_dia[1] = 0 ;
		p_dia[2] = 0 ;
		temp = 0 ; 
	
		if(p_win[1] || p_win[2] )
					break ; 
	}

		for (row = 0 ; row <  6 ; row++)
		{ roww = row ; 
		for(col= 6; col >= 1 ; col--){
		if (roww > 5  || roww <0) 
				continue ; 
			
		
			if (temp != game[roww][col])
				p_dia[temp] = 0 ;
			
			p_dia[game[roww][col]]++ ; 

			if (p_dia[game[roww][col]] >= 4 )
			{p_win[game[roww][col]] = 1 ; }
			
			temp = game[roww][col] ;
			roww++;
			 
		}
		p_dia[0] = 0 ;
		p_dia[1] = 0 ;
		p_dia[2] = 0 ;
		temp = 0 ; 
		if(p_win[1] || p_win[2] )
					break ;
	}

	for (row = 5 ; row >= 0  ; row--)
	{	roww2 = row ; 
		for(col=0; col < 7 ; col++){
	if (roww2 > 5  || roww2 <0) 
				continue ; 
					
			if (temp != game[roww][col])
				p_dia[temp] = 0 ; 
			
			
			p_dia[game[roww2][col]]++ ; 

			if (p_dia[game[roww2][col]] >= 4 )
			{p_win[game[roww2][col]] = 1 ; }
			
			temp = game[roww2][col] ;
			roww2++;
			 
		}
		p_dia[0] = 0 ;
		p_dia[1] = 0 ;
		p_dia[2] = 0 ;
		temp = 0 ; 
		if(p_win[1] || p_win[2] )
					break ;
	}
		
		
	for (row = 5 ; row >= 0  ; row--)
	{	roww2 = row ; 
	for(col= 6; col >= 1 ; col--){
	if (roww2 > 5  || roww2 <0) 
				continue ; 
			
		if (temp != game[roww2][col])
			p_dia[temp] = 0 ; 

			
			p_dia[game[roww2][col]]++ ; 

			if (p_dia[game[roww2][col]] >= 4 )
			{p_win[game[roww2][col]] = 1 ; }
			
			temp = game[roww2][col] ;
			roww2--;
			 
		}
		p_dia[0] = 0 ;
		p_dia[1] = 0 ;
		p_dia[2] = 0 ;
		temp = 0 ; 
		if(p_win[1] || p_win[2] )
					break ;
	
	}
	
	}




void play(int pos , int player){
		
		for(row=5 ; row >=0 ; row--)
		{	
			if (game[row][pos] > 0  ) 
					continue ;
			
			game[row][pos] = player ; 
			Nokia5110_PrintBMP( 13 +(8 * pos) ,7 + (8 * row) , Chips[player].spirit, 0); // player ship middle bottom 
			break ;
		}
}


void SysTick_Init(void){

  NVIC_ST_CTRL_R = 0;              // 1) disable SysTick during setup

//  NVIC_ST_RELOAD_R = 0x00FFFFFF;   // 2) maximum reload value

// NVIC_ST_CURRENT_R = 0;           // 3) any write to current clears it
	
	NVIC_ST_CTRL_R = 0x00000005;     // 4) enable SysTick with core clock

	
}
//for some reason this function not working
void SysTick_Handler (void){
//ticks++ ; 	
}
//but this works fine
void SysTick_Wait(unsigned long delay){

  NVIC_ST_RELOAD_R = delay-1;  // number of counts to wait

  NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears

  while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag

  }

}

// 800000*12.5ns equals 10ms
void SysTick_Wait10ms(unsigned long delay){

  unsigned long i;
	//ticks++ ;
  for(i=0; i<delay; i++){

    SysTick_Wait(800000);  // wait 10ms

  }

}



int ADC0_rescale(int val){

if (val < 585 )
	return 0 ; 
else if (val < 1170)
	return 1 ; 
else if (val < 1745)
	return 2 ; 
else if (val < 2330)
	return 3 ; 
else if (val < 2905)
	return 4 ; 
else if (val < 3490) 
	return 5 ; 
else if (val < 4100)
	return 6 ; 
else 
	return 6 ; 
}


void Timer2_Init(unsigned long period){ 

  unsigned long volatile delay;
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate timer2
  delay = SYSCTL_RCGCTIMER_R;
  TimerCount = 0;
  Semaphore = 0;
  TIMER2_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = period-1;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
  TIMER2_CTL_R = 0x00000001;    // 10) enable timer2A
}
void Timer2A_Handler(void){ 
  TIMER2_ICR_R = 0x00000001;   // acknowledge timer2A timeout
  TimerCount++;
  Semaphore = 1; // trigger
}


void Delay100ms(unsigned long count){unsigned long volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}
void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0        
}
