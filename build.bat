@echo off
setlocal

g++ -std=c++17 -Wall -Wextra src\*.cpp -o main.exe
if errorlevel 1 (
    echo.
    echo Build failed. Make sure g++ is installed and available on PATH.
    exit /b 1
)

echo Build finished: main.exe
