# Microsoft Visual C++ generated build script - Do not modify

PROJ = MATHWARS
DEBUG = 0
PROGTYPE = 6
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = C:\DJGPP\SOURCE\PROJECTS\MATHWARS\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = MATH.C      
FIRSTCPP =             
RC = rc
CFLAGS_D_DEXE = /Gs /G2 /W4 /AM /O2 /D "NDEBUG" /D "_DOS" 
CFLAGS_R_DEXE = /Gs /G3 /W0 /AM /D "NDEBUG" /D "_DOS" 
LFLAGS_D_DEXE = /STACK:12000 
LFLAGS_R_DEXE = /STACK:12000 
LIBS_D_DEXE = oldnames mlibce gamelib blacklib mylib 
LIBS_R_DEXE = oldnames mlibce gamelib graphics 
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
OBJS_EXT = OPEN.OBJ 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_DEXE)
LFLAGS = $(LFLAGS_D_DEXE)
LIBS = $(LIBS_D_DEXE)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_DEXE)
LFLAGS = $(LFLAGS_R_DEXE)
LIBS = $(LIBS_R_DEXE)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = MATH.SBR


OPEN_DEP = 

all:	$(PROJ).EXE

MATH.OBJ:	MATH.C $(MATH_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c MATH.C

$(PROJ).EXE::	MATH.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
MATH.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
c:\msvc\lib\+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
