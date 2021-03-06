/* hash_alph.h */

#ifndef HASH_ALPH_H
#  define HASH_ALPH_H

/*
  alphabets
*/
#define MAXASCII 256				/* number of ascii characters */
#define DNA0 "ACGT"				/* minimal DNA alphabet */
#define DNAB "ABCDGHKMNRSTUVWY*-" 		/* BLAST DNA alphabet */
#define PROTEIN0 "ACDEFGHIKLMNPQRSTVWY"		/* minimal protein alphabet */
#define PROTEINB "ABCDEFGHIKLMNPQRSTUVWXYZ*-"	/* BLAST PROTEIN alphabet */

/*
  alphabet substitutions 
*/
#ifndef DEFINE_GLOBALS
  extern char *dna_subst[];
  extern char *prot_subst[];
#else
  char *dna_subst[] = {
    "A",	/* A */
    "GTC",	/* B */
    "C",	/* C */
    "AGT",	/* D */
    "G",	/* G */
    "ATC",	/* H */
    "GT",	/* K */
    "AC",	/* M */
    DNA0,	/* N */
    "AG",	/* R */
    "GC",	/* S */
    "T",	/* T */
    "T",	/* U */
    "AGC",	/* V */
    "AT",	/* W */
    "TC",	/* Y */
    DNA0,	/* * */
    DNA0	/* - */
  };
  char *prot_subst[] = {
    "A",	/* A */
    "DN",	/* B */
    "C",	/* C */
    "D",	/* D */
    "E",	/* E */
    "F",	/* F */
    "G",	/* G */
    "H",	/* H */
    "I",	/* I */
    "K",	/* K */
    "L",	/* L */
    "M",	/* M */
    "N",	/* N */
    "P",	/* P */
    "Q",	/* Q */
    "R",	/* R */
    "S",	/* S */
    "T",	/* T */
    PROTEIN0,	/* U */
    "V",	/* V */
    "W",	/* W */
    PROTEIN0,	/* X */
    "Y",	/* Y */
    "EQ",	/* Z */
    PROTEIN0,	/* * */
    PROTEIN0	/* - */
  };
#endif

/*
  length of DNAB alphabet
*/
EXTERN int dnablen;

/*
  alphabet frequencies
*/
#ifndef DEFINE_GLOBALS
  extern double ntfreq[]; 
  extern double nrfreq[]; 
  extern double frame0[]; 
#else
  /* BLAST DNA alphabet: frequencies for NT 9/22/96 */
  double ntfreq[] = { 
    0.281475655 /* A */,
    0.000000649 /* B */,
    0.221785822 /* C */,
    0.000001389 /* D */,
    0.228634607 /* G */,
    0.000001612 /* H */,
    0.000006323 /* K */,
    0.000005848 /* M */,
    0.000991686 /* N */,
    0.000015904 /* R */,
    0.000009514 /* S */,
    0.267048106 /* T */,
    0.0         /* U; doesn't occur in NT */,
    0.000000968 /* V */,
    0.000005846 /* W */,
    0.000016070 /* Y */,
    0.0		/* *; doesn't occur in NT */,
    0.0		/* -; doesn't occur in NT */
  };
  /* BLAST protein alphabet: frequencies for NR 9/22/96 */
  double nrfreq[] = { 
    0.073091885 /* A */,
    0.000021047 /* B */,
    0.018145453 /* C */,
    0.051687956 /* D */,
    0.062278511 /* E */,
    0.040243411 /* F */,
    0.069259642 /* G */,
    0.022405456 /* H */,
    0.056227000 /* I */,
    0.058435042 /* K */,
    0.091621836 /* L */,
    0.023044274 /* M */,
    0.046032137 /* N */,
    0.050623807 /* P */,
    0.040715284 /* Q */,
    0.051846246 /* R */,
    0.073729031 /* S */,
    0.059352333 /* T */,
    0.000000110 /* U */,
    0.064298546 /* V */,
    0.013328158 /* W */,
    0.000941980 /* X */,
    0.032649745 /* Y */,
    0.000021111 /* Z */,
    0.0		/* *; doesn't occur in NR */,
    0.0		/* -; doesn't occur in NR */
  };
  /* Tranlated DNAB to PROTEINB */
  /* shuffle_db -nostop < est > tmp1; getsize -f -x tmp1
     est is the 11/11/97 EST database from ncbi
     Frequencies for U, *, - added by hand.
  */
  double frame0[] = {
   0.06995463 /* A */,
   0.00000228 /* B */,
   0.02398853 /* C */,
   0.03950999 /* D */,
   0.05199564 /* E */,
   0.03955549 /* F */,
   0.07363406 /* G */,
   0.02911862 /* H */,
   0.04092760 /* I */,
   0.05187635 /* K */,
   0.09566732 /* L */,
   0.01747826 /* M */,
   0.03216447 /* N */,
   0.06506016 /* P */,
   0.04180057 /* Q */,
   0.06748940 /* R */,
   0.08353402 /* S */,
   0.05231376 /* T */,
   0.0        /* U */,
   0.05927076 /* V */,
   0.01585647 /* W */,
   0.02720962 /* X */,
   0.02158787 /* Y */,
   0.00000413 /* Z */,
   0.0	      /* *; doesn't occur in NT */, 
   0.0	      /* -; doesn't occur in NT */
  };
