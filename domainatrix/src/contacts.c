/* @source contacts application
**
** Reads coordinate files and writes files of intra-chain 
** residue-residue contact data.
**
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
** 
*******************************************************************************
**  Application name
**  contacts
**
**  
**  Summary
**  Reads coordinate files and writes files of intra-chain residue-residue 
**  contact data.
**
**  
**  Input and Output
**  contacts reads a directory of domain or protein coordinate files and writes
**  a contacts file of intra-chain residue-residue contact data in embl-like 
**  format for each file in the input directory.  Each output file contains 
**  residue contact data for every chain of every model in a protein coordinate 
**  file, or contact data for the single scop domain where a domain coordinate 
**  file is read.  The paths and extensions for the coordinate (input) and 
**  contact (output) files are specified by the user.  The scop domain or pdb 
**  identifier codes are used as appropriate to name the output files.  A log 
**  file is also written.
**
**
**  Sister applications    
**  A 'protein coordinate file' contains protein coordinate and other data 
**  extracted from a single pdb file.  The files, generated by pdbparse, are in
**  embl-like format and contain 'cleaned-up' data that is self-consistent and
**  error-corrected.  
**  A 'domain coordinate file' contains coordinate and other data for a single
**  scop domain.  The files are generated by domainer and are in embl-like and
**  pdb formats.
**  siggen uses contacts files as input.
**
**  
**  Notes
**  
**  
**  
**  Known bugs & warnings
**  Must document threshold ignore distance option.
**  Must document skip option.
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
**  Knowledge of the physical contacts that amino acid residues within a 
**  protein or domain make with one another is required for several different
**  analyses.  contacts calculates intra-chain residue-residue contact data 
**  from protein and domain coordinate files.
**  
**  
**  
**  Algorithm
**  Contact between two residues is defined as when the van der Waals surface 
**  of any atom of the first residue comes within the threshold contact 
**  distance of the van der Waals surface of any atom of the second residue. 
**  The threshold contact distance is a user-defined distance with a default 
**  value of 1 Angstrom.
**  
**  
**  
**  Usage 
**  An example of interactive use of contacts is shown below.
**  
**  Unix % contacts
**  Reads coordinate files and writes files of intra-chain residue-residue
**  contact data.
**  Location of coordinate files (embl-like format input) [./]: testdata
**  Name of data file with van der Waals radii [Evdw.dat]:
**  Threshold contact distance [1.0]:
**  Location of contact files (output) [./]: testdata
**  Name of log file for the build [contacts.log]: testdata/contacts.log
**  D1CS4A_
**  D1II7A_
**  Unix %
**  
**  Coordinate files of file extension defined in the ACD file were read from 
**  testdata and the contact files (e.g. d1cs4a_.con and d1ii7a_.con)
**  were written to the same directory (the file extension .con would be set 
**  in the ACD file)  van der Waals radii were taken from 
**  the emboss data file Evdw.dat and the default contact distance of 1 
**  Angstrom was used to determine contacts.  The log file 
**  testdata/contacts.log was written.
**  
**  The following command line would achieve the same result.
**  contacts testdata testdata -thresh 1.0 -conerrf 
**  testdata/contacts.log -vdwf Evdw.dat
**
**  
**  
**  Input file format
**  The format of the clean coordinate file is described in pdbparse.c.
**  
**  
**  
**  Output file format
**  The embl-like format used for the contact files (below) uses the following 
**  records:
**  (1)  ID - either the 4-character PDB identifier code (where clean protein 
**  coordinate files are used as input) or the 7-character domain identifier 
**  code taken from scop (where domain coordinate files were used as input; see 
**  documentation for the EMBOSS application scope for further info.)
**  (2)  DE - bibliographic information.  The text "Residue-residue contact 
**  data" is always given.
**  (4)  EX - experimental information. The value of the threshold contact 
**  distance is given as a floating point number after 'THRESH'. The number of 
**  models and number of polypeptide chains are given after 'NMOD' and 'NCHA' 
**  respectively. domain coordinate files a 1 is always given. Following the EX 
**  record, the file will have a section containing a CN, IN and SM records 
**  (see below) for each chain. The sections for each chain of a model are given 
**  after the MO record.
**  (5) MO - model number. The number given in brackets after this record 
**  indicates the start of a section of model-specific data. 
**  (6) CN - chain number.  The number given in brackets after this record 
**  indicates the start of a section of chain-specific data.
**  (7) IN - chain specific data. The character given after ID is the PDB chain 
**  identifier taken from the input file, (a '.` given in cases where a chain 
**  identifier was not specified in the original pdb file or, for domain 
**  coordinate files, the domain is comprised of more than one domain).  The 
**  number of amino acid residues comprising the chain (or the chains from 
**  which a domain is comprised) is given after NR. The number of residue-residue
**  contacts is given after NSMCON. 
**  (8) SQ - domain or protein sequence. The number of residues is given before 
**  AA on the first line.  The sequece is given on subsequent lines. 
**  (9) SM - Line of residue contact data. Pairs of amino acid identifiers and 
**  residue numbers are delimited by a ';'. Residue numbers are taken from the 
**  clean coordinate file and give a correct index into the sequence (i.e. they
**  are not necessarily the same as the original pdb file).  This sequence is 
**  given in the contact file itself (SQ record).
**  (10) XX - used for spacing.
**  (11) // - given on the last line of the file only.
** 
**  Note - SM records are used for contacts between either either side-chain 
**  or main-chain atoms as defined above.  In a future implementation, SS will
**  be used for side-chain only contacts, MM will be used for main-chain only 
**  contacts, and there will probably be several other forms of contact too.
**  
**  Excerpt from contacts output file
**  ID   D1HBBB_
**  XX
**  DE   Residue-residue side-chain contact data
**  XX
**  EX   THRESH 10.0; NMOD 1; NCHA 1;
**  XX
**  MO   [1]
**  XX
**  CN   [1]
**  XX
**  IN   ID B; NR 146; NSMCON 2514;
**  XX
**  SQ   SEQUENCE    146 AA;   5817 MW;  47362A43 CRC32;
**       VHLTPEEKLA VQCTAQELVM TLNELFARFD KLAAENHCLR IKILGDCYYC VS
**  XX
**  SM   VAL 1 ; HIS 2
**  SM   VAL 1 ; LEU 3
**  SM   VAL 1 ; THR 4
**  SM   VAL 1 ; PRO 5
**  SM   VAL 1 ; GLU 6
**  SM   VAL 1 ; GLU 7
**  SM   VAL 1 ; LYS 8
**  SM   VAL 1 ; VAL 11
**  SM   VAL 1 ; PHE 71
**
**  
**  
**  Data files
**  contacts uses a data file containing van der Waals radii for atoms in 
**  proteins (below).  The file Evdw.dat is such a data file and is part
**  of the emboss distribution.
**  
**  DE   File of van der Waals radii for atoms in proteins
**  XX
**  NR   24
**  XX
**  AA   ALA
**  XX
**  ID   A 
**  XX
**  NN   12  
**  XX
**  AT   N    ; 1.7
**  AT   CA   ; 1.9
**  AT   C    ; 1.7   
**  AT   O    ; 1.4   
**  AT   CB   ; 1.9   
**  AT   OXT  ; 1.4
**  AT   H    ; 1.2
**  AT   OH   ; 1.4
**  AT   HA   ; 1.2
**  AT   HB   ; 1.2   
**  AT   HG   ; 1.2   
**  AT   D    ; 1.2
**  //
**  AA   ARG
**  XX
**  ID   R 
**  XX
**  NN   31
**  XX
**  AT   N    ; 1.7   
**  AT   CA   ; 1.9   
**  AT   C    ; 1.7   
**  AT   O    ; 1.4   
**  AT   N    ; 1.7
**  
**  < data ommitted for clarity >
**  
**  //
**  AA   XAA
**  XX
**  ID   X  
**  XX
**  NN   6
**  XX
**  AT   C    ; 1.9
**  AT   N    ; 1.7
**  AT   O    ; 1.4
**  AT   H    ; 1.2
**  AT   S    ; 1.8
**  AT   D    ; 1.2
**  //
**  
**  
**  
**  Diagnostic error messages
**  contacts generates a log file an excerpt of which is shown (Figure 2). If 
**  there is a problem in processing a coordinate file, three lines containing 
**  the record '//', the scop domain or pdb identifier code and an error message 
**  respectively are written. The text 'WARN  file open error filename',
**  'ERROR file read error filename' or 'ERROR file write error filename ' will 
**  be reported when an error was encountered during a file open, read or write 
**  respectively.  Various other error messages may also be given (in case of 
**  difficulty email Jon Ison, jison@hgmp.mrc.ac.uk).
**
**  Excerpt of log file
**  //
**  DS002__
**  WARN  Could not open for reading cpdb file s002.pxyz
**  //
**  DS003__
**  WARN  Could not open for reading cpdb file s003.pxyz
**
**
**
**  Authors
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
#include "math.h"

static AjBool contacts_WriteFile(AjPFile logf, AjPFile outf, float thresh, 
				 float ignore, AjPPdb pdb, AjPVdwall vdw, 
				 AjBool skip);

static AjBool contacts_ContactMapWrite(AjPFile outf, AjPInt2d mat, char *txt, 
				       ajint mod, ajint chn, AjPPdb pdb);

static AjBool contacts_ContactMapCalc(AjPInt2d *mat, ajint *ncon, ajint dim, 
				       float thresh, float ignore, ajint mod, 
				      ajint chn, AjPPdb pdb, AjPVdwall vdw, 
				      AjBool skip);





/* @prog contacts *******************************************************
**
** Reads coordinate files and writes files of intra-chain residue-residue 
** contact data.
**
******************************************************************************/

