#include <Heiko.h>

#include "TimeOut.h"

TimeOut::TimeOut (unsigned long _ms, bool timeout, bool enabled)
	: enabled (enabled), ms (_ms), overflow (timeout ? 0 : millis () + ms)
{
}

#if defined DEBUG_TimeOut
void TimeOut::toString (char* bfr)
{
	sprintf_P (bfr, PSTR ("%d, %ld"), ms, overflow);
}
#endif

bool TimeOut::hasOverflow ()
{	
	if (!enabled)
		return false;

	auto now = millis ();
	if (now < overflow)
		return false;

	overflow = now + ms;
	return true;
}

bool TimeOut::hasEnded ()
{	
	if (!enabled)
		return false;

	unsigned long now = millis ();
	if (now < overflow)
		return false;

	enabled = false;
	return true;
}

void TimeOut::reset ()
{
	enabled = true;
	overflow = millis () + ms;
}

void TimeOut::reset (unsigned long _ms)
{
	enabled = _ms != 0;
	ms = _ms;
	overflow = millis () + ms;
}

void TimeOut::disable ()
{
	enabled = false;
}


// _______________________________________________________________________________
bool RepetiveDelay::hasOverflow ()
{	
	if (!isRunning ())
		return false;

	unsigned long now = millis ();
	if (now < overflow)
		return false;

	if (++repeated > repetitions)
		disable ();
	else
		overflow = now + ms;
	return true;
}

void RepetiveDelay::reset ()
{
	TimeOut::reset ();
	repeated = 0;
}

void RepetiveDelay::reset (unsigned long ms) 
{
	TimeOut::reset (ms);
	repeated = 0;
}