#ifndef CONFIG_H_
#define CONFIG_H_

//#define RELEASE
//#define STEP_BY_STEP //OOOOHHH BABY! Gonna get to you gurrrrl.
#define LOGGING


#ifdef RELEASE  
#define DEBUGPRINT(format, args...) ((void)0)
#else
#define DEBUGPRINT printf
#endif

#define ERRORPRINT printf

#ifdef LOGGING  
#define LOGPRINT(format, args...) ((void)0)
#else
#define LOGPRINT printf
#endif


#endif
