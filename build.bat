@echo off

set bindir=bin\
set srcdir=..\src\

if not exist %bindir% mkdir %bindir%
pushd %bindir%

set cFlags=/Z7 /GF /FC /Oi /EHa /fp:fast /std:c++latest /Wall /MDd /Od /D"BUILD_DEBUG=1" /wd4464 /wd4514 /wd4505 /wd4710 /wd5045 /nologo /link /incremental:no

::echo math_sse...
::cl %srcdir%math_test.cpp %cFlags% /out:math_sse.exe
::if %errorlevel% neq 0 goto fail
::echo math_avx...
::cl %srcdir%math_test.cpp /arch:AVX %cFlags% /out:math_avx.exe
::if %errorlevel% neq 0 goto fail
::echo math_avx2...
::cl %srcdir%math_test.cpp /arch:AVX2 %cFlags% /out:math_avx2.exe
::if %errorlevel% neq 0 goto fail

echo string...
cl %srcdir%string_test.cpp %cFlags% /out:string.exe
if %errorlevel% neq 0 goto fail

goto success

:fail
echo [31mCompilation failed[0m
goto end
:success
echo [32mCompilation succeeded[0m
:end
popd