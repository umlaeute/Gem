#!/bin/sh
## this is shamelessly taken from Pd-extended...

SRCDIR=$1
PACKAGE_NAME=${SRCDIR%/}
VOLUME_NAME="${PACKAGE_NAME}"
VOLUME_NAME="Gem Installer"

DESTDIR="/Volumes/${VOLUME_NAME}"

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
SYSTEMINSTALLER="${DESTDIR}/Gem System-wide Installer.app"
/usr/bin/osacompile -o "${SYSTEMINSTALLER}" installer_system.applescript
./icon2app "${SYSTEMINSTALLER}" /System/Library/CoreServices/CoreTypes.bundle/Contents/Resources/GroupFolder.icns

## add link to ~/Library/Pd for easy install
USERINSTALLER="${DESTDIR}/Gem User-specific Installer.app"
/usr/bin/osacompile -o "${USERINSTALLER}" installer_user.applescript
./icon2app "${USERINSTALLER}" /System/Library/CoreServices/CoreTypes.bundle/Contents/Resources/UsersFolderIcon.icns

# a manually crafted .DS_Store file
test  -e DS_Store && install -p DS_Store "${DESTDIR}/.DS_Store"

# To enable the local image icon
#install -p VolumeIcon.icns "${DESTDIR}/.VolumeIcon.icns"
#/Developer/Tools/SetFile -a C "${DESTDIR}/.VolumeIcon.icns" "${DESTDIR}"

chmod -R a-w "${DESTDIR}/${PD_APP_NAME}.app/Contents/Resources"

hdiutil detach $(mount | grep "${VOLUME_NAME}" | cut -d ' ' -f 1)
echo "now creating RO-image ${PACKAGE_NAME}.dmg from build.dmg"
hdiutil convert -format UDZO -o "${PACKAGE_NAME}.dmg" build.dmg && rm -f build.dmg

# install license
hdiutil unflatten "${PACKAGE_NAME}.dmg"
/Developer/Tools/Rez /Developer/Headers/FlatCarbon/*.r SLA.r -a -o "${PACKAGE_NAME}.dmg"
hdiutil flatten "${PACKAGE_NAME}.dmg"

