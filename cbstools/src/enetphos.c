/* @source enetphos application
**
** Wrapper for netphos
**
** @author Copyright (C) Alan Bleasby
** @@
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
******************************************************************************/

#include <config.h>
#include "emboss.h"


/* @prog enetphos ************************************************************
**
** Wrapper for netphos
**
******************************************************************************/

int main(int argc, char **argv)
{
    AjPSeqset seqset = NULL;
    AjPStr    cl     = NULL;
    AjPSeqout seqout = NULL;
    float     cutoff = 0.;
    AjBool    best   = ajFalse;
    AjBool    gff    = ajFalse;
    AjBool    two    = ajFalse;
    AjBool    kinase = ajFalse;
    AjPStr    rsd    = NULL;
    AjBool    addseq = ajFalse;
    AjBool    plot   = ajFalse;

    AjPStr    fn     = NULL;
    AjPStr    stmp   = NULL;
    
    AjPStr  outfname = NULL;
    
    
    embInitPV("enetphos", argc, argv, "CBSTOOLS", VERSION);


    seqset  = ajAcdGetSeqset("sequence");
    outfname= ajAcdGetOutfileName("outfile");
    plot    = ajAcdGetBoolean("plot");
    best    = ajAcdGetBoolean("best");
    gff     = ajAcdGetBoolean("gff");
    two     = ajAcdGetBoolean("two");
    kinase  = ajAcdGetBoolean("kinase");
    addseq  = ajAcdGetBoolean("addseq");
    cutoff  = ajAcdGetFloat("cutoff");
    rsd     = ajAcdGetListSingle("residue");
    
    
    cl   = ajStrNewS(ajAcdGetpathC("netphos"));
    fn   = ajStrNew();
    stmp = ajStrNew();
    


    ajFilenameSetTempname(&fn);
    seqout = ajSeqoutNew();
    if(!ajSeqoutOpenFilename(seqout, fn))
	ajFatal("Cannot open temporary file %S",fn);
    ajSeqoutSetFormatC(seqout, "fasta");
    ajSeqoutWriteSet(seqout,seqset);
    ajSeqoutClose(seqout);


    if(ajStrMatchC(rsd,"serine"))
        ajStrAppendC(&cl," -s");
    else if(ajStrMatchC(rsd,"threonine"))
        ajStrAppendC(&cl," -t");
    else if(ajStrMatchC(rsd,"tyrosine"))
        ajStrAppendC(&cl," -y");
    
    if(plot)
        ajStrAppendC(&cl," -g");

    if(two)
        ajStrAppendC(&cl," -2");
    
    if(best)
        ajStrAppendC(&cl," -b");

    if(gff)
        ajStrAppendC(&cl," -f gff");

    if(kinase)
        ajStrAppendC(&cl," -k");
    
    if(addseq)
        ajStrAppendC(&cl," -S");

    ajFmtPrintS(&stmp," -c %f",cutoff);
    ajStrAppendS(&cl,stmp);

    ajFmtPrintS(&stmp," %S",fn);
    ajStrAppendS(&cl,stmp);

#if 0
    ajFmtPrint("%S\n",cl);
#endif

#if 1
    ajSysExecOutnameAppendS(cl, outfname);
#endif

    ajSysFileUnlinkS(fn);

    ajStrDel(&cl);
    ajStrDel(&fn);
    ajStrDel(&stmp);
    ajStrDel(&rsd);
    ajSeqoutDel(&seqout);
    ajSeqsetDel(&seqset);
    ajStrDel(&outfname);

    embExit();

    return 0;
}
