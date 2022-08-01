#include "triphase_moto.h"
phase_switcher::phase_switcher(void):state_number(0)//��ʼ������λ״̬��Ϊ״̬0
{
  /*
  �á��˿ڶ��塱�͡���λ˳����ϳɡ�������˿�-��λ״̬�������ͻ��ิ�Ӷ�
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
  if(state_number%2)    //����λ״̬��Ϊ����
  {
    P2SEL &= allup;     //����������λ�϶�
    P2SEL |= dw_phase_table[state_number];      //��һ�ض���λ�϶�
  }else{                //����λ״̬��Ϊż��
    P2OUT &= alldw;
    P2OUT |= up_phase_table[state_number];
  }
#else
  if(state_number%2)    //����λ״̬��Ϊ����
  {
    P2OUT &= allup;     //����������λ�϶�
    P2OUT |= dw_phase_table[state_number];      //��һ�ض���λ�϶�
  }else{                //����λ״̬��Ϊż��
    P2SEL &= alldw;     //����������λ�¶�
    P2SEL |= up_phase_table[state_number];      //��һ�ض���λ�¶�
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
  P2SEL &= allup;     //����������λ�϶�
  P2OUT &= alldw;
}
void phase_switcher::enable_SPSI(void)const
{
  if(state_number%2)    //����λ״̬��Ϊ����,�͵�ƽ����
  {
    P1IES=input_phase_table[state_number];
  }else{                //����λ״̬��Ϊż��,�ߵ�ƽ����
    P1IES=0x00;
  }
  P1IE=input_phase_table[state_number];
}
void phase_switcher::disable_SPSI(void)const
{
  P1IE&=~aainput;P1IE&=~bbinput;P1IE&=~ccinput;
}