int main(ajint argc, char **argv)
{
    AjPList     cpdb_path     =NULL;	/* Directory of cpdb files */
    AjPStr     cpdb_name     =NULL;	/* Name of cpdb file */
    AjPDir     con_path      =NULL;	/* Directory of contact files */
    AjPStr     con_name      =NULL;	/* Name of contact file */
    AjPStr     msg           =NULL;	/* Error message */
    AjPStr     temp          =NULL;	/* Temp string */

    AjPFile    cpdb_inf      =NULL;     /* cpdb input file pointer */
    AjPFile    con_outf      =NULL;     /* contact output file pointer */
    AjPFile    logf          =NULL;     /* log file pointer*/
    AjPFile    vdwf          =NULL;     /* van der Waals file pointer*/
/*    AjPStr     vdwfstr       =NULL; */
    
    AjPPdb     pdb           =NULL;

    float      thresh        =0;
    float      ignore        =0;
    AjBool     skip          = ajFalse; /* Whether to calculate contacts 
					   between residue adjacenet in 
					   sequence. */
    AjPVdwall  vdw           =NULL;     /* Structure for vdw radii */
    

    /* JISON DEBUG 
    AjBool found=ajFalse; */
    
    
    
    /* Initialise strings */
    temp          = ajStrNew();
    msg           = ajStrNew();
    cpdb_name     = ajStrNew();
    con_name      = ajStrNew();
/*    vdwfstr       = ajStrNew(); */
    

    /* Read data from acd */
    ajNamInit("emboss");
    ajAcdInitP("contacts",argc,argv,"DOMAINATRIX"); 
    cpdb_path     = ajAcdGetDirlist("cpdb");
    con_path      = ajAcdGetDirectory("con");
    logf          = ajAcdGetOutfile("conerrf");
    thresh        = ajAcdGetFloat("thresh");
    skip          = ajAcdGetBool("skip");
    ignore        = ajAcdGetFloat("ignore");
/*    vdwfstr       = ajAcdGetString("vdwf");*/
    vdwf       = ajAcdGetDatafile("vdwf");



    
    /* Allocate and read Vdwall object */
    /*
    ajFileDataNew(vdwfstr,&vdwf);
    if(!vdwf)
	ajFatal("Cannot open %S",vdwfstr); */

    if(!(vdw=ajVdwallReadNew(vdwf)))
	ajFatal("Error reading vdw radii file\n");



    /*Start of main application loop*/
    while(ajListPop(cpdb_path,(void **)&temp))
    {
/* 
This error was also reported - could easily be more like this
D1CQNB_
Warning: Zero sized list of sequences passed into contacts_ContactMapCalc
Warning: ERROR  file write error /data/structure/con_new/d1qjha_.con
rm /data/structure/con_new/d1qjha_.conD1G1XA_
*/

	/* JISON DEBUG start
	if(ajStrFindC(temp, "d1bd7a_")!=-1)  BUILD1_1 
	if(ajStrFindC(temp, "d1du1a_")!=-1)  BUILD3_1 
	if(ajStrFindC(temp, "d1f02t_")!=-1)  BUILD3_2 
	if(ajStrFindC(temp, "d1du1a_")!=-1)  BUILD3_3 
	if(ajStrFindC(temp, "d1f3mb_")!=-1)  BUILD3_3 
	if(ajStrFindC(temp, "d1dt7x_")!=-1)  BUILD3_3 
	{                       
	    found=ajTrue;
	    continue;
	}
	

	if(!found)
	    continue;

	if((ajStrFindC(temp, "d1bd7a_")!=-1)||
	   (ajStrFindC(temp, "d1bd7b_")!=-1)||
	   (ajStrFindC(temp, "d1f02t_")!=-1)||
	   (ajStrFindC(temp, "d1du1a_")!=-1)||
	   (ajStrFindC(temp, "d1f3mb_")!=-1)||
	   (ajStrFindC(temp, "d1dt7x_")!=-1))
	    continue;
	
	JISON DEBUG end*/


	/* Read clean coordinate file*/
	if((cpdb_inf=ajFileNewIn(temp))==NULL)
	{
	    ajFmtPrintS(&msg, "Could not open for reading %S", 
			temp);
	    ajWarn(ajStrStr(msg));
	    ajFmtPrintF(logf, "WARN  Could not open for reading %S\n", 
			temp);
	    ajFileClose(&cpdb_inf);
	    ajStrDel(&temp);	
	    continue;	    
	}


	/* Read pdb structure */
	if(!(pdb=ajPdbReadNew(cpdb_inf)))
	{
	    ajFmtPrintS(&msg, "ERROR file read error %S", temp);
	    ajWarn(ajStrStr(msg));
	    ajFmtPrintF(logf, "ERROR  file read error %S\n", temp);
	    ajFileClose(&cpdb_inf);
	    ajPdbDel(&pdb);
	    ajStrDel(&temp);	
	    continue;
	}

	/* Write diagnostic */
	ajFmtPrint("%S\n", pdb->Pdb);   
	fflush(stdout);
	ajFmtPrintF(logf, "%S\n", pdb->Pdb);   



	/* Open contact file for writing*/
	ajStrAssS(&con_name, pdb->Pdb);
	ajStrToLower(&con_name);
	if(!(con_outf=ajFileNewOutDir(con_path, con_name)))
	{
	    ajFmtPrintS(&msg, "ERROR file open error %S", 
			con_name);
	    ajWarn(ajStrStr(msg));
	    ajFmtPrintF(logf, "ERROR file open error %S\n", con_name);
	    ajFileClose(&cpdb_inf);
	    ajFileClose(&con_outf);
	    ajPdbDel(&pdb);
	    ajStrDel(&temp);	
	    continue;
	}  

	
	/* Write contacts file */
	if(!contacts_WriteFile(logf, con_outf, thresh, ignore, pdb, vdw, 
			       skip))
	{
	    ajFmtPrintS(&msg, "ERROR  file write error %S", con_name);
	    ajWarn(ajStrStr(msg));
	    ajFmtPrintF(logf, "ERROR  file write error %S\n", con_name);

	    ajFmtPrintS(&temp, "rm %S", con_name);
	    ajFmtPrint("%S", temp);
	    ajSystem(temp);

	    ajFileClose(&cpdb_inf);
	    ajFileClose(&con_outf);
	    ajPdbDel(&pdb);
	    ajStrDel(&temp);	
	    continue;
	}


	/* Tidy up*/
	ajFileClose(&cpdb_inf);
	ajFileClose(&con_outf);
	ajPdbDel(&pdb);
	ajStrDel(&temp);	
    }
    /*End of main application loop*/    



    /*Tidy up */
    ajListDel(&cpdb_path);
    ajStrDel(&cpdb_name);
    ajDirDel(&con_path);
    ajStrDel(&con_name);
    ajStrDel(&msg);
/*    ajStrDel(&vdwfstr); */
    
    ajFileClose(&logf);
    ajFileClose(&vdwf);

    ajVdwallDel(&vdw);
    

    /* Return */
    ajExit();
    return 0;
}	





