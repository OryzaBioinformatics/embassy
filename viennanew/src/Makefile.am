## Process this file with automake to produce Makefile.in

if ISCYGWIN
CYGWIN_LDFLAGS = -no-undefined -Wl,--add-stdcall-alias -Wl,--export-all-symbols -Wl,--enable-auto-import
if LOCALLINK
CYGWIN_LDVIENNA = -L../../../plplot -L../../../ajax/pcre \
-L../../../ajax/expat -L../../../ajax/zlib \
-L../../../ajax/core -L../../../ajax/graphics -L../../../ajax/ensembl \
-L../../../ajax/ajaxdb -L../../../ajax/acd -L../../../nucleus \
-lnucleus -lacd -lajaxdb -lensembl -lajaxg -lajax -leplplot -leexpat \
-lezlib -lepcre
else
CYGWIN_LDVIENNA = -L${embprefix}/lib -lnucleus -lacd -lajaxdb -lensembl \
                -lajaxg -lajax -leplplot -leexpat -lezlib -lepcre
endif
endif

if !ESYSTEMLIBS
LLINCLUDES = -I../../../ajax/expat -I../../../ajax/zlib
NLINCLUDES = -I${embprefix}/include/ezlib -I${embprefix}/include/eexpat
LLAIXLIBS  = -L../../../ajax/expat/.libs -L../../../ajax/zlib/.libs
NLAIXLIBS  = -leexpat -lezlib
LLADD = ../../../ajax/expat/libeexpat.la ../../../ajax/zlib/libezlib.la
NLADD = -leexpat -lezlib
endif


if LOCALLINK
AM_CPPFLAGS = -I../H -I../../../nucleus -I../../../ajax/pcre \
	$(LLINCLUDES) \
	-I../../../ajax/core -I../../../ajax/graphics \
	-I../../../ajax/ensembl -I../../../ajax/ajaxdb \
	-I../../../ajax/acd -I../../../plplot
else
AM_CPPFLAGS = -I../H -I${embprefix}/include -I${embprefix}/include/eplplot \
	$(NLINCLUDES) \
	-I${embprefix}/include/epcre
endif

if ISSHARED
if ISAIXIA64
if LOCALLINK
AIX_CFLAGS = -Wl,-bdynamic -Wl,-brtl -L../../../plplot/.libs \
-L../../../ajax/pcre/.libs $(LLAIXLIBS) \
-L../../../ajax/core/.libs \
-L../../../ajax/graphics/.libs -L../../../ajax/ensembl/.libs \
-L../../../ajax/ajaxdb/.libs -L../../../ajax/acd/.libs \
-L../../../nucleus/.libs \
-lnucleus -lacd -lajaxdb -lensembl -lajaxg -lajax -leexpat -lepcre \
 $(NLAIXLIBS) -leplplot
else
AIX_CFLAGS = -Wl,-bdynamic -Wl,-brtl -L${embprefix}/lib -lnucleus -lacd \
-lajaxdb -lensembl -lajaxg -lajax -leexpat -lepcre $(NLAIXLIBS) -leplplot
endif
endif
endif


AM_CFLAGS =  $(AIX_CFLAGS) $(WARN_CFLAGS) $(DEVWARN_CFLAGS)

lib_LTLIBRARIES = liboviennarna.la

bin_PROGRAMS = ovrnaalifold ovrnacofoldpf ovrnaevalpair ovrnainverse \
	ovrnaalifoldpf ovrnadistance ovrnafold ovrnalfold \
	ovrnacofold ovrnaduplex ovrnafoldpf ovrnaplot \
	ovrnacofoldconc ovrnaeval ovrnaheat ovrnasubopt

VIENNALIBSRC = dist_vars.c fold_vars.c part_func.c eread_epars.c \
        treedist.c energy_par.c inverse.c ProfileDist.c RNAstruct.c utils.c \
        fold.c naview.c ePS_dot.c stringdist.c params.c subopt.c list.c \
        cofold.c eLfold.c epart_func_co.c part_func_up.c ProfileAln.c \
	duplex.c alifold.c alipfold.c aln_util.c getconstraints.c


noinst_HEADERS = intloops.h list.h

EXTRA_DIST = alicircfold.inc circfold.inc config.h

liboviennarna_la_SOURCES = $(VIENNALIBSRC)

VERS_INF = -version-info 1:7:0

if ISCYGWIN
LINKFLAGS = $(VERS_INF) $(CYGWIN_LDFLAGS) $(CYGWIN_LDVIENNA)
else
LINKFLAGS = $(VERS_INF)
endif

liboviennarna_la_LDFLAGS = $(LINKFLAGS)

ovrnaalifold_SOURCES = vrnaalifold.c
ovrnaalifoldpf_SOURCES = vrnaalifoldpf.c
ovrnacofold_SOURCES = vrnacofold.c
ovrnacofoldconc_SOURCES = vrnacofoldconc.c
ovrnacofoldpf_SOURCES = vrnacofoldpf.c
ovrnadistance_SOURCES = vrnadistance.c
ovrnaduplex_SOURCES = vrnaduplex.c
ovrnaeval_SOURCES = vrnaeval.c
ovrnaevalpair_SOURCES = vrnaevalpair.c
ovrnafold_SOURCES = vrnafold.c
ovrnafoldpf_SOURCES = vrnafoldpf.c
ovrnaheat_SOURCES = vrnaheat.c
ovrnainverse_SOURCES = vrnainverse.c
ovrnalfold_SOURCES = vrnalfold.c
ovrnaplot_SOURCES = vrnaplot.c
ovrnasubopt_SOURCES = vrnasubopt.c


if LOCALLINK
LDADD = liboviennarna.la \
	../../../nucleus/libnucleus.la ../../../ajax/acd/libacd.la \
	../../../ajax/ajaxdb/libajaxdb.la \
	../../../ajax/ensembl/libensembl.la \
	../../../ajax/graphics/libajaxg.la \
	../../../ajax/core/libajax.la \
	$(LLADD) \
	../../../ajax/pcre/libepcre.la \
	../../../plplot/libeplplot.la \
	$(XLIB)
else
LDADD = liboviennarna.la -L${embprefix}/lib -lnucleus -lacd -lajaxdb \
	-lensembl -lajaxg -lajax -lepcre $(NLADD) -leplplot $(XLIB)
endif
