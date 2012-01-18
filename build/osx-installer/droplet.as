#compile this with 
# > /usr/bin/osacompile -o droplet.app droplet.as

on open thefiles    
    set outputFolder to (path to library folder from user domain as text) & "Pd:"
    do shell script "/bin/mkdir -p " & quoted form of POSIX path of outputFolder
    tell application "Finder"
        duplicate thefiles to outputFolder replacing true
    end tell    
end open