/* @funcstatic contact_WriteFile *****************************************
**
** Writes a file of INTRA-chain residue contact data in embl-like format 
** for a pdb structure.
**
** @param [w] logf   [AjPFile] Output file stream (log file)
** @param [w] outf   [AjPFile] Output file stream (contact file)
** @param [r] thresh [float]   Threshold contact distance
** @param [r] ignore [float]   Threshold ignore distance
** @param [r] pdb    [AjPPdb]  Pdb object
** @param [r] vdw    [AjPVdwall]   Vdwall object
** @param [r] skip   [AjBool]     Whether to calculate contacts between 
**                                residue adjacenet in sequence.
**
** @return [AjBool] True on success
** @@
**
******************************************************************************/

static AjBool contacts_WriteFile(AjPFile logf, AjPFile outf, float thresh, 
				 float ignore, AjPPdb pdb, AjPVdwall vdw, 
				 AjBool skip)
{
    AjPInt2d    mat   =NULL;		/* Contact map */
    ajint       x,y,z;			/* Loop counters */
    ajint       ncon  =0;		/* No. contacts (1's) in matrix */
    




    /* Error checking on args */
    if(!logf || !outf || !pdb)
	return ajFalse;
    
    
    /* Print out header data */
    ajFmtPrintF(outf, "%-5s%S\n", "ID", pdb->Pdb);
    ajFmtPrintF(outf, "XX\n");
    ajFmtPrintF(outf, "%-5s%s\n", 
		"DE", "Residue-residue side-chain contact data");
    ajFmtPrintF(outf, "XX\n");
    ajFmtPrintF(outf, "%-5s%s%.1f; %s%.1f; NMOD %d; NCHA %d;\n", "EX", 
		"THRESH ", thresh, "IGNORE ", ignore, pdb->Nmod, pdb->Nchn);


    /* Start of loop to print out data for each model / chain */
    for(x=0;x<pdb->Nmod;x++)
    {
	/* Print out model-specific data */
	ajFmtPrintF(outf, "XX\n%-5s[%d]\n", "MO", x+1);	
	ajFmtPrintF(outf, "XX\n");	


	for(y=0;y<pdb->Nchn;y++)
	{
	    /* Cope with chains with no residues */
	    if(pdb->Chains[y]->Nres == 0)
	    {
		/* Print out chain-specific data */
		ajFmtPrintF(outf, "%-5s[%d]\n", "CN", y+1);	
		ajFmtPrintF(outf, "XX\n");	
		ajFmtPrintF(outf, "%-5sID %c; NR %d; NSMCON 0;\n", 
			    "IN", 
			    pdb->Chains[y]->Id,
			    pdb->Chains[y]->Nres);
		ajFmtPrintF(outf, "XX\n");	

		/* Empty matrix ! */
		ajFmtPrintF(outf, ".\nXX\n");	
	    }
	    else
	    {
		/* Allocate memory for the contact map (a SQUARE 2d int array) */
		mat = ajInt2dNewL((ajint)pdb->Chains[y]->Nres);   	

		for(z=0;z<pdb->Chains[y]->Nres;++z)
		    ajInt2dPut(&mat, z, pdb->Chains[y]->Nres-1, (ajint) 0);
	    
		/* Write the contact map */
	
		if(!contacts_ContactMapCalc(&mat, &ncon, pdb->Chains[y]->Nres, 
					    thresh, ignore, x+1, y+1, pdb, vdw, 
					    skip))
		{
		    ajFmtPrintF(logf, "ERROR  Writing contact map\n");
		    ajInt2dDel(&mat);
		    return ajFalse;
		}

	    

		/* Print out chain-specific data */
		ajFmtPrintF(outf, "%-5s[%d]\n", "CN", y+1);	
		ajFmtPrintF(outf, "XX\n");	
		ajFmtPrintF(outf, "%-5sID %c; NR %d; NSMCON %d;\n", 
			    "IN", 
			    pdb->Chains[y]->Id,
			    pdb->Chains[y]->Nres,
			    ncon);
		ajFmtPrintF(outf, "XX\n");	
		ajSeqWriteXyz(outf, pdb->Chains[y]->Seq, "SQ");
		ajFmtPrintF(outf, "XX\n");	

		if(ncon)
		{
		    /*Matrix written OK and has at least 1 contact*/
		    contacts_ContactMapWrite(outf, mat, "SM", x+1, y+1, pdb);   
		    ajFmtPrintF(outf, "XX\n");	
		}
		else 
		{
		    /* Empty matrix ! */
		    ajFmtPrintF(outf, ".\nXX\n");	
		}


		/* Free memory for matrix */
		ajInt2dDel(&mat);
	    }
	    
	}
    }
    /* End of loop to print out data for each model / chain */
    ajFmtPrintF(outf, "//\n");    


    /* Return */
    return ajTrue;
}





