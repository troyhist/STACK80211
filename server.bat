@echo off
if exist C:\Kolevatov\EMU\LOG\*.*  del C:\Kolevatov\EMU\LOG\*.txt
if exist C:\Kolevatov\EMU\RECV\*.* del C:\Kolevatov\EMU\RECV\*.txt
start "Medium Server" c:\kolevatov\projects\mdm_access\debug\mdm_srv C:\Kolevatov\EMU\cfg_srv.txt
