@echo off
setlocal

set DIST_ROOT=dist
set PACKAGE_NAME=tetris-win
set PACKAGE_DIR=%DIST_ROOT%\%PACKAGE_NAME%
set ZIP_FILE=%DIST_ROOT%\%PACKAGE_NAME%.zip

call build.bat
if errorlevel 1 exit /b 1

if exist "%PACKAGE_DIR%" rmdir /s /q "%PACKAGE_DIR%"
if not exist "%DIST_ROOT%" mkdir "%DIST_ROOT%"
mkdir "%PACKAGE_DIR%"

copy /Y main.exe "%PACKAGE_DIR%\main.exe" >NUL
copy /Y README.md "%PACKAGE_DIR%\README.md" >NUL

if exist Font (
    xcopy /E /I /Y Font "%PACKAGE_DIR%\Font" >NUL
)

if exist lib\*.dll (
    copy /Y lib\*.dll "%PACKAGE_DIR%\" >NUL
)

if exist "%ZIP_FILE%" del /q "%ZIP_FILE%"

where powershell >NUL 2>NUL
if not errorlevel 1 (
    powershell -NoProfile -ExecutionPolicy Bypass -Command "Compress-Archive -Path '%PACKAGE_DIR%\*' -DestinationPath '%ZIP_FILE%' -Force"
    if errorlevel 1 exit /b 1
    echo Package finished: %ZIP_FILE%
    exit /b 0
)

echo Package folder finished: %PACKAGE_DIR%
echo Install PowerShell to also create %ZIP_FILE%.
