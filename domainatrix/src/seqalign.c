/* @source seqalign application
**
** Generate extended alignments for families in a scop families file by using 
** CLUSTALW with seed alignments.
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
**  seqalign
**  
**  
**
**  Summary
**  Generate extended alignments for families in a scop families file by using 
**  CLUSTALW with seed alignments.
**
**  
**  
**  Input and Output
**  seqalign reads a scop families file and a directory of scop family alignment
**  files.  Each alignment file should contain a structure-based sequence 
**  alignment of domains of known structure for a particular scop family.  The
**  directory should contain an alignment for each family in the scop families
**  file.  seqalign extends each alignment with apppropriate sequences (of 
**  unknown structure) from the scop families file one sequence at a time by 
**  using clustalw.  A directory of scop family alignment files containing the
**  extended alignments is written.  The base names of the output alignments 
**  are the same as the input alignments.  The paths and file extensions for 
**  the alignments (input and output) are specified by the user.
**  
**   
**  
**  Sister applications
**  A 'scop families file' contains sequence relatives (hits) for each of a 
**  number of different scop families found from psiblast searches of a 
**  sequence database.  The file contains the collated search results for the
**  indvidual scop families; only those hits of unambiguous family assignment
**  are included.  Hits of ambiguous family assignment are assigned as 
**  relatives to a scop superfamily or fold instead and are collated into a 
**  'scop ambiguities file'.  The scop families and ambiguities files are 
**  generated by seqsort and are in embl-like format.
**  A 'scop family alignment file' contains a sequence alignment of domains 
**  belonging to the same scop family.  The file is in embl-like format and is
**  annotated with records describing the SCOP classification of the family.  A
**  scop family alignment file generatd by scopalign will contain a structure-
**  based sequence alignment of domains of known structure only.  Such 
**  alignments can be extended with sequence relatives (of unknown structure)
**  to the family in question by using seqalign.
**  Extended scop family alignments are used by hmmgen, profgen and siggen.
**  
**  
**  
**  Notes
**  When running seqalign at the HGMP it is essential that the command 'use 
**  clustal' (which runs the script /packages/menu/USE/clustal) is given 
**  before it is run.  If run elsewhere, seqalign requires a working version
**  of clustalw.
**  
**  If the scop family alignment file (input) does not contain any sequences
**  then sequences from the scop families file only will be used to construct
**  the extended alignment.  However, if a single or no sequences are given in
**  the scop families file for a family then no extended alignment will be 
**  generated.
**  
**  Known bugs & warnings
**  seqalign uses a temporary file in fasta reather than clustal format and
**  passes this to clustalw (i..e ajXyzScopalgWriteFasta is called).  This
**  was necessary because jalview, used for hand-editing alignments, writes
**  out a different clustal format file than which clustalw expects, causing
**  clustalw to fail.  scopalgwriteclustal was writing the clustalw file
**  from the scop alignment file (which for manually edited alignments 
**  contains jalview output) without modification, causing seqalign to crash
**  in those cases.
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
**  The generation of sequence alignments for large datasets such as scop 
**  potentially requires a lot of time for preparation of datasets, running 
**  jobs and so on, in addition to the compute time required for the 
**  alignments themselves. 
**  seqalign will generate an extended alignment for each family in a scop 
**  families file by using CLUSTALW.  Hits for a family in the scop families
**  file are added one by one to the appropriate structure-based sequence
**  alignment.
**  
**  
**  
**  Algorithm
**  
**  
**  
**  Usage 
**  An example of interactive use of seqalign is shown below.
**  
**  Unix % seqalign
**  Generate extended alignments for families in a scop families file by using
**  CLUSTALW with seed alignments.
**  Location of scop alignment files (input) [./]: /test_data
**  Extention of scop alignment files [.salign]: 
**  Name of scop families file (input): /test_data/scop.all
**  Location of extended alignment files (output) [./]: 
**  Extention of extended alignment files [.ealign]: 
**  
**  clustalw -type=protein -profile1=./seqalign-1031838650.30257.aln -sequences -profile2=./seqalign-1031838650.30257.seqs -outfile=./seqalign-1031838650.30257.out
**  
**  CLUSTAL W (1.82) Multiple Sequence Alignments
**  
** 
**  
**  Sequence type explicitly set to Protein
**  Sequence format is Pearson
**  No. of seqs=2
**  Sequences assumed to be PROTEIN
**  Sequence 1: d1cs4a__0       252 aa
**  Sequence 2: d1fx2a__1       252 aa
**  Sequence format is Pearson
**  No. of seqs in profile=106
**  Total no. of seqs     =108
**  Sequences assumed to be PROTEIN
**  Sequence 3: Q9WVI4_514_667        154 aa
**  Sequence 4: Q9DGG6_1032_1200      169 aa
**  Sequence 5: Q9DGG6_377_542        166 aa
**  Sequence 6: Q99396_266_473        208 aa
**  Sequences (104:108) Aligned. Score:  30
**  
**  < rest of clustalw output omitted for clarity >
**  
**  Sequence:8     Score:1487
**  Sequence:3     Score:1407
**  Alignment Score 98183
**  CLUSTAL-Alignment file created  [./seqalign-1031838785.15389.out]
**  
**  seqalign read all structure-based sequence alignments with the file 
**  extension .salign from the directory /test_data, and extended these
**  alignments using the sequences for each family listed in the scop 
**  families file /test_data/scop.all by using clustalw.  Extended scop
**  alignment files with the file extension .ealign were written to 
**  /test_data/.
**  
**  The following command line would achieve the same result.
**  seqalign /test_data .salign /test_data .ealign -scopin /test_data/scop.all
**  
**  
** 
**  Input file format
**  The format of the scop alignment file is described in scopalign.c
**  The format of the scop families file is described in seqsort.c. 
**  
**  
**  Output file format
**  The format of the extended scop alignment file (below) is exactly the same
**  as the scop alignment file described in scopalign.c except that the 
**  'Number' and 'Post_similar' records are omitted.  Also, the code given
**  ahead of each sequence consists of a string and two numbers seperated by
**  underscores.  The string is the accession number of the sequence, the
**  first and second numbers are the start and end point of the sequence in
**  the full length sequence respectively. 
**  
**  CL   Alpha and beta proteins (a+b)
**  XX
**  FO   Ferredoxin-like
**  XX
**  SF   Aspartate carbamoyltransferase, Regulatory-chain, N-terminal domain
**  XX
**  FA   Aspartate carbamoyltransferase, Regulatory-chain, N-terminal domain
**  XX
**  SI   54894
**  XX
**  d4at1b1_0          GVEAIKRGTVIDHIPAQIGFKLLSLFKLTETD---QRITIGLNLP-SGEMGRKDLIKIEN
**  d4at1d1_1          GVEAIKRGTVIDHIPAQIGFKLLSLFKLTETD---QRITIGLNLPSG-EMGRKDLIKIEN
**  P00478_8_99        -VEAIKRGTVIDHIPAQIGFKLLSLFKLTETD---QRITIGLNLPSG-EMGRKDLIKIEN
**  Q8Z130_8_99        -VEAIKCGTVIDHIPAQVGFKLLSLFKLTETD---QRITIGLNLPSG-EMGRKDLIKIEN
**  P08421_8_99        -VEAIKCGTVIDHIPAQVGFKLLSLFKLTETD---QRITIGLNLPSG-EMGRKDLIKIEN
**  Q8ZB38_9_100       -VEAIKCGTVIDHIPAQIGFKLLSLFKLTATD---QRITIGLNLPSK-RSGRKDLIKIEN
**  P19936_8_99        -VEAIKCGTVIDHIPAQIGFKLLTLFKLTATD---QRITIGLNLPSN-ELGRKDLIKIEN
**  P57451_8_99        -VEAIKSGSVIDHIPEYIGFKLLSLFRFTETE---KRITIGLNLPSK-KLGRKDIIKINE
**
**  < data ommitted for clarity >
**  
**  O26938_29_119      RELKPSEVDQIALIAPRATINIVRDYKIVEKAKVRL-
**  Q58801_9_99        IELKKEDVDKISLISPDVTINIIRNGKVVEKLKPQIP
**  Q97B28_8_100       RSLDKNELDMISLTAPNATISIIKNYEISEKFKVELP
**  Q9HKM3_7_99        RFLDRNELDMISLIAPNATISIIKNYEISEKFQVELP
**  Q9YBD5_11_105      YKPRQRDLEVLGIIAPGATFNVIEDYKVVEKVKLKLP
**  
**  
**  
**  Data files
**  seqalign does not use any data files.
**  
**  
**  
**  Diagnostic error messages
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
******************************************************************************/




