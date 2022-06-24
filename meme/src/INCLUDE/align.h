/* align.h */

#ifndef ALIGN_H
#  define ALIGN_H 

#include <macros.h>
#include <user.h>
#include <logodds.h>
#include <hash_alph.h>

extern int align(
  int imotif,                           /* motif number */
  LOGODDS logodds,			/* log-odds array */
  int seqno,				/* sequence number (from 1) 
					   if <= 0, print alignment, 
					   otherwise, print .motif file */
  double threshold,			/* align sites above this */
  char *sample_name,			/* name of sample */
  char *eseq,				/* integer-coded sequence */
  BOOLEAN d[4],				/* strand directions to use */
  int lseq,				/* length of sequence */
  int w,				/* length of site */
  double *scores,			/* array to put scores in */
  FILE *outfile				/* stream for output */
);

#endif
/* $Header: /packages/emboss/MASTER/emboss/emboss/embassy/meme/src/INCLUDE/align.h,v 1.1.1.1 2001/12/24 09:05:38 ableasby Exp $ */
