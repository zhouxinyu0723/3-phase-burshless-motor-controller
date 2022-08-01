#include "triphase_moto.h"
int round_count(int a,int n)
{
  a++;
  if(a==n)
    a=0;
  return a;
}
int round_count(int a,int b,int n)
{
  return (a+b)%n;
}
void wait_us(unsigned long a)
{
  #if DCO_F==8
  for(int j=0;j<a;j++)
  {
    __delay_cycles(8);
  }
  #endif
  #if DCO_F==16
  for(int j=0;j<a;j++)
  {
    __delay_cycles(16);
  }
  #endif
}
void wait_100us(unsigned long a)
{
  #if DCO_F==8
  for(int j=0;j<a;j++)
  {
    __delay_cycles(800);
  }
  #endif
  #if DCO_F==16
  for(int j=0;j<a;j++)
  {
    __delay_cycles(1600);
  }
  #endif
}