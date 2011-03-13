#ifndef _DUTILS_H
#define	_DUTILS_H

#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define MILLISECS_IN_SECOND 1000

#define POS_DB_NAME         "posDB"
#define TEAM_DB_NAME        "teamDB"
#define POS_PATTERN         "%d %f %f %f %c"    /* <id posX posY orientation hasPuck> */
#define TEAM_PATTERN        "%d %f %f %d %d %d" /* <id posX posY colR colG colB> */
#define MAX_POS_KEYS        100
#define TEAM_ID_SHIFT       1024

#endif	/* _DUTILS_H */
