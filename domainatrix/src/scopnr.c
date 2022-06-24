/* @source scopnr application
**
** Removes redundant domains from a scop classification file.
**
** @author: Copyright (C) Ranjeeva Ranasinghe (rranasin@hgmp.mrc.ac.uk)
** @author: Copyright (C) Jon Ison (jison@hgmp.mrc.ac.uk)
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
** 
*******************************************************************************
**  Application name
**  scopnr 
**
**  
**  
**  Summary
**  Removes redundant domains from a scop classification file.
**
**  
**  
**  Input and Output
**  scopnr reads a scop classification file containing domain sequence 
**  information and writes a file containing the same data in the same embl-like
**  format, except that redundant domains are removed from each family.  The 
**  input and output files are specified by the user.  A log file is also 
**  written.
**
**  
**   
**  Sister applications
**  A 'scop classification file' contains classification and other data for 
**  domains from the scop database.  The file is in embl-like format and is 
**  generated by scopparse.  Domain sequence information can be added to the 
**  file by using scopseqs.
**  domainer, funky, scopreso, scopseqs, scoprep, scopalign, seqsearch, seqnr 
**  and seqalign use a scop classification file as input.  
**  
**
**  
**  Notes
**  The current version of scopnr removes redundancy at the level of the scop 
**  family, i.e. entries belonging to the same family will be non-redundant.  
**  
**  
**  
**  Known bugs & warnings
**  
**  
**  
**  Description
**  This program is part of a suite of EMBOSS applications that directly or 
**  indirectly make use of the protein structure databases pdb and scop.  
**  This program is part of an experimental analysis pipeline described in an
**  accompanying document.  We provide the software in the hope that it will
**  be useful.  The applications were designed for specific research purposes
**  and may not be useful or reliable in contexts other than the described 
**  pipeline.  The development of the suite was coordinated by Jon Ison to
**  whom enquiries and bug reports should be sent (email jison@hgmp.mrc.ac.uk).
**  
**  The inclusion of very similar sequences in certain analyses will introduce
**  undesirable bias.  For example, a family may possess 100 sequences in the 
**  sequence database, but 90 of these might be essentially the same sequence,
**  e.g. very close relatives or mutations of a single sequence.  Although 100
**  sequences are known, the family only contains 11 sequences that are 
**  essentially unique.  For many methods it is desirable to use sets of 
**  sequences that are truly representative of the larger family.  scopnr reads
**  a scop classification file and writes a scop classification file with 
**  redundant domains removed from each family.
**  
**  
**  
**  Algorithm
**  All permutations of pair-wise sequence alignments are calculated for each 
**  scop family in turn using the EMBOSS implementation of the Needleman and 
**  Wunsch global alignment algorithm.  Redundant sequences are removed in one
**  of two modes as follows: (i) If a pair of proteins achieve greater 
**  than a threshold percentage sequence similarity (specified by the user) 
**  the shortest sequence is discarded. (ii) If a pair of proteins have a 
**  percentage sequence similarity that lies outside an acceptable range 
**  (specified by the user) the shortest sequence is discarded.
**  The user must specify gap insertion 
**  and extension penalties and a residue substitution matrix for use in the 
**  alignments.  % sequence similarity is calculated by using the EMBOSS 
**  function embAlignCalcSimilarity.
**  
**  
**  
**  Usage 
**  An example of interactive use of scopnr is shown below.
**  
**  Unix % 
**  Unix % scopnr
**  Removes redundant domains from a scop classification file.
**  Name of scop classification file (embl format input): /test_data/all_s.scop
**  Redundancy removal options
**           1 : Remove redundancy at a single threshold % sequence similarity
**           2 : Remove redundancy outside a range of acceptable threshold % similarity
**  Select number [1]: 1
**  The % sequence identity redundancy threshold [95.0]: 
**  Name of non-redundant scop classification file (embl format output) [test.scop]: /test_data/all_nr.scop
**  Name of log file for the build [scopnr.log]: /test_data/scopnr.log
**  // DNA double-strand break repair nuclease
**  D1II7A_
**  // Adenylyl and guanylyl cyclase catalytic domain
**  D1CS4A_
**  Unix % 
**  
**  The scop classification file /test_data/all_s.scop with families containing
**  redundant domains was read and a scop classificaiton called 
**  /test_data/all_nr.scop was written in which redundant domains were removed
**  from each family.  A redundant domain was one sharing at least 95% sequence
**  similarity to another domain.  The log file /test_data/scopnr.log was 
**  written.
**  
**  The following command line would achieve the same result.
**  scopnr  /test_data/all_s.scop /test_data/all_nr.scop -mode 1 -thresh 95 
**  -errf /test_data/scopnr.log
**  
**  
**  
**  Input file format
**  The format of the scop classification file is described in scopparse.c
**  
**  
**  
**  Output file format
**  The format of the scop classification file is described in scopparse.c
**  
**  
**  
**  Data files
**  scopnr requires a residue substitution matrix.
**  
**  
**  
**  Diagnostic error messages
**  scopnr generates a log file an excerpt of which is shown below. The 
**  first two lines describe the level in the scop hierarchy at which redundancy 
**  was removed (always 'FAMILIES' for the current implementation) and the value
**  of the redundancy threshold.  The file then contains a section for each scop 
**  family. Each section contains a line with the record '//' immediately 
**  followed by the name of the scop family, and two lines containing 'Retained' 
**  and 'Rejected' respectively. Domain identifier codes of domains that were 
**  discarded by scopnr are listed under 'Rejected' while those that appear in 
**  the output file are listed under 'Retained'.  The text 'WARN  filename not 
**  found' is given in cases where a clean domain coordinate file could not be 
**  found and 'WARN  Empty family' where no files for an entire family could be 
**  found.  'ERROR filename file read error' will be given when an error was 
**  encountered during a file read.
**
**  Excerpt from scopnr log file
**  FAMILIES are non-redundant
**  95% redundancy threshold
**  // Homeodomain
**  Retained
**  D2HDDA_
**  D1AKHA_
**  D1MNMC_
**  Rejected
**  D2HDDB_
**  D1ENH__
**  D3HDDA_
**  WARN  d3hdda_.pxyz not found
**  // Di-haem cytohrome c peroxidase
**  WARN  ds005__.pxyz not found
**  WARN  Empty family
**  // Nuclear receptor coactivator Src-1
**  Retained
**  D2PRGC_
**  Rejected
**  
**  
**  
**  Authors
**  Ranjeeva Ranasinghe (rranasin@hgmp.mrc.ac.uk)
**  Jon Ison (jison@hgmp.mrc.ac.uk)
**  
**  
**  
**  References
**  Please cite the authors.
**  
**  
**
********************************************************************************/






