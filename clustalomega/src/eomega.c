/* @source eomega application
**
** Wrapper for clustalo
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


typedef enum {TYPE_BOOL, TYPE_TOGGLE, TYPE_INT, TYPE_FLOAT, TYPE_STR,
              TYPE_LIST, TYPE_LISTSINGLE, TYPE_LISTSPECIAL,
              TYPE_INFILE, TYPE_OUTFILE, TYPE_DIR,
              TYPE_UNKNOWN
} OmegaQualType;
    

/* @datastatic OmegaQual *******************************************************
**
** OMEGA qualifier names and their prefixes
**
** @alias OmegaSPrefix
** @alias OmegaOPrefix
**
** @attr ename [const char*] emboss qualifier name
** @attr qname [const char*] omega qualifier name
** @attr prefix [const char*] omega prefix
** @attr type [OmegaQualType] qualifier type
** @@
******************************************************************************/

typedef struct OmegaSQual
{
    const char* ename;
    const char* qname;
    const char* prefix;
    OmegaQualType type;
} OmegaOQual;
#define OmegaPQual OmegaOQual*




static OmegaOQual omegaqualsgeneral[] =
{
    {"indistfile",   "distmat-in",      "",    TYPE_INFILE},
    {"inguidefile",  "guidetree-in",    "",    TYPE_INFILE},
    {"outdistfile",  "distmat-out",     "",    TYPE_OUTFILE},
    {"outguidefile", "guidetree-out",   "",    TYPE_OUTFILE},
    {NULL, NULL, NULL, 0}
};

static OmegaOQual omegaqualsadv[] =
{
    {"dealign",        "dealign",       "",  TYPE_TOGGLE},
    {"cluster",        "full",          "",  TYPE_LISTSPECIAL},
    {"maxiterations",  "iterations",    "",  TYPE_INT},
    {"maxgiterations", "max-guidetree-iterations", "",  TYPE_INT},
    {"maxhiterations", "max-hmm-iterations",       "",  TYPE_INT},
    {"maxseqs",        "maxnumseqs",    "",  TYPE_INT},
    {"maxlenseq",      "maxseqlen",     "",  TYPE_INT},
    {"self",           "auto",          "",  TYPE_TOGGLE},
    {"log",            "verbose",       "",  TYPE_TOGGLE},
    {"outformat",      "outfmt",        "",  TYPE_LISTSPECIAL},
    {NULL, NULL, NULL, 0}
};





/* @datastatic OmegaQuals **************************************************
**
** OMEGA qualifier names and their types
**
** @alias OmegaSQuals
** @alias OmegaOQuals
**
** @attr qname [const char*] emboss qualifier name
** @attr mname [const char*] omega qualifier name
** @attr prefix [const char*] omega prefix
** @@
******************************************************************************/

typedef struct OmegaSQuals
{
    const char* qname;
    const char* mname;
    const char* prefix;
} OmegaOQuals;
#define OmegaPQuals OmegaOQuals*

static void eomega_doquals(AjPStr *cl, OmegaOQual[]);

static void eomega_dobool(AjPStr *cl, OmegaOQual qual);
static void eomega_dotoggle(AjPStr *cl, OmegaOQual qual);
static void eomega_dostring(AjPStr *cl, OmegaOQual qual);
static void eomega_doinfile(AjPStr *cl, OmegaOQual qual);
static void eomega_dooutfile(AjPStr *cl, OmegaOQual qual);
static void eomega_dodirectory(AjPStr *cl, OmegaOQual qual);
static void eomega_dolistsingle(AjPStr *cl, OmegaOQual qual);
static void eomega_dolistspecial(AjPStr *cl, OmegaOQual qual);
static void eomega_dolist(AjPStr *cl, OmegaOQual qual);
static void eomega_dointeger(AjPStr *cl, OmegaOQual qual);
static void eomega_dofloat(AjPStr *cl, OmegaOQual qual);




/* @datastatic OmegaProcess ****************************************************
**
** OMEGA qualifier names and their prefixes
**
** @alias OmegaSPrefix
** @alias OmegaOPrefix
**
** @attr type [OmegaQualType] qualifier type
** @attr Process [(void*)] Processing function
** @@
******************************************************************************/

