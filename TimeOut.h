// TimeOut.h

#ifndef _TIMEOUT_h
#define _TIMEOUT_h

#include <arduino.h>
#include <Heiko.h>

//#define DEBUG_TimeOut

class TimeOut
{
protected:
	bool enabled;
	unsigned long ms;
	unsigned long overflow;

public:
	explicit TimeOut (unsigned long ms, bool timeout = false, bool enabled = true);

#if defined DEBUG_TimeOut
	void toString (char* bfr);
#endif

	unsigned long getMilliseconds () const { return ms; }
	unsigned getSeconds () const { return ms / 1000; }

	operator bool () const { return enabled && millis () >= overflow; };

	unsigned long getRest () const { return overflow - millis (); }
	unsigned getRemainingSec () const { return getRest () / 1000; }

	virtual void reset ();
	virtual void reset (unsigned long ms);


	void disable ();
	bool isEnabled () const { return enabled; }

	// Sets state overflow.
	void setOverflow ()
	{
		ms = 0;
	}

	// Check for running (enabled && < overflow)
	bool isRunning () const
	{
		return enabled && millis () < overflow;
	}


	// Check for overflow. (enabled && >= overflow)
	bool isOverflow () const
	{
		return enabled && millis () >= overflow;
	}

	// Check & reset. False if disabled.
	// enabled & Overflow
	virtual bool hasOverflow ();

	// Check & disable.
	// enabled & Overflow
	bool hasEnded ();

	// deleted.
	TimeOut & operator=(const TimeOut&) = delete;
	TimeOut (const TimeOut&) = delete;
};

class TimeOutSec : public TimeOut
{
public:
	explicit TimeOutSec (unsigned long sec, bool timeout = false, bool enabled = true)
		: TimeOut (sec * 1000ul, timeout, enabled)
	{}
};
class TimeOutMin : public TimeOutSec
{
public:
	explicit TimeOutMin (unsigned long min, bool timeout = false, bool enabled = true)
		: TimeOutSec (min * 60ul, timeout, enabled)
	{}
};
class TimeOutHour : public TimeOutMin
{
public:
	explicit TimeOutHour (unsigned long hour, bool timeout = false, bool enabled = true)
		: TimeOutMin (hour * 60ul, timeout, enabled)
	{}
};

// _______________________________________________________________________________
// One shot only.
class RepetiveDelay : public TimeOut
{
	uint repetitions;
	uint repeated;

public:
	explicit RepetiveDelay (unsigned long ms, uint repetitions)
		: TimeOut (ms, false, false),
		repetitions (repetitions), repeated (0)
	{}

	uint getRepetitions () const { return repetitions; };
	uint getRepeated () const { return repeated; };

	// Check for time and repetitions, disable instead of reset. False if disabled.
	bool hasOverflow () override;

	void reset () override;
	void reset (unsigned long ms) override;


	// deleted.
	RepetiveDelay & operator=(const RepetiveDelay&) = delete;
	RepetiveDelay (const RepetiveDelay&) = delete;
};

class Debounce : public TimeOut
{

public:
	explicit Debounce (unsigned long ms)
		: TimeOut (ms, true, false)
	{}

	bool isDebouncing () { return isRunning () && !hasEnded (); }
	void reset () { TimeOut::reset (); }
};

class Stopwatch
{
	unsigned long msStart;
	unsigned long msEnd;

public:
	explicit Stopwatch ()
		: msStart (millis ()), msEnd (msStart)
	{}

	operator unsigned () const { return getMilliseconds (); };

	void reset ()
	{
		msStart = millis ();
		msEnd = msStart;
	}

	void start ()
	{
		msStart = millis ();
	}

	void stop ()
	{
		msEnd = millis ();
	}

	unsigned ellapsed () const
	{
		return millis() - msStart;
	}

	unsigned getMilliseconds () const
	{
		return msEnd - msStart;
	}

	unsigned getSeconds () const
	{
		return (millis () - msStart) / 1000ul;
	}
};



#endif

