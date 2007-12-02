#ifndef MISCUTILS_H
#define MISCUTILS_H

/*
   MiscUtils.h - misc helper routines

   Copyright (C) 2003, 2004 Matthias Reichl <hias@horus.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <stdio.h>
#include "DiskImage.h"
#include "Dos2xUtils.h"
#ifndef WINVER
#include <sys/time.h>
#endif

namespace MiscUtils {
	char* ShortenFilename(const char* filename, unsigned int maxlen);

#ifndef WINVER
	bool drop_root_privileges();
	bool set_realtime_scheduling(int priority);
	bool drop_realtime_scheduling();

	typedef unsigned long long TimestampType;

	inline TimestampType TimevalToTimestamp(struct timeval& tv)
	{
		return (TimestampType)tv.tv_sec * 1000000 + tv.tv_usec;
	}
	
	inline TimestampType GetCurrentTime()
	{
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return TimevalToTimestamp(tv);
	}

	void WaitUntil(TimestampType endTime);

#endif

	inline void EatSpace(const char* &string)
	{
		if (string) {
			while (*string == ' ') {
				string++;
			}
		}
	}

};


#endif