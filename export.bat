set workingdir="%cd%"
move "%cd%\build\bot.dll" "%cd%\build\Robot.dll"
copy /Y "%cd%\build\Robot.dll" %APPDATA%\MetaQuotes\Terminal\3294B546D50FEEDA6BF3CFC7CF858DB7\MQL4\Libraries\
rem copy /Y "%cd%\build\Expert.cpp" "%cd%\build\Robot.mq4"
rem move "%cd%\build\Robot.mq4" %APPDATA%\MetaQuotes\Terminal\3294B546D50FEEDA6BF3CFC7CF858DB7\MQL4\Experts\
rem copy /Y "%cd%\build\Indicator.cpp" "%cd%\build\Robot.mq4"
rem move "%cd%\build\Robot.mq4" %APPDATA%\MetaQuotes\Terminal\3294B546D50FEEDA6BF3CFC7CF858DB7\MQL4\Indicators\
echo "Exported files to MT4."
pause
exit 0