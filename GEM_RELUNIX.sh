#! /bin/sh
#
# package GEM up for release
#
# Mark Danks - mark@danks.org
#

case $# in
    1)  echo Creating GEM release version $1
    	;;
    *)  echo Specify a release number
    	exit 1
    	;;
esac

# go into the source directory and remake GEM
#########
cd src
make clobber
touch Makefile.depend
make depend
make release

# remove the object files
make clean

# remove windows garbage
rm gem.*

# make the README
cd ..

echo "This is GEM ver$1\nYou can get the current distribution from:\n\n\
ftp://iem.kug.ac.at/Gem\n\n\
To install GEM on SGI:\n\n\
1) untar the GEM package so that it is located at pd/gem\n\n\
2) run GEM_INSTALL.sh\n    - this will put everything where it should be\n\n\n\
3) Read the FAQ for instructions to run Pd and GEM\n\n\
4) Check the release notes for changes and usage information\n\n\
5) Read the manual html files to learn how to use GEM\n\n\
zmoelnig@iem.kuga.c.at" > GEM.README

# set the permissions on the install file
chmod 777 GEM_INSTALL.sh


# go above the GEM directory and tar it all up
cd ..

# Full version
##############
echo Tarring full version up...
tar cvf gem-$1.tar gem
compress gem-$1.tar

# Binary version
################
echo Tarring binary version up...

tar cvf gem-$1bin.tar gem/Gem.pd_irix5 gem/GEM.LICENSE.TERMS gem/GnuGPL.LICENSE gem/GEM.README gem/GEM_INSTALL.sh gem/doc/* gem/help/* gem/examples/* gem/manual/*

compress gem-$1bin.tar

# DSO version
#############
echo Tarring DSO version up...

tar cvf gem-$1dso.tar gem/Gem.pd_irix5 gem/GEM.LICENSE.TERMS gem/GnuGPL.LICENSE gem/GEM.README gem/GEM_INSTALL.sh gem/doc/* gem/manual/*

compress gem-$1dso.tar
