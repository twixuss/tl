@echo off

set bindir=bin\
set srcdir=..\src\

if not exist %bindir% mkdir %bindir%
pushd %bindir%

set cFlags=/Z7 /GF /FC /Oi /EHa /fp:fast /std:c++latest /Wall /MDd /Od /D"BUILD_DEBUG=1" /wd4464 /wd4514 /wd4505 /wd4710 /wd5045 /nologo /link /incremental:no

echo sse...
cl %srcdir%test.cpp %cFlags% /out:test_sse.exe
if %errorlevel% neq 0 goto fail
echo avx...
cl %srcdir%test.cpp /arch:AVX %cFlags% /out:test_avx.exe
if %errorlevel% neq 0 goto fail
echo avx2...
cl %srcdir%test.cpp /arch:AVX2 %cFlags% /out:test_avx2.exe
if %errorlevel% neq 0 goto fail

goto success

:fail
echo [31mCompilation failed[0m
goto end
:success
echo [32mCompilation succeeded[0m
:end
popd