typedef struct OmegaSProcess
{
    OmegaQualType type;
    void (*Process) (AjPStr *cl, OmegaOQual qual);
} OmegaOProcess;
#define OmegaPProcess OmegaOProcess*

static OmegaOProcess omegaprocess[] =
{
    {TYPE_BOOL, eomega_dobool},
    {TYPE_TOGGLE, eomega_dotoggle},
    {TYPE_INT, eomega_dointeger},
    {TYPE_FLOAT, eomega_dofloat},
    {TYPE_STR, eomega_dostring},
    {TYPE_LIST, eomega_dolist},
    {TYPE_LISTSINGLE, eomega_dolistsingle},
    {TYPE_LISTSPECIAL, eomega_dolistspecial},
    {TYPE_INFILE, eomega_doinfile},
    {TYPE_OUTFILE, eomega_dooutfile},
    {TYPE_DIR, eomega_dodirectory},

};



/* @prog eomega ***************************************************************
**
** Wrapper for clustalo
**
******************************************************************************/

int main(int argc, char **argv)
{
    AjPSeqset seqs   = NULL;
    AjPSeqout outseq = NULL;
    AjPSeqout seq_file = NULL;
    
    AjPStr cl = NULL;
    AjPStr tmpfile = NULL;
    AjPStr outname = NULL;
    
    embInitPV("eomega", argc, argv, "CLUSTALOMEGA",VERSION);

    tmpfile = ajStrNew();
    outname = ajStrNew();
    
    cl = ajStrNewS(ajAcdGetpathC("clustalo"));

    seqs   = ajAcdGetSeqset("sequences");
    outseq = ajAcdGetSeqoutset("outseq");

    /*
    ** Start by writing sequences into a unique temporary file
    ** get file pointer to unique file
    */


    seq_file = ajSeqoutNew();
    ajFmtPrintS(&tmpfile,"clustalotmp%d",getpid());

    if(!ajSeqoutOpenFilename(seq_file,tmpfile))
	embExitBad();

    /* Set output format to fasta */
    ajSeqoutSetFormatC(seq_file, "fasta");
    ajSeqoutWriteSet(seq_file, seqs);
    ajSeqoutClose(seq_file);
    ajSeqoutDel(&seq_file);

    ajStrAssignS(&outname,ajSeqoutGetFilename(outseq));
    ajSeqoutCloseEmpty(outseq);
    ajSeqoutDel(&outseq);
    ajSysFileUnlinkS(outname);    
    
    ajFmtPrintAppS(&cl," --infile=%S",tmpfile);
    ajFmtPrintAppS(&cl," --outfile=%S",outname);

    eomega_doquals(&cl, omegaqualsgeneral);
    eomega_doquals(&cl, omegaqualsadv);

    ajDebug("Constructed command line: %S\n",cl);

#if 0
    ajFmtPrint("%S\n",cl);
#endif

#if 1
    ajSysExecS(cl);
#endif

#if 1
    ajSysFileUnlinkS(tmpfile);
#endif

    ajStrDel(&cl);
    ajStrDel(&tmpfile);
    ajStrDel(&outname);

    ajSeqsetDel(&seqs);

    embExit();

    return 0;
}




/* @funcstatic eomega_doquals *********************************************
**
** Get ACD bools.
** Only add to command line if value different from the default
**
** @param [w] cl [AjPStr*] command line
** @param [r] table [AjPTable] table relating qualifiers to prefixes
**
** @return [void]
** @@
******************************************************************************/

static void eomega_doquals(AjPStr *cl, OmegaOQual omegaquals[])
{
    ajuint i;
    
    i = 0;

    while(omegaquals[i].ename)
    {
        omegaprocess[omegaquals[i].type].Process(cl, omegaquals[i]);
	++i;
    }

    return;
}




/* @funcstatic eomega_dobool *********************************************
**
** Get ACD bools.
** Only add to command line if value different from the default
**
** @param [w] cl [AjPStr*] command line
** @param [r] i [ajuint] Qualifier index
**
** @return [void]
** @@
******************************************************************************/

