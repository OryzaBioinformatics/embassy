/* @source elipop application
**
** Wrapper for LipoP
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


/* @prog elipop ************************************************************
**
** Wrapper for LipoP
**
******************************************************************************/

int main(int argc, char **argv)
{
    AjPSeqset seqset = NULL;
    AjPStr    cl     = NULL;
    AjPSeqout seqout = NULL;
    ajint     cutoff = 0;
    AjBool    sht    = ajFalse;
    AjBool    plot   = ajFalse;
    AjBool    html   = ajFalse;
    AjBool    format = ajFalse;

    AjPStr    fn     = NULL;
    AjPStr    stmp   = NULL;
    
    AjPStr  outfname = NULL;
    
    
    embInitPV("elipop", argc, argv, "CBSTOOLS", VERSION);


    seqset  = ajAcdGetSeqset("sequence");
    outfname= ajAcdGetOutfileName("outfile");
    plot    = ajAcdGetBoolean("plot");
    sht     = ajAcdGetBoolean("short");
    html    = ajAcdGetBoolean("html");
    format  = ajAcdGetBoolean("format");
    cutoff  = ajAcdGetInt("cutoff");
    
    cl   = ajStrNewS(ajAcdGetpathC("LipoP"));
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
    
    if(html)
        ajStrAppendC(&cl," -html");

    if(format)
        ajStrAppendC(&cl," -format");
    else
        ajStrAppendC(&cl," -noformat");

    ajFmtPrintS(&stmp," -c %d",cutoff);
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
    ajSeqoutDel(&seqout);
    ajSeqsetDel(&seqset);
    ajStrDel(&outfname);

    embExit();

    return 0;
}
