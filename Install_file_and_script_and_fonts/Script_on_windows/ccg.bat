@echo off

if [%*] == [] (
	echo The argument is empty.
	goto :eof
)

if not defined GLSC3D_VCVARS_INCLUDED (
	pushd

	if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat" (
		rem Visual Studio 2019 Community
		call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
	) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat" (
		rem Visual Studio 2017 Community
		call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
	) else if exist "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\amd64\vcvars64.bat" (
		rem Visual Studio 2015 Community
		call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\amd64\vcvars64.bat"
	) else (
		echo 'ccg' requires Visual Studio 2019, 2017 or 2015 Community. None of these is found.
		goto :eof
	)

	popd

	set GLSC3D_VCVARS_INCLUDED=1
)

cl %* /nologo /O2 /EHsc /MD /utf-8 /I "%~dp0..\Include" /link /NODEFAULTLIB:LIBCMT "%~dp0..\Lib\*" opengl32.lib user32.lib
