@echo off

echo Installing GEM...

mkdir ..\doc\gem\01.basic
mkdir ..\doc\gem\02.advanced
mkdir ..\doc\gem\03.lighting
mkdir ..\doc\gem\04.pix
mkdir ..\doc\gem\04.video
mkdir ..\doc\gem\05.text
mkdir ..\doc\gem\06.particle
mkdir ..\doc\gem\08.io
mkdir ..\doc\gem\99.games
mkdir ..\doc\gem\manual

mkdir ..\doc\5.reference\Gem

echo Copying examples...
copy examples\01.basic\* ..\doc\gem\01.basic > tempInstall.trash
copy examples\02.advanced\* ..\doc\gem\02.advanced > tempInstall.trash
copy examples\03.lighting\* ..\doc\gem\03.lighting > tempInstall.trash
copy examples\04.pix\* ..\doc\gem\04.pix > tempInstall.trash
copy examples\04.video\* ..\doc\gem\04.video > tempInstall.trash
copy examples\05.text\* ..\doc\gem\05.text > tempInstall.trash
copy examples\06.particle\* ..\doc\gem\06.particle > tempInstall.trash
copy examples\07.texture\* ..\doc\gem\07.texture > tempInstall.trash
copy examples\08.io\* ..\doc\gem\08.io > tempInstall.trash
copy examples\99.games\* ..\doc\gem\99.games > tempInstall.trash

echo Copying documentation...
copy doc\gem*.* ..\doc\gem\manual > tempInstall.trash
copy manual\*.html ..\doc\gem\manual > tempInstall.trash
echo Copying reference files...
copy help\* ..\doc\5.reference\Gem > tempInstall.trash

del tempInstall.trash

echo Install finished.
