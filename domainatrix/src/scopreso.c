/* @source scopreso application
**
** Removes low resolution domains from a scop classification file.
**
** @author: Copyright (C) Matt Blades (mblades@hgmp.mrc.ac.uk)
** @author: Copyright (C) Jon Ison (jison@hgmp.mrc.ac.uk)
** @@
**
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
**  scoprep 
**  
**  
**  
**  Summary
**  Removes low resolution domains from a scop classification file.  
**  
**  
**  
**  Input and Output
**  scoprep reads a scop classification file and writes a scop classification 
**  file containing the same domains excluding those whose resolution (Angstrom)
**  of structure determination exceeds a user-defined threshold.  The resolution
**  data are taken from domain coordinate files whose path and file extension 
**  are specified by the user.  The input and output files are specified by the 
**  user.  
**  
**  
**   
**  Sister applications
**  A 'scop classification file' contains classification and other data for 
**  domains from the scop database.  The file is in embl-like format and is 
**  generated by scopparse.  
**  A 'domain coordinate file' contains coordinate and other data for a single
**  scop domain.  The files are generated by domainer and are in embl-like and
**  pdb formats.
**  domainer, funky, scopseqs, scopnr, scoprep, scopalign, seqsearch, seqnr and 
**  seqalign use a scop classification file as input.  However scopnr, seqnr and
**  seqalign require the file to contain domain sequence information, which can 
**  be added by using scopseqs.
**  
**  
**  
**  Notes
**  Domains that lack a value for resolutution in the domain coordinate file
**  (e.g. theoretical models and NMR structures) are included in the output
**  file.
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
**  It is desirable to omit poor resolution protein structures when performing
**  certain analyses of protein domains.  scopreso reads a scop classification
**  file and writes one containing the same domains excluding those whose 
**  resolution (Angstrom) of structure determination exceeds a user-defined 
**  threshold.
**  
**  
**  
**  Algorithm
**  
**  
**  
**  Usage 
**  An example of interactive use of scopreso is shown below.
**  
**  Unix % 
**  Unix % scopreso
**  Removes low resolution domains from a scop classification file.
**  Location of domain coordinate files for input (embl format) [./]: /test_data
**  Extension of coordinate files (embl format) [.pxyz]: .dxyz
**  Name of scop data file for input (embl format): /test_data/all.scop
**  Threshold for inclusion (Angstroms) [2.8]: 2.3
**  Name of scop data file for output [test.scop]: /test_data/all_2.3.scop
**  Unix %
**  
**  The protein classification file /test_data/all.scop was read and a protein
**  classificaiton file called /test_data/all_2.3.scop was written containing
**  only those domains of 2.3 Angstrom resolution or better.  Resolution data
**  were taken from domain coordinte files in the directory /test_data.
** 
**  The following command line would achieve the same result.
**  scopreso /test_data .dxyz /test_data/all.scop /test_data/all_2.3.scop -threshold 2.3
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
**  scopreso does not use any data files.
**  
**  
**  
**  Diagnostic error messages
**  
**  
**  
**  Authors
**  Matt Blades (mblades@hgmp.mrc.ac.uk)
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



#include <math.h>

#include "emboss.h"


ajint StrBinSearchScop(AjPStr id, AjPStr *arr, ajint siz);
ajint StrComp(const void *str1, const void *str2);



/* @prog scopreso *************************************************************
**

** Removes low resolution domains from a scop classification file.
**
******************************************************************************/
int main(ajint argc, char **argv)
{

    AjPStr      cpdb_path     = NULL;    /* Location of coordinate files for input */
    AjPStr      cpdb_extn     = NULL;    /* Extn. of coordinate files              */
    AjPStr      cpdb_name     = NULL;    /* Name of coordinate file                */
    AjPStr      temp          = NULL;    /* temp string                            */
    AjPStr      temp2         = NULL;    /* temp string                            */
    AjPList     entry        = NULL;     /* List of pdb codes with resolution      */
                                         /* ABOVE the threshold                    */
    AjPStr     *entryarr      = NULL;    /* entry as an array                      */
    

    AjPFile     fptr_cpdb     = NULL;    /* Pointer to current coordinate file     */
    AjPFile     scopinf       = NULL;    /* Escop.dat input file                   */
    AjPFile     scopoutf      = NULL;    /* New Escop.dat output file              */

    AjPList     list          = NULL;    /* List of files in alignment directory   */   

    AjPPdb      pdb           = NULL;    /* Pdb object pointer                     */    
    AjPScop     scop          = NULL;    /* Scop structure                         */
 

    float       threshold     = 0.0;     /* Resolution threshold                   */
    ajint       num           = 0;       /* number of nodes in list                */




    
    
                                           
    /* Read data from acd */
    ajNamInit("emboss");
    ajAcdInitP("scopreso",argc,argv,"DOMAINATRIX"); 
    cpdb_path     = ajAcdGetString("cpdbpath");    
    cpdb_extn     = ajAcdGetString("cpdbextn");    
    threshold     = ajAcdGetFloat("threshold");
    scopinf       = ajAcdGetInfile("scopinf");
    scopoutf      = ajAcdGetOutfile("scopoutf");


    /* Check args */
    if(!ajFileDir(&cpdb_path))
    {
	ajStrDel(&cpdb_path);
	ajStrDel(&cpdb_extn);
	ajFileClose(&scopoutf);
	ajFileClose(&scopinf);	    
	ajWarn("Could not open coordinate file directory");
	ajExit();
	return 1;
    }
    
    /* Allocate strings etc */
    cpdb_name     = ajStrNew();
    temp          = ajStrNew();

    /* Create list  */
    list     = ajListNew();
    entry    = ajListNew();


    /* Create list of files in CPDB directory */
    ajStrAssC(&temp, "*");      
    if((ajStrChar(cpdb_extn, 0)=='.'))
        ajStrApp(&temp, cpdb_extn);    
    else
    {
        ajStrAppC(&temp, ".");    
        ajStrApp(&temp, cpdb_extn);    
    }
    ajFileScan(cpdb_path, temp, &list, ajFalse, ajFalse, 
               NULL, NULL, ajFalse, NULL); 
    ajStrDel(&temp);
    

    /* Determine number of nodes on list    */
    num = ajListLength(list);
/*    printf("Number of cpdb files in directory = %d\n", num);*/



/*
 scopreso reads a directory of cpdb file, creates a list of the files, then
 reads every list entry and extracts the resolution of the structure.  If 
 the value is less than a threshold (user defined) then the scop identifier
 is pushed onto a list.  The Escop.dat file is then read and scop identifiers
 compared to those on the list, if found then the scop structure data is written 
 the new Escop_X.X.dat scop data file.    
*/

    /* Start of main application loop                         */
    /* Produce list of pdb codes with resolution              */
    /* ABOVE the threshold.                                   */
    while(ajListPop(list,(void **)&temp))
    {
        /* Open coordinate file */
        if((fptr_cpdb=ajFileNewIn(temp))==NULL)
        {
	    ajWarn("Could not open cpdb file");
            ajStrDel(&temp);
            continue;       
        }


        /* Read coordinate data file */ 
	pdb = ajPdbReadNew(fptr_cpdb);
        

        /* Check if resolution is above threshold */
        if(pdb->Reso > threshold)
	{
	    /* assign ID to list */
	    temp2=ajStrNew();
	    ajStrAssS(&temp2, pdb->Pdb);
	    ajListPush(entry, (AjPStr) temp2);
	}        
	
        /* Close coordinate file and tidy up*/
        ajPdbDel(&pdb);
        ajFileClose(&fptr_cpdb);
	ajStrDel(&temp);
    }
    num = ajListLength(entry);
/*    printf("Number of cpdb files with > threshold resolution = %d\n", num); */
    

    /* Sort the list of pdb codes & convert to an array */
    ajListSort(entry, StrComp);
    ajListToArray(entry, (void ***)&entryarr);
    
    
    /* Read Escop.dat and compare IDs to those in list          */
    /* if not present then write scop structure data to output  */
    while((scop=(ajScopReadCNew(scopinf, "*"))))
    {
	/* SCOP id not found in the list of domains with resolution 
	   above the threshold, so include it in the output file */
	if((StrBinSearchScop(scop->Entry, entryarr, num))==-1)
	    ajScopWrite(scopoutf, scop);

        /* Delete scop structure */
        ajScopDel(&scop);
    }


    /* Tidy up */
    ajStrDel(&cpdb_path);
    ajStrDel(&cpdb_extn);
    ajStrDel(&cpdb_name);
    ajFileClose(&scopoutf);
    ajFileClose(&scopinf);
    ajListDel(&list);
    while(ajListPop(list,(void **)&temp))
	ajStrDel(&temp);
    AJFREE(entryarr);
    
  
    /* Return */
    ajExit();
    return 0;
}





