set workingdir="%cd%"
copy /Y "%cd%\output\Robot.dll" %APPDATA%\MetaQuotes\Terminal\3294B546D50FEEDA6BF3CFC7CF858DB7\MQL4\Libraries\
copy /Y "%cd%\output\Expert.cpp" "%cd%\output\Robot.mq4"
move "%cd%\output\Robot.mq4" %APPDATA%\MetaQuotes\Terminal\3294B546D50FEEDA6BF3CFC7CF858DB7\MQL4\Experts\
copy /Y "%cd%\output\Indicator.cpp" "%cd%\output\Robot.mq4"
move "%cd%\output\Robot.mq4" %APPDATA%\MetaQuotes\Terminal\3294B546D50FEEDA6BF3CFC7CF858DB7\MQL4\Indicators\
echo "Exported files to MT4."
pause
exit 0