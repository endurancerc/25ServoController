/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>
#include <usart.h>
#include <delays.h>
#include "typedefs.h"
#include "usb.h"
#include "io_cfg.h"             // I/O pin mapping
#include "user_generic_hid.h"

/** V A R I A B L E S ********************************************************/
#pragma udata

byte number_of_bytes_read;
char transmit_buffer[EP0_BUFF_SIZE];
char receive_buffer[EP0_BUFF_SIZE];

int count;
unsigned int pulse1, pulse2, pulse3, pulse4, pulse5, pulse6, pulse7, pulse8;
unsigned int pulse9, pulse10, pulse11, pulse12, pulse13, pulse14, pulse15, pulse16;
unsigned int pulse17, pulse18, pulse19, pulse20, pulse21, pulse22, pulse23, pulse24, pulse25;

unsigned int dif1, dif2, dif3, dif4, dif5, dif6, dif7, dif8;
unsigned int dif9, dif10, dif11, dif12, dif13;

/** P R I V A T E  P R O T O T Y P E S ***************************************/
void BlinkUSBStatus(void);
void ReportLoopback(void);
void HandleControlOutReport(void);
void step(int);

/** D E C L A R A T I O N S **************************************************/
#pragma code
void UserInit(void) {

	//PWM Pins
	TRISDbits.TRISD2 = 0; //pin d2 output, output 1
	TRISDbits.TRISD3 = 0; //pin d3 output, output 2
	TRISCbits.TRISC6 = 0; //pin c6 output, output 3
	//TRISCbits.TRISC7 = 0; //pin c7 output, output 4
	TRISDbits.TRISD4 = 0; //pin d4 output, output 5
	TRISDbits.TRISD6 = 0; //pin d6 output, output 6
	TRISDbits.TRISD7 = 0; //pin d7 output, output 7
	TRISBbits.TRISB0 = 0; //pin b0 output, output 8
	TRISBbits.TRISB1 = 0; //pin b1 output, output 9
	TRISBbits.TRISB2 = 0; //pin b2 output, output 10
	TRISAbits.TRISA4 = 0; //pin a4 output, output 11
	TRISAbits.TRISA3 = 0; //pin a3 output, output 12
	TRISAbits.TRISA2 = 0; //pin a2 output, output 13
	TRISAbits.TRISA1 = 0; //pin a1 output, output 14
	TRISAbits.TRISA0 = 0; //pin a0 output, output 15
	TRISBbits.TRISB4 = 0; //pin b4 output, output 16
	TRISBbits.TRISB3 = 0; //pin b3 output, output 17
	TRISAbits.TRISA5 = 0; //pin a5 output, output 18
	TRISEbits.TRISE0 = 0; //pin e0 output, output 19
	TRISEbits.TRISE1 = 0; //pin e1 output, output 20
	TRISCbits.TRISC0 = 0; //pin c0 output, output 21
	TRISCbits.TRISC1 = 0; //pin c1 output, output 22
	TRISCbits.TRISC2 = 0; //pin c2 output, output 23
	TRISDbits.TRISD0 = 0; //pin d0 output, output 24
	TRISDbits.TRISD1 = 0; //pin d1 output, output 25

	//LED Pin
	TRISCbits.TRISC7 = 0; //pin C7 output
	LATCbits.LATC7 = 1; //on
	//TRISDbits.TRISD5 = 0; //pin d5 output
	//LATDbits.LATD5 = 1; //on

	// Timer2 prescale = 16
	T2CONbits.T2CKPS0 = 0;
	T2CONbits.T2CKPS1 = 1;
	
	// Timer2 postscale = 16
	T2CONbits.T2OUTPS0 = 1;
	T2CONbits.T2OUTPS1 = 1;
	T2CONbits.T2OUTPS2 = 1;
	T2CONbits.T2OUTPS3 = 1;

	// Timer period
	PR2 = 0xE9;  //233
	//83.3*16*16*(233+1)= 5ms

	// Timer2 is on
	T2CONbits.TMR2ON = 1;

    // Enable Timer2 interrupt
    PIE1bits.TMR2IE = 1;

	count = 2;
	pulse1 = 150;
	pulse2 = 150;
	pulse3 = 150;
	pulse4 = 150;
	pulse5 = 150;
	pulse6 = 150;
	pulse7 = 150;
	pulse8 = 150;
	pulse9 = 150;
	pulse10 = 150;
	pulse11 = 150;
	pulse12 = 150;
	pulse13 = 150;
	pulse14 = 150;
	pulse15 = 150;
	pulse16 = 150;
	pulse17 = 150;
	pulse18 = 150;
	pulse19 = 150;
	pulse20 = 150;
	pulse21 = 150;
	pulse22 = 150;
	pulse23 = 150;
	pulse24 = 150;
	pulse25 = 150;

	dif1 = 0;
	dif2 = 0;
	dif3 = 0;
	dif4 = 0;
	dif5 = 0;
	dif6 = 0;
	dif7 = 0;
	dif8 = 0;
	dif9 = 0;
	dif10 = 0;
	dif11 = 0;
	dif12 = 0;
	dif13 = 0;
}


