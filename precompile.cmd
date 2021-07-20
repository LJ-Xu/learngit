@echo off
setlocal enabledelayedexpansion
set dir=%1
::��ȡgit��Ϣ
for /f "delims=" %%i in ('git config user.name') do (
set gitusr=%%i
)
for /f "delims=" %%i in ('git config user.email') do (
set gitemail=%%i
)
::main
for /f "delims=" %%i in (!dir!\.git\HEAD) do (
set head=%%i
set "branch=!head:ref: refs/heads/=!"
)
for /f "delims=" %%i in (!dir!\.git\refs\heads\!branch!) do (
set hash=%%i
)

::sub
for /f "delims=" %%i in (!dir!\.git\modules\ExLib\HEAD) do (
set subhead=%%i
set "subbranch=!subhead:ref: refs/heads/=!"
)
for /f "delims=" %%i in (!dir!\.git\modules\ExLib\refs\heads\!subbranch!) do (
set subhash=%%i
)





set date=%date:~0,4%%date:~5,2%%date:~8,2%
set time=%time:~0,2%%time:~3,2%%time:~6,2%
::set gitusr=%gitusr%#%gitemail%
echo !gitusr!
echo !branch!
echo !hash!
echo !subbranch!
echo !subhash!

echo !date!
echo !time!
set file=!dir!\HmiPart\HmiComm\include\__ExtInfo.h
echo #pragma once > !file!
echo #define EXE_GIT_AUTHOR  "!gitusr! " >> !file!
echo #define EXE_GIT_BRANCH  "!branch!" >> !file!
echo #define EXE_GIT_HASH  "!hash!" >> !file!
echo #define EXE_GIT_SUBBRANCH  "!subbranch!" >> !file!
echo #define EXE_GIT_SUBHASH  "!subhash!" >> !file!
echo #define EXE_VER_DATE  "!date!" >> !file!
echo #define EXE_VER_TIME  "!time!" >> !file!