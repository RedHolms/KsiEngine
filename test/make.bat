@echo off
setlocal
for %%f in (%*) do (
   if "%%f"=="-s" set _STATIC=1
   if "%%f"=="-d" set _DEBUG=1
)
if defined _STATIC ( set _CL_FLAGS=/DKSI_STATIC ) else set _CL_FLAGS=
if defined _DEBUG (
   set _CL_FLAGS= %_CL_FLAGS%/DKSI_DEBUG /Zi
   set _LK_FLAGS= /DEBUG
   copy ..\bin\*.pdb .
   copy ..\bin\*.ilk .
) else set _LK_FLAGS=
cl /nologo /c%_CL_FLAGS% /I"..\include" main.cpp /Fo:main.o
link /nologo%_LK_FLAGS% *.o ..\lib\ksiEngine.lib User32.lib
del *.o *.ilk *.dll *.pdb *.cso
if not defined _STATIC copy ..\bin\ksiEngine.dll .
copy ..\temp\vertex.cso .
copy ..\temp\pixel.cso .
endlocal