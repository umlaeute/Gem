#!/bin/sh
## this is shamelessly taken from Pd-extended...

SRCDIR=$1
VOLUME_NAME = Gem
PACKAGE_NAME = ${VOLUME_NAME}
DESTDIR=/Volumes/${VOLUME_NAME}

if [ -d "${SRCDIR}" ]; then
 echo "creating Gem installer from ${SRCDIR}"
else
 echo "please provide a valid /path/to/Gem" 1>&2
 exit 1
fi


hdiutil create -format UDRW -fs HFS+ -srcfolder "${SRCDIR}" -volname "${VOLUME_NAME}" build.dmg
# detach one with the same name first
hdiutil detach "${DESTDIR}"
hdiutil attach build.dmg
#install -p ${SRCDIR}${manualsdir}/Pd/ReadMe.html "${DESTDIR}"

# add link to /Library/Pd for easy install
/usr/bin/osacompile -o "${DESTDIR}/System-wide Installation.app" droplet_system.as
./icon2app "${DESTDIR}/System-wide Installation.app" /System/Library/CoreServices/CoreTypes.bundle/Contents/Resources/LibraryFolderIcon.icns

# add link to ~/Library/Pd for easy install
/usr/bin/osacompile -o "${DESTDIR}/User-specific Installation.app" droplet_user.as
./icon2app "${DESTDIR}/User-specific Installation.app" /System/Library/CoreServices/CoreTypes.bundle/Contents/Resources/HomeFolderIcon.icns

# Install to .background for easier manual DS_Store makeing 
install -d "${DESTDIR}/.background"
# The full path to the background image needs to be added to .DS_Store to work
# Hence the background image will only show when the disk image is called 
# VOLUME_NAME=Pd-extended - that is what the current DS_Store uses.
# To create the DS_Store file one must... search the net.
install -p background.png "${DESTDIR}/.background/"
install -p DS_Store "${DESTDIR}/.DS_Store"

# To enable the local image icon
install -p VolumeIcon.icns "${DESTDIR}/.VolumeIcon.icns"
/Developer/Tools/SetFile -a C "${DESTDIR}/.VolumeIcon.icns" "${DESTDIR}"

#chmod -R a-w "${DESTDIR}/${PD_APP_NAME}.app/Contents/Resources"


hdiutil detach $(mount | grep "${VOLUME_NAME}" | cut -d ' ' -f 1)
hdiutil convert -format UDZO -o "${PACKAGE_NAME}.dmg" build.dmg
rm -f build.dmg


# install license
hdiutil unflatten "${PACKAGE_NAME}.dmg"
/Developer/Tools/Rez /Developer/Headers/FlatCarbon/*.r SLA.r -a -o "${PACKAGE_NAME}.dmg"
hdiutil flatten "${PACKAGE_NAME}.dmg"