static void eomega_dobool(AjPStr *cl, OmegaOQual qual)
{
    AjBool bqual = ajFalse;

    bqual = ajAcdGetBoolean(qual.ename);

    if(ajAcdIsUserdefinedC(qual.ename))
    {
        ajFmtPrintAppS(cl," --%s%s=",
                       qual.prefix,
                       qual.qname);
        if(bqual)
            ajFmtPrintAppS(cl,"yes");
        else
            ajFmtPrintAppS(cl,"no");
    }

    return;
}




/* @funcstatic eomega_dotoggle *********************************************
**
** Get ACD toggle.
** Only add to command line if value different from the default
** use 'no' prefix for false, unlike boolean qualifiers.
**
** @param [w] cl [AjPStr*] command line
** @param [r] i [ajuint] Qualifier index
**
** @return [void]
** @@
******************************************************************************/

static void eomega_dotoggle(AjPStr *cl, OmegaOQual qual)
{
    AjBool bqual = ajFalse;

    bqual = ajAcdGetToggle(qual.ename);

    if(ajAcdIsUserdefinedC(qual.ename))
    {
        if(bqual)
            ajFmtPrintAppS(cl," --%s%s",
                           qual.prefix,
                           qual.qname);
    }

    return;
}




/* @funcstatic eomega_dostring *********************************************
**
** Get ACD strings.
** Only add to command line if value different from the default
**
** @param [w] cl [AjPStr*] command line
**
** @return [void]
** @@
******************************************************************************/

static void eomega_dostring(AjPStr *cl, OmegaOQual qual)
{
    AjPStr squal = NULL;
    
    squal = ajAcdGetString(qual.ename);

    if(ajAcdIsUserdefinedC(qual.ename))
    {
        ajFmtPrintAppS(cl," -%s%s=%S",
                       qual.prefix,
                       qual.qname,
                       squal);
    }

    ajStrDel(&squal);

    return;
}




/* @funcstatic eomega_doinfile ************************************************
**
** Get ACD input files.
** Only add to command line if value different from the default
**
** @param [w] cl [AjPStr*] command line
**
** @return [void]
** @@
******************************************************************************/

static void eomega_doinfile(AjPStr *cl, OmegaOQual qual)
{
    AjPStr squal = NULL;
    AjPFile infile = NULL;
    
    infile = ajAcdGetInfile(qual.ename);

    if(ajAcdIsUserdefinedC(qual.ename))
    {

        if(infile)
        {
            squal = ajStrNewS(ajFileGetNameS(infile));

            ajFmtPrintAppS(cl," --%s%s=%S",
                           qual.prefix,
                           qual.qname,
                           squal);
            ajStrDel(&squal);
        }
    }
    
    ajFileClose(&infile);

    return;
}




/* @funcstatic eomega_dooutfile ************************************************
**
** Get ACD output files.
** Only add to command line if value different from the default
**
** @param [w] cl [AjPStr*] command line
**
** @return [void]
** @@
******************************************************************************/

static void eomega_dooutfile(AjPStr *cl, OmegaOQual qual)
{
    AjPStr squal = NULL;
    AjPFile outfile = NULL;
    
    outfile = ajAcdGetOutfile(qual.ename);

    if(outfile)
    {
        squal = ajStrNewS(ajFileGetNameS(outfile));
        
        ajFmtPrintAppS(cl," --%s%s=%S",
                       qual.prefix,
                       qual.qname,
                       squal);
        ajFileClose(&outfile);

        ajSysFileUnlinkS(squal);
        ajStrDel(&squal);
    }

    return;
}




/* @funcstatic eomega_dodirectory **********************************************
**
** Get ACD directories.
** Only add to command line if value different from the default
**
** @param [w] cl [AjPStr*] command line
**
** @return [void]
** @@
******************************************************************************/

static void eomega_dodirectory(AjPStr *cl, OmegaOQual qual)
{
    AjPStr squal = NULL;
    
    squal = ajAcdGetDirectoryName(qual.ename);

    if(ajAcdIsUserdefinedC(qual.ename))
    {
        ajFmtPrintAppS(cl," -%s%s=%S",
                       qual.prefix,
                       qual.qname,
                       squal);
    }

    ajStrDel(&squal);

    return;
}




