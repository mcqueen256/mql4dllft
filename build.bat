call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86
pip install -r requirements.txt
python generator
call scons dll
call export.bat
pause