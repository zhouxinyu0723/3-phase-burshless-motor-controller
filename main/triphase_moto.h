#ifndef TRIPHASE_MOTO_H
#define TRIPHASE_MOTO_H
#include "msp430.h"
#include <algorithm>
//--------------------IO�ڶ��壬��������--------------------//
/*
MSP430G2553���Բ���PWM��IO��
*/
#define pwm_io BIT1+BIT2+BIT4+BIT5
/*
����������ƶ˿ڣ�����PWM������λ�϶˻����¶ˡ�
*/
#define pwm_dw 1
#define aaup BIT0
#define aadw BIT1 
#define bbup BIT3
#define bbdw BIT2
#define ccup BIT4
#define ccdw BIT5
#define aainput BIT5
#define bbinput BIT6
#define ccinput BIT7
/*
�Զ˿ڶ�������
*/
#define allup aaup+bbup+ccup
#define alldw aadw+bbdw+ccdw
#define allinput aainput+bbinput+ccinput
/*
�Զ˿ڽ��кϷ��Լ���
*/
#if (allup+alldw)!=0x3F
#error Io definition interrupt
#endif
#if pwm_dw
#if ((aadw|pwm_io)!=pwm_io)||((bbdw|pwm_io)!=pwm_io)||((ccdw|pwm_io)!=pwm_io)
#error Down io should be included in BIT1,BIT2,BIT4 or BIT5
#endif
#else
#if ((aaup|pwm_io)!=pwm_io)||((bbup|pwm_io)!=pwm_io)||((ccup|pwm_io)!=pwm_io)
#error Up io should be included in BIT1,BIT2,BIT4 or BIT5
#endif
#endif
/*
�����˿ڶ��塱�͡���λ˳�򡱴洢�������У�����������
*/
const unsigned char up_phase_define[3]={aaup,bbup,ccup};
const unsigned char dw_phase_define[3]={aadw,bbdw,ccdw};
const unsigned char input_phase_define[3]={aainput,bbinput,ccinput};
const unsigned char up_phase_state_define[6]={1,1,2,2,0,0};
const unsigned char dw_phase_state_define[6]={2,0,0,1,1,2};
const unsigned char input_phase_state_define[6]={1,0,2,1,0,2};
//--------------------оƬʱ�ӣ�PWM��ʼֵ�������ã���������--------------------//
/*
����оƬ��Ƶ�ʺ�PWM����
*/
#define DCO_F 8//MHz            ��8MHz��16MHz��ѡ��
#define PWM_F 15625//Hz         ��15.625kHz��31.250kHz��62.500kHz,125.000kHz��ѡ��
#define PWM_START_RATE 65       //PWM������512ʱ�ĸߵ�ƽʱ��
/*
��Ӧ��PWM����
        15.625  31.250  62.250 125.000
8MHz    512     256     128     ///////
16MHz   /////// 512     256     128
*/
/*
��оƬ��Ƶ�ʺ�PWM������������
*/
#if ((DCO_F==8)&&(PWM_F==15625))
#define PWMT_START 512
#define PWMH_START PWM_START_RATE
#endif
#if ((DCO_F==8)&&(PWM_F==31250))
#define PWMT_START 256
#define PWMH_START PWM_START_RATE/2
#endif
#if ((DCO_F==8)&&(PWM_F==62250))
#define PWMT_START 128
#define PWMH_START PWM_START_RATE/4
#endif
#if ((DCO_F==16)&&(PWM_F==31250))
#define PWMT_START 512
#define PWMH_START PWM_START_RATE
#endif
#if ((DCO_F==16)&&(PWM_F==62250))
#define PWMT_START 256
#define PWMH_START PWM_START_RATE/2
#endif
#if ((DCO_F==16)&&(PWM_F==125000))
#define PWMT_START 128
#define PWMH_START PWM_START_RATE/4
#endif
/*
��оƬʱ�ӣ�PWM ��ʼ�������ý��кϷ��Լ���
*/
#if ((DCO_F!=8)&&(DCO_F!=16))
#error The main clock source DCO 's frequency must be choosen between 8 and 16.
#endif
#if ((PWM_F!=15625)&&(PWM_F!=31250)&&(PWM_F!=62250)&&(PWM_F!=125000))
#error The PWM 's frequency must be choosen between 15625, 31250, 62250 and 125000.
#endif
#if (((DCO_F==8)&&(PWM_F==125000))||((DCO_F==16)&&(PWM_F==15625)))
#error The Cambination of DCO frequency and PWM frequency is not in the table.
#endif
//--------------------�����������ã�����--------------------//
/*
������������
*/
#define SPT_START  200          //XX*100us
#define SHRINK_RATE_1 25          //XX-1/XX�ٶȽ���
#define SPT_MID 70             //XX*100us
#define SHRINK_RATE_2 35          //XX-1/XX�ٶȽ���
#define SPT_END 13             //XX*100us
#define DIS_SPSI_TIMES 6        //1/XX�����ڲ��ܽ�ֹ�����ź��ж�
/*
�����������Ƴ�������Ļ���ʱ�䣨��TA0��ʱ��ԴΪ��λ�ģ�
�����൱��׼ȷ
*/
#define SPT_MIDDLE (TIME_OF_A_SWITCH_START+TIME_OF_A_SWITCH_END)/2
#if DCO_F==8
#define SPT_END_TA0 TIME_OF_A_SWITCH_END*100//TIME_OF_A_SWITCH_END*(100/1000000) * 8*1000000/8
#endif
#if DCO_F==16
#define SPT_END_TA0 TIME_OF_A_SWITCH_END*200//TIME_OF_A_SWITCH_END*(100/1000000) * 16*1000000/8
#endif
/*
��������
������Ȧ����
*/
int round_count(int a,int n);
int round_count(int a,int b,int n);
void wait_us(unsigned long a);
void wait_100us(unsigned long a);
/*
оƬ����������ʼ������
�Լ�      ��ʼ���ܺ���
*/
void initial(void);
void ini_crystal(void);
void ini_pwm(void);
void ini_input_io(void);
void ini_uart();
/*
��ʱ��������ʼ���Ͳ��ٺ���
*/
void ini_timer(void);
/*
������
*/
class phase_switcher{
public:
    phase_switcher(void);
    void FP(void);
    void SP(void);
    void SP_false(void);
    void CP(void)const;
    void enable_SPSI(void)const;       //enable switch phase signal interrupt
    void disable_SPSI(void)const;      //disable switch phase signal interrupt
private:
    int state_number;
    unsigned char up_phase_table[6];
    unsigned char dw_phase_table[6];
    unsigned char input_phase_table[6];
};
extern phase_switcher switcher;
/*
ʱ��Ϳ�����
*/
class TAV_controler
{
public:
    TAV_controler(void);
    void record_SPT(void);
    void set_time_to(void);
    void predict_SPT(void);
    void diable_time_interrupt(void);
    //
    int read_SPT(void)
    {
      return SPT;
    }
    
    unsigned char work_state;                //0,�ɼ�ʱ�����ݣ�1����ʱ���ѣ�2���������ѣ�3��STT_allow_SPS_interrupt
    unsigned char pure_control;                  //0,SP_flalse,1,SP
    unsigned char pure_control_times;
private:
    int last_TA0;
#define RECORD_NUMBER 5
    unsigned int SPT_record[RECORD_NUMBER];
    unsigned char pointer;
    int SPT;
    int read_TA0(void)const;
};
extern TAV_controler controler;
class Uart
{
public:
    Uart():SW_pointer(NULL),SW_char(0),SW_number(0),SW_position(0){}
    _Bool send(const char[],unsigned int);
    _Bool send(char,unsigned int);
    void send_subfuntion(void);
private:
    unsigned char *SW_pointer;
    unsigned char SW_char;
    unsigned int SW_number;
    unsigned int SW_position;
    unsigned int mode;
};
extern Uart uart;
extern int code;
#endif
