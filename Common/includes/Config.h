#ifndef CONFIG_H_
#define CONFIG_H_

#define DEBUG true

#ifdef DEBUG 
#define DEBUGPRINT(format, args...) ((void)0)
#else
#define DEBUGPRINT printf
#endif

#endif
