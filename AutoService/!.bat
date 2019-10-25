@echo off
echo.
echo STACK80211 ver. 1.0 (22.07.2003)
if %1.==. goto nocom
echo !A_SRC ..\ STACK80211 %1 ..\LastBackup.h !exclude.txt C:\Kolevatov\BACKUP
echo.
echo !A_SRC ..\ STACK80211 %1 ..\LastBackup.h !exclude.txt C:\Kolevatov\BACKUP %2
if %2.==.     call !A_SRC ..\ STACK80211_ %1 ..\LastBackup.h !exclude.txt C:\Kolevatov\BACKUP
if not %2.==. call !A_SRC ..\ STACK80211_ %1 ..\LastBackup.h !exclude.txt C:\Kolevatov\BACKUP %2
goto end
:nocom
echo Type Build number !!! Ex: !makeres 10
:end
