@echo off

if not defined GLSC3D_VCVARS_INCLUDED (
	call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
	set GLSC3D_VCVARS_INCLUDED=1
)

cl /I Include %1 Lib\glsc3d.lib opengl32.lib SDL2.lib freetype271.lib user32.lib