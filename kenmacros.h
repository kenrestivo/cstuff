/*
* 	$Id$
* 	macros for doing system calls and such

 Copyright (C) 2001-2007 by Ken Restivo <ken@restivo.org>
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Library General Public License as
 published by the Free Software Foundation; either version 2 of the
 License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful, but
 WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 Library General Public License for more details.

 You should have received a copy of the GNU Library General Public
 License along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 USA
*/


#ifndef KENMACROS_H
#define KENMACROS_H

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#ifdef HAVE_PTHREADS
#include <pthread.h>
#endif

#define _GNU_SOURCE

extern int debug  ;

/* MACROS */

#define STAT_ARRAY_SIZE(YYY) ((sizeof((YYY))/sizeof(*(YYY))))
#define STAT_ARRAY_END(YYY) ((YYY) + (sizeof((YYY))/sizeof(*(YYY))))

#define RETCALL(XXX) {  \
	if ( (XXX) < 0){ \
		if(debug){ \
			fprintf(stderr, "%s at %s:%d  %s\n", \
				#XXX,  __FILE__, __LINE__, strerror(errno)); \
		}  \
		return -1; \
	} \
}


#define NRETCALL(XXX) {  \
	if ( (XXX) < 0){ \
		if(debug){ \
			fprintf(stderr, "%s at %s:%d  %s\n", \
				#XXX,  __FILE__, __LINE__, strerror(errno)); \
		}  \
		return NULL; \
	} \
}


#define SYSCALL(XXX) {  \
	if ( (XXX) < 0){ \
		if(debug){ \
			fprintf(stderr, "%s at %s:%d  %s\n", \
				#XXX,  __FILE__, __LINE__, strerror(errno)); \
		}  \
		exit(1); \
	} \
}

#define NULLCALL(XXX) {  \
	if ( (XXX) == NULL ){ \
		if(debug){ \
			fprintf(stderr, "%s at %s:%d  %s\n", \
				#XXX,  __FILE__, __LINE__, strerror(errno)); \
		}  \
		exit(1); \
	} \
}

#define RETMEM(XXX) {   \
	if ( (XXX) == NULL){ \
		if(debug){ \
				fprintf(stderr, "%s at %s:%d  %s\n", \
					#XXX,  __FILE__, __LINE__, strerror(errno)); \
		}  \
		return -1; \
	} \
}


#define DPRINTF(level, string, var...) \
	if (debug >= level) fprintf(stderr, string, ## var)

/******************
	PTHREADS MACROS
******************/
#ifdef HAVE_PTHREADS
#define NEWMUTEX(XXX) { \
   NULLCALL(XXX = (pthread_mutex_t *) malloc (sizeof (pthread_mutex_t)));\
    pthread_mutex_init (XXX, NULL); \
}

#define KILLMUTEX(XXX) { \
   SYSCALL(pthread_mutex_destroy (XXX)); \
    free (XXX); \
}
#endif


/******************
	GDBM MACROS
******************/
#ifdef _GDBM_H_
#define GDBMNULL(XXX) {  \
	if ( (XXX) == NULL ){ \
		if(debug){ \
			fprintf(stderr, "%s at %s:%d  %s\n", \
				#XXX,  __FILE__, __LINE__, gdbm_strerror(gdbm_errno)); \
		}  \
		exit(1); \
	} \
}

#define GDBMCALL(XXX) {  \
	if ( (XXX) < 0){ \
		if(debug){ \
			fprintf(stderr, "%s at %s:%d  %s\n", \
				#XXX,  __FILE__, __LINE__, gdbm_strerror(gdbm_errno)); \
		}  \
		exit(1); \
	} \
}
#endif /* _GDBM_H_ */


/******************
	MYSQL MACROS
	AAA= the dbh connection handle
	BBB= a debug string to print (usually a query or name of function)
	CCC = the expression to test
******************/
/* TODO: put havemysqlh or some better test in here! */
#ifdef MYSQL_VERSION_ID
#define SQLCALL(AAA, BBB, CCC) {  \
	if ( (CCC) ){ \
		if(debug){ \
			fprintf(stderr, "%s failed: in %s at %s:%d  %s\n", \
						BBB, #CCC,  __FILE__, __LINE__, mysql_error(AAA)); \
		}  \
		exit(1); \
	} \
}

#define SQLNULL(AAA, BBB, CCC) {  \
	if ( (CCC) == NULL ){ \
		if(debug){ \
			fprintf(stderr, "%s failed: in %s at %s:%d  %s\n", \
						BBB, #CCC,  __FILE__, __LINE__, mysql_error(AAA)); \
		}  \
		exit(1); \
	} \
}

#define SQLCURSE(AAA, BBB, CCC) {  \
	if ( (CCC) < 0){ \
		if(debug){ \
			fprintf(glogf, "%s failed: in %s at %s:%d  %s\n", \
						BBB, #CCC,  __FILE__, __LINE__, mysql_error(AAA)); \
		}  \
		exit(1); \
	} \
}
#define SQLNULLCURSE(AAA, BBB, CCC) {  \
	if ( (CCC) == NULL){ \
		if(debug){ \
			fprintf(glogf, "%s failed: in %s at %s:%d  %s\n", \
						BBB, #CCC,  __FILE__, __LINE__, mysql_error(AAA)); \
		}  \
		exit(1); \
	} \
}


#endif /* MYSQL_VERSION_ID */

/******************
	BERKELEY DB MACROS
******************/
#ifdef _DB_H_
/* TODO: deal with closing these things gracefully, not just an exit(1)! */
#define DBNULL(XXX) {  \
	int rv; \
	if ( (rv = (XXX)) == NULL){ \
		if(debug){ \
			fprintf(stderr, "%s at %s:%d  %s\n", \
				#XXX,  __FILE__, __LINE__, db_strerror(rv)); \
		}  \
		exit(1); \
	} \
}

#define DBCALL(XXX) {  \
	int rv; \
	if ( (rv = (XXX)) != 0){ \
		if(debug){ \
			fprintf(stderr, "%s at %s:%d  %s\n", \
				#XXX,  __FILE__, __LINE__, db_strerror(rv)); \
		}  \
		exit(1); \
	} \
}
#endif /* _DB_H_ */




/******************
	NCURSES MACROS
	to use these, defint a global glogf which is an fd for your error log
******************/
#define SYSCURSE(XXX) {  \
	if ( (XXX) < 0){ \
		if(debug){ \
			fprintf(glogf, "%s at %s:%d  %s\n", \
					#XXX,  __FILE__, __LINE__, strerror(errno)); \
		}  \
		exit(1); \
	} \
}
#define NULLCURSE(XXX) {  \
	if ( (XXX) == NULL){ \
		if(debug){ \
			fprintf(glogf, "%s at %s:%d  %s\n", \
					#XXX,  __FILE__, __LINE__, strerror(errno)); \
		}  \
		exit(1); \
	} \
}


#endif  /* KENMACROS_H */

/* EOF */
