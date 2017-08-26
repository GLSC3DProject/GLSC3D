@echo off

set /P run_sample="Do you want to run the Sample programs (Y/N)? "

if /I %run_sample% == Y (
	for %%x in (*.c) do (
		call ccg.bat %%x
		call %%~nx.exe
	)
)

set /P run_advanced="Do you want to run the Advanced programs (Y/N)? "

if /I %run_advanced% == Y (
	pushd Advanced
	for %%x in (*.c*) do (
		call ccg.bat %%x
		call %%~nx.exe
	)
	popd
)

echo Script finished.
pause