#include "emboss.h"

AjBool ajXyzScopalgWriteFasta(AjPScopalg align, AjPFile* outf);


/* @prog seqalign *************************************************************
**
** Generate extended alignments for families in a scop families file by using 
** CLUSTALW with seed alignments.
**
******************************************************************************/
int main(int argc, char **argv)
{
    AjPStr  syscmd      = NULL;  /* command line arguments */
    AjPStr  inpath      = NULL;  /* path to the directory that contain the 
				    alignment files*/
    AjPStr  outpath     = NULL;  /* the path to the output directory */
    AjPStr  extn        = NULL;  /* the file extention of the alignment files */
    AjPStr  outextn     = NULL;  /* the file extention of the extended 
				    alignment files */
    AjPStr  tmp_str     = NULL;  /* temparary string */
    AjPStr  tmp_name    = NULL;  /* string object for holding randomly 
				    generated file names */

    AjPStr  filename    = NULL;  /* the name of the present working file */
    AjPStr  outfilename = NULL;  /* the name of the current output file */
    AjPStr  clustinf1   = NULL;  /* the name of the input alignment file 
				    for clustalw */
    AjPStr  clustinf2   = NULL;  /* the name of the input sequence file 
				    for clustalw */
    AjPStr  clustoutf   = NULL;  /* the name of the Clustal file that will
				    be created ... this is reformated into 
				    scop alignment format*/
    AjPStr  clustdndf   = NULL;  /* the name of the Clustal tree file
				   to clean up*/
    
    AjPFile scopin     = NULL;  /* scop families file (for reading)*/
/*    AjPFile scopout    = NULL; */ /* scop families file (for writing)*/
    

    AjPStr fam          = NULL;   /* family name */
    AjPStr sfam         = NULL;   /* superfamily name */
    AjPStr fold         = NULL;   /* fold name */
    AjPStr class        = NULL;   /* class name */
    AjPStr msg          = NULL;   /* Message string */
    
   
    AjPFile alnf       	= NULL;   /* the seed alignment file with the 
				     scop info and the stamp info */
    AjPFile outf1       = NULL;   /* the outfile for the parsed database 
				     sequences in CLUSTAL format */
    AjPFile outf2       = NULL;   /* the outfile for the parsed seed 
				     alignment in CLUSTAL format */
/*    AjPFile scopf       = NULL; */  /* the scop families file */
            
    AjPList list        = NULL;   /* a list of relavant file names */
    AjPList tmp_list    = NULL;   /* temp list */
    
    AjPScopalg align    = NULL;   /* a scop alignment structure */

    AjPHitlist temphl   = NULL;   /* Temp. pointer to Hitlist object */
    
    ajint   posdash     = 0;      /* Position of last '/' in filenames 
				     from 'list' */
    ajint   posdot      = 0;      /* Position of last '.' in filenames 
				     from 'list' */

    ajint   sunid       = 0;
    AjPFile alg_in      = NULL;   /* the alignment file for reading*/
    AjPFile alg_out     = NULL;   /* the alignment file for writing*/
    AjPStr  outname     = NULL;
    AjPStr  line        = NULL;
    AjIList iter     = NULL; 
    


    outname    = ajStrNew();
    line       = ajStrNew();
    


    msg        = ajStrNew();
    outfilename= ajStrNew();
    syscmd     = ajStrNew();
    inpath     = ajStrNew();
    outpath    = ajStrNew();
    extn       = ajStrNew();
    outextn    = ajStrNew();
    tmp_str    = ajStrNew();
    tmp_name   = ajStrNew();
    clustinf1  = ajStrNew();
    clustinf2  = ajStrNew();
    clustoutf  = ajStrNew();
    clustdndf  = ajStrNew();
    
    fam        = ajStrNew();
    sfam       = ajStrNew();
    fold       = ajStrNew();
    class      = ajStrNew();
   
    list       = ajListNew();
    
    ajNamInit("emboss");
    ajAcdInitP("seqalign",argc,argv,"DOMAINATRIX");
    
    inpath    = ajAcdGetString("inpath");
    extn      = ajAcdGetString("extn");
    outextn   = ajAcdGetString("outextn");
    scopin   = ajAcdGetInfile("scopin");
    outpath   = ajAcdGetString("outpath");
    
    /* Check directories */
    if((!ajFileDir(&inpath)) || (!(extn)))
	ajFatal("Could not open seed alignment directory or file "
		"extension NULL");
    
    if((!ajFileDir(&outpath)) || (!(outextn)))
	ajFatal("Could not open extended alignment directory or file "
		"extension NULL");
    
    


    /* Add a '.' to outextn if one does not already exist */
    if((ajStrChar(outextn, 0)!='.')) 	/* checks if the file extension 
					   starts with "." */
	ajStrInsertC(&outextn, 0, ".");



    /* Create list of files in the path */
    ajStrAssC(&tmp_str, "*");  		/* assign a wildcard to tmp_str */
	
    /* checks if the file extension starts with "." */
    /* assign the acd input file extension to tmp */
    if((ajStrChar(extn, 0)=='.')) 	
	ajStrApp(&tmp_str, extn);    
    /* this picks up situations where the user has specified an 
       extension without a "." */
    else
    {
	ajStrAppC(&tmp_str, ".");       /* assign "." to tmp */  
	ajStrApp(&tmp_str, extn);       /* append tmp with a user specified 
					   extension */  
    }	

    /* all files containing seed alignments will be in a list */
    ajFileScan(inpath, tmp_str, &list, ajFalse, ajFalse, NULL, NULL, 
	       ajFalse, NULL);  


    /*Initialise random number generator for naming of tmp_name. files and 
      create clustalw input files */
    ajRandomSeed();
    ajStrAssC(&tmp_name,ajFileTempName(NULL));


    /* read each psiblast file and create a list of Scophit structures */
    while(ajListPop(list,(void **)&filename))
    {
	ajFmtPrint("Processing %S\n", filename);
	

	/* open a scop seed alignment file */
	if((alnf = ajFileNewIn(filename)) == NULL)
	{
	    ajFmtPrintS(&msg, "Could not open seed alignment file %S", filename);
	    ajWarn(ajStrStr(msg));
	}
	else
	{
	    /* set up the random file for the input alignment for clustal */
	    ajStrAssS(&clustinf1,tmp_name);
	    ajStrAppC(&clustinf1,".aln");

	    /* set up the random file for the input sequences for clustal */
	    ajStrAssS(&clustinf2,tmp_name);
	    ajStrAppC(&clustinf2,".seqs");

	    /* setup the .dnd tree file to delete later */
	    ajStrAssS(&clustdndf,tmp_name);
	    ajStrAppC(&clustdndf,".dnd");
	    ajStrAssS(&clustoutf,tmp_name);
	    ajStrAppC(&clustoutf,".out");
	    
	    /* read the scop seed alignment file into a scopalg object */
	    ajXyzScopalgRead(alnf,&align);

	    /* extract the scop information */
	    ajStrAssS(&fam,align->Family);
	    ajStrAssS(&sfam,align->Superfamily);
	    ajStrAssS(&fold,align->Fold);
	    ajStrAssS(&class,align->Class);
	    sunid = align->Sunid_Family;
	    

	    if(align->N !=0)
	    { 
		/* write out the seed alignment (from the scopalg object) 
		   in CLUSTAL format */
		if((outf1 = ajFileNewOut(clustinf1))==NULL)
		{
		    ajFatal("Could not open %S for writing\n", clustinf1);
		}

		 ajXyzScopalgWriteFasta(align,&outf1);  
		/* ajXyzScopalgWriteClustal(align,&outf1);   */
		/*	    ajXyzScopalgWrite(outf1, align);  */
		ajFileClose(&outf1);
	    } 
	    
	    
	    /*open the scopfamilies file and pass a file pointer into each 
	      of the functions */
	    /* scopf = ajFileNewIn(scopfam); */
	    /* extract the relavent family or superfamily or etc.. into a 
	       list of Hitlist objects. */
	    tmp_list = ajListNew();
	    if(!ajXyzHitlistReadNode(scopin,&tmp_list, fam, sfam, fold, class))
	    {
		ajWarn("Hitlist not found in Scop hits file");
		

		ajListDel(&tmp_list);

		/* Delete Scopalg structure */
		ajXyzScopalgDel(&align);
		
		ajFileClose(&alnf);
		

		/* Rewind the scop families input file, write scop families output file.  
		   The Typeobj element of the hits used in the alignment are given as SEED */
		ajFileSeek(scopin,0,SEEK_SET);

		continue;
	    }
	    /* tmp_list will only ever contain one Hitlist */
	    iter = ajListIter(tmp_list);
	    temphl = (AjPHitlist)ajListIterNext(iter);
	    ajListIterFree(iter);

	    if((temphl->N == 1) || (temphl->N ==0))
	    {
		ajWarn("Hitlist containing no or single sequence found in Scop hits file");

		ajListDel(&tmp_list);

		/* Delete Scopalg structure */
		ajXyzScopalgDel(&align);
		
		ajFileClose(&alnf);


		/* Rewind the scop families input file, write scop families output file.  
		   The Typeobj element of the hits used in the alignment are given as SEED */
		ajFileSeek(scopin,0,SEEK_SET);

		continue;
	    }
	    
	    

	    
	    /* Rewind the scop families input file, write scop families output file.  
	       The Typeobj element of the hits used in the alignment are given as SEED */
	    ajFileSeek(scopin,0,SEEK_SET);

/*	    seqalign_WriteScopout(scopin, scopout, fam, sfam, fold, class); */
	    

/*	    ajFileClose(&scopin);
	    ajFileClose(&scopout); */
	    
	    

	    /* write out the sequences in the list of Hitlist structures in 
	       CLUSTAL format. */
	    outf2 = ajFileNewOut(clustinf2);
	    ajXyzHitlistsWriteFasta(&tmp_list,&outf2);
	    ajFileClose(&alnf);

	    ajFileClose(&outf2);

	    while(ajListPop(tmp_list,(void **)&temphl))
		ajXyzHitlistDel(&temphl);
	    ajListDel(&tmp_list);


	    /* Create the name of the output file */
	    posdash = ajStrRFindC(filename, "/");
	    posdot  = ajStrRFindC(filename, ".");
	    if(posdash >= posdot)
		ajFatal("Could not create filename. Email rranasin@hgmp.mrc.ac.uk");
	    else
	    {
		ajStrAssSub(&outfilename, filename, posdash+1, posdot-1);
	    }
	    
	    if(align->N !=0)
	    {
		/* run clustalw in profile to sequence mode */
		ajFmtPrintS(&syscmd,"clustalw -type=protein -profile1=%S -sequences"
			    " -profile2=%S -MATRIX=BLOSUM -GAPOPEN=10"
			    " -GAPEXT=0.5 -outfile=%S\n",
			    clustinf1,clustinf2,clustoutf);
	    }
	    else
	    {
		/* run clustalw in multiple sequence mode */
		ajFmtPrintS(&syscmd,"clustalw -infile=%S -align"
			    " -MATRIX=BLOSUM -GAPOPEN=10"
			    " -GAPEXT=0.5 -outfile=%S\n",
			    clustinf2,clustoutf);
	    }
	    
	    ajFmtPrint("\n%S\n", syscmd);
	    system(ajStrStr(syscmd));
	    
	    
	    /* Reformat output file into scop format */
	    /* First open files */
	    if((alg_in = ajFileNewIn(clustoutf))==NULL)
	    {
		ajFatal("Could not read clustal output file %S", clustoutf);
	    }
	    
	    ajStrAssS(&outname, outpath);
	    ajStrApp(&outname, outfilename);
	    ajStrApp(&outname, outextn);
	    if((alg_out = ajFileNewOut(outname))==NULL)
	    {
		ajFatal("Could not write clustal output file");
	    }

	    /* Then write scop classification data */
	    ajFmtPrintF(alg_out,"CL   %S",class);
	    ajFmtPrintSplit(alg_out,fold,"\nXX\nFO   ",75," \t\n\r");
	    ajFmtPrintSplit(alg_out,sfam,"XX\nSF   ",75," \t\n\r");
	    ajFmtPrintSplit(alg_out,fam,"XX\nFA   ",75," \t\n\r");
	    ajFmtPrintF(alg_out,"XX\n");
	    ajFmtPrintF(alg_out,"SI   %d\nXX\n",sunid);


	    /* Then parse the clustal file and write the alignment */
	    while(ajFileReadLine(alg_in,&line))
	    {
		if(ajStrPrefixC(line, "CLUSTAL"))
		    continue;
		if(MAJSTRLEN(line)==0)
		    continue;
		if(line->Ptr[0]==' ')
		    ajFmtPrintF(alg_out, "\n");
		else
		    ajFmtPrintF(alg_out, "%S\n", line);
	    }
	    
	    
	    ajFileClose(&alg_in);
	    ajFileClose(&alg_out);
	    

	    /* clean up directory */
	    if(align->N !=0)
		ajSysUnlink(&clustinf1);

	    ajSysUnlink(&clustinf2);
	    ajSysUnlink(&clustoutf);
	    ajSysUnlink(&clustdndf);


	    /* Delete Scopalg structure */
	    ajXyzScopalgDel(&align);
	}
	/* Free the nodes ! */
	ajStrDel(&filename);
    }
    

    /* clean up */    
    ajStrDel(&outname);
    ajStrDel(&line);
    ajStrDel(&msg);
    ajStrDel(&syscmd);
    ajStrDel(&fam);
    ajStrDel(&sfam);
    ajStrDel(&fold);
    ajStrDel(&class);
    ajStrDel(&inpath);
    ajStrDel(&outpath);
    ajStrDel(&extn);
    ajStrDel(&outextn);
    ajStrDel(&tmp_str);
    ajStrDel(&tmp_name);
    ajStrDel(&clustinf1);
    ajStrDel(&clustinf2);
    ajStrDel(&clustoutf);
    ajStrDel(&clustdndf);
    ajStrDel(&outfilename);
    ajListDel(&list);
    
    ajExit();
    return 0;
    
}






			      
				 
/* @func ajXyzScopalgWriteFasta ********************************************
**
** Writes a Scopalg object to a specified file in FASTA format (just the 
** alignment without the SCOP classification information).
**
** @param [r] align      [AjPScopalg]  A list Hitlist structures.
** @param [w] outf       [AjPFile *]     Outfile file pointer
** 
** @return [AjBool] True on success (a file has been written)
** @@
******************************************************************************/
AjBool ajXyzScopalgWriteFasta(AjPScopalg align, AjPFile* outf)
{
    ajint i;
    
    /*Check args*/
    if(!align)
    {
	ajWarn("Null args passed to ajXyzScopalgWriteClustal ");
	return ajFalse;
    }
    
    /* remove i from the print statement before commiting
    ajFmtPrintF(*outf,"CLUSTALW\n\n");
    ajFmtPrintF(*outf, "\n");*/ 

    for(i=0;i<align->N;++i)
    	ajFmtPrintF(*outf,">%S_%d\n%S\n",align->Codes[i],i,align->Seqs[i]);
    ajFmtPrintF(*outf,"\n");
    ajFmtPrintF(*outf,"\n"); 
    
    return ajTrue;
}	






