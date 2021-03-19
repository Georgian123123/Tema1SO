OBJ_LIST = so-cpp.obj
CFLAGS   = /nologo  /EHsc /MD
 
EXE_NAMES = so-cpp.exe

build : $(OBJ_LIST)
  cl $(CFLAGS) $** /Fe$(EXE_NAMES)

so-cpp.obj: so-cpp.c hashmap.c utilities.c hashmap.h utilities.h
  cl /c $(CFLAGS) $**
  
clean : exe_clean obj_clean
 
obj_clean :
  del *.obj
 
exe_clean :
  del $(EXE_NAMES)