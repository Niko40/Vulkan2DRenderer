@echo off



set /A build_static=0
set /A build_tests=0
set /A build_examples=1

:OptionsLoop
call :PrintOptionsMenu ret, %build_static%, %build_tests%, %build_examples%
if %ret%==1 call :ToggleValue build_static, %build_static%
if %ret%==2 call :ToggleValue build_tests, %build_tests%
if %ret%==3 call :ToggleValue build_examples, %build_examples%
if %ret%==4 goto Build
if %ret%==5 goto End
goto OptionsLoop







:Build

call :GetAsOnOffString build_static_str, %build_static%
call :GetAsOnOffString build_tests_str, %build_tests%
call :GetAsOnOffString build_examples_str, %build_examples%


set build_name=VisualStudio2019
set project="VK2D"
set generator="Visual Studio 16 2019"
set architecture="x64"
set source_dir=..\..\..

mkdir Build
cd Build
mkdir %build_name%
cd %build_name%
cd ..\..

cd ExternalLibraries
call Build_VisualStudio2019.bat

cd ..\Tools
call Build_VisualStudio2019.bat

cd ..

cd Build
cd %build_name%

mkdir %project%
cd %project%
cmake -G %generator% -A %architecture% -D CMAKE_BUILD_TYPE=Release -D BUILD_STATIC_LIBRARY=%build_static_str% -D BUILD_TESTS=%build_tests_str% -D BUILD_EXAMPLES=%build_examples_str% "%source_dir%"

"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe" %project%.sln /property:Configuration=Release -m
cd ..

cd ..\..

goto End






:PrintOptionsMenu
call :GetAsOnOffString build_static_str, %~2
call :GetAsOnOffString build_tests_str, %~3
call :GetAsOnOffString build_examples_str, %~4

cls
echo:
echo ************************************************************
echo *
echo * Choose from the following list and press enter to select.
echo *
echo * [ 1 ] Build static library (EXPERIMENTAL): %build_static_str%
echo * [ 2 ] Build tests: %build_tests_str%
echo * [ 3 ] Build examples: %build_examples_str%
echo *
echo * [ 4 ] Build with current settings
echo * [ 5 ] Exit
echo *
echo ************************************************************
echo:

set /p "%~1=Selection: "
exit /B 0



REM First parameter is return value, second parameter is the original value
:ToggleValue
if %~2 == 0 (set /A "%~1"=1) else (set /A "%~1"=0)
exit /B 0



REM return numerals 0 and 1 as "OFF" and "ON"
REM First parameter is return value, second parameter is the original value
:GetAsOnOffString
if %~2 == 0 (set "%~1=OFF") else (set "%~1=ON")
exit /B 0



:End
exit /B %ERRORLEVEL%