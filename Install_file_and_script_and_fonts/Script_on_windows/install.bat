@echo off

echo "Starting the Install GLSC3D Library on your Windows!"

if not exist Lib mkdir Lib
if not exist Include mkdir Include
if not exist Bin mkdir Bin

copy GLSC3D\Out\Release\glsc3d.lib Lib\glsc3d.lib
copy GLSC3D\Include\glsc3d.h Include\glsc3d.h
copy GLSC3D\Include\glsc3d_math.h Include\glsc3d_math.h
copy GLSC3D\Install_file_and_script_and_fonts\ccg_windows.bat Bin\ccg.bat
copy GLSC3D\Install_file_and_script_and_fonts\Hello_GLSC3D.c Hello_GLSC3D.c
