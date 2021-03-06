/* @source etmhmm application
**
** Wrapper for TMHMM
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


/* @prog etmhmm ************************************************************
**
** Wrapper for TMHMM
**
******************************************************************************/

int main(int argc, char **argv)
{
    AjPSeqset seqset = NULL;
    AjPStr    cl     = NULL;
    AjPSeqout seqout = NULL;
    AjBool    plot   = ajFalse;
    AjBool    html   = ajFalse;
    AjBool    sht    = ajFalse;
    AjBool    one    = ajFalse;
    AjPStr    fn     = NULL;
    AjPStr    stmp   = NULL;
    
    AjPStr  outfname = NULL;
    
    
    embInitPV("etmhmm", argc, argv, "CBSTOOLS", VERSION);


    seqset  = ajAcdGetSeqset("sequence");
    outfname= ajAcdGetOutfileName("outfile");
    plot    = ajAcdGetBoolean("plot");
    html    = ajAcdGetBoolean("html");
    sht     = ajAcdGetBoolean("short");
    one     = ajAcdGetBoolean("one");
    
    cl   = ajStrNewS(ajAcdGetpathC("tmhmm"));
    fn   = ajStrNew();
    stmp = ajStrNew();



    ajFilenameSetTempname(&fn);
    seqout = ajSeqoutNew();
    if(!ajSeqoutOpenFilename(seqout, fn))
	ajFatal("Cannot open temporary file %S",fn);
    ajSeqoutSetFormatC(seqout, "fasta");
    ajSeqoutWriteSet(seqout,seqset);
    ajSeqoutClose(seqout);

    if(plot)
        ajStrAppendC(&cl," -plot");
    else
        ajStrAppendC(&cl," -noplot");        

    if(sht)
        ajStrAppendC(&cl," -short");
    else
        ajStrAppendC(&cl," -noshort");        

    if(one)
        ajStrAppendC(&cl," -v1");
    else
        ajStrAppendC(&cl," -nov1");        

    if(html)
        ajStrAppendC(&cl," -html");
    else
        ajStrAppendC(&cl," -nohtml");        

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
    ajSeqoutDel(&seqout);
    ajSeqsetDel(&seqset);
    ajStrDel(&outfname);

    embExit();

    return 0;
}
