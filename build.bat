@echo off

if not exist build mkdir build 
pushd build 

rem Name
set name=Pathfinder

rem Include directories 
set SDL=W:\SDL2

set inc=/I w:\gs
set SDL_INC=/I %SDL%\include
set SDL_LIB=%SDL%\lib\x64\SDL2.lib %SDL%\lib\x64\SDL2main.lib %SDL%\lib\x64\SDL2_image.lib 

rem Source files
:: set src_main=..\src\win32_platform.c
set src_main=..\src\gs_platform.c

rem OS Libraries
set os_libs= opengl32.lib kernel32.lib user32.lib ^
shell32.lib vcruntime.lib msvcrt.lib gdi32.lib Advapi32.lib

rem Link options
set l_options=/EHsc /link /SUBSYSTEM:CONSOLE /NODEFAULTLIB:msvcrt.lib

rem Compile Release
:: cl /MP /FS /Ox /W0 /Fe%name%.exe %src_all% %inc% ^
:: /EHsc /link /SUBSYSTEM:CONSOLE /NODEFAULTLIB:msvcrt.lib /NODEFAULTLIB:LIBCMT ^
:: %os_libs%

rem Compile Debug
cl /w /MP -Zi /DEBUG:FULL /Fe%name%.exe %src_main% %inc% ^
%l_options% /NODEFAULTLIB:LIBCMT %os_libs%

rem SDL Build
:: cl /w /MP -Zi /DEBUG:FULL /Fe%name%.exe %src_main% %inc% %SDL_INC% ^
:: %l_options% /NODEFAULTLIB:LIBCMT %os_libs% %SDL_LIB%


popd
