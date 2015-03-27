# ---------------------------------------------------------------------------
VERSION = BCB.01
# ---------------------------------------------------------------------------
!ifndef BCB
BCB = $(MAKEDIR)\..
!endif
# ---------------------------------------------------------------------------
PROJECT = ProjInvad.exe
OBJFILES = ProjInvad.obj About.obj Invader.obj Options.obj PT_object.obj
RESFILES = ProjInvad.res
RESDEPEN = $(RESFILES) About.dfm Invader.dfm Options.dfm
LIBFILES =
DEFFILE =
# ---------------------------------------------------------------------------
CFLAG1 = -O2 -Hc -w- -k- -r -vi -c -a4 -b- -w-par -w-inl -Vx -Ve -x
CFLAG2 = -I$(BCB)\bin;c:\windows\desktop;$(BCB)\include;$(BCB)\include\vcl \
   -H=$(BCB)\lib\vcl.csm 
PFLAGS = -U$(BCB)\bin;c:\windows\desktop;$(BCB)\lib\obj;$(BCB)\lib \
   -I$(BCB)\bin;c:\windows\desktop;$(BCB)\include;$(BCB)\include\vcl \
   -AWinTypes=Windows;WinProcs=Windows;DbiTypes=BDE;DbiProcs=BDE;DbiErrs=BDE -$U \
   -$L- -$I- -$D- -JPHNV -M
RFLAGS = -i$(BCB)\bin;c:\windows\desktop;$(BCB)\include;$(BCB)\include\vcl 
LFLAGS = -L$(BCB)\bin;c:\windows\desktop;$(BCB)\lib\obj;$(BCB)\lib -aa -Tpe -x \
   -V4.0 
IFLAGS =
LINKER = ilink32
# ---------------------------------------------------------------------------
ALLOBJ = c0w32.obj $(OBJFILES)
ALLRES = $(RESFILES)
ALLLIB = $(LIBFILES) vcl.lib import32.lib cp32mt.lib 
# ---------------------------------------------------------------------------
.autodepend

$(PROJECT): $(OBJFILES) $(RESDEPEN) $(DEFFILE)
    $(BCB)\BIN\$(LINKER) @&&!
    $(LFLAGS) +
    $(ALLOBJ), +
    $(PROJECT),, +
    $(ALLLIB), +
    $(DEFFILE), +
    $(ALLRES) 
!

.pas.hpp:
    $(BCB)\BIN\dcc32 $(PFLAGS) { $** }

.pas.obj:
    $(BCB)\BIN\dcc32 $(PFLAGS) { $** }

.cpp.obj:
    $(BCB)\BIN\bcc32 $(CFLAG1) $(CFLAG2) -o$* $*

.c.obj:
    $(BCB)\BIN\bcc32 $(CFLAG1) $(CFLAG2) -o$* $**

.rc.res:
    $(BCB)\BIN\brcc32 $(RFLAGS) $<
#-----------------------------------------------------------------------------