/* @funcstatic contacts_ContactMapWrite **************************************
**
** Writes data in a contact map to file for a certain model and chain in a pdb 
** structure. The contact map must contain intra-chain contacts. Rows and
** columns in the square contact map correspond to residues in the chain.
**
** @param [w] outf [AjPFile]     Output file stream (contact file)
** @param [r] mat  [AjPInt2d]    Contact map
** @param [r] txt  [char*]        Text to print at start of each line
** @param [r] mod  [ajint]         Model number
** @param [r] chn  [ajint]         Chain number
** @param [r] pdb  [AjPPdb]      Pdb object
** 
** It writes a line to the output file for each contact. In the example below, 
** txt has a value of "SM":
** 
** SM   His 2 ; Val 8
** SM   Ala 6 ; Trp 21
** 
** @return [AjBool] True if file was succesfully written.
** @@
**
****************************************************************************/
static AjBool contacts_ContactMapWrite(AjPFile outf, AjPInt2d mat, char *txt,
				       ajint mod, ajint chn, AjPPdb pdb)
{
    ajint      x,y;			/* Loop counters */    
    AjPStr   res1;			/* ID of residue 1 */
    AjPStr   res2;			/* ID of residue 2 */


    /* Error checking on args */
    if(!outf || !mat || !pdb || !txt)
    {
	ajWarn("Null arg. passed to contacts_ContactMapWrite");
	return ajFalse;
    }
    

    /* Allocate strings */
    res1 = ajStrNew();
    res2 = ajStrNew();


    /* Start of loop for printing out contacts */
    for(x=0; x<pdb->Chains[chn-1]->Nres; x++)
	for(y=x+1; y<pdb->Chains[chn-1]->Nres; y++)
	{
	    if((ajInt2dGet(mat, x, y)==1))
	    {
		/* Assign residue id */
		if(!ajBaseAa1ToAa3(ajStrChar(pdb->Chains[chn-1]->Seq, x), 
				   &res1))
		{
		    ajStrDel(&res1);
		    ajStrDel(&res2);  
		    ajWarn("Index out of range in "
			   "contacts_PrintPdbSeqresChain");		
		    return ajFalse;
		}
		if(!ajBaseAa1ToAa3(ajStrChar(pdb->Chains[chn-1]->Seq, y), 
				   &res2))
		{
		    ajStrDel(&res1);
		    ajStrDel(&res2);  
		    ajWarn("Index out of range in "
			   "contacts_PrintPdbSeqresChain");		
		    return ajFalse;
		}
 		/* Print out the contact */
		ajFmtPrintF(outf, "%-5s%S %d ; %S %d\n", txt, res1, x+1, 
			    res2, y+1);	
	    }
	}

    
    /*Tidy up */
    ajStrDel(&res1);
    ajStrDel(&res2);  


    /* Return */
    return ajTrue;
}	




















