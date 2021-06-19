#include "stm32f10x.h"

#include "app.h"
#include <stdio.h>
#include "stm32f10x_usart.h"

#ifndef USB_STOP_MODE
#define TIMER_COUNT 36000  //1Khz
#else
#define TIMER_COUNT 16000  //500hz
#endif

#define MAX_PWM_COUNT  TIMER_COUNT

void PWM_Configuration(void)
	{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��ʹ��
		
			/*GPIOB Configuration: TIM3 channel2*/	  //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		
		
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	//����Ƶ��PWMƵ��=72000000/900=80Khz
	/* ---------------------------------------------------------------
	TIM3CLK ��PCLK1=36MHz
	TIM3 Configuration: generate 1 PWM signals :
    TIM3CLK = 36 MHz, Prescaler = 0x0, TIM3 counter clock = 36 MHz
    TIM3 ARR Register = 900 => TIM3 Frequency = TIM3 counter clock/(ARR + 1)
    TIM3 Frequency = 36 KHz.
    TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 
	TIM3CLK = 36 MHz, Prescaler = 0, TIM3 counter clock = 36MHz
	--------------------------------------------------------------- */
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = TIMER_COUNT; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =0; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	/* Output Compare Active Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����


	TIM_SetCompare2(TIM3,0);
	}

void LedInit(void )
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(LED1_CLOCK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = LED1_PIN|LED2_PIN|LED3_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED1_GPIO, &GPIO_InitStructure);

	GPIO_SetBits(LED1_GPIO,LED1_PIN);
	GPIO_SetBits(LED2_GPIO,LED2_PIN);
	GPIO_SetBits(LED3_GPIO,LED3_PIN);
}


void config_pwm_gpio_pin(void )
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_SetBits(GPIOA,GPIO_Pin_7);
}


void config_pwm_pin(void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��ʹ��
		
			/*GPIOB Configuration: TIM3 channel2*/	  //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}



void pwm_gpio_trigle(void)
{
	static unsigned int st = 0;
	if(st & 0x1)
		GPIO_SetBits(GPIOA,GPIO_Pin_7);
	else
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	
	st++;
}




void led_trigle(uint16_t led )
{
	static unsigned int on = 0;
	if(on & 0x1)
		GPIO_SetBits(LED1_GPIO,led);
	else
		GPIO_ResetBits(LED1_GPIO,led);
	
	on++;
}

void Init_All_Periph(void)
{
	RCC_ClocksTypeDef RCC_ClocksStatus;
	
	//use clock default,8Mhz
#ifndef USB_STOP_MODE
	RCC_Configuration();
#endif	
	NVIC_Configuration();

	ComInit(COM1,115200);
	
	printf("com init done\r\n");
	//usart1TxDMAInit();
	delay_init( );
	
	//TIM3_UserConfiguration();

	LedInit();
	
	printf("rtc init ...\r\n");
	RTC_UserInit();
	
	printf("pwm init ...\r\n");
	PWM_Configuration();

	
	RCC_GetClocksFreq(&RCC_ClocksStatus);
	printf("system clock : sysclk %d ,hclk %d ,APB11 %d ,APB2 %d \r\n", \
					RCC_ClocksStatus.SYSCLK_Frequency,RCC_ClocksStatus.HCLK_Frequency, \
					RCC_ClocksStatus.PCLK1_Frequency,RCC_ClocksStatus.PCLK2_Frequency);

}



extern void main_loop(void );
tm timer;

int g_user_alarm_hour = 0;
int g_user_alarm_min = 0;
int g_user_pwm_min = 0;

#define USR_DEFINE_HOUR g_user_alarm_hour
#define USR_DEFINE_MIN g_user_alarm_min
#define USR_DEFINE_PWM_COUNT g_user_pwm_min  //the min from 0 to Max
#define USR_DEFINE_PWM_HOUR 1			//the hour from on to off


void pwm_start_out(void)
{
	unsigned int msec,step,pwmval,cnt,delay;
	int stat = 0;
	
	msec = USR_DEFINE_PWM_COUNT*600;
	step = 1;//TIMER_COUNT/4/msec;
	pwmval = 0;
	delay = 1000;
	//cnt = 0;
	//step = 1;
	while(1){
			TIM_SetCompare2(TIM3,pwmval);
			pwmval += step;
			cnt++;
			delay_ms(delay);
			if(pwmval > TIMER_COUNT)
				break;
			if((stat == 0) && (cnt >= USR_DEFINE_PWM_COUNT*60)){
				stat = 1;
				delay = 1000 ;
				step = (TIMER_COUNT - pwmval)/(60); // һ�����ڴﵽȫ������
				printf("goto big step\r\n");
			}
			
			
			led_trigle(LED1_PIN);
	}
}


int pwm_start = 0,pwm_stop = 0;

void pwm_start_irqhandler(void *arg)
{
	pwm_start = 1;
	//__DSB();
	//GPIO_ResetBits(LED2_GPIO,LED2_PIN);
}

void pwm_stop_irqhandler(void *arg)
{
	pwm_stop = 1;
	//__DSB();
}
	
	

int main(void)
{  
	
	unsigned int sec,cc = 0;
	uint16_t back;
	tm timer_dest;
	Init_All_Periph();

	g_user_alarm_hour = 6;
	g_user_alarm_min = 20;
	g_user_pwm_min = 20;

	//pwm_start_out();
	
	
	
	
	printf("system startimg...\r\n");
	back = BKP_ReadBackupRegister(BKP_DR1);
	if(back != 0){
		g_user_alarm_hour = ((back &0xff00)>>8)%24;
		g_user_alarm_min = (back &0xff)%60;
	}	
	back = BKP_ReadBackupRegister(BKP_DR2);
	if(back != 0){
		g_user_pwm_min = (back & 0xff)%60;
	}
	
	back = BKP_ReadBackupRegister(BKP_DR4);
	
	
	
	
	RTC_Get(&timer);

	printf("last flag value is 0x%x\r\n",back);
	printf("current timer is :\r\n");
	printf("	%d	%02d %02d,%02d:%02d:%02d\r\n",timer.w_year,timer.w_month,timer.w_date,timer.hour,timer.min,timer.sec);
	printf("user set timer is %d:%d ,pwm mini %d\r\n",USR_DEFINE_HOUR,USR_DEFINE_MIN,USR_DEFINE_PWM_COUNT);
	
	timer_dest.hour = USR_DEFINE_HOUR;
	timer_dest.min = USR_DEFINE_MIN;
	RTC_SetAlarm_user(&timer_dest,pwm_start_irqhandler);
	
	if(usartCharGet_timeout()){
		printf("\r\nno user stop,step into normal mode\r\n");
		while(1){
					
			//RTC_Get(&timer);

			//delay_ms(500);
			//led_trigle(LED3_PIN);

			if(pwm_start){
					RTC_Get(&timer);

					printf("pwm startimg...\r\n");
					printf("current timer is :\r\n");
					printf("	%d	%02d %02d,%02d:%02d:%02d\r\n",timer.w_year,timer.w_month,timer.w_date,timer.hour,timer.min,timer.sec);
				
						

					pwm_start_out();
					pwm_start = 0;
				
					config_pwm_gpio_pin();
					while(cc < 30){
							delay_ms(1000);
							pwm_gpio_trigle();
							cc++;
					}
					
					/*
					RTC_Get(&timer);
					timer_dest.hour = timer.hour+USR_DEFINE_PWM_HOUR;
					timer_dest.min = timer.min;
					RTC_SetAlarm_user(&timer_dest,pwm_stop_irqhandler);
					*/

					printf("pwm end...\r\n");
					
					config_pwm_pin();
					pwm_stop = 1;
				
			}
			
			if(pwm_stop){
					printf("shutdown the LED ...\r\n");
					TIM_SetCompare2(TIM3,0);
					pwm_stop = 0;
				
					timer_dest.hour = USR_DEFINE_HOUR;
					timer_dest.min = USR_DEFINE_MIN;
					RTC_SetAlarm_user(&timer_dest,pwm_start_irqhandler);
			}
			
			printf("\r\n\r\ngo to sleep\r\n");
#ifdef USB_STOP_MODE
			
			PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI); // when resum from stop mode,the system clock is HSI 8Mhz
#else
			__WFI();
#endif
			//GPIO_ResetBits(LED1_GPIO,LED1_PIN);
			//delay_ms(10);
			printf("system resum\r\n");
			
			//BKP_WriteBackupRegister(BKP_DR4, (pwm_start<<8) | pwm_stop);
			//PWR_EnterSTANDBYMode();
			
			
		}
	}
	else{
		 //GPIO_ResetBits(LED2_GPIO,LED2_PIN);
		main_loop();
	}

	
}





#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
  while (1)
  {
  }
}
#endif
