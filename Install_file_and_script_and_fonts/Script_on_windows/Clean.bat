@echo off

set /P run_clean="Do you want to clean the products (*.obj, *.exe files and Frames.* directories) (Y/N)? "

if /I %run_clean% == Y (
	del /S *.obj *.exe
	for /D %%x in (Frames.*) do rmdir /S /Q %%x
)

echo Script finished.
pause