/* @funcstatic contacts_ContactMapCalc **************************************
**
** Write a contact map for a certain model and chain in a pdb structure. The
** contact map must contain intra-chain contacts. Rows and columns in the 
** square contact map correspond to residues in the chain.
**
** @param [w] mat    [AjPInt2d *] Contact map
** @param [w] ncon   [ajint *]    No. contacts
** @param [r] dim    [ajint]      Dimension of matrix (no. residues) 
** @param [r] thresh [float]      Threshold distance at which contact between 
**                                two residues is defined.
** @param [r] ignore [float]      Threshold "ignore" distance - this is a 
**                                speed-up.  Contact is not checked for between 
**                                residues with CA atoms a further distance 
**                                apart than this.
** @param [r] mod    [ajint]      Model number
** @param [r] chn    [ajint]      Chain number
** @param [r] pdb    [AjPPdb]     Pdb object
** @param [r] vdw    [AjPVdwall]  Vdwall object
** @param [r] skip   [AjBool]     Whether to calculate contacts between 
**                                residue adjacenet in sequence.
** 
** @return [AjBool] True if file was succesfully written.
** @@
**
****************************************************************************/
static AjBool contacts_ContactMapCalc(AjPInt2d *mat, ajint *ncon, ajint dim, 
				      float thresh, float ignore, ajint mod, 
				      ajint chn, AjPPdb pdb, AjPVdwall vdw,
				      AjBool skip)
{	
    /* Contact is checked for between two residues, residue 1 and residue 2 */
    
    
    AjPAtom    *arr  =NULL;   /* Array of AjPAtom objects from list of 
				 AjPAtom objects for chain <chn> from <pdb>*/
    ajint       siz  =0;      /* Size of <arr> */
    
    
    ajint       idxfirst=0;   /* Index in <arr> of first atom belonging to 
				 model <mod> */
    ajint       idxlast=0;    /* Index in <arr> of last atom belonging to 	
				 model <mod> */
    
    
    ajint       resfirst=0;   /* Residue number of first atom belonging 
				 to model <mod> */
    ajint       reslast=0;    /* Residue number of last atom belonging 
				 to model <mod> */
    
    
    ajint       idx1=0;       /* Index in <arr> for atom from residue 1 */
    ajint       idx2=0;       /* Index in <arr> for atom from residue 2 */
    
    ajint       idx1first=0;  /* Index in <arr> of first atom from residue 1 */
    ajint       idx2first=0;  /* Index in <arr> of first atom from residue 2 */
    
    ajint       res1=0;       /* Residue number of residue 1 */
    ajint       res2=0;       /* Residue number of residue 2 */
    
    AjBool      done=ajFalse; /* Flag */
    
    float       dis=0.0;      /* Inter-atomic distance */
    ajint      offset        = 0;
    

    /*JC this block */
    /*
    ajint x=0;
    
    xxxtempstr=ajStrNew();
    ajStrAssC(&xxxtempstr, "bolox");
    xxxtemp=ajFileNewOut(xxxtempstr); 
    */
    /*JC*/


    /* Error checking on args */
    if(!mat || !pdb || !chn || !mod)
    {	
	ajWarn("Null or zero arg. passed to contacts_ContactMapCalc");
	return ajFalse;
    }
    

    /* Initialise no. contacts to zero */
    (*ncon)=0;
    
    
    /*Convert the AjPList of atoms to an array of AjPAtom*/
    if(!(siz=ajListToArray((AjPList)pdb->Chains[chn-1]->Atoms,
			   (void ***)&arr)))
    {
	ajWarn("Zero sized list of sequences passed into "
	       "contacts_ContactMapCalc");
	return ajFalse;
    }


    if(skip) 
	offset = 2;
    else	
        offset = 1;



    /*JC WAS RETURNING HERE */


/*    printf("...");
    fflush(stdout); */

/*    
    ajFmtPrintF(xxxtemp, "Last node %d res %d\n", siz-1, arr[siz-1]->Idx);
    ajFmtPrint("Last node %d res %d\n", siz-1, arr[siz-1]->Idx);    
  */
  


    /*JC*/	
    /*
    ajFmtPrintF(xxxtemp, "%5S\n", pdb->Pdb);
    ajFmtPrintF(xxxtemp, "siz : %d\n", siz);
*/
    

    /*
    for(x=0;x<siz;x++)
    {
	ajFmtPrintF(xxxtemp, "idx%5d res%5d\n", x, arr[x]->Idx);
	fflush(xxxtemp->fp);
    }
    */
    

    
    /* Find index in <arr> of first atom belonging to correct model */
    for(done=ajFalse, idxfirst=0; idxfirst<siz; idxfirst++) 
	/* Find the correct model */
	if(arr[idxfirst]->Mod==mod && arr[idxfirst]->Type=='P')
	{
	    done=ajTrue;
	    resfirst=arr[idxfirst]->Idx;
	    break;
	}
    if(!done)
    {
	ajWarn("Model not found in contacts_ContactMapCalc");
	AJFREE(arr);
	return ajFalse;
    } 
    
    
    /* Find index in <arr> of last atom belonging to correct model */
    for(idxlast=idxfirst; idxlast<siz; idxlast++)
    {
	if(arr[idxlast]->Mod!=mod  || arr[idxlast]->Type!='P')
	{
	    idxlast--;
	    break;	
	}
    }
    

    
    if(idxlast==siz)
	idxlast--;
    if(idxlast == idxfirst)
    {
	ajWarn("No atoms for the chain in contacts_ContactMapCalc");
	AJFREE(arr);
	return ajFalse;
    } 
    reslast=arr[idxlast]->Idx;
    



    
    /*Loop for first residue */
    for(res1=resfirst,  idx1=idxfirst; res1<reslast; res1++)
    {
	/*Assign position of first atom of res1 */
	for(done=ajFalse, idx1first=idx1; idx1first<idxlast; idx1first++)
	    if(arr[idx1first]->Idx == res1)
	    {
		done=ajTrue;
		break;
	    }
	if(!done)
	    continue;
		
	/* Loop for second residue */
	/* Offset is normally 1 but will be 2 if contacts between adjacent
	   residue are not calculated */
	for(res2=res1+offset, idx2=idx1; res2<=reslast; res2++)
	{
	    /*Assign position of first atom of res2 */
	    for(done=ajFalse, idx2first=idx2; idx2first<idxlast; idx2first++)
		if(arr[idx2first]->Idx == res2)
		{
		    done=ajTrue;
		    break;
		}
	    if(!done)
		continue;
	
	    /*Loop for atoms in first residue */
	    for(done=ajFalse, idx1=idx1first; idx1<siz; idx1++)
	    {
		if(arr[idx1]->Idx != res1)
		    break;
		
		/*Loop for atoms in second residue */
		for(idx2=idx2first; idx2<siz; idx2++)
		{
		    if(arr[idx2]->Idx != res2)
			break;
		    
		    /*ajFmtPrintF(xxxtemp, "Trying%5d%5d (res %5d-%-5d)...", 
		      idx1, idx2,arr[idx1]->Idx, arr[idx2]->Idx);
		    fflush(xxxtemp->fp); */
		    

/*		    if(embAtomInContact(arr[idx1], arr[idx2], thresh, vdw))		 */
		    if((dis = embAtomDistance(arr[idx1], arr[idx2], vdw))<=thresh)
		    {
/*			ajFmtPrintF(xxxtemp, "CONTACT\n");
			fflush(xxxtemp->fp);  */

			/* Increment no. contacts and write contact map */
			(*ncon)++;

			if((arr[idx1]->Idx==0)||(arr[idx2]->Idx==0))
			{
			    ajWarn("Indexing error, contact not written !\n");
			}
			else
			{
			    ajInt2dPut(mat, arr[idx1]->Idx-1, arr[idx2]->Idx-1, (ajint) 1);
			    ajInt2dPut(mat, arr[idx2]->Idx-1, arr[idx1]->Idx-1, (ajint) 1); 
			}
			
			done=ajTrue;
			break;
		    }
		    else if(dis >= ignore)
			{
			    /*The atoms are so far apart we should not bother finding
			      any more distances for this pair of atoms */
			    done=ajTrue;
			    break;
			}
		    
		    /* ajFmtPrintF(xxxtemp, "NO CONTACT\n");  */

		}	
		if(done)
		    break;
	    }
	}
    }


    
    /* Tidy up */
    AJFREE(arr);
    

    /* Return */
    return ajTrue;
}











    
    

