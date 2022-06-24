/* @source emast application
**
** EMBOSS wrapper to meme from Timothy Bailey's MEME package version 3.0.14 
** Searches sequences for motifs. 
** 
** @author Copyright (C) Jon Ison (jison@ebi.ac.uk)
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





/* @prog emast ****************************************************************
**
** EMBOSS wrapper to meme from Timothy Bailey's MEME package version 4.0.0 
** Searches sequences for motifs. 
**
******************************************************************************/

int main(int argc, char **argv)
{
    /* ACD data item variables */
    AjPFile mfile    = NULL;
    AjPFile sfile    = NULL;
    AjPDirout outdir  = NULL;
    const AjPStr  outdirname = NULL;
    AjBool  dblist   = ajFalse;
    AjPFile bfile    = NULL;
    float   ev       = 0.0;
    float   mt       = 0.0;
    AjBool  text     = ajFalse;
    AjBool  html     = ajFalse;
    AjBool  dna      = ajFalse;
    AjBool  comp     = ajFalse;
    AjBool  overwrite = ajFalse;
    AjBool  hitlist  = ajFalse;
    AjBool  best     = ajFalse;
    AjBool  remcorr  = ajFalse;
    AjBool  nostatus = ajFalse;
    ajint   c        = 0;
    AjBool  sep      = ajFalse;
    AjBool  norc     = ajFalse;
    AjBool  w        = ajFalse;
    AjBool  seqp     = ajFalse;
    AjPStr  mf       = NULL;
    AjPStr  df       = NULL;
    AjPStr  dl       = NULL;
    ajint   minseqs  = 0;
    float   mev      = 0.0;
    ajint   m        = 0;
    AjPStr  diag     = NULL;

    /* Housekeeping variables */
    AjPStr          cmd = NULL;
    ajint ret;
    
    /* ACD file processing */
    embInitPV("emast",argc,argv,"MEME",VERSION);
    mfile    = ajAcdGetInfile("mfile");
    sfile    = ajAcdGetInfile("sfile");
    outdir   = ajAcdGetOutdir("outdirname");
    overwrite  = ajAcdGetBoolean("overwrite");
    hitlist  = ajAcdGetBoolean("hitlist");
    dblist    = ajAcdGetBoolean("dblist");
    bfile    = ajAcdGetInfile("bfile");
    ev       = ajAcdGetFloat("ev");
    mt       = ajAcdGetFloat("mt");
    text     = ajAcdGetBoolean("text");
    html     = ajAcdGetBoolean("html");
    dna      = ajAcdGetBoolean("dna");
    comp     = ajAcdGetBoolean("comp");
    best     = ajAcdGetBoolean("best");
    remcorr  = ajAcdGetBoolean("remcorr");
    nostatus = ajAcdGetBoolean("nostatus");
    c        = ajAcdGetInt("c");
    sep      = ajAcdGetBoolean("sep");
    norc     = ajAcdGetBoolean("norc");
    w        = ajAcdGetBoolean("weak");
    seqp     = ajAcdGetBoolean("seqp");
    mf       = ajAcdGetString("mf");
    df       = ajAcdGetString("df");
    dl       = ajAcdGetString("dl");
    minseqs  = ajAcdGetInt("minseqs");
    mev      = ajAcdGetFloat("mev");
    m        = ajAcdGetInt("m");
    diag     = ajAcdGetString("diag");





    /* MAIN APPLICATION CODE */
    /* 1. Housekeeping */
    cmd         = ajStrNew();

    /* 2. Build emast command line */
    /* Command line is built in this order: 
       i.  Application name.
       ii. Original MAST options (in order they appear in ACD file)
       iii.Original MAST options (that don't appear in ACD file)
       iv. EMBASSY MAST new qualifiers and parameters.
    */
    ajStrAssignS(&cmd, ajAcdGetpathC("mast"));
    ajFmtPrintAppS(&cmd, " %S %S",
                   ajFileGetNameS(mfile), ajFileGetNameS(sfile));

    if(bfile)
	ajFmtPrintAppS(&cmd, " -bfile %S", ajFileGetNameS(bfile));
    if(dblist)
	ajFmtPrintAppS(&cmd, " -dblist");
    outdirname = ajDiroutGetPath(outdir);
    if(ajDiroutCreated(outdir))
        ajSysCommandRemovedirS(outdirname);

    if(overwrite)
	ajFmtPrintAppS(&cmd, " -oc %S", outdirname);
    else
	ajFmtPrintAppS(&cmd, " -o %S", outdirname);
    /* disregard hit_list if text is false */
    if(text && hitlist)
	ajFmtPrintAppS(&cmd, " -hit_list");
    if(remcorr)
	ajFmtPrintAppS(&cmd, " -remcorr");
    if(m != -1)
	ajFmtPrintAppS(&cmd, " -m %d", m);
    if(c != -1)
	ajFmtPrintAppS(&cmd, " -c %d", c);
    if(mev != -1)
	ajFmtPrintAppS(&cmd, " -mev %f", mev);
    if(MAJSTRGETLEN(diag))
	ajFmtPrintAppS(&cmd, " -diag %S", diag);
    if(norc)
	ajFmtPrintAppS(&cmd, " -norc");
    if(sep)
	ajFmtPrintAppS(&cmd, " -sep");
    if(dna)
	ajFmtPrintAppS(&cmd, " -dna");
    if(comp)
	ajFmtPrintAppS(&cmd, " -comp");
    ajFmtPrintAppS(&cmd, " -ev %f", ev);
    ajFmtPrintAppS(&cmd, " -mt %f", mt);
    if(w)
	ajFmtPrintAppS(&cmd, " -w");
    if(best)
	ajFmtPrintAppS(&cmd, " -best");
    if(seqp)
	ajFmtPrintAppS(&cmd, " -seqp");
    if(MAJSTRGETLEN(mf))
	ajFmtPrintAppS(&cmd, " -mf %S", mf);
    if(MAJSTRGETLEN(df))
	ajFmtPrintAppS(&cmd, " -df %S", df);
    if(MAJSTRGETLEN(dl))
	ajFmtPrintAppS(&cmd, " -dl %S", dl);
    if(minseqs != -1)
	ajFmtPrintAppS(&cmd, " -minseqs %d", minseqs);
    if(nostatus)
	ajFmtPrintAppS(&cmd, " -nostatus");
    if(!text)
	ajFmtPrintAppS(&cmd, " -notext");
    if(!html)
	ajFmtPrintAppS(&cmd, " -nohtml");

    /* 3. Close files from ACD before calling mast */
    ajFileClose(&mfile);
    ajFileClose(&sfile);
    ajFileClose(&bfile);


    /* 4. Call mast */
    ajDebug("%S\n", cmd);
    ret = system(ajStrGetPtr(cmd));    
    if(ret == -1)
        ajFatal("Error (%d) from mast program. Aborting.", ret);

    

    /* 5. Exit cleanly */

    /* Free housekeeping variables */

    ajStrDel(&cmd);

    ajDiroutDel(&outdir);

    ajStrDel(&mf);
    ajStrDel(&df);
    ajStrDel(&dl);
    ajStrDel(&diag);

    embExit();

    return 0;
}