void ProcessIO(void) {   
	unsigned int arr[3];
 	unsigned int sorted[3];
	int a, i;

    if((usb_device_state < CONFIGURED_STATE)||(UCONbits.SUSPND==1)) {
		return;
	}
    
	ReportLoopback();

    if (PIR1bits.TMR2IF) {
    	if (--count == 0) {
			T2CONbits.TMR2ON = 0; 	//turn off the timer

			//Block 1
			//Channel 1, 2, 3
			//---------------------------------------------------
				arr[0] = pulse1;
				arr[1] = pulse2;
				arr[2] = pulse3;
				sorted[0] = 0;
				sorted[1] = 0;
				sorted[2] = 0;
				
				//find the largest value in the array
				for(a = 0; a < 3; a++){
					int removeIndex;
					for(i = 0; i < 3; i++){
						if(arr[i] > sorted[a]){
							sorted[a] = arr[i];
							removeIndex = i;
						}
					}
					arr[removeIndex] = 0;
				}
			
				if(sorted[0] == pulse1) { 		//pulse 1 first large
					if(sorted[1] == pulse2) {	//pulse 2 second med
													//pulse 3 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse1 - pulse2;
						dif2 = pulse2 - pulse3;
			
						LATDbits.LATD2 = 1; 
						LATDbits.LATD3 = 1; 
						LATCbits.LATC6 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse3); 
						LATCbits.LATC6 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATDbits.LATD3 = 0; 
						if(dif1 != 0) {Delay100TCYx(dif1);}
						LATDbits.LATD2 = 0; 
					}
												//pulse 1 first large
					else {						//pulse 3 second med
												//pulse 2 third small
			
						//get the differences between all 3 channels
						dif1 = pulse1 - pulse3;
						dif2 = pulse3 - pulse2;
			
						LATDbits.LATD2 = 1; 
						LATDbits.LATD3 = 1; 
						LATCbits.LATC6 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse2); 
						LATDbits.LATD3 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATCbits.LATC6 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}	
						LATDbits.LATD2 = 0; 
					}
					
				}
				else if(sorted[0] == pulse2){ 	//pulse 2 first large
					if(sorted[1] == pulse1) {	//pulse 1 second med
												//pulse 3 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse2 - pulse1;
						dif2 = pulse1 - pulse3;
			
						LATDbits.LATD2 = 1; 
						LATDbits.LATD3 = 1; 
						LATCbits.LATC6 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse3); 
						LATCbits.LATC6 = 0;
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATDbits.LATD2 = 0; 
						if(dif1 != 0) {Delay100TCYx(dif1);}	
						LATDbits.LATD3 = 0; 
					}
												//pulse 2 first large
					else {						//pulse 3 second med
												//pulse 1 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse2 - pulse3;
						dif2 = pulse3 - pulse1;
			
						LATDbits.LATD2 = 1; 
						LATDbits.LATD3 = 1; 
						LATCbits.LATC6 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse1); 
						LATDbits.LATD2 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATCbits.LATC6 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}	
						LATDbits.LATD3 = 0; 
					}		
				}
				else {							//pulse 3 first large
					if(sorted[1] == pulse1) {	//pulse 1 second med
												//pulse 2 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse3 - pulse1;
						dif2 = pulse1 - pulse2;
			
						LATDbits.LATD2 = 1; 
						LATDbits.LATD3 = 1; 
						LATCbits.LATC6 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse2); 
						LATDbits.LATD3 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}	
						LATDbits.LATD2 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}
						LATCbits.LATC6 = 0; 
					}
												//pulse 3 first large
					else {						//pulse 2 second med
												//pulse 1 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse3 - pulse2;
						dif2 = pulse2 - pulse1;
						
						LATDbits.LATD2 = 1; 
						LATDbits.LATD3 = 1; 
						LATCbits.LATC6 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse1); 
						LATDbits.LATD2 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATDbits.LATD3 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}
						LATCbits.LATC6 = 0; 
					}		
				}
			//---------------------------------------------------
			
			
			//Block 2
			//Channel 4, 5, 6
			//---------------------------------------------------
				arr[0] = pulse4;
				arr[1] = pulse5;
				arr[2] = pulse6;
				sorted[0] = 0;
				sorted[1] = 0;
				sorted[2] = 0;
				
				//find the largest value in the array
				for(a = 0; a < 3; a++){
					int removeIndex;
					for(i = 0; i < 3; i++){
						if(arr[i] > sorted[a]){
							sorted[a] = arr[i];
							removeIndex = i;
						}
					}
					arr[removeIndex] = 0;
				}
			
				if(sorted[0] == pulse4) { 		//pulse 4 first large
					if(sorted[1] == pulse5) {	//pulse 5 second med
												//pulse 6 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse4 - pulse5;
						dif2 = pulse5 - pulse6;
			
//						LATCbits.LATC7 = 1; 
						LATDbits.LATD4 = 1; 
						LATDbits.LATD6 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse6); 
						LATDbits.LATD6 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATDbits.LATD4 = 0; 
						if(dif1 != 0) {Delay100TCYx(dif1);}
//						LATCbits.LATC7 = 0; 
					}
												//pulse 4 first large
					else {						//pulse 6 second med
												//pulse 5 third small
			
						//get the differences between all 3 channels
						dif1 = pulse4 - pulse6;
						dif2 = pulse6 - pulse5;
			
//						LATCbits.LATC7 = 1; 
						LATDbits.LATD4 = 1; 
						LATDbits.LATD6 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse5); 
						LATDbits.LATD4 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATDbits.LATD6 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}	
