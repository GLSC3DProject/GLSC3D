@echo off

echo "Starting the Test GLSC3D Library on your Windows!"

rem $WDR = "GLSC3D_Working_Directory"
rem mkdir -Force $WDR
rem cd $WDR

rem git clone "https://github.com/GLSC3DProject/GLSC3D"

cd GLSC3D
rem mkdir -Force MyProject
cd MyProject

set CMAKE="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
%CMAKE% -G "Visual Studio 15 2017 Win64" ..

set MSBUILD="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\amd64\MSBuild.exe"
%MSBUILD% ALL_BUILD.vcxproj /p:configuration=release;platform=x64

cd %~dp0
