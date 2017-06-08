#include <avr/interrupt.h>

#include "events.hpp"
#include "midi.hpp"
#include "servo.hpp"
#include "gpiodef.hpp"

using namespace midi;
using namespace servo;

volatile bool NearSensorEvent = false;
volatile bool FarSensorEvent = false;
volatile bool MidiNoteEvent = false;

volatile uint8_t MidiBendState = 0;
volatile uint8_t MidiNoteIntensity = 0;
volatile uint32_t UniversalTime = 0;
static Servo Rock;
static Midi MidiInterface;

static void MidiNoteON(uint8_t Key, uint8_t Velocity)
{
	MidiNoteEvent = true;
	MidiNoteIntensity = Velocity*2;
	LedMIDION();
}

static void MidiNoteOFF(uint8_t Key, uint8_t Velocity)
{
	LedMIDIOFF();
}

static void MidiRockCtrl(uint8_t Bend, uint8_t useless)
{
	static uint8_t RockState = 0;
	
	switch (Bend)
	{
	case 0:
		Rock.SetAngle(-10);
		RockState = 0;
		break;

	case 1:
		if (RockState == 0)
		{
			Rock.SetAngle(15);
		}
		else if (RockState == 2)
		{
			Rock.SetAngle(14);
		}

		RockState = 1;
		break;

	case 2:
		Rock.SetAngle(27);
		RockState = 2;
		break;
		
	default:
		break;
	}
}


void EventsInit()
{
	/*
	 * Enable the external interrupts 
	 */
	EICRA = (1 << ISC00) | (0 << ISC00) |
		(1 << ISC10) | (0 << ISC10);
	EIMSK = (1 << INT0) | (1 << INT1);
	EIFR = 0;
	sei();

	MidiInterface.SetChannelCall(&ReadDIPSwitch);
	MidiInterface.AttachEvent(NoteON, &MidiNoteON);
	MidiInterface.AttachEvent(NoteOFF, &MidiNoteOFF);
	MidiInterface.AttachEvent(PitchBend, &MidiRockCtrl);
}

/*
 * NearSensor interrupt routine
 */
ISR(INT0_vect)
{
	NearSensorEvent = true;
}

/*
 * FarSensor interrupt routine
 */
ISR(INT1_vect)
{
	FarSensorEvent = true;
}