//						LATCbits.LATC7 = 0; 
					}
					
				}
				else if(sorted[0] == pulse5){ 	//pulse 5 first large
					if(sorted[1] == pulse4) {	//pulse 4 second med
												//pulse 6 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse5 - pulse4;
						dif2 = pulse4 - pulse6;
			
//						LATCbits.LATC7 = 1; 
						LATDbits.LATD4 = 1; 
						LATDbits.LATD6 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse6); 
						LATDbits.LATD6 = 0;
						if(dif2 != 0) {Delay100TCYx(dif2);}
//						LATCbits.LATC7 = 0; 
						if(dif1 != 0) {Delay100TCYx(dif1);}	
						LATDbits.LATD4 = 0; 
					}
												//pulse 5 first large
					else {						//pulse 6 second med
												//pulse 4 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse5 - pulse6;
						dif2 = pulse6 - pulse4;
			
//						LATCbits.LATC7 = 1; 
						LATDbits.LATD4 = 1; 
						LATDbits.LATD6 = 1;  
						Delay100TCYx(30);
						Delay100TCYx(pulse4); 
//						LATCbits.LATC7 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATDbits.LATD6 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}	
						LATDbits.LATD4 = 0; 
					}		
				}
				else {							//pulse 6 first large
					if(sorted[1] == pulse4) {	//pulse 4 second med
												//pulse 5 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse6 - pulse4;
						dif2 = pulse4 - pulse5;
			
//						LATCbits.LATC7 = 1; 
						LATDbits.LATD4 = 1; 
						LATDbits.LATD6 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse5); 
						LATDbits.LATD4 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}	
						//LATCbits.LATC7 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}
						LATDbits.LATD6 = 0; 
					}
												//pulse 6 first large
					else {						//pulse 5 second med
												//pulse 4 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse6 - pulse5;
						dif2 = pulse5 - pulse4;
						
						//LATCbits.LATC7 = 1; 
						LATDbits.LATD4 = 1; 
						LATDbits.LATD6 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse4); 
						//LATCbits.LATC7 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATDbits.LATD4 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}
						LATDbits.LATD6 = 0; 
					}		
				}
			//---------------------------------------------------
			
			
			//Block 3
			//Channel 7, 8, 9
			//---------------------------------------------------
				arr[0] = pulse7;
				arr[1] = pulse8;
				arr[2] = pulse9;
				sorted[0] = 0;
				sorted[1] = 0;
				sorted[2] = 0;
				
				//find the largest value in the array
				for(a = 0; a < 3; a++){
					int removeIndex;
					for(i = 0; i < 3; i++){
						if(arr[i] > sorted[a]){
							sorted[a] = arr[i];
							removeIndex = i;
						}
					}
					arr[removeIndex] = 0;
				}
			
				if(sorted[0] == pulse7) { 		//pulse 7 first large
					if(sorted[1] == pulse8) {	//pulse 8 second med
												//pulse 9 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse7 - pulse8;
						dif2 = pulse8 - pulse9;
			
						LATDbits.LATD7 = 1; 
						LATBbits.LATB0 = 1; 
						LATBbits.LATB1 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse9); 
						LATBbits.LATB1 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATBbits.LATB0 = 0; 
						if(dif1 != 0) {Delay100TCYx(dif1);}
						LATDbits.LATD7 = 0; 
					}
												//pulse 7 first large
					else {						//pulse 9 second med
												//pulse 8 third small
			
						//get the differences between all 3 channels
						dif1 = pulse7 - pulse9;
						dif2 = pulse9 - pulse8;
			
						LATDbits.LATD7 = 1; 
						LATBbits.LATB0 = 1; 
						LATBbits.LATB1 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse8); 
						LATBbits.LATB0 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATBbits.LATB1 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}	
						LATDbits.LATD7 = 0; 
					}
					
				}
				else if(sorted[0] == pulse8){ 	//pulse 8 first large
					if(sorted[1] == pulse7) {	//pulse 7 second med
												//pulse 9 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse8 - pulse7;
						dif2 = pulse7 - pulse9;
			
						LATDbits.LATD7 = 1; 
						LATBbits.LATB0 = 1;
						LATBbits.LATB1 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse9);
						LATBbits.LATB1 = 0;
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATDbits.LATD7 = 0; 
						if(dif1 != 0) {Delay100TCYx(dif1);}	
						LATBbits.LATB0 = 0; 
					}
												//pulse 8 first large
					else {						//pulse 9 second med
												//pulse 7 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse8 - pulse9;
						dif2 = pulse9 - pulse7;
			
						LATDbits.LATD7 = 1; 
						LATBbits.LATB0 = 1; 
						LATBbits.LATB1 = 1;  
						Delay100TCYx(30);
						Delay100TCYx(pulse7); 
						LATDbits.LATD7 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATBbits.LATB1 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}	
						LATBbits.LATB0 = 0; 
					}		
				}
				else {							//pulse 9 first large
					if(sorted[1] == pulse7) {	//pulse 7 second med
												//pulse 8 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse9 - pulse7;
						dif2 = pulse7 - pulse8;
			
						LATDbits.LATD7 = 1; 
						LATBbits.LATB0 = 1; 
						LATBbits.LATB1 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse8); 
						LATBbits.LATB0 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}	
						LATDbits.LATD7 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}
						LATBbits.LATB1 = 0; 
					}
												//pulse 9 first large
					else {						//pulse 8 second med
												//pulse 7 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse9 - pulse8;
						dif2 = pulse8 - pulse7;
						
						LATDbits.LATD7 = 1; 
						LATBbits.LATB0 = 1; 
						LATBbits.LATB1 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse7); 
						LATDbits.LATD7 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATBbits.LATB0 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}
						LATBbits.LATB1 = 0; 
					}		
				}
			//---------------------------------------------------
			
			
			//Block 4
			//Channel 10, 11, 12
			//---------------------------------------------------
				arr[0] = pulse10;
				arr[1] = pulse11;
				arr[2] = pulse12;
				sorted[0] = 0;
				sorted[1] = 0;
				sorted[2] = 0;
				
				//find the largest value in the array
				for(a = 0; a < 3; a++){
					int removeIndex;
					for(i = 0; i < 3; i++){
						if(arr[i] > sorted[a]){
							sorted[a] = arr[i];
							removeIndex = i;
						}
					}
					arr[removeIndex] = 0;
				}
			
				if(sorted[0] == pulse10) { 		//pulse 10 first large
					if(sorted[1] == pulse11) {	//pulse 11 second med
												//pulse 12 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse10 - pulse11;
						dif2 = pulse11 - pulse12;
			
						LATBbits.LATB2 = 1; 
						LATAbits.LATA4 = 1; 
						LATAbits.LATA3 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse12); 
						LATAbits.LATA3 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATAbits.LATA4 = 0; 
						if(dif1 != 0) {Delay100TCYx(dif1);}
						LATBbits.LATB2 = 0; 
					}
												//pulse 10 first large
					else {						//pulse 12 second med
												//pulse 11 third small
			
						//get the differences between all 3 channels
						dif1 = pulse10 - pulse12;
						dif2 = pulse12 - pulse11;
			
						LATBbits.LATB2 = 1; 
						LATAbits.LATA4 = 1; 
						LATAbits.LATA3 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse11); 
						LATAbits.LATA4 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATAbits.LATA3 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}	
						LATBbits.LATB2 = 0; 
					}
					
				}
				else if(sorted[0] == pulse11){ 	//pulse 11 first large
					if(sorted[1] == pulse10) {	//pulse 10 second med
												//pulse 12 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse11 - pulse10;
						dif2 = pulse10 - pulse12;
			
						LATBbits.LATB2 = 1; 
						LATAbits.LATA4 = 1; 
						LATAbits.LATA3 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse12);
						LATAbits.LATA3 = 0;
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATBbits.LATB2 = 0; 
						if(dif1 != 0) {Delay100TCYx(dif1);}	
						LATAbits.LATA4 = 0; 
					}
												//pulse 11 first large
					else {						//pulse 12 second med
												//pulse 10 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse11 - pulse12;
						dif2 = pulse12 - pulse10;
			
						LATBbits.LATB2 = 1; 
						LATAbits.LATA4 = 1; 
						LATAbits.LATA3 = 1;  
						Delay100TCYx(30);
						Delay100TCYx(pulse10); 
						LATBbits.LATB2 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATAbits.LATA3 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}	
						LATAbits.LATA4 = 0; 
					}		
				}
				else {							//pulse 12 first large
					if(sorted[1] == pulse10) {	//pulse 10 second med
												//pulse 11 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse12 - pulse10;
						dif2 = pulse10 - pulse11;
			
						LATBbits.LATB2 = 1; 
						LATAbits.LATA4 = 1; 
						LATAbits.LATA3 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse11); 
						LATAbits.LATA4 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}	
						LATBbits.LATB2 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}
						LATAbits.LATA3 = 0; 
					}
												//pulse 12 first large
					else {						//pulse 11 second med
												//pulse 10 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse12 - pulse11;
						dif2 = pulse11 - pulse10;
			
						LATBbits.LATB2 = 1; 
						LATAbits.LATA4 = 1; 
						LATAbits.LATA3 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse10); 
						LATBbits.LATB2 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATAbits.LATA4 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}
						LATAbits.LATA3 = 0; 
					}		
				}
			//---------------------------------------------------
			
			
			//Block 5
			//Channel 13, 14, 15
			//---------------------------------------------------
				arr[0] = pulse13;
				arr[1] = pulse14;
				arr[2] = pulse15;
				sorted[0] = 0;
				sorted[1] = 0;
				sorted[2] = 0;
				
				//find the largest value in the array
				for(a = 0; a < 3; a++){
					int removeIndex;
					for(i = 0; i < 3; i++){
						if(arr[i] > sorted[a]){
							sorted[a] = arr[i];
							removeIndex = i;
						}
					}
					arr[removeIndex] = 0;
				}
			
				if(sorted[0] == pulse13) { 		//pulse 13 first large
					if(sorted[1] == pulse14) {	//pulse 14 second med
												//pulse 15 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse13 - pulse14;
						dif2 = pulse14 - pulse15;
			
						LATAbits.LATA2 = 1; 
						LATAbits.LATA1 = 1; 
						LATAbits.LATA0 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse15); 
						LATAbits.LATA0 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATAbits.LATA1 = 0; 
						if(dif1 != 0) {Delay100TCYx(dif1);}
						LATAbits.LATA2 = 0; 
					}
												//pulse 13 first large
					else {						//pulse 15 second med
												//pulse 14 third small
			
						//get the differences between all 3 channels
						dif1 = pulse13 - pulse15;
						dif2 = pulse15 - pulse14;
			
						LATAbits.LATA2 = 1; 
						LATAbits.LATA1 = 1; 
						LATAbits.LATA0 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse14); 
						LATAbits.LATA1 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATAbits.LATA0 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}	
						LATAbits.LATA2 = 0; 
					}
					
				}
				else if(sorted[0] == pulse14){ 	//pulse 14 first large
					if(sorted[1] == pulse13) {	//pulse 13 second med
												//pulse 15 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse14 - pulse13;
						dif2 = pulse13 - pulse15;
			
						LATAbits.LATA2 = 1; 
						LATAbits.LATA1 = 1; 
						LATAbits.LATA0 = 1;  
						Delay100TCYx(30);
						Delay100TCYx(pulse15);
						LATAbits.LATA0 = 0;
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATAbits.LATA2 = 0; 
						if(dif1 != 0) {Delay100TCYx(dif1);}	
						LATAbits.LATA1 = 0; 
					}
												//pulse 14 first large
					else {						//pulse 15 second med
												//pulse 13 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse14 - pulse15;
						dif2 = pulse15 - pulse13;
			
						LATAbits.LATA2 = 1; 
						LATAbits.LATA1 = 1; 
						LATAbits.LATA0 = 1;  
						Delay100TCYx(30);
						Delay100TCYx(pulse13); 
						LATAbits.LATA2 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATAbits.LATA0 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}	
						LATAbits.LATA1 = 0; 
					}		
				}
				else {							//pulse 15 first large
					if(sorted[1] == pulse13) {	//pulse 13 second med
												//pulse 14 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse15 - pulse13;
						dif2 = pulse13 - pulse14;
			
						LATAbits.LATA2 = 1; 
						LATAbits.LATA1 = 1; 
						LATAbits.LATA0 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse14); 
						LATAbits.LATA1 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}	
						LATAbits.LATA2 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}
						LATAbits.LATA0 = 0; 
					}
												//pulse 15 first large
					else {						//pulse 14 second med
												//pulse 13 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse15 - pulse14;
						dif2 = pulse14 - pulse13;
			
						LATAbits.LATA2 = 1; 
						LATAbits.LATA1 = 1; 
						LATAbits.LATA0 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse13); 
						LATAbits.LATA2 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATAbits.LATA1 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}
						LATAbits.LATA0 = 0; 
					}		
				}
			//---------------------------------------------------
			
			
			//Block 6
			//Channel 16, 17, 18
			//---------------------------------------------------
				arr[0] = pulse16;
				arr[1] = pulse17;
				arr[2] = pulse18;
				sorted[0] = 0;
				sorted[1] = 0;
				sorted[2] = 0;
				
				//find the largest value in the array
				for(a = 0; a < 3; a++){
					int removeIndex;
					for(i = 0; i < 3; i++){
						if(arr[i] > sorted[a]){
							sorted[a] = arr[i];
							removeIndex = i;
						}
					}
					arr[removeIndex] = 0;
				}
			
				if(sorted[0] == pulse16) { 		//pulse 16 first large
					if(sorted[1] == pulse17) {	//pulse 17 second med
												//pulse 18 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse16 - pulse17;
						dif2 = pulse17 - pulse18;
			
						LATBbits.LATB4 = 1; 
						LATBbits.LATB3 = 1; 
						LATAbits.LATA5 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse18); 
						LATAbits.LATA5 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATBbits.LATB3 = 0; 
						if(dif1 != 0) {Delay100TCYx(dif1);}
						LATBbits.LATB4 = 0; 
					}
												//pulse 16 first large
					else {						//pulse 18 second med
												//pulse 17 third small
			
						//get the differences between all 3 channels
						dif1 = pulse16 - pulse18;
						dif2 = pulse18 - pulse17;
			
						LATBbits.LATB4 = 1; 
						LATBbits.LATB3 = 1; 
						LATAbits.LATA5 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse17); 
						LATBbits.LATB3 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATAbits.LATA5 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}	
						LATBbits.LATB4 = 0; 
					}
					
				}
				else if(sorted[0] == pulse17){ 	//pulse 17 first large
					if(sorted[1] == pulse16) {	//pulse 16 second med
												//pulse 18 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse17 - pulse16;
						dif2 = pulse16 - pulse18;
			
						LATBbits.LATB4 = 1; 
						LATBbits.LATB3 = 1; 
						LATAbits.LATA5 = 1;  
						Delay100TCYx(30);
						Delay100TCYx(pulse18);
						LATAbits.LATA5 = 0;
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATBbits.LATB4 = 0; 
						if(dif1 != 0) {Delay100TCYx(dif1);}	
						LATBbits.LATB3 = 0; 
					}
												//pulse 17 first large
					else {						//pulse 18 second med
												//pulse 16 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse17 - pulse18;
						dif2 = pulse18 - pulse16;
			
						LATBbits.LATB4 = 1; 
						LATBbits.LATB3 = 1; 
						LATAbits.LATA5 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse16); 
						LATBbits.LATB4 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATAbits.LATA5 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}	
						LATBbits.LATB3 = 0; 
					}		
				}
				else {							//pulse 18 first large
					if(sorted[1] == pulse16) {	//pulse 16 second med
												//pulse 17 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse18 - pulse16;
						dif2 = pulse16 - pulse17;
			
						LATBbits.LATB4 = 1; 
						LATBbits.LATB3 = 1; 
						LATAbits.LATA5 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse17); 
						LATBbits.LATB3 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}	
						LATBbits.LATB4 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}
						LATAbits.LATA5 = 0; 
					}
												//pulse 18 first large
					else {						//pulse 17 second med
												//pulse 16 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse18 - pulse17;
						dif2 = pulse17 - pulse16;
			
						LATBbits.LATB4 = 1; 
						LATBbits.LATB3 = 1; 
						LATAbits.LATA5 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse16); 
						LATBbits.LATB4 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATBbits.LATB3 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}
						LATAbits.LATA5 = 0; 
					}		
				}
			//---------------------------------------------------
			
			
			//Block 7
			//Channel 19, 20, 21
			//---------------------------------------------------
				arr[0] = pulse19;
				arr[1] = pulse20;
				arr[2] = pulse21;
				sorted[0] = 0;
				sorted[1] = 0;
				sorted[2] = 0;
				
				//find the largest value in the array
				for(a = 0; a < 3; a++){
					int removeIndex;
					for(i = 0; i < 3; i++){
						if(arr[i] > sorted[a]){
							sorted[a] = arr[i];
							removeIndex = i;
						}
					}
					arr[removeIndex] = 0;
				}
			
				if(sorted[0] == pulse19) { 		//pulse 19 first large
					if(sorted[1] == pulse20) {	//pulse 20 second med
												//pulse 21 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse19 - pulse20;
						dif2 = pulse20 - pulse21;
			
						LATEbits.LATE0 = 1; 
						LATEbits.LATE1 = 1; 
						LATCbits.LATC0 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse21); 
						LATCbits.LATC0 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATEbits.LATE1 = 0; 
						if(dif1 != 0) {Delay100TCYx(dif1);}
						LATEbits.LATE0 = 0; 
					}
												//pulse 19 first large
					else {						//pulse 21 second med
												//pulse 20 third small
			
						//get the differences between all 3 channels
						dif1 = pulse19 - pulse21;
						dif2 = pulse21 - pulse20;
			
						LATEbits.LATE0 = 1; 
						LATEbits.LATE1 = 1; 
						LATCbits.LATC0 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse20); 
						LATEbits.LATE1 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATCbits.LATC0 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}	
						LATEbits.LATE0 = 0; 
					}
					
				}
				else if(sorted[0] == pulse20){ 	//pulse 20 first large
					if(sorted[1] == pulse19) {	//pulse 19 second med
												//pulse 21 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse20 - pulse19;
						dif2 = pulse19 - pulse21;
			
						LATEbits.LATE0 = 1; 
						LATEbits.LATE1 = 1; 
						LATCbits.LATC0 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse21);
						LATCbits.LATC0 = 0;
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATEbits.LATE0 = 0; 
						if(dif1 != 0) {Delay100TCYx(dif1);}	
						LATEbits.LATE1 = 0; 
					}
												//pulse 20 first large
					else {						//pulse 21 second med
												//pulse 19 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse20 - pulse21;
						dif2 = pulse21 - pulse19;
			
						LATEbits.LATE0 = 1; 
						LATEbits.LATE1 = 1; 
						LATCbits.LATC0 = 1;  
						Delay100TCYx(30);
						Delay100TCYx(pulse19); 
						LATEbits.LATE0 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATCbits.LATC0 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}	
						LATEbits.LATE1 = 0; 
					}		
				}
				else {							//pulse 21 first large
					if(sorted[1] == pulse19) {	//pulse 19 second med
												//pulse 20 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse21 - pulse19;
						dif2 = pulse19 - pulse20;
			
						LATEbits.LATE0 = 1; 
						LATEbits.LATE1 = 1; 
						LATCbits.LATC0 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse20); 
						LATEbits.LATE1 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}	
						LATEbits.LATE0 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}
						LATCbits.LATC0 = 0; 
					}
												//pulse 21 first large
					else {						//pulse 20 second med
												//pulse 19 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse21 - pulse20;
						dif2 = pulse20 - pulse19;
			
						LATEbits.LATE0 = 1; 
						LATEbits.LATE1 = 1; 
						LATCbits.LATC0 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse19); 
						LATEbits.LATE0 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATEbits.LATE1 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}
						LATCbits.LATC0 = 0; 
					}		
				}
			//---------------------------------------------------
			
			
			//Block 8
			//Channel 22, 23, 24
			//---------------------------------------------------
				arr[0] = pulse22;
				arr[1] = pulse23;
				arr[2] = pulse24;
				sorted[0] = 0;
				sorted[1] = 0;
				sorted[2] = 0;
				
				//find the largest value in the array
				for(a = 0; a < 3; a++){
					int removeIndex;
					for(i = 0; i < 3; i++){
						if(arr[i] > sorted[a]){
							sorted[a] = arr[i];
							removeIndex = i;
						}
					}
					arr[removeIndex] = 0;
				}
			
				if(sorted[0] == pulse22) { 		//pulse 22 first large
					if(sorted[1] == pulse23) {	//pulse 23 second med
												//pulse 24 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse22 - pulse23;
						dif2 = pulse23 - pulse24;
			
						LATCbits.LATC1 = 1; 
						LATCbits.LATC2 = 1; 
						LATDbits.LATD0 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse24); 
						LATDbits.LATD0 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATCbits.LATC2 = 0; 
						if(dif1 != 0) {Delay100TCYx(dif1);}
						LATCbits.LATC1 = 0; 
					}
												//pulse 22 first large
					else {						//pulse 24 second med
												//pulse 23 third small
			
						//get the differences between all 3 channels
						dif1 = pulse22 - pulse24;
						dif2 = pulse24 - pulse23;
			
						LATCbits.LATC1 = 1; 
						LATCbits.LATC2 = 1; 
						LATDbits.LATD0 = 1;
						Delay100TCYx(30);
						Delay100TCYx(pulse23); 
						LATCbits.LATC2 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATDbits.LATD0 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}	
						LATCbits.LATC1 = 0; 
					}
					
				}
				else if(sorted[0] == pulse23){ 	//pulse 23 first large
					if(sorted[1] == pulse22) {	//pulse 22 second med
												//pulse 24 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse23 - pulse22;
						dif2 = pulse22 - pulse24;
			
						LATCbits.LATC1 = 1; 
						LATCbits.LATC2 = 1; 
						LATDbits.LATD0 = 1;
						Delay100TCYx(30);
						Delay100TCYx(pulse24);
						LATDbits.LATD0 = 0;
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATCbits.LATC1 = 0; 
						if(dif1 != 0) {Delay100TCYx(dif1);}	
						LATCbits.LATC2 = 0; 
					}
												//pulse 23 first large
					else {						//pulse 24 second med
												//pulse 22 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse23 - pulse24;
						dif2 = pulse24 - pulse22;
			
						LATCbits.LATC1 = 1; 
						LATCbits.LATC2 = 1; 
						LATDbits.LATD0 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse22); 
						LATCbits.LATC1 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATDbits.LATD0 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}	
						LATCbits.LATC2 = 0; 
					}		
				}
				else {							//pulse 24 first large
					if(sorted[1] == pulse22) {	//pulse 22 second med
												//pulse 23 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse24 - pulse22;
						dif2 = pulse22 - pulse23;
			
						LATCbits.LATC1 = 1; 
						LATCbits.LATC2 = 1; 
						LATDbits.LATD0 = 1;
						Delay100TCYx(30);
						Delay100TCYx(pulse23); 
						LATCbits.LATC2 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}	
						LATCbits.LATC1 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}
						LATDbits.LATD0 = 0; 
					}
												//pulse 24 first large
					else {						//pulse 23 second med
												//pulse 22 thrid small
			
						//get the differences between all 3 channels
						dif1 = pulse24 - pulse23;
						dif2 = pulse23 - pulse22;
			
						LATCbits.LATC1 = 1; 
						LATCbits.LATC2 = 1; 
						LATDbits.LATD0 = 1;
						Delay100TCYx(30);
						Delay100TCYx(pulse22); 
						LATCbits.LATC1 = 0; 
						if(dif2 != 0) {Delay100TCYx(dif2);}
						LATCbits.LATC2 = 0;
						if(dif1 != 0) {Delay100TCYx(dif1);}
						LATDbits.LATD0 = 0; 
					}		
				}
			//---------------------------------------------------
			
			
			//Block 9
			//Channel 25
			//---------------------------------------------------
						LATDbits.LATD1 = 1; 
						Delay100TCYx(30);
						Delay100TCYx(pulse25); 
						LATDbits.LATD1 = 0; 
			//---------------------------------------------------

			count = 2;
			T2CONbits.TMR2ON = 1; 	//turn the timer on
		}
		PIR1bits.TMR2IF = 0; 	
    } 
}


