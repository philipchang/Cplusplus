#ifndef _LOG_H
#define _LOG_H

#include <memory.h>
#include <stdarg.h>
#include <iostream>
using namespace std;

inline void MN_LOG( const char* mod, const char* fmt, ... )
{
	const int BUF_SIZE = 512;
	char tcs[BUF_SIZE];
	memset(tcs, 0, sizeof(char)*BUF_SIZE);

	va_list args;
	va_start (args, fmt );
	vsnprintf(tcs, BUF_SIZE-1, fmt, args);
	va_end (args);

	string strLog;
	if ( mod != NULL )
	{
		/*Time time = Time::getCurrentTime();
		strLog = String::formatted( ("[%02d:%02d:%02d %03d]"), 
			time.getHours(), time.getMinutes(), time.getSeconds(), time.getMilliseconds() );
		*/
		strLog += "["; 
		strLog += mod;
		strLog += "] ";
	}

	strLog += tcs;
	cout<<strLog.c_str()<<endl;
}

#endif
