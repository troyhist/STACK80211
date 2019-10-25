# Microsoft Developer Studio Project File - Name="STACK802_11" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=STACK802_11 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "STACK802_11.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "STACK802_11.mak" CFG="STACK802_11 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "STACK802_11 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "STACK802_11 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "STACK802_11 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "STACK802_11 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "XUSE_GENERIC_FUNC" /D "_Windows" /D "IC86" /D "SCTDEBCOM" /YX /FD /GZ /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 user32.lib gdi32.lib comdlg32.lib advapi32.lib ws2_32.lib netapi32.lib comctl32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Cmds=c:\AutoBuild
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "STACK802_11 - Win32 Release"
# Name "STACK802_11 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "GEN"

# PROP Default_Filter "c;"
# Begin Source File

SOURCE=.\GENER\ctypes.c
# End Source File
# Begin Source File

SOURCE=.\GENER\sysincl_exp_gen.c
# End Source File
# Begin Source File

SOURCE=.\GENER\SysSTA.c
# End Source File
# End Group
# Begin Group "MDMA"

# PROP Default_Filter "c;"
# Begin Source File

SOURCE=..\MDM_ACCESS\mdm_access.c
# End Source File
# Begin Source File

SOURCE=..\MDM_ACCESS\mdm_cli.c
# End Source File
# End Group
# Begin Group "SDLLIB"

# PROP Default_Filter "c;"
# Begin Source File

SOURCE=..\SDLLIB\sctenv.c
# End Source File
# Begin Source File

SOURCE=..\SDLLIB\sctmon.c
# End Source File
# Begin Source File

SOURCE=..\SDLLIB\sctos.c
# End Source File
# Begin Source File

SOURCE=..\SDLLIB\sctpost.c
# End Source File
# Begin Source File

SOURCE=..\SDLLIB\sctpred.c
# End Source File
# Begin Source File

SOURCE=..\SDLLIB\sctsdl.c
# End Source File
# Begin Source File

SOURCE=..\SDLLIB\sctutil.c
# End Source File
# Begin Source File

SOURCE=..\SDLLIB\libpost.lib
# End Source File
# End Group
# Begin Group "SOCKET"

# PROP Default_Filter "c;"
# Begin Source File

SOURCE=..\TCILIB\CLASS_C\SOCKETOP\SOCKETOP.C
# End Source File
# End Group
# Begin Source File

SOURCE=..\TCILIB\CLASS_C\C_LOGGER\c_logger.c
# End Source File
# Begin Source File

SOURCE=.\c_vector.c
# End Source File
# Begin Source File

SOURCE=.\coder_intface.c
# End Source File
# Begin Source File

SOURCE=.\export_sdl_exp.c
# End Source File
# Begin Source File

SOURCE=.\FileCtrl.c
# End Source File
# Begin Source File

SOURCE=.\proc_mac.c
# End Source File
# Begin Source File

SOURCE=.\proc_phy.c
# End Source File
# Begin Source File

SOURCE=.\proc_phyctrl.c
# End Source File
# Begin Source File

SOURCE=.\procpkt.c
# End Source File
# Begin Source File

SOURCE=.\sta_emul.c
# End Source File
# Begin Source File

SOURCE=.\sta_inst.c
# End Source File
# Begin Source File

SOURCE=..\TCILIB\FUNC\GETVAL\strgetval.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "EXP"

# PROP Default_Filter "h;"
# Begin Source File

SOURCE=.\proc_mac_exp.h
# End Source File
# Begin Source File

SOURCE=.\proc_phy_exp.h
# End Source File
# Begin Source File

SOURCE=.\proc_phyctrl_exp.h
# End Source File
# Begin Source File

SOURCE=.\procpkt_exp.h
# End Source File
# Begin Source File

SOURCE=.\sysincl_exp.h
# End Source File
# End Group
# Begin Group "GENH"

# PROP Default_Filter "h;"
# Begin Source File

SOURCE=.\GENER\ctypes.h
# End Source File
# Begin Source File

SOURCE=.\GENER\sysincl_exp.h
# End Source File
# Begin Source File

SOURCE=.\GENER\sysincl_exp_gen.h
# End Source File
# End Group
# Begin Group "MDMAH"

# PROP Default_Filter "h;"
# Begin Source File

SOURCE=..\MDM_ACCESS\mdm_access.h
# End Source File
# Begin Source File

SOURCE=..\MDM_ACCESS\mdm_cli.h
# End Source File
# End Group
# Begin Group "SDLLIB_H"

# PROP Default_Filter "h;"
# Begin Source File

SOURCE=..\SDLLIB\dll.h
# End Source File
# Begin Source File

SOURCE=..\SDLLIB\itex.h
# End Source File
# Begin Source File

SOURCE=..\SDLLIB\itex2.h
# End Source File
# Begin Source File

SOURCE=..\SDLLIB\post.h
# End Source File
# Begin Source File

SOURCE=..\SDLLIB\sctlocal.h
# End Source File
# Begin Source File

SOURCE=..\SDLLIB\sctpred.h
# End Source File
# Begin Source File

SOURCE=..\SDLLIB\scttypes.h
# End Source File
# Begin Source File

SOURCE=..\SDLLIB\sdt.h
# End Source File
# End Group
# Begin Group "SOCKET_H"

# PROP Default_Filter "h;"
# Begin Source File

SOURCE=..\TCILIB\CLASS_C\SOCKETOP\SOCKETOP.H
# End Source File
# End Group
# Begin Source File

SOURCE=.\_DEBUG.H
# End Source File
# Begin Source File

SOURCE=..\MDM_ACCESS\AutoBuild.h
# End Source File
# Begin Source File

SOURCE=..\TCILIB\CLASS_C\C_LOGGER\c_logger.h
# End Source File
# Begin Source File

SOURCE=.\c_vector.h
# End Source File
# Begin Source File

SOURCE=.\coder_intface.h
# End Source File
# Begin Source File

SOURCE=.\emul_cfg.h
# End Source File
# Begin Source File

SOURCE=.\export_sdl_exp.h
# End Source File
# Begin Source File

SOURCE=.\FileCtrl.h
# End Source File
# Begin Source File

SOURCE=.\LastBackup.h
# End Source File
# Begin Source File

SOURCE=.\lib_path.h
# End Source File
# Begin Source File

SOURCE=.\proc_mac.h
# End Source File
# Begin Source File

SOURCE=.\proc_phy.h
# End Source File
# Begin Source File

SOURCE=.\proc_phyctrl.h
# End Source File
# Begin Source File

SOURCE=.\procpkt.h
# End Source File
# Begin Source File

SOURCE=.\sta_emul.h
# End Source File
# Begin Source File

SOURCE=.\sta_inst.h
# End Source File
# Begin Source File

SOURCE=..\TCILIB\FUNC\GETVAL\strgetval.h
# End Source File
# Begin Source File

SOURCE=..\SDLLIB\user_cc.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=..\prot_ifc\lib\vrelib.lib
# End Source File
# End Target
# End Project