void ReportLoopback(void) {
	byte count;

	// Find out if an Output report has been received from the host.
	number_of_bytes_read = HIDRxReport(receive_buffer, HID_OUTPUT_REPORT_BYTES);

	if (number_of_bytes_read > 0) {
		// An Output report was received.
		pulse1 = receive_buffer[0]; //servo1
		pulse2 = receive_buffer[1]; //servo2
		pulse3 = receive_buffer[2]; //servo3
		pulse4 = receive_buffer[3]; //servo4
		pulse5 = receive_buffer[4]; //servo5
		pulse6 = receive_buffer[5]; //servo6
		pulse7 = receive_buffer[6]; //servo7
		pulse8 = receive_buffer[7]; //servo8
		pulse9 = receive_buffer[8]; //servo9
		pulse10 = receive_buffer[9]; //servo10
		pulse11 = receive_buffer[10]; //servo11
		pulse12 = receive_buffer[11]; //servo12
		pulse13 = receive_buffer[12]; //servo13
		pulse14 = receive_buffer[13]; //servo14
		pulse15 = receive_buffer[14]; //servo15
		pulse16 = receive_buffer[15]; //servo16
		pulse17 = receive_buffer[16]; //servo17
		pulse18 = receive_buffer[17]; //servo18
		pulse19 = receive_buffer[18]; //servo19
		pulse20 = receive_buffer[19]; //servo20
		pulse21 = receive_buffer[20]; //servo21
		pulse22 = receive_buffer[21]; //servo22
		pulse23 = receive_buffer[22]; //servo23
		pulse24 = receive_buffer[23]; //servo24
		pulse25 = receive_buffer[24]; //servo25

		for (count = 1; count <= HID_OUTPUT_REPORT_BYTES; count = count + 1) {				
//			transmit_buffer[count-1] = receive_buffer[count-1];			
			transmit_buffer[count-1] = 0;			
		}

		// If necessary, wait until the interrupt IN endpoint isn't busy.
		while(mHIDTxIsBusy()) {
			// Service USB interrupts.
			USBDriverService(); 
		}
		// The report will be sent in the next interrupt IN transfer.
   	    HIDTxReport(transmit_buffer, HID_INPUT_REPORT_BYTES);
	}
}


