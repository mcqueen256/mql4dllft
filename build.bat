
set workingdir="%cd%"
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86_amd64
cd /D "%workingdir%"
call scons dll

pause