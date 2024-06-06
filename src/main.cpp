#include <Arduino.h>

constexpr int frequencyPin = GPIO_NUM_35;

volatile unsigned long lastMeasure = 0;
volatile unsigned long periodSum = 0;

volatile unsigned int counter = 0;

unsigned long timer = 1000;
unsigned long lastTime = 0;

void Frequency_ISR()
{
	unsigned long current = micros();
	periodSum += current - lastMeasure;
	lastMeasure = current;
	counter++;
}

void setup()
{
	Serial.begin(115200);
	pinMode(frequencyPin, INPUT);

	attachInterrupt(digitalPinToInterrupt(frequencyPin), Frequency_ISR, RISING);

	delay(1000);
}

void loop()
{
	if (millis() - lastTime >= timer)
	{
		lastTime = millis();

		float period = (float)periodSum / counter;
		float frequency = 1000000.0/period;
		Serial.printf("Frequency: %f    Measures: %d\n", frequency, counter);
		counter = 0;
		periodSum = 0;
	}
}