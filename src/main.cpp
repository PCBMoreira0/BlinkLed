#include <Arduino.h>

#define CODE 1

#if CODE == 0

constexpr int frequencyPin = GPIO_NUM_35;

volatile unsigned long lastMeasure = 0;
volatile unsigned long period = 0;

void Frequency_ISR()
{
	unsigned long current = micros();
	period = current - lastMeasure;
	lastMeasure = current;
}

void setup()
{
	Serial.begin(9600);
	pinMode(frequencyPin, INPUT);

	attachInterrupt(digitalPinToInterrupt(frequencyPin), Frequency_ISR, RISING);

	delay(1000);
}

void loop()
{
	float frequency;
	if (!period)
		frequency = 0;
	else
		frequency = 1000000.0 / period;

	Serial.printf("Frequency: %.2f\t\r", frequency);
}

#endif

#if CODE == 1

constexpr int frequencyPin = GPIO_NUM_35;

volatile unsigned long lastMeasure = 0;
volatile unsigned long period = 0;

volatile unsigned int counter = 0;

unsigned long timer = 1000;
unsigned long lastTime = 0;

void Frequency_ISR()
{
	unsigned long current = micros();
	period = current - lastMeasure;
	lastMeasure = current;

	counter++;
}

void setup()
{
	Serial.begin(9600);
	pinMode(frequencyPin, INPUT);

	attachInterrupt(digitalPinToInterrupt(frequencyPin), Frequency_ISR, RISING);

	delay(1000);
}

void loop()
{
	if (millis() - lastTime >= timer)
	{
		lastTime = millis();
		Serial.printf("Frequency: %d\n", counter);
		counter = 0;
	}
}

#endif