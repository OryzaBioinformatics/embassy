#

SUBDIRS = src emboss_acd

EXTRA_DIST = copyright depcomp ltmain.sh install-sh config.sub config.guess

dist-hook:
	tar cBf - emboss_acd | ( cd $(distdir); tar xBf - ; cd emboss_acd; rm -rf CVS)

