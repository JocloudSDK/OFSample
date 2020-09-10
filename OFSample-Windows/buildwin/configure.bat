@echo off

echo [INFO]build work dir: %cd%

set workDir=%~dp0
set dependsDir=%workDir%..\depends
set dependCfg=%workDir%config\depend.list
set binDir=%workDir%..\bin
set releaseBinDir=%binDir%\release
set libDir=%workDir%..\lib
set releaseLibDir=%libDir%\release

::get_depend_data.py %depends_dir% %cfg_file%
python %workDir%pytools\get_depend_data.py %dependsDir% %dependCfg%
if ERRORLEVEL 1 exit /b 11

:: copy depends data to bin dir
set dependBinConfig=%workDir%config\depend_bin_config.list
set copy_depends_flag=%1
if not "%copy_depends_flag%"=="not_copy_depends" (
echo [INFO]copy depneds bin data
python %workDir%pytools\batch_copy.py %workDir%.. %releaseBinDir% %dependBinConfig%
if ERRORLEVEL 1 exit /b 12
)

:: copy depends data to lib dir
set dependLibConfig=%workDir%config\depend_lib_config.list
set copy_depends_flag=%1
if not "%copy_depends_flag%"=="not_copy_depends" (
echo [INFO]copy depneds lib data
python %workDir%pytools\batch_copy.py %workDir%.. %releaseLibDir% %dependLibConfig%
if ERRORLEVEL 1 exit /b 12
)

::copy 3rd data to dest dir
::xcopy /e /y /r /i %srcdir% %dstdir%

cd %buildDir%
exit /b ERRORLEVEL