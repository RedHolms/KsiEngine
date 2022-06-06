@echo off
setlocal

@REM Available flags:
@REM  -s    Build a static lib
@REM  -d    Build a debug version

set KSI_LIB_DIR=%cd%\lib
set KSI_BIN_DIR=%cd%\bin
set KSI_SRC_DIR=%cd%\src
set KSI_OBJ_DIR=%cd%\temp
set KSI_INC_DIR=%cd%\include\ksiEngine

set KSI_LIBS= User32.lib D3D11.lib d3dcompiler.lib Winmm.lib

set KSI_CC_FLAGS= /DKSI_BUILD /O2
set KSI_LK_FLAGS= /DLL
set KSI_LB_FLAGS=

set KSI_OBJ_EXT=o

:: without extensions
set KSI_SOURCES=^
   Camera ^
   Mesh ^
   GameObject ^
   Renderer ^
   Input ^
   Texture ^
   ObjReader ^
   Engine

:: with extensions
set KSI_INCLUDE=^
   *.hpp ^
   *.inl

::: End of config :::

for %%a in (%*) do (
   if "%%a"=="-s"          ( set KSI_STATIC=1
   ) else if "%%a"=="-d"   ( set KSI_DEBUG=1
   ) else (
      if "%%a"=="all" if exist %KSI_OBJ_DIR% rmdir /S /Q %KSI_OBJ_DIR%
      if exist %KSI_OBJ_DIR%\%%a.%KSI_OBJ_EXT% del %KSI_OBJ_DIR%\%%a.%KSI_OBJ_EXT%
      if exist %KSI_OBJ_DIR%\%%a.cso del %KSI_OBJ_DIR%\%%a.cso
      if exist %KSI_OBJ_DIR%\%%a del %KSI_OBJ_DIR%\%%a
   )
)

set KSI_ORIGIN=%cd%

set KSI_CC_=cl /nologo /c
set KSI_LK_=link /nologo
set KSI_LB_=lib /nologo

if defined KSI_STATIC set KSI_CC_FLAGS=%KSI_CC_FLAGS% /DKSI_STATIC
if defined KSI_DEBUG (
   set KSI_LK_FLAGS=%KSI_LK_FLAGS% /DEBUG
   set KSI_CC_FLAGS=%KSI_CC_FLAGS% /DKSI_DEBUG /Zi
)

set KSI_CC=%KSI_CC_%%KSI_CC_FLAGS%
set KSI_LK=%KSI_LK_%%KSI_LK_FLAGS%%KSI_LIBS%
set KSI_LB=%KSI_LB_%%KSI_LB_FLAGS%%KSI_LIBS%

rmdir /S /Q %KSI_INC_DIR%
rmdir /S /Q %KSI_LIB_DIR%
rmdir /S /Q %KSI_BIN_DIR%

if not exist %KSI_LIB_DIR% mkdir %KSI_LIB_DIR%
if not exist %KSI_BIN_DIR% mkdir %KSI_BIN_DIR%
if not exist %KSI_OBJ_DIR% mkdir %KSI_OBJ_DIR%
if not exist %KSI_INC_DIR% mkdir %KSI_INC_DIR%

pushd %KSI_SRC_DIR%
for %%f in (%KSI_INCLUDE%) do if exist %%f copy %%f %KSI_INC_DIR% > nul

if not exist %KSI_OBJ_DIR%\pixel.cso   fxc /nologo /T ps_5_0 pixel.hlsl /Fo:%KSI_OBJ_DIR%\pixel.cso
if not exist %KSI_OBJ_DIR%\vertex.cso  fxc /nologo /T vs_5_0 vertex.hlsl /Fo:%KSI_OBJ_DIR%\vertex.cso

for %%f in (%KSI_SOURCES%) do (
   if not exist %KSI_OBJ_DIR%\%%f.%KSI_OBJ_EXT% %KSI_CC% /Fo:%KSI_OBJ_DIR%\%%f.%KSI_OBJ_EXT% %%f.cpp
   if errorlevel 1 (
      echo.
      echo Compile error
      exit /b 1
   )
)

if exist *.pdb move *.pdb %KSI_OBJ_DIR%
popd

if defined KSI_STATIC (
   %KSI_LB% %KSI_OBJ_DIR%\*.%KSI_OBJ_EXT% /out:%KSI_LIB_DIR%\ksiEngine.lib
   if errorlevel 1 (
      echo.
      echo Link error
      exit /b 1
   )
) else (
   %KSI_LK% %KSI_OBJ_DIR%\*.%KSI_OBJ_EXT% /out:%KSI_BIN_DIR%\ksiEngine.dll /implib:%KSI_LIB_DIR%\ksiEngine.lib
   if errorlevel 1 (
      echo.
      echo Link error
      exit /b 1
   )
)
if exist %KSI_OBJ_DIR%\*.pdb copy %KSI_OBJ_DIR%\*.pdb %KSI_LIB_DIR%

endlocal