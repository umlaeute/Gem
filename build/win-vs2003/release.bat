mkdir gem-release

xcopy Gem.dll gem-release\

cd ..\..\

xcopy /E /I abstractions build\win-vs2003\gem-release\abstractions

xcopy /E /I help build\win-vs2003\gem-release\help

xcopy /E /I examples build\win-vs2003\gem-release\examples
xcopy /E /I doc build\win-vs2003\gem-release\doc
xcopy /E /I manual build\win-vs2003\gem-release\manual

xcopy /E /I tests build\win-vs2003\gem-release\tests

copy ChangeLog build\win-vs2003\gem-release
copy GEM_INSTALL.bat build\win-vs2003\gem-release
copy GEM.LICENSE.TERMS build\win-vs2003\gem-release
copy GEM.README build\win-vs2003\gem-release
copy GnuGPL.LICENSE build\win-vs2003\gem-release


cd build\win-vs2003

copy README_W32.txt.template gem-release\README_W32.txt