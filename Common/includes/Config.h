#ifndef CONFIG_H_
#define CONFIG_H_

#define DEBUG

#ifdef RELEASE 
#define DEBUGPRINT(format, args...) ((void)0)
#else
#define DEBUGPRINT printf
#endif

#endif
