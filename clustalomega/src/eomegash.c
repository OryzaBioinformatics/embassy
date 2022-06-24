/* @source eomegash application
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


typedef enum {TYPE_BOOL, TYPE_TOGGLE, TYPE_INT, TYPE_FLOAT, TYPE_STR, TYPE_LIST,
              TYPE_LISTSINGLE, TYPE_INFILE, TYPE_OUTFILE, TYPE_DIR,
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
    {"hmm",          "hmm-in",          "",    TYPE_INFILE},
    {"indist",       "distmat-in",      "",    TYPE_INFILE},
    {"inguide",      "guidetree-in",    "",    TYPE_INFILE},
    {"outdist",      "distmat-out",     "",    TYPE_OUTFILE},
    {"outguide",     "guidetree-out",   "",    TYPE_OUTFILE},
    {NULL, NULL, NULL, 0}
};

static OmegaOQual omegaqualsadv[] =
{
    {"dealign",        "dealign",       "",  TYPE_TOGGLE},
    {"mbed",           "mbed",          "",  TYPE_TOGGLE},
    {"intermbed",      "mbediter",      "",  TYPE_TOGGLE},
    {"maxiterations",  "iterations",    "",  TYPE_INT},
    {"maxgiterations", "max-guidetree-iterations", "",  TYPE_INT},
    {"maxhiterations", "max-hmm-iterations",       "",  TYPE_INT},
    {"maxseqs",        "maxnumseqs",    "",  TYPE_INT},
    {"maxlenseq",      "maxseqlen",     "",  TYPE_INT},
    {"self",           "auto",          "",  TYPE_TOGGLE},
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

static void eomegash_doquals(AjPStr *cl, OmegaOQual[]);

static void eomegash_dobool(AjPStr *cl, OmegaOQual qual);
static void eomegash_dotoggle(AjPStr *cl, OmegaOQual qual);
static void eomegash_dostring(AjPStr *cl, OmegaOQual qual);
static void eomegash_doinfile(AjPStr *cl, OmegaOQual qual);
static void eomegash_dooutfile(AjPStr *cl, OmegaOQual qual);
static void eomegash_dodirectory(AjPStr *cl, OmegaOQual qual);
static void eomegash_dolistsingle(AjPStr *cl, OmegaOQual qual);
static void eomegash_dolist(AjPStr *cl, OmegaOQual qual);
static void eomegash_dointeger(AjPStr *cl, OmegaOQual qual);
static void eomegash_dofloat(AjPStr *cl, OmegaOQual qual);




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
    {TYPE_BOOL, eomegash_dobool},
    {TYPE_TOGGLE, eomegash_dotoggle},
    {TYPE_INT, eomegash_dointeger},
    {TYPE_FLOAT, eomegash_dofloat},
    {TYPE_STR, eomegash_dostring},
    {TYPE_LIST, eomegash_dolist},
    {TYPE_LISTSINGLE, eomegash_dolistsingle},
    {TYPE_INFILE, eomegash_doinfile},
    {TYPE_OUTFILE, eomegash_dooutfile},
    {TYPE_DIR, eomegash_dodirectory},

};



/* @prog eomegash *************************************************************
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
    
    embInitPV("eomegash", argc, argv, "CLUSTALOMEGA",VERSION);

    tmpfile = ajStrNew();
    outname = ajStrNew();
    
    cl = ajStrNewS(ajAcdGetpathC("clustalo"));

    seqs   = ajAcdGetSeqset("sequences");

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
    
    ajFmtPrintAppS(&cl," --infile=%S",tmpfile);

    eomegash_doquals(&cl, omegaqualsgeneral);

    outseq = ajAcdGetSeqoutset("outseq");
    ajStrAssignS(&outname,ajSeqoutGetFilename(outseq));
    ajSeqoutCloseEmpty(outseq);
    ajSeqoutDel(&outseq);
    ajSysFileUnlinkS(outname);    
    ajFmtPrintAppS(&cl," --outfile=%S",outname);

    eomegash_doquals(&cl, omegaqualsadv);

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




/* @funcstatic eomegash_doquals *********************************************
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

static void eomegash_doquals(AjPStr *cl, OmegaOQual omegaquals[])
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




/* @funcstatic eomegash_dobool *********************************************
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

static void eomegash_dobool(AjPStr *cl, OmegaOQual qual)
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




/* @funcstatic eomegash_dotoggle *********************************************
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

static void eomegash_dotoggle(AjPStr *cl, OmegaOQual qual)
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




/* @funcstatic eomegash_dostring *********************************************
**
** Get ACD strings.
** Only add to command line if value different from the default
**
** @param [w] cl [AjPStr*] command line
**
** @return [void]
** @@
******************************************************************************/

static void eomegash_dostring(AjPStr *cl, OmegaOQual qual)
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




/* @funcstatic eomegash_doinfile ************************************************
**
** Get ACD input files.
** Only add to command line if value different from the default
**
** @param [w] cl [AjPStr*] command line
**
** @return [void]
** @@
******************************************************************************/

static void eomegash_doinfile(AjPStr *cl, OmegaOQual qual)
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




/* @funcstatic eomegash_dooutfile ************************************************
**
** Get ACD input files.
** Only add to command line if value different from the default
**
** @param [w] cl [AjPStr*] command line
**
** @return [void]
** @@
******************************************************************************/

static void eomegash_dooutfile(AjPStr *cl, OmegaOQual qual)
{
    AjPStr squal = NULL;
    AjPFile outfile = NULL;
    
    outfile = ajAcdGetOutfile(qual.ename);

    if(ajAcdIsUserdefinedC(qual.ename))
    {

        if(outfile)
        {
            squal = ajStrNewS(ajFileGetNameS(outfile));

            ajFmtPrintAppS(cl," --%s%s=%S",
                           qual.prefix,
                           qual.qname,
                           squal);
            ajStrDel(&squal);
        }
    }
    
    ajFileClose(&outfile);

    return;
}




/* @funcstatic eomegash_dodirectory **********************************************
**
** Get ACD directories.
** Only add to command line if value different from the default
**
** @param [w] cl [AjPStr*] command line
**
** @return [void]
** @@
******************************************************************************/

static void eomegash_dodirectory(AjPStr *cl, OmegaOQual qual)
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




/* @funcstatic eomegash_dolistsingle ********************************************
**
** Get ACD single value list entries.
** Only add to command line if value different from the default
**
** @param [w] cl [AjPStr*] command line
**
** @return [void]
** @@
******************************************************************************/

static void eomegash_dolistsingle(AjPStr *cl, OmegaOQual qual)
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




/* @funcstatic eomegash_dolist ********************************************
**
** Get ACD single value list entries.
** Only add to command line if value different from the default
**
** @param [w] cl [AjPStr*] command line
**
** @return [void]
** @@
******************************************************************************/

static void eomegash_dolist(AjPStr *cl, OmegaOQual qual)
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




/* @funcstatic eomegash_dointeger ********************************************
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

static void eomegash_dointeger(AjPStr *cl, OmegaOQual qual)
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



/* @funcstatic eomegash_dofloat ********************************************
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

static void eomegash_dofloat(AjPStr *cl, OmegaOQual qual)
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
