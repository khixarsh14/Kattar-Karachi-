@echo off
echo Building Kattar Karachi...

cd build
cmake .. -G "MinGW Makefiles"
cmake --build .

if %errorlevel% neq 0 (
    echo.
    echo BUILD FAILED - check errors above
    pause
    exit /b 1
)

echo.
echo Build successful! Starting game...
cd ..
build\KattarKarachi.exe