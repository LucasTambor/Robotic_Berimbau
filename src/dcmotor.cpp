#include "dcmotor.hpp"
#include "gpiodef.hpp"

#include <avr/io.h>

using namespace dcmotor;

DCMotor::DCMotor()
{
	/*
	 * Configure PB3 pin as
	 * an output (PWM)
	 */
	DDRB |= (1 << 3);
	
	OCR2A = 0;
	
	TCCR2A = (1 << COM2A1) | (1 << WGM21) |
		(1 << WGM20);

	TCCR2B = (1 << CS22) | (0 << CS21) |
		(0 << CS20);
}

void DCMotor::Action(MotorAction Action, uint8_t Power)
{
	switch (Action)
	{
	case TurnCCW:
		M2OFF();
		M1ON();
		OCR2A = Power;
		break;

	case TurnCW:
		M1OFF();
		M2ON();
		OCR2A = Power;
		break;

	case OFF:
		OCR2A = 0;
		M2OFF();
		M1OFF();
		break;

	case Break:
		OCR2A = 255;
		M2OFF();
		M1OFF();
		break;

	default:
		break;
	}
}

DCMotor::~DCMotor()
{
	TCCR2A = 0;
	TCCR2B = 0;
	M2OFF();
	M1OFF();
}