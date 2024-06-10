#include <Arduino.h>

constexpr int frequencyPin_1 = GPIO_NUM_34;
constexpr int frequencyPin_2 = GPIO_NUM_22;

volatile unsigned long lastMeasure_1 = 0;
volatile unsigned long periodSum_1 = 0;

volatile unsigned int counter_1 = 0;

volatile unsigned long lastMeasure_2 = 0;
volatile unsigned long periodSum_2 = 0;

volatile unsigned int counter_2 = 0;

unsigned long timer = 1250;
unsigned long lastTime = 0;

int block = 0;

float ConstantPCB(float value){
	return value/2.0;	
}

float PeriodToFrequency(float value){
	return 1000000.0/value;	
}

void ResetValues(){
	block = 1;
	periodSum_1 = 0;
	periodSum_2 = 0;
	counter_1 = 0;
	counter_2 = 0;
	block = 0;
}

void Frequency_ISR_1()
{
	if(!block){
		unsigned long current = micros();
		periodSum_1 += current - lastMeasure_1;
		lastMeasure_1 = current;
		counter_1++;
	}
}

void Frequency_ISR_2()
{
	if(!block){
		unsigned long current = micros();
		periodSum_2 += current - lastMeasure_2;
		lastMeasure_2 = current;
		counter_2++;
	}
}

void setup()
{
	Serial.begin(115200);
	pinMode(frequencyPin_1, INPUT);
	pinMode(frequencyPin_2, INPUT);

	attachInterrupt(digitalPinToInterrupt(frequencyPin_1), Frequency_ISR_1, RISING);
	attachInterrupt(digitalPinToInterrupt(frequencyPin_2), Frequency_ISR_2, RISING);

	delay(1000);
}

void loop()
{
	if (millis() - lastTime >= timer)
	{
		lastTime = millis();

		float period_1 = (float)periodSum_1 / counter_1;
		float period_2 = (float)periodSum_2 / counter_2;

		float frequency_1 =PeriodToFrequency(period_1);
		float frequency_2 = PeriodToFrequency(period_2);

		Serial.printf("Frequency_1: %f    Measures_1: %d\n", frequency_1, counter_1);
		Serial.printf("Frequency_2: %f    Measures_2: %d\n", frequency_2, counter_2);
		Serial.println();
		
		ResetValues();
	}
}