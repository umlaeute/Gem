#! /bin/sh
#
# Install GEM
#
# IOhannes m zmoelnig, zmoelnig@iem.kug.ac.at
# IEM, Graz
#

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
cp examples/01.basic/* ../doc/gem/01.basic 
cp examples/02.advanced/* ../doc/gem/02.advanced 
cp examples/03.lighting/* ../doc/gem/03.lighting 
cp examples/04.pix/* ../doc/gem/04.pix 
cp examples/04.video/* ../doc/gem/04.video 
cp examples/05.text/* ../doc/gem/05.text 
cp examples/06.particle/* ../doc/gem/06.particle 
cp examples/07.texture/* ../doc/gem/07.texture 
cp examples/08.io/* ../doc/gem/08.io 
cp examples/99.games/* ../doc/gem/99.games

echo Copying documentation...
cp doc/* ../doc/gem/manual
cp manual/*.html ../doc/gem/manual

echo Copying reference files...
cp help/* ../doc/5.reference/Gem

echo done


