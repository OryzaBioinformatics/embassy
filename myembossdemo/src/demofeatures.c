#include "emboss.h"




/* @prog demofeatures *********************************************************
**
** Testing
**
******************************************************************************/

int main (int argc, char **argv)
{
    AjPFeattable feattable;
    AjPStr name   = NULL;
    AjPStr source = NULL;
    AjPStr type   = NULL;
    char strand   = '+';
    ajint frame   = 0;
    AjBool sortbytype;
    AjBool sortbystart;
    AjPFeattabOut output = NULL;
    ajint i;
    float score = 0.0;

    embInitPV("demofeatures", argc, argv, "myembossdemo", VERSION);

    output      = ajAcdGetFeatout("outfeat");
    sortbytype  = ajAcdGetBoolean("typesort");
    sortbystart = ajAcdGetBoolean("startsort");

    ajStrAssignC(&name,"seq1");

    feattable = ajFeattableNew(name);

    ajStrAssignC(&source,"demofeature");
    score = 1.0;

    for(i=1;i<11;i++)
    {
	if(i & 1)
	    ajStrAssignC(&type,"CDS");
	else
	    ajStrAssignC(&type,"misc_feature");

	ajFeatNew(feattable, source, type, i, i+10, score, strand,
                  frame) ;
    }


    if(sortbytype)
	ajFeatSortByType(feattable);

    if(sortbystart)
	ajFeatSortByStart(feattable);

    ajFeattableWrite(output, feattable);

    ajStrDel(&source);
    ajStrDel(&name);
    ajStrDel(&type);
    ajFeattableDel(&feattable);
    ajFeattabOutDel(&output);

    embExit();

    return 0;
}
