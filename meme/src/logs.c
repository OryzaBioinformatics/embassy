/***********************************************************************
*                                                                      *
*       MEME++                                                         *
*       Copyright 1994, The Regents of the University of California    *
*       Author: Timothy L. Bailey                                      *
*                                                                      *
***********************************************************************/
/* logs.c */

#include "meme.h"
#include "logs.h"

/**********************************************************************/
/*
	init_log

	Setup lookup table for log(x), 0 < x <= 1
*/
/**********************************************************************/
extern void init_log(void)
{
  int i;
  double x;

  for (i=0; i<=log_precision; i++) {
    x = (double) i/log_precision;
    log_table[i] = LOG(x);
    /*printf("%d %f\n", i, log_table[i]);*/
  }
  log_table[i] = 0;		/* for use in iterpolation when x=1 */
}
/* $Header: /packages/emboss/MASTER/emboss/emboss/embassy/meme/src/logs.c,v 1.1.1.1 2001/12/24 09:05:38 ableasby Exp $ */