#include "emboss.h"




/* @prog scopnr **************************************************************
**
** Removes redundant domains from a scop classification file.
**
******************************************************************************/

int main(int argc, char **argv)
{
    ajint        x         = 0;
    ajint        nset      = 0;	 /*No. proteins in our  set*/
    ajint        nsetnr    = 0;	 /*No. proteins in the non-redundant set */
    ajint        nfam      = 0;	 /* Counter of SCOP families */

    float      gapopen=0.0;		 /* Gap insertion penalty*/
    float      gapextend=0.0;	 /* Gap extension penalty*/
    float      thresh=0.0;		 /* Threshold for non-redundancy (single value)*/
    float      threshlow=0.0;	 /* Threshold for non-redundancy (lower limit)*/
    float      threshup=0.0;	 /* Threshold for non-redundancy (upper limit)*/

    ajint     last_famid=    0; /* SCOP Sunid of last family that was processed */
    AjPStr    last_fam   = NULL; /* Last family that was processed*/
    AjPStr    msg        = NULL; /* String used for messages */

    AjPFile   scop_inf   = NULL; /* File pointer for original scop file */
    AjPFile   scop_outf  = NULL; /* File pointer for scop output file */
    AjPFile   errf       = NULL;

    AjPList    list_seqs = NULL; /* List of sequences for a family*/
    AjPList    list_scop = NULL; /* List of scop structures for a family*/
    AjIList    iter      = NULL; /* Iterator for scop list */    
    AjPInt     keep      = NULL; /*1: This sequence was kept after */
                                 /*redundancy removal, 0: it was discarded*/
    AjPSeq     seq       = NULL; /* A temporary pointer*/
    AjPMatrixf matrix;		 /* Substitution matrix*/
    AjPScop   scop       = NULL; /* Pointer to scop structure */
    AjPScop   scop_tmp   = NULL; /* Pointer to scop structure */
    AjPStr      *mode   =NULL;      /* Mode of operation from acd*/
    ajint     moden      =0;     /* Program mode as int 1: use single threshold
				    value to remove redundancy, 2: use lower and
				    upper limit to remove redundancy */
    AjBool    ret=ajFalse;
    



    
    /* Initialise strings etc */
    msg       = ajStrNew();
    last_fam  = ajStrNew();
    keep = ajIntNew();  	    


    /* Initialise last_fam with a value that is not in SCOP */
    ajStrAssC(&last_fam,"!!!!!");

    
    /* Read data from acd */
    ajNamInit("emboss");
    ajAcdInitP("scopnr",argc,argv,"DOMAINATRIX");
    scop_inf  = ajAcdGetInfile("scopin");
    scop_outf = ajAcdGetOutfile("scopout");
    matrix    = ajAcdGetMatrixf("datafile");
    gapopen   = ajAcdGetFloat("gapopen");
    gapextend = ajAcdGetFloat("gapextend");
    errf      = ajAcdGetOutfile("errf");
    mode     = ajAcdGetList("mode");

    if(ajStrChar(*mode, 0) == '1')
	{
	    moden=1;
	    thresh    = ajAcdGetFloat("thresh");
	}
    
    else if(ajStrChar(*mode, 0) == '2')
	{
	    moden=2;
	    threshlow    = ajAcdGetFloat("threshlow");
	    threshup     = ajAcdGetFloat("threshup");
	}
    
    


    /* Write header of log file */
    /* JCI Replace hard-coded 'FAMILIES' with appropriate string when */
    /* redundancy removal is implemented for any node in SCOP */
    ajFmtPrintF(errf, "FAMILIES are non-redundant\n"
		"%.0f%% redundancy threshold\n", thresh); 


    


    
    /* Start of main application loop */
    while((scop=(ajScopReadCNew(scop_inf, "*"))))
    {
	/* If we are on to a new family*/
/*	if(ajStrMatch(last_fam, scop->Family)==ajFalse) */
	if( last_famid !=  scop->Sunid_Family)
	{
	    /* If we have done the first family*/
	    if(nfam)
	    {
		/* If the family does not have any members*/
		if(!nset)
		{
		    /* Write diagnostic */
        	    ajFmtPrint("WARN  Empty family\n"); 
		    ajFmtPrintF(errf, "WARN  Empty family\n");
		}
		/* Last family had members, so remove redundancy*/
		else
		{
		    /* Remove redundancy from list_seqs*/
		    if(moden==1)
			ret=embDmxSeqNR(list_seqs, &keep, &nsetnr, matrix, gapopen, 
					   gapextend,thresh);
		    else
			ret=embDmxSeqNRRange(list_seqs, &keep, &nsetnr, matrix, gapopen, 
					   gapextend,threshlow, threshup);		    
		    if(!ret)
		    {
			ajStrDel(&last_fam);
			ajStrDel(&msg);
			ajIntDel(&keep);	
			ajMatrixfDel(&matrix);
			ajFileClose(&errf);    
			ajFileClose(&scop_outf);	    
			ajStrDel(&mode[0]);
			AJFREE(mode);
			ajFatal("Unexpected embDmxSeqNR error");
		    }
		    

		    /* Write file with SCOP entries that are retained*/
		    for(iter=ajListIterRead(list_scop), x=0;
			(scop_tmp=(AjPScop)ajListIterNext(iter));
			x++)
			if(ajIntGet(keep,x))
			    ajScopWrite(scop_outf, scop_tmp);
		    ajListIterFree(&iter);	


		    /* Write diagnostic */
 		    ajFmtPrintF(errf, "Retained\n");
		    for(iter=ajListIterRead(list_scop), x=0;
			(scop_tmp=(AjPScop)ajListIterNext(iter));
			x++)
			if(ajIntGet(keep,x))
			    ajFmtPrintF(errf, "%S\n", scop_tmp->Entry);
		    ajListIterFree(&iter);
		    ajFmtPrintF(errf, "Rejected\n");
		    for(iter=ajListIterRead(list_scop), x=0;
			(scop_tmp=(AjPScop)ajListIterNext(iter));
			x++)
			if(!(ajIntGet(keep,x)))
			    ajFmtPrintF(errf, "%S\n", scop_tmp->Entry);
		    ajListIterFree(&iter);	

		    
		    /* Write diagnostic */
		    ajFmtPrintF(errf, "// %S\n", scop->Family);
		    ajFmtPrint("// %S\n", scop->Family);
		}
		/* Intitiliase counter for number in family*/
		nset=0;


		/* Free up the scop list and create a new one*/
		iter=ajListIterRead(list_scop);
		while((scop_tmp=(AjPScop)ajListIterNext(iter)))
		    ajScopDel(&scop_tmp);
		ajListIterFree(&iter);	
		ajListDel(&list_scop);	    


		/* Free up the seqs list and create a new one*/
		iter=ajListIterRead(list_seqs);
		while((seq=(AjPSeq)ajListIterNext(iter)))
		    ajSeqDel(&seq);
		ajListIterFree(&iter);	
		ajListDel(&list_seqs);	    
	    }
	    else
	    {
		/* We have not done the first family*/
		/* Write diagnostic */
		ajFmtPrintF(errf, "// %S\n", scop->Family);
		ajFmtPrint("// %S\n", scop->Family);
	    }
	    
	    
	    /* Write the current family to last_fam*/
	    ajStrAssS(&last_fam, scop->Family);
	    last_famid = scop->Sunid_Family;

	    /*Create new list for scop and seqs*/
	    list_seqs = ajListNew();
	    list_scop = ajListNew();

	    
	    /* Increment counter for number of families*/
	    nfam++;
	}
	/* Write diagnostic */
	ajFmtPrint("%S\n", scop->Entry);

		
	/*Add a new sequence to the list - only ever one chain (chain 0) 
	  for domain coordinate files */
	nset++;
	seq=ajSeqNew();

	/* pdb sequence has priority */
        if((ajStrLen(scop->SeqPdb))==0)
        {
            ajStrAssS(&seq->Seq, scop->SeqSpr);
        }
        
        else
        {
            ajStrAssS(&seq->Seq, scop->SeqPdb);
        }

	ajStrAssS(&seq->Name, scop->Pdb);
	ajListPushApp(list_seqs,seq);	


	/* Add the current scop structure to the list */
	ajListPushApp(list_scop,scop);	
    }
    /* End of main application loop */






    /*Close the scop input file*/
    ajFileClose(&scop_inf);	
    
    
    /* Process the last family*/
    /* If the family does not have any members*/
    if(!nset)
    {
	/* Write diagnostic */
	ajFmtPrint("WARN  Empty family\n");
	ajFmtPrintF(errf, "WARN  Empty family\n");
    }
    /* Last family had members, so remove redundancy*/
    else
    {
	/* Remove redundancy from list_seqs*/
	if(moden==1)
	    embDmxSeqNR(list_seqs, &keep, &nsetnr, matrix, gapopen, 
			   gapextend,thresh);		
	else
	    embDmxSeqNRRange(list_seqs, &keep, &nsetnr, matrix, gapopen, 
				gapextend,threshlow, threshup);		

	/* Write file with SCOP entries that are retained*/
	for(iter=ajListIterRead(list_scop), x=0;
	    (scop_tmp=(AjPScop)ajListIterNext(iter));
	    x++)
	    if(ajIntGet(keep,x))
		ajScopWrite(scop_outf, scop_tmp);
	ajListIterFree(&iter);	
    


	/* Write diagnostic */
	ajFmtPrintF(errf, "Retained\n");
	for(iter=ajListIterRead(list_scop), x=0;
	    (scop_tmp=(AjPScop)ajListIterNext(iter));
	    x++)
	    if(ajIntGet(keep,x))
		ajFmtPrintF(errf, "%S\n", scop_tmp->Entry);
	ajListIterFree(&iter);	
	ajFmtPrintF(errf, "Rejected\n");
	for(iter=ajListIterRead(list_scop), x=0;
	    (scop_tmp=(AjPScop)ajListIterNext(iter));
	    x++)
	    if(!(ajIntGet(keep,x)))
		ajFmtPrintF(errf, "%S\n", scop_tmp->Entry);
	ajListIterFree(&iter);	
    }

    
    /* Free up the scop list */
    iter=ajListIterRead(list_scop);
    while((scop_tmp=(AjPScop)ajListIterNext(iter)))
	ajScopDel(&scop_tmp);
    ajListIterFree(&iter);	
    ajListDel(&list_scop);	    
    
    
    /* Free up the seqs list */
    iter=ajListIterRead(list_seqs);
    while((seq=(AjPSeq)ajListIterNext(iter)))
	ajSeqDel(&seq);
    ajListIterFree(&iter);	
    ajListDel(&list_seqs);	    
    
    
    /* Tidy up*/
    ajMatrixfDel(&matrix);
    ajIntDel(&keep);	
    ajStrDel(&last_fam);
    ajStrDel(&msg);
    ajFileClose(&errf);    
    ajFileClose(&scop_outf);	    
    ajStrDel(&mode[0]);
    AJFREE(mode);

    /* Bye Bye */
    ajExit();
    return 0;
}