/* @funcstatic eomega_dolistsingle ********************************************
**
** Get ACD single value list entries.
** Only add to command line if value different from the default
**
** @param [w] cl [AjPStr*] command line
**
** @return [void]
** @@
******************************************************************************/

static void eomega_dolistsingle(AjPStr *cl, OmegaOQual qual)
{
    AjPStr squal = NULL;

    squal = ajAcdGetListSingle(qual.ename);

    if(ajAcdIsUserdefinedC(qual.ename))
    {
        ajFmtPrintAppS(cl," -%s%s=%S",
                       qual.prefix,
                       qual.qname,
                       squal);
    }
    
    ajStrDel(&squal);

    return;
}




/* @funcstatic eomega_dolistspecial *******************************************
**
** Get ACD special value list entries, tailored to convert to native options
** Only add to command line if value different from the default
**
** @param [w] cl [AjPStr*] command line
**
** @return [void]
** @@
******************************************************************************/

static void eomega_dolistspecial(AjPStr *cl, OmegaOQual qual)
{
    AjPStr squal = NULL;

    squal = ajAcdGetListSingle(qual.ename);

    if(ajAcdIsUserdefinedC(qual.ename))
    {
        if(ajCharMatchC(qual.ename, "cluster"))
        {
            if(ajStrMatchC(squal, "full"))
                ajFmtPrintAppS(cl," --full");
            else if(ajStrMatchC(squal, "iter"))
                ajFmtPrintAppS(cl," --full-iter");
            else if(!ajStrMatchC(squal, "mbed"))
                ajDie("Unknown '-%s' value '%S'", qual.ename, squal);
        }
        else if(ajCharMatchC(qual.ename, "outformat"))
        {
            ajFmtPrintAppS(cl," --outfmt=%S", squal);
        }
        else
            ajDie("Unknown special list option'-%s'", qual.ename);
    }

    ajStrDel(&squal);

    return;
}




/* @funcstatic eomega_dolist **************************************************
**
** Get ACD single value list entries.
** Only add to command line if value different from the default
**
** @param [w] cl [AjPStr*] command line
**
** @return [void]
** @@
******************************************************************************/

static void eomega_dolist(AjPStr *cl, OmegaOQual qual)
{
    AjPStr *squal = NULL;
    ajuint j;

    squal = ajAcdGetList(qual.ename);

    if(ajAcdIsUserdefinedC(qual.ename))
    {

        ajFmtPrintAppS(cl," -%s%s=",
                       qual.prefix,
                       qual.qname,
                       squal);
        j = 0;
        while(squal[j]) 
        {
            if(j)
                ajFmtPrintAppS(cl,",");
            ajFmtPrintAppS(cl,"%S",
                           squal[j++]);
        }
    }
    
    ajStrDelarray(&squal);

    return;
}




/* @funcstatic eomega_dointeger ********************************************
**
** Get ACD integers.
** Only add to command line if value different from the default
**
** @param [w] cl [AjPStr*] command line
** @param [r] table [AjPTable] table relating qualifiers to prefixes
**
** @return [void]
** @@
******************************************************************************/

static void eomega_dointeger(AjPStr *cl, OmegaOQual qual)
{
    ajint  iqual;

    iqual = ajAcdGetInt(qual.ename);

    if(ajAcdIsUserdefinedC(qual.ename))
    {
        ajFmtPrintAppS(cl," --%s%s=%d",
                       qual.prefix,
                       qual.qname,
                       iqual);
    }
    
    return;
}



/* @funcstatic eomega_dofloat ********************************************
**
** Get ACD integers.
** Only add to command line if value different from the default
**
** @param [w] cl [AjPStr*] command line
** @param [r] table [AjPTable] table relating qualifiers to prefixes
**
** @return [void]
** @@
******************************************************************************/

static void eomega_dofloat(AjPStr *cl, OmegaOQual qual)
{
    ajint  fqual;

    fqual = ajAcdGetFloat(qual.ename);

    if(ajAcdIsUserdefinedC(qual.ename))
    {
        ajFmtPrintAppS(cl," -%s%s=%f",
                       qual.prefix,
                       qual.qname,
                       fqual);
    }
    
    return;
}
