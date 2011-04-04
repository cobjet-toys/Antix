#ifndef CONFIG_H_
#define CONFIG_H_

//#define RELEASE
#define DEBUG
#define STEP_BY_STEP //OOOOHHH BABY! Gonna get to you gurrrrl.
#define LOGGING
//#define GRIDTEST // Used when wanting to compile with individual unit testing
                 // GridGame instances
//#define AITESTING

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

#ifdef AITESTING
#define AIPRINT(format, args...) ((void)0)
#else
#define AIPRINT printf
#endif


#endif
