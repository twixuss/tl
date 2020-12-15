@echo off
set bindir=bin\

if not exist %bindir% mkdir %bindir%
pushd %bindir%

set task=main_sse
call:doTask
if %errorlevel% neq 0 goto fail

::set task=main_avx
::call:doTask
::if %errorlevel% neq 0 goto fail

::set task=main_avx2
::call:doTask
::if %errorlevel% neq 0 goto fail

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