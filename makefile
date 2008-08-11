# $Id: makefile,v 19.2 2003/09/26 16:39:17 hravlin Exp $

# Use the generic AIPS++ application makefile.
#---------------------------------------------
XLIBLIST := GLISH PGPLOT TCL Motif X11 WCSLIB LAPACK F77 MIRLIB
XLIBLIST_shared := GLISH XmPGPLOT XPM Motif TCL X11 WCSLIB LAPACK F77
CPPMODU := -DGLISH -DPGPLOT -DUSE_UNAME -DAIPSPLUSPLUS -DTCL
# Uncommenting this will enable MIRIAD support.
#CPPMODU += -DMIRIAD
include $(word 1, $(AIPSPATH))/code/install/makefile.app
