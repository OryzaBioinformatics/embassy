#include "libc.h"
#include "types.h"
#include "misc.h"
#include "args.h"
#include "seq.h"
#include "dna.h"
#include "discrim.h"

#ifndef __lint
static const char rcsid[] =
"$Id: discrim.c,v 1.2 2004/08/05 16:02:32 rice Exp $";
#endif

/* DNA characters */
const sim4_uchar dchars[] = "ABCDGHKMNRSTVWXY";

static int is_dchar(int ch);

bool is_DNA(sim4_uchar *s, int len)
{
	int ACGT, i;

	for (ACGT = i = 0; i < len; ++i)
		if (strchr("ACGTNXacgtnx", s[i]))
			++ACGT;
	if (10*ACGT < 9*len) /* ACGT < 90% of len */
		return 0;
	
	for (i = 0; i < len; ++i)
		if (!is_dchar(s[i])) {
			fatalf("Illegal character '%c' in sequence file.\n", s[i]);
			exit(1);
		}
	return 1;
}

static int is_dchar(int ch)
{ 
        return !!strchr((const char*)dchars, toupper(ch));
}

