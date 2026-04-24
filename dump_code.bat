@echo off
set OUT=code_dump.txt

echo Creating full project dump...
echo. > %OUT%

:: =========================
:: SOURCE FILES (.cpp)
:: =========================
echo ===== SOURCE FILES ===== >> %OUT%
for /r src %%f in (*.cpp) do (
    echo. >> %OUT%
    echo ===== %%f ===== >> %OUT%
    type "%%f" >> %OUT%
)

:: =========================
:: HEADER FILES (.h)
:: =========================
echo. >> %OUT%
echo ===== HEADER FILES ===== >> %OUT%
for /r include %%f in (*.h) do (
    echo. >> %OUT%
    echo ===== %%f ===== >> %OUT%
    type "%%f" >> %OUT%
)

:: =========================
:: ROOT IMPORTANT FILES
:: =========================
echo. >> %OUT%
echo ===== ROOT FILES ===== >> %OUT%

if exist CMakeLists.txt (
    echo. >> %OUT%
    echo ===== CMakeLists.txt ===== >> %OUT%
    type CMakeLists.txt >> %OUT%
)

if exist README.md (
    echo. >> %OUT%
    echo ===== README.md ===== >> %OUT%
    type README.md >> %OUT%
)

if exist .gitignore (
    echo. >> %OUT%
    echo ===== .gitignore ===== >> %OUT%
    type .gitignore >> %OUT%
)

:: =========================
:: OPTIONAL (CONFIG FILES)
:: =========================
echo. >> %OUT%
echo ===== CONFIG FILES ===== >> %OUT%

for /r .vscode %%f in (*.json) do (
    echo. >> %OUT%
    echo ===== %%f ===== >> %OUT%
    type "%%f" >> %OUT%
)

:: =========================
:: SAVE FILES (optional)
:: =========================
if exist saves (
    echo. >> %OUT%
    echo ===== SAVE FILES ===== >> %OUT%
    for %%f in (saves\*) do (
        echo ===== %%f ===== >> %OUT%
        type "%%f" >> %OUT%
        echo. >> %OUT%
    )
)

:: =========================
:: DONE
:: =========================
echo. >> %OUT%
echo ===== END OF DUMP ===== >> %OUT%

echo Done! Open code_dump.txt and copy everything.
pause