/* @func StrBinSearchScop *************************************************
**
** Performs a binary search for a SCOP domain code over an array of AjPStr
** (which of course must first have been sorted, e.g. by StrComp). This is a 
** case-insensitive search.
**
** @param [r] id  [AjPStr]      Search term
** @param [r] arr [AjPStr*]     Array of AjPStr objects
** @param [r] siz [ajint]       Size of array
**
** @return [ajint] Index of first AjPStr object found with an PDB code
** matching id, or -1 if id is not found.
** @@
******************************************************************************/
ajint StrBinSearchScop(AjPStr id, AjPStr *arr, ajint siz)
{
    int l;
    int m;
    int h;
    int c;


    l = 0;
    h = siz-1;
    while(l<=h)
    {
        m = (l+h)>>1;

        if((c=ajStrCmpCase(id, arr[m])) < 0)
	    h = m-1;
        else if(c>0) 
	    l = m+1;
        else 
	    return m;
    }

    return -1;
}




/* @func StrComp *******************************************************
**
** Function to sort strings.
**
** @param [r] str1  [const void*] AjPStr 1
** @param [r] str2  [const void*] AjPStr 2
**
** @return [ajint] -1 if str1 should sort before str2, +1 if the str2 should 
** sort first. 0 if they are identical in length and content. 
** @@
******************************************************************************/

ajint StrComp(const void *str1, const void *str2)
{
    AjPStr p = NULL;
    AjPStr q = NULL;

    p = (*(AjPStr*)str1);
    q = (*(AjPStr*)str2);
    
    return ajStrCmpO(p, q);
}








