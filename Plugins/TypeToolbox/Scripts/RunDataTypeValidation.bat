@echo off
setlocal

rem Run the ValidateDataTypes function using the Unreal Automation Tool
"%UE4_ROOT%\Engine\Build\BatchFiles\RunUAT.bat" ValidateDataTypes -Module=TypeToolbox

rem Exit with error code if validation fails
if %errorlevel% neq 0 (
    exit /b %errorlevel%
)

endlocal