@echo off
REM where does PD reside ??
set PDPATH=C:\programme\pd

REM which pd-version do we have ? 
set PDVERSION=0.37


rem ==============================================
rem do not edit below this line
rem ==============================================

echo Installing GEM...
IF NOT EXIST %PDPATH%\bin\pd.exe goto location_error

rem setting path for reference-patches accordingly
set REFPATH=extra\help-Gem
if %PDVERSION% LSS 0.37 set REFPATH=doc\5.reference\Gem

echo Copying binary...
copy Gem.dll %PDPATH%\extra > tempInstall.trash

mkdir %PDPATH%\doc\gem\01.basic	> tempInstall.trash
mkdir %PDPATH%\doc\gem\02.advanced	> tempInstall.trash
mkdir %PDPATH%\doc\gem\03.lighting	> tempInstall.trash
mkdir %PDPATH%\doc\gem\04.pix	> tempInstall.trash
mkdir %PDPATH%\doc\gem\04.video	> tempInstall.trash
mkdir %PDPATH%\doc\gem\05.text	> tempInstall.trash
mkdir %PDPATH%\doc\gem\06.particle	> tempInstall.trash
mkdir %PDPATH%\doc\gem\08.io	> tempInstall.trash
mkdir %PDPATH%\doc\gem\99.games	> tempInstall.trash
mkdir %PDPATH%\doc\gem\data > tempInstall.trash
mkdir %PDPATH%\doc\gem\manual	> tempInstall.trash

echo assuming you are using pd-%PDVERSION%
mkdir %PDPATH%\%REFPATH%

echo Copying examples...
copy examples\01.basic\* %PDPATH%\doc\gem\01.basic > tempInstall.trash
copy examples\02.advanced\* %PDPATH%\doc\gem\02.advanced > tempInstall.trash
copy examples\03.lighting\* %PDPATH%\doc\gem\03.lighting > tempInstall.trash
copy examples\04.pix\* %PDPATH%\doc\gem\04.pix > tempInstall.trash
copy examples\04.video\* %PDPATH%\doc\gem\04.video > tempInstall.trash
copy examples\05.text\* %PDPATH%\doc\gem\05.text > tempInstall.trash
copy examples\06.particle\* %PDPATH%\doc\gem\06.particle > tempInstall.trash
copy examples\07.texture\* %PDPATH%\doc\gem\07.texture > tempInstall.trash
copy examples\08.io\* %PDPATH%\doc\gem\08.io > tempInstall.trash
copy examples\99.games\* %PDPATH%\doc\gem\99.games > tempInstall.trash
copy examples\data\*  %PDPATH%\doc\gem\data > tempInstall.trash

echo Copying documentation...
copy doc\gem*.* %PDPATH%\doc\gem\manual > tempInstall.trash
copy manual\*.html %PDPATH%\doc\gem\manual > tempInstall.trash
echo Copying reference files...
copy help\* %PDPATH%\%REFPATH% > tempInstall.trash

del tempInstall.trash

echo Install finished.
goto end


:location_error
echo :
echo : i believe i am in the wrong directory
echo : i thought that the pd-executable would be %PDPATH%\bin\pd.exe
echo : obviously it is not !!!
echo : please edit this file and set the PDPATH-variable apropriatly
echo :
echo : stopping installation
echo :
pause


:end
