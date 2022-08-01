#include "triphase_moto.h"
void ini_uart()
{
    /*
    设置通信时先屏蔽通信
    */
    UCA0CTL1 |=UCSWRST;
    /*
    串口io口初始化
    */
    P1DIR |= BIT2;
    P1OUT &= ~BIT2;
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
}
_Bool Uart::send(const char word[],unsigned int number)
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
_Bool Uart::send(char word,unsigned int repeats)
{
  mode=1;
  if(IE2&UCA0TXIE) return 1;
  SW_char=word;
  SW_number=repeats;
  SW_position=1;
  IFG2&=~UCA0TXIFG;
  if(SW_position!=SW_number)
          IE2 |= UCA0TXIE;
  UCA0TXBUF=SW_char;
  return 0;
}
void Uart::send_subfuntion(void)
{
  
  if(!mode)
     UCA0TXBUF = *(SW_pointer+SW_position);
  else
    UCA0TXBUF = SW_char;
  SW_position++;
  if (SW_position == SW_number)					// TX over?
    IE2 &= ~UCA0TXIE;
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
    case 'E':uart.send('E',2);break;
    case 'T':code='T';break;
    case 'F':code='F';break;
    case 'U':code='U';break;
    case 'D':code='D';break;
    case 'G':code='G';break;
  }
}