@echo off
prompt $G 
if "%1" equ "start" (
    goto :start
) else if "%1" equ "stop" (
    goto :stop
) else if "%1" equ "clean" (
    goto :clean
)

@echo on
    echo Usage: trace [start^|stop^|clean]
@echo off
goto :end

:start
@echo on
    vcperf /start MySessionName
@echo off
goto :end

:stop
@echo on
    vcperf /stopnoanalyze MySessionName outputTraceFile.etl
    vcperf /analyze outputTraceFile.etl /timetrace trace.json
    del outputTraceFile.etl
@echo off
goto :end

:clean
@echo on
    del outputTraceFile.etl
    del trace.json
@echo off
goto :end

:end
prompt $P$G