#endif

/*
  alphabet hashing functions

    setalph: 	set current alphabet to DNAB or PROTEINB
		  0 -> DNAB alphabet
		  1 -> PROTEINB alphabet
		  2 -> user specified alphabet
    hash: 	character to position in current alphabet
    unhash:	position in alphabet to alphabet character
    dnabhash: 	DNAB character to position in alphabet
    protbhash: 	PROTEINB character to position in alphabet
    chash:	hash contents of pointer (letter or codon) to position 
		in current alphabet:
		  !X -> hash single letter (DNAB or PROTEINB)
		  X -> hash DNAB codon to PROTEINB
    dhash:	hash two letters to single (double-letter) index
		  L1, L2- letters in alphabet
		  A 	- length of alphabet
*/
#define setalph(I) hindex = (I==0 ? dnabindex : (I==1 ? protbindex : cpindex))
#define hash(L) hindex[(int)(L)]
#define unhash(I) pcindex[(int)(I)]
#define dnabhash(L) dnabindex[(int)(L)]
#define protbhash(L) protbindex[(int)(L)]
#define chash(X, L) ( X ? hash_dnab2protb(*(L), *(L+1), *(L+2)) : hash(*(L)) )
#define dhash(L1, L2, A)  ((L2) + ((A)+1) * L1)
EXTERN int *hindex;   		/* character vs. position in alphabet */
EXTERN int cpindex[MAXASCII];	/* index: character vs. position in alphabet */
EXTERN char pcindex[MAXASCII];	/* index: position in alphabet vs. character */
EXTERN int dnabindex[MAXASCII];	/* DNAB character vs. position in alphabet */
EXTERN int protbindex[MAXASCII];/* PROTEINB character vs. pos. in alphabet */

/*
  converting to DNA complement
    comp_dnab:	convert from DNAB letter to complementary DNAB letter
*/
/*                          ABCDGHKMNRSTUVWY */
DEXTERN(char *, dnab_comp, "TVGHCDMKNYSAABWR");
#define comp_dnab(X) dnab_comp[dnabhash(X)];

/*
  Converting DNA to protein
*/
/*
  genetic code
  Order is UUU, UUC, UUA, UUG, UCU, UCC, ..., GGG.  
  U=T, so T=0, C=1, A=2, G=3.
*/
DEXTERN(char *, gene_code,
  "FFLLSSSSYY**CC*WLLLLPPPPHHQQRRRRIIIMTTTTNNKKSSRRVVVVAAAADDEEGGGG");
#ifdef DEFINE_GLOBALS
/* lookup table from letter to order in "TCAG"; other letters get 0 */
/*                            A B C D E F G */
  int tcag_index[MAXASCII] = {2,0,1,0,0,0,3};	/* TCAG index */
/* lookup table from DNAB codon to PROTEINB position */
  int *dnab2protb_index;
#else
  extern int tcag_index[MAXASCII];
  extern int *dnab2protb_index;
#endif
/* convert nucleic acid to index in alphabet "TCAG" */
#define tcag_hash(R) (tcag_index[(R) - 'A'])
/* convert codon (DNA0 alphabet) to protein letter (PROTEIN0 alphabet) */
#define dna0_to_prot0(R1, R2, R3) \
  gene_code[tcag_hash(R3)+(4*(tcag_hash(R2)+4*tcag_hash(R1)))]
/* hash codon (DNAB alphabet) to protein letter position (PROTEINB alphabet) */
#define hash_dnab2protb(R1, R2, R3) \
  dnab2protb_index[dnabhash(R3)+\
    ((dnablen)*(dnabhash(R2)+((dnablen)*dnabhash(R1))))]

/* set things up for hashing */
extern int setup_hash_alph(
  char *alphabet
);

/* unhash an integer-coded sequence */
extern void r2seq(
  char *seq,
  char *res,
  int len
);

extern char *get_alphabet(
  char *old_alph, 		/* old alphabet */
  int *p[MAXASCII]		/* permutation and substitution matrix */
);

extern void setup_hash_dnab2protb();

extern void invcomp_dna(
  char *sequence,                       /* DNAB sequence */
  int length                            /* length of sequence */
);

extern int *dhash_it(
  BOOLEAN xlate_dna,			/* database is DNA and motifs protein */
  int alen,				/* length of alphabet */
  char *sequence,			/* sequence of sample */
  int length 				/* length of sequence */
);

#endif

/* $Header: /home/repository/emboss/emboss/emboss/embassy/meme/src/INCLUDE/hash_alph.h,v 1.1 2000/11/05 21:47:55 ajb Exp $ */
