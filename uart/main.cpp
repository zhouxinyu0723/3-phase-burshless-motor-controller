
#include "io430.h"
#include<iostream>
unsigned int i;

class Uart
{
public:
    Uart():SW_pointer(NULL),SW_int(0),SW_number(0),SW_position(0){}
    _Bool send(const char word[],unsigned int number)
    {
      mode=0;
      if(IE2&UCA0TXIE) return 1;
      SW_pointer=(unsigned char*)&word[0];
      SW_number=number;
      SW_position=1;
      IFG2&=~UCA0TXIFG;
      if(SW_position!=SW_number)
              IE2 |= UCA0TXIE;
      UCA0TXBUF=*SW_pointer;
      return 0;
    }
    _Bool send(char word,unsigned int repeats)
    {
      mode=1;
      if(IE2&UCA0TXIE) return 1;
      SW_int=word;
      SW_number=repeats;
      SW_position=1;
      IFG2&=~UCA0TXIFG;
      if(SW_position!=SW_number)
              IE2 |= UCA0TXIE;
      UCA0TXBUF=SW_int;
      return 0;
    }
    void send_subfuntion(void)
    {
      
      if(!mode)
         UCA0TXBUF = *(SW_pointer+SW_position);
      else
        UCA0TXBUF = SW_int;
      SW_position++;
      if (SW_position == SW_number)					// TX over?
        IE2 &= ~UCA0TXIE;
    }
private:
    unsigned char *SW_pointer;
    unsigned char SW_int;
    unsigned int SW_number;
    unsigned int SW_position;
    unsigned int mode;
};
Uart uart;

int main(void)
{
  
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	if (CALBC1_8MHZ==0xFF)					// If calibration constant erased
	{
	  while(1);                             // do not load, trap CPU!!
	}
	DCOCTL = 0;                             // Select lowest DCOx and MODx settings
	BCSCTL1 = CALBC1_8MHZ;                  // Set range
        
	DCOCTL = CALDCO_8MHZ;
	__bis_SR_register(GIE);
        /*
        设置通信时先屏蔽通信
        */
	UCA0CTL1 |=UCSWRST;
        /*
        串口io口初始化
        */
	P1DIR |= BIT2;
        P1OUT = 0;
	P1DIR &= ~BIT1;
	P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
	P1SEL2 = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
        /*
        设置通信时间状况并解除屏蔽
        */
	UCA0CTL1 |= UCSSEL_2;                     // CLK = MCLK
	UCA0BR0 = 0x41;                           // 1MHz/9600 = 104.166
	UCA0BR1 = 0x03;                           //
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
	IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
        
	//uart.send("destroy",7);
        uart.send(110,1);
	while(1);       // Enter LPM3 w/ int until Byte RXed
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0TX_VECTOR))) USCI0TX_ISR (void)
#else
#error Compiler not supported!
#endif
{
    uart.send_subfuntion();
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCI0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(UCA0RXBUF)
  {
    case 'A':uart.send(109,2);break;
    case 'B':uart.send(110,2);break;
    case 'C':uart.send(111,2);break;
  }
}