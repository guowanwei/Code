//add by gww 2017/6/12
#ifndef _COUNT_PERF_
#define _COUNT_PERF_
#include <Windows.h>
#include <iostream>
//std::cout << "gww_"#name":" << name##totaltime /100.0 << " ms" << std::endl;
extern DWORD FramesFromGameBegin;
#define FRAMES 100
//#define _COUT_PRINT_INFO_
#ifdef _COUT_PRINT_INFO_
#define BeginCountPerformance(name) static DWORD name##totaltime = 0; \
	static DWORD name##PrintedFrames = 0;\
	DWORD name##starttime = GetTickCount();

#define EndCountPerformance(name) DWORD name##endtime = GetTickCount();\
	if(FramesFromGameBegin > 0 && FramesFromGameBegin % FRAMES == 0 && FramesFromGameBegin != name##PrintedFrames)\
	{\
		printf("%s%f%s\n","gww_"#name":",name##totaltime /100.0," ms");\
		name##PrintedFrames = FramesFromGameBegin;\
		name##totaltime = name##endtime - name##starttime;\
	}\
	else\
	{\
		name##totaltime += name##endtime - name##starttime;\
	}



#define GwwCountPerformance(name) static DWORD name##totaltime = 0; \
	static DWORD name##PrintedFrames = 0;\
	if(FramesFromGameBegin > 0 && FramesFromGameBegin % FRAMES == 0 && FramesFromGameBegin != name##PrintedFrames)\
	{\
		printf("%s%f%s\n","gww_"#name"_calltimes:",name##totaltime /100.0," ´ÎÊý");\
		name##PrintedFrames = FramesFromGameBegin;\
		name##totaltime= 1;\
	}\
	else\
	{\
		name##totaltime++;\
	}



#else
#define BeginCountPerformance(name)
#define EndCountPerformance(name)
#define GwwCountPerformance(name)
#endif


#endif