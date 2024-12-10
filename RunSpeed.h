// RunSpeed.h

#ifndef _RUNSPEED_h
#define _RUNSPEED_h

#include <Arduino.h>
#include <Print.h>
#include <Statistics.h>
#include "TimeOut.h"



class RunSpeed
{
	struct Part
	{
		const char* name;
		Average time;
	};

	// ReSharper disable once CppInconsistentNaming
	static constexpr byte RunSpeed_MAXPARTS = 8;
	Average loop;
	Part parts [RunSpeed_MAXPARTS] = {};
	unsigned long loopTime, startTime;
	unsigned part;

 public:
	explicit RunSpeed ()
		: loopTime(0), startTime (0), part (0)
	{
	}

	void clear ()
	{
		loop.clear ();
		for (auto &part : parts)
			part.time.clear ();
	}

	void add (const char* name)
	{
		for (auto &part : parts)
		{
			if (part.name == nullptr)
			{
				part.name = name;
				return;
			}
		}
	}

	void start ()
	{
		const auto now = millis ();

		// Handle loop time, not possible first time.
		if (loopTime != 0)
		{
			const auto time = now-loopTime;
			if (time > 1)
				loop.add (time);
		}
		loopTime = now;

		// Start measuring loop parts.
		startTime = now;
		part = 0;
	}

	void next ()
	{
		done (part++);
	}

	void done (byte part)
	{
		const auto time = millis ()-startTime;

		// Ignore too less times without action calls.
		if (time > 1)
			parts[part].time.add (time);

		startTime = millis ();
	}

	void print (Print& out)
	{
		out.println ("Speed:");
		out.print ("Loop ");
		out.println (loop.getValue ());

		for (const auto &part : parts)
		{
			// Ignore parts too fast.
			if (part.time.getSamples () == 0)
				continue;

			// End when no name.
			if (part.name == nullptr)
				return;

			out.print (part.name);
			out.print (' ');
			out.print (part.time.getValue ());
			out.println ();
		}
	}
};


#endif

