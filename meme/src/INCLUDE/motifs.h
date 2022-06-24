/* motifs.h */

#ifndef MOTIFS_H
#  define MOTIFS_H

extern int read_motifs (
  FILE *fdata,                          /* opened dataset file */
  char *filename,                       /* motif file */
  MOTIF motif[NMOTIFS],                 /* motif info */
  BOOLEAN save_dataset,                 /* return dataset in memory */
  DATASET *dataset                      /* integer-encoded dataset */
);

#endif
/* $Header: /packages/emboss/MASTER/emboss/emboss/embassy/meme/src/INCLUDE/motifs.h,v 1.1.1.1 2001/12/24 09:05:38 ableasby Exp $ */