void HandleControlOutReport()
{
	byte count;

	// Find out if an Output or Feature report has arrived on the control pipe.
	// Get the report type from the Setup packet.

	switch (MSB(SetupPkt.W_Value))
    {
		case 0x02: // Output report 

			// Get the report ID from the Setup packet.

    		switch(LSB(SetupPkt.W_Value))
		    {
				case 0: // Report ID 0

					// This example application copies the Output report data 
					// to hid_report_in. 
					// (Assumes Input and Output reports are the same length.)
					// A "real" application would do something more useful with the data.

				    // wCount holds the number of bytes read in the Data stage.
					// This example assumes the report fits in one transaction.	
				
					for (count = 1; count <= HID_OUTPUT_REPORT_BYTES; count = count + 1)
					{
						hid_report_in[count-1] = hid_report_out[count-1];
					}				
					// The number of bytes in the report (from usbcfg.h).
		
					break;		
			} // end switch(LSB(SetupPkt.W_Value))

		case 0x03: // Feature report 

			// Get the report ID from the Setup packet.

    		switch(LSB(SetupPkt.W_Value))
		    {
				case 0: // Report ID 0

				// The Feature report data is in hid_report_feature.
				// This example application just sends the data back in the next
				// Get_Report request for a Feature report.			
			
			    // wCount holds the number of bytes read in the Data stage.
				// This example assumes the report fits in one transaction.	
		
				break;
			} // end switch(LSB(SetupPkt.W_Value))		

	} // end switch(MSB(SetupPkt.W_Value))

} // end HandleControlOutReport


