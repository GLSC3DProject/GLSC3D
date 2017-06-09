@echo off

if [%*] == [] (
	echo The argument is empty.
	goto :eof
)

if not defined GLSC3D_VCVARS_INCLUDED (
	if exist "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat" (
		rem Visual Studio 2017 Community
		pushd "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build"
	) else if exist "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\amd64\vcvars64.bat" (
		rem Visual Studio 2015 Community
		pushd "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\amd64"
	) else (
		echo 'ccg' requires Visual Studio 2017 Community or 2015 Community. Neither is found.
		goto :eof
	)

	call vcvars64.bat
	popd

	set GLSC3D_VCVARS_INCLUDED=1
)

cl %* /nologo /O2 /EHsc /MD /I "%~dp0..\Include" /link "%~dp0..\Lib\*" opengl32.lib user32.lib
