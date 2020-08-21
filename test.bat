@echo off
set bindir=bin\
set srcdir=..\src\

if not exist %bindir% mkdir %bindir%
pushd %bindir%

::set task=math_sse
::call:doTask
::if %errorlevel% neq 0 goto fail
::set task=math_avx
::call:doTask
::if %errorlevel% neq 0 goto fail
::set task=math_avx2
::call:doTask
::if %errorlevel% neq 0 goto fail
set task=string
call:doTask
if %errorlevel% neq 0 goto fail

goto success

:doTask
echo Running %task%.exe...
call %task%
goto:eof

:fail
echo [31mTest failed on %task%[0m
goto end
:success
echo [32mTest succeeded[0m
:end
popd