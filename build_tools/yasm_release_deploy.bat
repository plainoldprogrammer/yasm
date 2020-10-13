SET build_tools_path=%cd%

SET build_path=C:\Plainoldprogrammer\Yasm
CD %build_path%

 C:\Qt\Qt5.14.2\5.14.2\mingw73_32\bin\windeployqt.exe "C:\Plainoldprogrammer\Yasm\release"

copy "C:\Qt\Qt5.14.2\5.14.2\mingw73_32\bin\libgcc_s_dw2-1.dll"		release\
copy "C:\Qt\Qt5.14.2\5.14.2\mingw73_32\bin\libstdc++-6.dll"			release\
copy "C:\Qt\Qt5.14.2\5.14.2\mingw73_32\bin\libwinpthread-1.dll"		release\


DEL Makefile
DEL .qmake.stash
DEL Makefile.Debug
DEL Makefile.Release
DEL ui_mainwindow.h
DEL ui_optionsdialog.h
DEL Yasm_resource.rc

DEL release\*.cpp
DEL release\*.h
DEL release\*.o

MOVE release\* .
robocopy /move /e release/ .

RMDIR debug

REM Create a backup of the fresh deployed program
tar -zcvf ..\Yasm_Build_Release.tar.gz *

CD %build_tools_path%

