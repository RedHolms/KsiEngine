@echo off
setlocal
for %%f in (%*) do (
   if "%%f"=="-s" set _STATIC=1
)
if defined _STATIC ( set _CL_FLAGS=/DKSI_STATIC ) else set _CL_FLAGS=
cl /nologo /c%_CL_FLAGS% /I"..\include" main.cpp
link /nologo *.obj ..\lib\ksiEngine.lib User32.lib
del *.obj
if not defined _STATIC copy ..\bin\ksiEngine.dll .
copy ..\temp\vertex.cso .
copy ..\temp\pixel.cso .
endlocal