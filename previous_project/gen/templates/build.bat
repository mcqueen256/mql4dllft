if exist output (
	goto outputexists
) else (
    mkdir output
)
:outputexists

set workingdir="%cd%"
cd /D "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\"
call vcvarsall amd64
cd /D "%workingdir%"
call "C:\Program Files (x86)\GnuWin32\bin\make"

pause