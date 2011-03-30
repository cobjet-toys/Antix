#ifndef CONFIG_H_
#define CONFIG_H_

#define RELEASE
#define LOGGING

#ifdef RELEASE  
#define DEBUGPRINT(format, args...) ((void)0)
#else
#define DEBUGPRINT printf
#endif

#ifdef LOGGING  
#define LOGPRINT(format, args...) ((void)0)
#else
#define LOGPRINT printf
#endif


#endif
