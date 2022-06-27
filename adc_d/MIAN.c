#include"stm32f10x.h"
#include"delay.h"
#include"My_adc.h"
extern uint16_t adc_data;

void D_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	GPIO_InitTypeDef ee;
	ee.GPIO_Mode=GPIO_Mode_Out_PP;
	ee.GPIO_Speed=GPIO_Speed_50MHz;
	ee.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOB,&ee);
}
void set_data(uint8_t bit, uint8_t data)//10代表点
{
	uint32_t outD=GPIOB->ODR|0xF0FF;
	switch(data)
	{
		case 0:outD&=0XFFC0;break;
		case 1:outD&=0XFFF9;break;
		case 2:outD&=0XFFA4;break;
		case 3:outD&=0XFFB0;break;
		case 4:outD&=0XFF99;break;
		case 5:outD&=0XFF92;break;
		case 6:outD&=0XFF82;break;
		case 7:outD&=0XFFF8;break;
		case 8:outD&=0XFF80;break;
		case 9:outD&=0XFF90;break;
		case 10:outD&=0XFF7F;break;
		default:outD&=0XFFFF;break;
	}
	switch(bit)
	{
		case 0:outD&=0X7FFF;break;
		case 1:outD&=0XBFFF;break;
		case 2:outD&=0XDFFF;break;
		case 3:outD&=0XEFFF;break;
	}
	GPIOB->ODR=outD;
}

void showL(uint8_t data[])
{
	u8 wait=1;
	uint8_t bit=0;
	while(bit<4)
	{
		set_data(bit,data[bit]);
		delay_ms(wait);
		bit++;
	}
}

void show_x5msV2(uint16_t msx5,uint16_t data)//时间x5ms，显示数据
{
	uint8_t set[4],flag=1;
	int8_t bit=0;
	while(bit<4)
	{
		u8 show=data%10;
		data/=10;
		set[bit]=show;
		bit++;
	}
	bit=3;
	while(bit>=0)
	{
		if(flag)
		{
			if(!set[bit])
			{
				set[bit]=99;
				if(!bit)
				{
					set[bit]=0;
				}
			}
			else
			{
				flag=0;
			}
		}
		bit--;
	}
	while(msx5>0)
	{
		showL(set);
		msx5--;
	}
}

int main()
{
	delay_init();
	D_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	My_ADC_Init();
	
	GPIOB->ODR&=0xFF;
	while(1)
	{
		float out=My_ADC_voltage(adc_data);
		out*=1000;
		uint16_t outnum=out;
		
		
		
		if(outnum<10)
		{
			set_data(1,10);
			delay_ms(1);
			set_data(1,0);
			delay_ms(1);
		}
		else if(outnum<100)
		{
			set_data(2,10);
			delay_ms(1);
			set_data(2,0);
			delay_ms(1);
		}
		else if(outnum<1000)
		{
			set_data(3,10);
			delay_ms(1);
			set_data(3,0);
			delay_ms(1);
		}
		else
		{
			set_data(3,10);
			delay_ms(1);
		}
		delay_ms(1);
		show_x5msV2(1,outnum);
		
		
	}
}
