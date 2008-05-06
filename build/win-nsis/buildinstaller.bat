set NSISDIR=%ProgramFiles%\NSIS\
set GEMNSIS=%CD%

cd %NSISDIR%

makensis %GEMNSIS%\gem.nsi
makensis %GEMNSIS%\gem-binary.nsi


cd %GEMNSIS%
