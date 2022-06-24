/* @source demofilelist application
**
** Demonstration of how the filelist datatype should be used.

** @author Copyright (C) Peter Rice (pmr@ebi.ac.uk)
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

#include "emboss.h"

/* @prog demofilelist *********************************************************
**
** Testing
**
******************************************************************************/


int main (int argc, char *argv[]) 
{
    AjPList filelist;
    AjPStr filename = NULL;
    AjPFile infile;
    AjPFile outfile;
    AjIList iter;
    AjPStr line = NULL;

    ajuint nfiles = 0;
    ajuint nlines = 0;
    ajuint nchars = 0;

    embInitPV("demofilelist", argc, argv, "myembossdemo", VERSION);

    filelist = ajAcdGetFilelist("infile");
    outfile = ajAcdGetOutfile("outfile");

    iter = ajListIterNewread(filelist);
    while(!ajListIterDone(iter))
    {
        filename = (AjPStr) ajListIterGet(iter);
        infile = ajFileNewInNameS(filename);

        if(!infile) 
        {
            ajFmtPrintF(outfile, "%S FILE NOT FOUND\n", filename);
        }
        else 
        {
            nfiles++;
            nlines = 0;
            nchars = 0;
            while(ajReadline(infile, &line))
            {
                nlines++;
                nchars += ajStrGetLen(line);
            }
            ajFmtPrintF(outfile, "%F: %u lines, %u chars\n",
                        infile, nlines, nchars);
            ajFileClose(&infile);
        }
    }

    ajListIterDel(&iter);

    ajFmtPrintF(outfile, "Read %u files\n", nfiles);

    ajFileClose(&outfile);

    ajListstrFreeData(&filelist);
    ajStrDel(&line);

    embExit();

    return 0;
}