void BlinkUSBStatus(void)
{
    static word led_count=0;
    
    if(led_count == 0)led_count = 10000U;
    led_count--;

    #define mLED_Both_Off()         {mLED_1_Off();mLED_2_Off();}
    #define mLED_Both_On()          {mLED_1_On();mLED_2_On();}
    #define mLED_Only_1_On()        {mLED_1_On();mLED_2_Off();}
    #define mLED_Only_2_On()        {mLED_1_Off();mLED_2_On();}

    if(UCONbits.SUSPND == 1)
    {
        if(led_count==0)
        {
            mLED_1_Toggle();
            mLED_2 = mLED_1;        // Both blink at the same time
        }//end if
    }
    else
    {
        if(usb_device_state == DETACHED_STATE)
        {
            mLED_Both_Off();
        }
        else if(usb_device_state == ATTACHED_STATE)
        {
            mLED_Both_On();
        }
        else if(usb_device_state == POWERED_STATE)
        {
            mLED_Only_1_On();
        }
        else if(usb_device_state == DEFAULT_STATE)
        {
            mLED_Only_2_On();
        }
        else if(usb_device_state == ADDRESS_STATE)
        {
            if(led_count == 0)
            {
                mLED_1_Toggle();
                mLED_2_Off();
            }//end if
        }
        else if(usb_device_state == CONFIGURED_STATE)
        {
            if(led_count==0)
            {
                mLED_1_Toggle();
                mLED_2 = !mLED_1;       // Alternate blink                
            }//end if
        }//end if(...)
    }//end if(UCONbits.SUSPND...)

}//end BlinkUSBStatus


void GetInputReport0(void)
{
	byte count;

	// Set pSrc.bRam to point to the report.
	// 

	pSrc.bRam = (byte*)&hid_report_in;

	// The number of bytes in the report (from usbcfg.h).

	LSB(wCount) = HID_INPUT_REPORT_BYTES;

} // end GetInputReport0


void GetFeatureReport0(void)
{
	byte count;

	// Set pSrc.bRam to point to the report.

	pSrc.bRam = (byte*)&hid_report_feature;

	// The number of bytes in the report (from usbcfg.h).

    LSB(wCount) = HID_FEATURE_REPORT_BYTES;

} // end GetFeatureReport0


//47TCY
void step(int input){
	int i;
	for(i = 0; i < input; i++) {
		Delay10TCYx(4);
		Delay1TCY();
		Delay1TCY();
		Delay1TCY();
		Delay1TCY();
		Delay1TCY();
		Delay1TCY();
		Delay1TCY();
	}
}

/** EOF user_generic_hid.c *********************************************************/
