#include <stdint.h>
#include <stdio.h>
#include <conio.h>
#include <6502.h>

#include "ping.inc"
#include "crash.inc"

struct YM2151_t {
	uint8_t reg;
	uint8_t data;
};

static uint8_t eatDelay;
static uint8_t crashDelay;
static uint8_t levelNote;
static uint8_t levelDelay;

#define YM2151 (*(volatile struct YM2151_t*) 0x9fe0)

static void writeYM2151Reg(uint8_t reg, uint8_t value)
{
	uint8_t i;
	YM2151.reg = reg;
	YM2151.data = value;
	
	// delay between writes must be at least 64 YM2151 cyclces, which is
	// 224 cyckes if tge 6502, if it runs at 14 MHz, and the YM2151 at 4 MHz.
	// The function and call needs about 50 cycles. One loop 22 cycles.
	// Add some reserve in case CC65 optimizes it better in future versions.
	for (i = 0; i < 10; i++) {
		asm("nop");
	}
}

static void loadInstrument(uint8_t channel, uint8_t* data)
{
	uint8_t i;
	uint8_t ofs = 0;
	uint8_t r = 0;
	writeYM2151Reg(0x20 + channel, data[ofs++]);
	writeYM2151Reg(0x38 + channel, data[ofs++]);
	for (i = 0; i < 4; i++) {
		writeYM2151Reg(0x40 + r + channel, data[ofs++]);
		writeYM2151Reg(0x60 + r + channel, data[ofs++]);
		writeYM2151Reg(0x80 + r + channel, data[ofs++]);
		writeYM2151Reg(0xa0 + r + channel, data[ofs++]);
		writeYM2151Reg(0xc0 + r + channel, data[ofs++]);
		writeYM2151Reg(0xe0 + r + channel, data[ofs++]);
		r += 8;
	}
}

static void noteOn(uint8_t channel, uint8_t octave, uint8_t note)
{
	// set frequency
	writeYM2151Reg(0x28 + channel, (octave << 4) | note);
	
	// note on
	writeYM2151Reg(8, 0x78 | channel);
}
					 
static void noteOff(uint8_t channel)
{
	writeYM2151Reg(8, channel);
}
					 
void platAudioInit()
{
	uint16_t i;
	for (i = 0; i < 256; i++) {
		writeYM2151Reg(i, 0);
	}
	loadInstrument(0, pingInstrument);
	loadInstrument(1, crashInstrument);
	loadInstrument(2, pingInstrument);
	eatDelay = 0;
	crashDelay = 0;
	levelNote = 0;
	levelDelay = 0;
}

void platAudioEat()
{
	noteOn(0, 3, 0);
	eatDelay = 2;
}

void platAudioCrash()
{
	noteOn(1, 3, 0);
	crashDelay = 5;
}

void platAudioExitLevel()
{
	levelNote = 7;
	levelDelay = 2;
	noteOn(2, 5, 7 - levelNote);
}

void platAudioTick()
{
	if (eatDelay) {
		eatDelay--;
		if (eatDelay == 0) {
			noteOff(0);
		}
	}
	if (crashDelay) {
		crashDelay--;
		if (crashDelay == 0) {
			noteOff(1);
		}
	}
	if (levelDelay) {
		levelDelay--;
		if (levelDelay == 0) {
			if (levelNote) {
				levelNote--;
				if (levelNote == 0) {
					noteOff(2);
				} else {
					noteOn(2, 5, 7 - levelNote);
					levelDelay = 2;
				}
			}
		}
	}
}
