#include "triphase_moto.h"
phase_switcher::phase_switcher(void):state_number(0)//初始化”相位状态“为状态0
{
  /*
  用”端口定义”和”相位顺序”组合成”需操作端口-相位状态“表，降低换相复杂度
  */
  for(int i=0;i<6;i++)
  {
     up_phase_table[i]=up_phase_define[up_phase_state_define[i]];
     dw_phase_table[i]=dw_phase_define[dw_phase_state_define[i]];
     input_phase_table[i]=input_phase_define[input_phase_state_define[i]];
  }
}
void phase_switcher::FP(void)
{
  P2SEL |= dw_phase_table[5];
  P2OUT |= up_phase_table[5];
}
void phase_switcher::SP(void)
{
#if pwm_dw
  if(state_number%2)    //”相位状态“为奇数
  {
    P2SEL &= allup;     //置零所有相位上端
    P2SEL |= dw_phase_table[state_number];      //置一特定相位上端
  }else{                //”相位状态“为偶数
    P2OUT &= alldw;
    P2OUT |= up_phase_table[state_number];
  }
#else
  if(state_number%2)    //”相位状态“为奇数
  {
    P2OUT &= allup;     //置零所有相位上端
    P2OUT |= dw_phase_table[state_number];      //置一特定相位上端
  }else{                //”相位状态“为偶数
    P2SEL &= alldw;     //置零所有相位下端
    P2SEL |= up_phase_table[state_number];      //置一特定相位下端
  } 
#endif
  state_number=round_count(state_number,6);
}
void phase_switcher::SP_false(void)
{
  state_number=round_count(state_number,6);
}
void phase_switcher::CP(void)const
{
  P2SEL &= allup;     //置零所有相位上端
  P2OUT &= alldw;
}
void phase_switcher::enable_SPSI(void)const
{
  if(state_number%2)    //”相位状态“为奇数,低电平触发
  {
    P1IES=input_phase_table[state_number];
  }else{                //”相位状态“为偶数,高电平触发
    P1IES=0x00;
  }
  P1IE=input_phase_table[state_number];
}
void phase_switcher::disable_SPSI(void)const
{
  P1IE&=~aainput;P1IE&=~bbinput;P1IE&=~ccinput;
}