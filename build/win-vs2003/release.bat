set GEMDIR=gem-%DATE:~6,4%%DATE:~3,2%%DATE:~0,2%

mkdir %GEMDIR%

xcopy Gem.dll %GEMDIR%\
copy README_W32.txt.template %GEMDIR%\README_W32.txt

cd ..\..\

xcopy /E /I abstractions build\win-vs2003\%GEMDIR%\abstractions

xcopy /E /I help build\win-vs2003\%GEMDIR%\help

xcopy /E /I examples build\win-vs2003\%GEMDIR%\examples
xcopy /E /I doc build\win-vs2003\%GEMDIR%\doc
xcopy /E /I manual build\win-vs2003\%GEMDIR%\manual

xcopy /E /I tests build\win-vs2003\%GEMDIR%\tests

copy ChangeLog build\win-vs2003\%GEMDIR%
copy GEM_INSTALL.bat build\win-vs2003\%GEMDIR%
copy GEM.LICENSE.TERMS build\win-vs2003\%GEMDIR%
copy GEM.README build\win-vs2003\%GEMDIR%
copy GnuGPL.LICENSE build\win-vs2003\%GEMDIR%


cd build\win-vs2003
