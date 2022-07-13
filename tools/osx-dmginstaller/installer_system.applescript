try
	-- create the output folder if necessary
	set outputFolder to (path to library folder as text) & "Pd:"
	do shell script "/bin/mkdir -p " & quoted form of POSIX path of outputFolder
	
	-- find the directories on the dmg disk
	set myPath to path to me
	tell application "Finder"
		set myContainer to container of myPath
		set installDirs to (folders of myContainer) as alias list
	end tell
	
	-- copy the templates to the output folder
	-- NOTE: the script will error if any of the templates already exist
	-- therefore we use a repeat loop and duplicate each file separately with a try block
	-- around it to avoid errors in case some templates have already been installed.
	tell application "Finder"
		repeat with aDir in installDirs
			try
				duplicate aDir to folder outputFolder with replacing
			end try
		end repeat
	end tell
	
	-- tell the user everything was OK
	tell me to activate
	display dialog "Gem and friends were successfully installed! You may now use them in Pd." buttons {"OK"} default button 1 with title "Gem System-wide Installer" with icon note
on error
	tell me to activate
	display dialog "There was an error installing Gem and friends. Please manually install them by copying" & (installDirs) & "to the following folder." & return & return & (POSIX path of outputFolder) buttons {"OK"} default button 1 with title "Gem System-wide Installer"
end try
