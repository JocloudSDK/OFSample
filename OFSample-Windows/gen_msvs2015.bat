set workDir=%~dp0
set buildWinDir=%workDir%buildwin\
call %buildWinDir%configure.bat not_copy_depends
python.exe gen_msvs2015.py
pause