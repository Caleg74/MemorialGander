@echo off
setlocal EnableExtensions

rem Run on the development PC after rebuilding all three applications in Release.
set "REPO=%~dp0"
set "QT_BIN=C:\Qt\6.12.0\mingw_64\bin"
set "MINGW_BIN=C:\Qt\Tools\mingw1310_64\bin"
set "BUILD_DIR=Desktop_Qt_6_12_0_MinGW_64_bit_Release"
set "PATH=%QT_BIN%;%MINGW_BIN%;%PATH%"
set "CHECK_ONLY="
if /I "%~1"=="--check" set "CHECK_ONLY=1"

if not exist "%QT_BIN%\windeployqt.exe" (
    echo ERROR: windeployqt not found. Check QT_BIN in this file.
    goto :failed
)
for %%D in (g++.exe libgcc_s_seh-1.dll libstdc++-6.dll libwinpthread-1.dll) do if not exist "%MINGW_BIN%\%%D" (
    echo ERROR: MinGW not found. Check MINGW_BIN in this file.
    goto :failed
)

for %%A in (MGServer MGInsertScore MGShowRanking) do (
    if not exist "%REPO%%%A\build\%BUILD_DIR%\release\%%A.exe" (
        echo ERROR: Release executable missing for %%A.
        echo Rebuild in Release or update BUILD_DIR in this file.
        goto :failed
    )
)

echo Rebuild all three applications in Release before preparing a new package.
for %%A in (MGServer MGInsertScore MGShowRanking) do (
    call :deploy %%A
    if errorlevel 1 goto :failed
)

if defined CHECK_ONLY (
    echo Dependency check completed. No deployment files were copied.
    exit /b 0
)
echo.
echo Deployment completed: "%REPO%MGDeploy"
echo Copy each entire application folder, including its subfolders.
echo Target PCs also require PostgreSQL ODBC 64-bit and the appropriate DSN:
echo MGServer / MGInsertScore: PostgreSQL35W. MGShowRanking: PostgreSQL35R.
pause
exit /b 0

:deploy
set "APP=%~1"
set "SOURCE_EXE=%REPO%%~1\build\%BUILD_DIR%\release\%~1.exe"
set "DEST=%REPO%MGDeploy\%~1"
echo.
echo Preparing %APP%...
rem Scan source QML only: never scan setup, build or MGDeploy cloud copies.
if defined CHECK_ONLY (
    "%QT_BIN%\windeployqt.exe" --dry-run --release --no-compiler-runtime --qmldir "%REPO%%APP%\src\qml" --qmldir "%REPO%MGCommon\qml-styles" --include-plugins qsqlodbc "%SOURCE_EXE%"
    exit /b
)
if not exist "%DEST%" mkdir "%DEST%"
if not exist "%DEST%" exit /b 1
copy /Y "%SOURCE_EXE%" "%DEST%\%APP%.exe" >nul
if errorlevel 1 exit /b 1
"%QT_BIN%\windeployqt.exe" --release --force --compiler-runtime --qmldir "%REPO%%APP%\src\qml" --qmldir "%REPO%MGCommon\qml-styles" --include-plugins qsqlodbc "%DEST%\%APP%.exe"
exit /b

:failed
echo.
echo ERROR: Deployment incomplete. Do not distribute the output folders.
if not defined CHECK_ONLY pause
exit /b 1


