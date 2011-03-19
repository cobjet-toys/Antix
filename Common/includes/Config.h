#ifndef CONFIG_H_
#define CONFIG_H_

#define DEBUG false

#ifdef RELEASE 
#define DEBUGPRINT printf
#else
#define DEBUGPRINT(format, args...) ((void)0)
#endif

#endif