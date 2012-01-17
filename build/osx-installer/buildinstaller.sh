#!/bin/sh

## this is shamelessly taken from Pd-extended...

VOLUME_NAME = Gem
hdiutil create -format UDRW -fs HFS+ -srcfolder "$(BUILD_BASE)" -volname $(VOLUME_NAME) build.dmg
# detach one with the same name first
hdiutil detach "/Volumes/$(VOLUME_NAME)"
hdiutil attach build.dmg
install -p $(DESTDIR)$(manualsdir)/Pd/ReadMe.html "/Volumes/$(VOLUME_NAME)"

# add link to /Applications for easy install
ln -s -f /Applications /Volumes/$(VOLUME_NAME)/Applications

# Install to .background for easier manual DS_Store makeing 
install -d "/Volumes/$(VOLUME_NAME)/.background"
# The full path to the background image needs to be added to .DS_Store to work
# Hence the background image will only show when the disk image is called 
# "$(VOLUME_NAME)=Pd-extended - that is what the current DS_Store uses.
# To create the DS_Store file one must... search the net.
install -p background.png "/Volumes/$(VOLUME_NAME)/.background/"
install -p DS_Store "/Volumes/$(VOLUME_NAME)/.DS_Store"
install -p VolumeIcon.icns "/Volumes/$(VOLUME_NAME)/.VolumeIcon.icns"

# To enable the local image icon
/Developer/Tools/SetFile -a C /Volumes/$(VOLUME_NAME)/.VolumeIcon.icns /Volumes/$(VOLUME_NAME)
$(MAKE) -C $(packages_src) DESTDIR="/Volumes/$(VOLUME_NAME)/$(PD_APP_NAME).app/Contents/Resources" doc_format

chmod -R a-w "/Volumes/$(VOLUME_NAME)/$(PD_APP_NAME).app/Contents/Resources"


hdiutil detach $(mount | grep $(VOLUME_NAME) | cut -d ' ' -f 1)
hdiutil convert -format UDZO -o "$(PACKAGE_NAME).dmg" build.dmg
rm -f build.dmg


# install license
hdiutil unflatten "$(PACKAGE_NAME).dmg"
/Developer/Tools/Rez /Developer/Headers/FlatCarbon/*.r SLA.r -a -o "$(PACKAGE_NAME).dmg"
hdiutil flatten "$(PACKAGE_NAME).dmg"

