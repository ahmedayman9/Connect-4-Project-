
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
// Reset         (RST, pin 1) connected to PA7
// SSI0Fss       (CE,  pin 2) connected to PA3
// Data/Command  (DC,  pin 3) connected to PA6
// SSI0Tx        (Din, pin 4) connected to PA5
// SSI0Clk       (Clk, pin 5) connected to PA2
// 3.3V          (Vcc, pin 6) power
// back light    (BL,  pin 7) not connected, consists of 4 white LEDs which draw ~80mA total
// Ground        (Gnd, pin 8) ground

#include "tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "PortF.h"
#include "uart.h"


// *************************** Images ***************************
// width=16 x height=10

#define BOARD_ROWS 6
#define BOARD_COLS 7


const unsigned char logo1[] ={
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

void printBoard(char *board);
int takeTurn(char *board, int player, const char *);
int takeTurnRemote(char *board, int player, const char *);
int takeTurnAI(char *board, int player, const char *);
int hasEmptyCol(char *board, int col);
int changeBoard(char *board, int player, const char *PIECES, int col);
int checkWin(char *board);
int checkFour(char *board, int, int, int, int);
int horizontalCheck(char *board);
int verticalCheck(char *board);
int diagonalCheck(char *board);
void Delay100ms(unsigned long count); // time delay in 0.1 seconds

int horiCheck(char *board);
int vertiCheck(char *board);
int fourth(char *board);
int checkThree(char *board, int a, int b, int c, int d);

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

int flag;
int k=0;
int flag2=0;
int cols;
int avoid;

int count;


unsigned long SW1, SW2; // input from PF4,PF0
int selectMode(void);
void startingScreen(void);


int i = 0; 
int turn = 0, done = 0;
const char *PIECES = "xo";                  //array 2 values PIECES[0] = X, PIECES[1] = O
char board1[(BOARD_ROWS * BOARD_COLS) + 1]; // board1[6x7 + 1] = [43] .. leh msh 42??? 8aleban board[42] = '/0' bma eno array of char

int main(void)
{
    //TExaS_Init(SSI0_Real_Nokia5110_Scope);  // set system clock to 80 MHz
    // initialization goes here
    int mode,w;

    PortF_Init(); // Call initialization of port PF4, PF3, PF2, PF1, PF0
		EdgeCounter_Init();
    for (i = 0; i < BOARD_COLS * BOARD_ROWS + 1; i++)  // ngrb nshel el +1
    {
        board1[i] = ' ';
        //input[i]=' ';
    }
    //  kol elli gwa array el board space "fady"
    //   0  1  2  3  4  5  6
    //                          0
    //                          1
    //                          2
    //                          3
    //                          4
    //                          5

    Nokia5110_Init();
    UARTB_init();
    Nokia5110_Clear();
    startingScreen();
    mode = selectMode(); // mode 0 or 2
    Nokia5110_Clear();
    if (mode) // if mode = 1 .. p1 vs p2 .. UART
    { 

        printBoard(board1);
        for (turn = 0; turn < (BOARD_ROWS * BOARD_COLS) && !done; turn++)  
        { // from 0 to 42 and the game is not done yet (no winner)

		do
		{
		    printBoard(board1);
		} while (!takeTurn(board1, (turn) % 2, PIECES));
		done = checkWin(board1);
		turn++;
		if (done)
		    break;
		do
		{
		    printBoard(board1);
		} while (!takeTurnRemote(board1, (turn) % 2, PIECES));
		done = checkWin(board1);
            
        }
        printBoard(board1);
        if (turn == BOARD_ROWS * BOARD_COLS && !done)
        { // grid is full but there's no winner
            Nokia5110_OutString("It's a tie!");
        }
        else
        { // done = 1, there's a winner
            turn--;                 // bsbb enna zawdna el turn abl el check of winner (done)
            Nokia5110_Clear();
            Nokia5110_SetCursor(1, 1);
            Nokia5110_OutString("Player");
            Nokia5110_SetCursor(8, 1);
            Nokia5110_OutString(turn % 2 == 0 ? "X" : "O");
            Nokia5110_SetCursor(3, 2);
            Nokia5110_OutString("wins!");
            Nokia5110_SetCursor(1, 4);
            Nokia5110_OutString("GAME OVER");
					
		for (w = 0; w < 7; w++)
		{
			GPIO_PORTF_DATA_R |= (1<<1)| (1<<3);
			Delay100ms(5);
			GPIO_PORTF_DATA_R &= ~((1<<1)| (1<<3));

			GPIO_PORTF_DATA_R |= (1<<2);
			Delay100ms(5);

			GPIO_PORTF_DATA_R &= ~(1<<2);
			Delay100ms(5);
		}
        }
    }
    else
    {
        // playes vs ai
        printBoard(board1);
        for (turn = 0; turn < (BOARD_ROWS * BOARD_COLS) && !done; turn++)
        { // 42
           
	do
            {
                printBoard(board1);
            } while (!takeTurn(board1, (turn) % 2, PIECES));
            done = checkWin(board1);
            turn++;
            if (done)
                break;
	 do
            {
                printBoard(board1);
            } while (!takeTurnAI(board1, (turn) % 2, PIECES));
            
            done = checkWin(board1);
        }
        printBoard(board1);
        if (turn == BOARD_ROWS * BOARD_COLS && !done)
        {
            Nokia5110_OutString("It's a tie!");
        }
        else
        {
            turn--;
            Nokia5110_Clear();
            Nokia5110_SetCursor(1, 1);
            Nokia5110_OutString("Player");
            Nokia5110_SetCursor(8, 1);
            Nokia5110_OutString(turn % 2 == 0 ? "X" : "O");
            Nokia5110_SetCursor(3, 2);
            Nokia5110_OutString("wins!");
            Nokia5110_SetCursor(1, 4);
            Nokia5110_OutString("GAME OVER");
					
	for (w = 0; w < 7; w++)
	{
		GPIO_PORTF_DATA_R |= (1<<1)| (1<<3);
		Delay100ms(5);
		GPIO_PORTF_DATA_R &= ~((1<<1)| (1<<3));

		GPIO_PORTF_DATA_R |= (1<<2);
		Delay100ms(5);

		GPIO_PORTF_DATA_R &= ~(1<<2);
		Delay100ms(5);
	}
        }
    }
	while(1){
		WaitForInterrupt();
	}
		
}


void startingScreen()
{
    int w;
    Nokia5110_SetCursor(0, 0);
    Nokia5110_ClearBuffer();
    Nokia5110_PrintBMP(10, 35, logo1, 0)
    Nokia5110_DisplayBuffer();
    Nokia5110_SetCursor(0, 5);
    Nokia5110_OutString("Welcome :D!");
    Delay100ms(40);
    for (w = 0; w < 7; w++)
    {
        Nokia5110_SetCursor(0, w);
        Nokia5110_OutString("           ");
	GPIO_PORTF_DATA_R |= (1<<1)| (1<<3);
	Delay100ms(5);
	GPIO_PORTF_DATA_R &= ~((1<<1)| (1<<3));

	GPIO_PORTF_DATA_R |= (1<<2);
	Delay100ms(5);

	GPIO_PORTF_DATA_R &= ~(1<<2);
        Delay100ms(5);
    }
}

// take turn for the Kit player and PIECES can be X or O
int takeTurn(char *board, int player, const char *PIECES)
{
		int g;
    cols = 0;
	
    while (1)
    {

			SW1 = GPIO_PORTF_DATA_R & 0x10; // read PF4 into SW1
			SW2 = GPIO_PORTF_DATA_R & 0x01; // read PF4 into SW2

			Nokia5110_SetCursorChar(cols, 0, PIECES[player]);
			g = cols == 0 ? 6 : cols - 1;
			Nokia5110_SetCursorChar(g, 0, board[g]);            // momken nst5dm setcursor b3dha out char


			if (flag)
			{
				
				UARTB_OutChar(cols + 1 + '0');			//momken ashelha
				flag=0;
				return changeBoard(board, player, PIECES, cols);
			}
						
    }
   
}

int takeTurnAI(char *board, int player, const char *PIECES)
{
	int col ; 
	int v=0;

	for(i=0;i<BOARD_COLS;i++)
	{
		if(hasEmptyCol(board,i))
		{
			v+=1;
		}
	}
	
	while(v>0){

		if(fourth(board))
		{
			return changeBoard(board , player , PIECES , avoid );
		}
		else{
			col=rand()%BOARD_COLS; //func el random
			if(hasEmptyCol(board,col))
			{
				return changeBoard(board, player, PIECES, col); 
			}
  	}
	}
return 0;	
}

int takeTurnRemote(char *board, int player, const char *PIECES) //x o
{

    int col = 0;
    col = UARTB_InChar(); // bkhod el data elli f reg el data fl uart .. arkam in asscci
    col = col - 1 - '0';  // col -1 3shan ana array mn 0 .... -'0' 3shan a7wl mn char to intg
		col = col%7;
   
    return changeBoard(board, player, PIECES, col); // 1 y3ni 7atetha successfully , 0 yb2a el col malyan w error
}

int hasEmptyCol(char *board, int col)       // bymshi mn ta7t l fo2 fi nfs el col y-check 3la 7eta fadya
{
    int row;
    for (row = BOARD_ROWS - 1; row >= 0; row--)
    {
        if (board[BOARD_COLS * row + col] == ' ')
        {
            return 1;
        }
    }
    return 0;
}
volatile unsigned long FallingEdges = 0;
void EdgeCounter_Init(void){                          
  SYSCTL_RCGC2_R |= 0x00000020; // (a) activate clock for port F
	
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0        
	
  FallingEdges = 0;             // (b) initialize counter
  
  GPIO_PORTF_IS_R &= ~0x11;     // (d) PF4 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x11;    //     PF4 is not both edges
  GPIO_PORTF_IEV_R &= ~0x11;    //     PF4 falling edge event
  GPIO_PORTF_ICR_R = 0x11;      // (e) clear flag4
  GPIO_PORTF_IM_R |= 0x11;      // (f) arm interrupt on PF4
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00A00000; // (g) priority 5
  NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC
  EnableInterrupts();           // (i) Clears the I bit
}

void GPIOPortF_Handler(void){
		flag=0;

		FallingEdges = FallingEdges + 1;
		if(GPIO_PORTF_RIS_R&(1<<4)){
	    GPIO_PORTF_ICR_R = 0x10;
			if(flag2){
				cols++;
				if (cols > 6)                    // aw n7ot %7
						cols = 0;
			}
			else{
				Nokia5110_SetCursor(0, k + 2);
				Nokia5110_OutString("  ");
				k += 2;
				if (k > 2)
						k = 0;
				Nokia5110_SetCursor(0, k + 2);
				Nokia5110_OutString("->");
				}

		}
	if(GPIO_PORTF_RIS_R&(1<<0)){
	    GPIO_PORTF_ICR_R = 0x01;
			flag = 1;
	}

}

void Delay100ms(unsigned long count)
{
    unsigned long volatile time;
    while (count)
    {
        time = 72724; // 0.1sec at 80 MHz
        while (time)
        {
            time--;
        }
        count--;
    }
}

int selectMode()
{ // here is selecting if the mode is P1 VS AI or PI vs P2
    Nokia5110_SetCursor(0, 0);
    Nokia5110_OutString("Select");
    Nokia5110_SetCursor(7, 0);
    Nokia5110_OutString("mode");
    Nokia5110_SetCursor(3, 2);
    Nokia5110_OutString("P1 VS AI");
    Nokia5110_SetCursor(3, 4);
    Nokia5110_OutString("P1 VS P2");
    Nokia5110_SetCursor(0, 2);
    Nokia5110_OutString("->");

		while(1){
		SW1 = GPIO_PORTF_DATA_R & 0x10; // read PF4 into SW1  pin no.4  0001 0000
		
		SW2 = GPIO_PORTF_DATA_R & 0x01; // read PF4 into SW2
		if (flag)
		{
			flag2 =1;
			flag =0;
			return k;
		}
	}
    
}

void printBoard(char *board)
{
    int row, col;
    Nokia5110_ClearBuffer();
    for (row = 0; row < BOARD_ROWS; row++)
    { //row =1
        for (col = 0; col < BOARD_COLS; col++)
        {                                                                     // col = 0
            Nokia5110_SetCursorChar(col, row, board[BOARD_COLS * row + col]); //board[7x1 + 0] ,.. el row el tany bybda2 mn 7
        }
    }
}

int changeBoard(char *board, int player, const char *PIECES, int col)
{
    int row;
    for (row = BOARD_ROWS - 1; row >= 0; row--)
    { // b3ml check fl chosen col mn ta7t l fo2 3shan a7ot f awl mkan empty al2eh
        if (board[BOARD_COLS * row + col] == ' ')
        {
            board[BOARD_COLS * row + col] = PIECES[player]; // b7ot el player fl row da
            return 1;
        }
    }
    return 0; // da m3nah en kol el col malyan fa mynf3sh y7ot feyh
}

int checkWin(char *board)
{
    return (horizontalCheck(board) || verticalCheck(board) || diagonalCheck(board));
}

int checkFour(char *board, int a, int b, int c, int d)
{
    return (board[a] == board[b] && board[b] == board[c] && board[c] == board[d] && board[a] != ' ');
			
}

int horizontalCheck(char *board)
{
    int row, col, idx;
    const int WIDTH = 1;

    for (row = 0; row < BOARD_ROWS; row++)  //6 7
        { // mn zero to 6
        for (col = 0; col < BOARD_COLS - 3; col++) 
        {                                 // mn 0 to 4 , l2n el func nfsaha bt inc bl width 3 cols kman
            idx = BOARD_COLS * row + col; // bmshi row a check 3la kol el cols b3dha adkhol 3l row el b3deh w hakza
            if (checkFour(board, idx, idx + WIDTH, idx + WIDTH * 2, idx + WIDTH * 3))
            {
                return 1; // check 3l kol 4 gamb b3d l7d col 4
            }
        }
    }
    return 0; // mafesh 4 horz gamb b3d
}

int verticalCheck(char *board)
{
    int row, col, idx;
    const int HEIGHT = 7;

    for (row = 0; row < BOARD_ROWS - 3; row++)
    { // mn 0 to 4 , l2n el func nfsaha bt inc bl width 3 rows kman
        for (col = 0; col < BOARD_COLS; col++)
        {                                 // mn zero to 6
            idx = BOARD_COLS * row + col; // bmshi row a check col mo3yn b3dha a3ml inc b 7 3shan adkhol fl row elli b3deh f nfs el saf
            if (checkFour(board, idx, idx + HEIGHT, idx + HEIGHT * 2, idx + HEIGHT * 3))
            {
                return 1; // check 3l kol element fl row w 3l 3 eli t7teh nfs el col
            }
        }
    }
    return 0; // mafesh 4 vert
}

int diagonalCheck(char *board)
{
    int row, col, idx, count = 0;
    const int DIAG_RGT = 6, DIAG_LFT = 8;
    //  el far2 ben el element w east-south = 8, west-south = 6
    // 4 + 6 = 10 .... 4 + 8 = 12

    //            4
    //       10      12

    for (row = 0; row < BOARD_ROWS - 3; row++)
    {
        for (col = 0; col < BOARD_COLS; col++)
        {
            idx = BOARD_COLS * row + col;
            if ((count <= 3 && checkFour(board, idx, idx + DIAG_LFT, idx + DIAG_LFT * 2, idx + DIAG_LFT * 3)) || (count >= 3 && checkFour(board, idx, idx + DIAG_RGT, idx + DIAG_RGT * 2, idx + DIAG_RGT * 3)))
            {
                return 1;
            }
            count++;
        }
        count = 0;
    }
    return 0;
}
// el DIAG_LFT a5rha l7d col 3 ... el DIAG_RGT btbda2 tzhar mn col 3
//            *
//            *
//   0  1  2  3  4  5  6

//   0  1  2  3  4  5  6    0
//   7  8  9  10 11 12 13   1
//   21 22 23 24 25 26 27   3
//   38 29 30 31 32 33 34   4
//   35 36 37 38 39 40 41   5
//               46

int checkThree(char *board, int a, int b, int c, int d){
    		return (board[a] == board[b] && board[b] == board[c] && board[a] != ' ' && board[d]==' ' && (board[d+7]!=' ' || (d+7 < 41)));
}

int fourth(char *board)
{
    if(horiCheck(board)||vertiCheck(board))
	{
		return 1;
	} 
	else{
		return 0;
	}
		
}


int horiCheck(char *board)
{
    int row, col, idx;
    const int WIDTH = 1;

    for (row = 0; row < BOARD_ROWS; row++)  //6 7
        { // mn zero to 6
        for (col = 0; col < BOARD_COLS - 3; col++) 
        {                                 // mn 0 to 4 , l2n el func nfsaha bt inc bl width 3 cols kman
            idx = BOARD_COLS * row + col; // bmshi row a check 3la kol el cols b3dha adkhol 3l row el b3deh w hakza
            if (checkThree(board, idx, idx + WIDTH, idx + WIDTH * 2, idx + WIDTH * 3))
            {
							 avoid = col+3;
                return 1; // check 3l kol 4 gamb b3d l7d col 4
            }
        }
    }
    return 0; // mafesh 4 horz gamb b3d
}

int vertiCheck(char *board)
{
    int row, col, idx;
    const int HEIGHT = 7;

    for (row = 0; row < BOARD_ROWS - 3; row++)
    { // mn 0 to 4 , l2n el func nfsaha bt inc bl width 3 rows kman
        for (col = 0; col < BOARD_COLS; col++)
        {                                 // mn zero to 6
            idx = BOARD_COLS * row + col; // bmshi row a check col mo3yn b3dha a3ml inc b 7 3shan adkhol fl row elli b3deh f nfs el saf
            if (checkThree(board, idx, idx + HEIGHT, idx + HEIGHT * 2, idx + HEIGHT * 3))
            {
								avoid = col;
                return 1; // check 3l kol element fl row w 3l 3 eli t7teh nfs el col
            }
        }
    }
    return 0; // mafesh 4 vert
}


