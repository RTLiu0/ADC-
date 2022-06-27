#include"My_adc.h"

//extern uint16_t adc_data;
uint16_t adc_data;
void My_ADC_Adjust(void)
{
	ADC_ResetCalibration(ADC1); //开启复位校准 
	while(ADC_GetResetCalibrationStatus(ADC1)); //等待复位校准结束
	ADC_StartCalibration(ADC1); //开启 AD 校准
	while(ADC_GetCalibrationStatus(ADC1)); //等待校准结束
}

void My_ADC_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
	
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Mode=GPIO_Mode_AIN;
	gpio.GPIO_Pin=GPIO_Pin_0;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
	
	ADC_DeInit(ADC1);
	ADC_InitTypeDef adc;
	adc.ADC_ContinuousConvMode= ENABLE;
	adc.ADC_DataAlign=ADC_DataAlign_Right;
	adc.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	adc.ADC_Mode=ADC_Mode_Independent;
	adc.ADC_NbrOfChannel=1;
	adc.ADC_ScanConvMode=DISABLE;
	ADC_Init(ADC1,&adc);
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_239Cycles5);
	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
	My_ADC_Nvic();
	ADC_Cmd(ADC1,ENABLE);
	My_ADC_Adjust();
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}

void My_ADC_Nvic()
{
	NVIC_InitTypeDef nvic;
	nvic.NVIC_IRQChannel = ADC1_2_IRQn;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority = 3;
	nvic.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&nvic);

}

void ADC1_2_IRQHandler()
{
	if(ADC_GetITStatus(ADC1,ADC_IT_EOC))
	{
		adc_data=ADC_GetConversionValue(ADC1);
	}
	ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
}

float My_ADC_voltage(uint16_t data)
{
	float k=(float)data/4096*3.3;
	return k;
}	
