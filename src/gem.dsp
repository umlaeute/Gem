# Microsoft Developer Studio Project File - Name="gem" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=gem - Win32 Release
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "gem.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "gem.mak" CFG="gem - Win32 Release"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "gem - Win32 Release" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "gem - Win32 Debug" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "gem - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir ".\Objects"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /W3 /O2 /Ob2 /I "../../src" /I "../src" /I "../../GemLibs/Jpeg" /I "../../GemLibs/tiff/libtiff" /I "../../GemLibs/gltt" /I "../../GemLibs/td" /I "../../GemLibs/wintab/include" /I "../../GemLibs/particle" /I "../../GemLibs/liborb" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "NT" /D "_LANGUAGE_C_PLUS_PLUS" /D "WIN32_LEAN_AND_MEAN" /D "GEM_INTERNAL" /FD /c
# SUBTRACT CPP /Fr /YX
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib opengl32.lib glu32.lib vfw32.lib ..\..\bin\pd.lib ../../GemLibs\tiff\libtiff.lib ../../GemLibs\gltt\freetype\lib\libttf.lib ../../GemLibs\gltt\gltt.lib ../../GemLibs\Jpeg\libjpeg.lib ../../GemLibs\td\td.lib ../../GemLibs\particle\particle.lib ../../GemLibs\liborb\liborb.lib /nologo /subsystem:windows /dll /profile /machine:I386 /out:"../gem.dll"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "gem - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir ".\Objects"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /ML /W3 /Zi /Od /Ob2 /I "../../pd/src" /I "../src" /I "../../GemLibs/Jpeg" /I "../../GemLibs/tiff/libtiff" /I "../../GemLibs/gltt" /I "../../GemLibs/wintab/include" /I "../../GemLibs/particle" /I "../../GemLibs/liborb" /I "../../GemLibs/glut" /I "../../GemLibs/glm" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "NT" /D "_LANGUAGE_C_PLUS_PLUS" /D "WIN32_LEAN_AND_MEAN" /D "GEM_INTERNAL" /FD /c
# SUBTRACT CPP /X /Fr /YX
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib opengl32.lib glu32.lib vfw32.lib ../../GemLibs\tiff\libtiff.lib ../../GemLibs\gltt\freetype\lib\libttf.lib ../../GemLibs\gltt\gltt.lib ../../GemLibs\Jpeg\libjpeg.lib ../../GemLibs\particle\particle.lib ../../GemLibs\liborb\liborb.lib ../../GemLibs\glut\glut32.lib ../../GemLibs\glut\glut.lib ../../GemLibs/glm/glm.lib D:\pd\bin\pd.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../gem.dll"
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "gem - Win32 Release"
# Name "gem - Win32 Debug"
# Begin Group "Base"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Base\CPPExtern.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\CPPExtern.h
# End Source File
# Begin Source File

SOURCE=.\Base\GemBase.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\GemBase.h
# End Source File
# Begin Source File

SOURCE=.\Base\GemCache.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\GemCache.h
# End Source File
# Begin Source File

SOURCE=.\Base\GemEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\GemEvent.h
# End Source File
# Begin Source File

SOURCE=.\Base\GemExportDef.h
# End Source File
# Begin Source File

SOURCE=.\Base\GemFuncUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\GemFuncUtil.h
# End Source File
# Begin Source File

SOURCE=.\Base\GemGluObj.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\GemGluObj.h
# End Source File
# Begin Source File

SOURCE=.\Base\GemGlutObj.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\GemGlutObj.h
# End Source File
# Begin Source File

SOURCE=.\Base\GemLoadObj.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\GemLoadObj.h
# End Source File
# Begin Source File

SOURCE=.\Base\GemMan.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\GemMan.h
# End Source File
# Begin Source File

SOURCE=.\Base\GemModelData.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\GemModelData.h
# End Source File
# Begin Source File

SOURCE=.\Base\GemPathBase.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\GemPathBase.h
# End Source File
# Begin Source File

SOURCE=.\Base\GemPixDualObj.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\GemPixDualObj.h
# End Source File
# Begin Source File

SOURCE=.\Base\GemPixImageLoad.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\GemPixImageLoad.h
# End Source File
# Begin Source File

SOURCE=.\Base\GemPixImageSave.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\GemPixImageSave.h
# End Source File
# Begin Source File

SOURCE=.\Base\GemPixObj.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\GemPixObj.h
# End Source File
# Begin Source File

SOURCE=.\Base\GemPixUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\GemPixUtil.h
# End Source File
# Begin Source File

SOURCE=.\Base\GemSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\GemShape.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\GemShape.h
# End Source File
# Begin Source File

SOURCE=.\Base\GemState.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\GemState.h
# End Source File
# Begin Source File

SOURCE=.\Base\GemVersion.h
# End Source File
# Begin Source File

SOURCE=.\Base\GemWinCreate.h
# End Source File
# Begin Source File

SOURCE=.\Base\GemWinCreateNT.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\Matrix.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\Matrix.h
# End Source File
# Begin Source File

SOURCE=.\Base\sgiimage.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\sgiimage.h
# End Source File
# Begin Source File

SOURCE=.\Base\TextBase.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\TextBase.h
# End Source File
# End Group
# Begin Group "Geos"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Geos\circle.cpp
# End Source File
# Begin Source File

SOURCE=.\Geos\circle.h
# End Source File
# Begin Source File

SOURCE=.\Geos\colorSquare.cpp
# End Source File
# Begin Source File

SOURCE=.\Geos\colorSquare.h
# End Source File
# Begin Source File

SOURCE=.\Geos\cone.cpp
# End Source File
# Begin Source File

SOURCE=.\Geos\cone.h
# End Source File
# Begin Source File

SOURCE=.\Geos\cube.cpp
# End Source File
# Begin Source File

SOURCE=.\Geos\cube.h
# End Source File
# Begin Source File

SOURCE=.\Geos\curve.cpp
# End Source File
# Begin Source File

SOURCE=.\Geos\curve.h
# End Source File
# Begin Source File

SOURCE=.\Geos\cylinder.cpp
# End Source File
# Begin Source File

SOURCE=.\Geos\cylinder.h
# End Source File
# Begin Source File

SOURCE=.\Geos\disk.cpp
# End Source File
# Begin Source File

SOURCE=.\Geos\disk.h
# End Source File
# Begin Source File

SOURCE=.\Geos\imageVert.cpp
# End Source File
# Begin Source File

SOURCE=.\Geos\imageVert.h
# End Source File
# Begin Source File

SOURCE=.\Geos\model.cpp
# End Source File
# Begin Source File

SOURCE=.\Geos\model.h
# End Source File
# Begin Source File

SOURCE=.\Geos\multimodel.cpp
# End Source File
# Begin Source File

SOURCE=.\Geos\multimodel.h
# End Source File
# Begin Source File

SOURCE=.\Geos\polygon.cpp
# End Source File
# Begin Source File

SOURCE=.\Geos\polygon.h
# End Source File
# Begin Source File

SOURCE=.\Geos\primTri.cpp
# End Source File
# Begin Source File

SOURCE=.\Geos\primTri.h
# End Source File
# Begin Source File

SOURCE=.\Geos\rectangle.cpp
# End Source File
# Begin Source File

SOURCE=.\Geos\rectangle.h
# End Source File
# Begin Source File

SOURCE=.\Geos\sphere.cpp
# End Source File
# Begin Source File

SOURCE=.\Geos\sphere.h
# End Source File
# Begin Source File

SOURCE=.\Geos\square.cpp
# End Source File
# Begin Source File

SOURCE=.\Geos\square.h
# End Source File
# Begin Source File

SOURCE=.\Geos\teapot.cpp
# End Source File
# Begin Source File

SOURCE=.\Geos\teapot.h
# End Source File
# Begin Source File

SOURCE=.\Geos\text2d.cpp
# End Source File
# Begin Source File

SOURCE=.\Geos\text2d.h
# End Source File
# Begin Source File

SOURCE=.\Geos\text3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Geos\text3d.h
# End Source File
# Begin Source File

SOURCE=.\Geos\textoutline.cpp
# End Source File
# Begin Source File

SOURCE=.\Geos\textoutline.h
# End Source File
# Begin Source File

SOURCE=.\Geos\triangle.cpp
# End Source File
# Begin Source File

SOURCE=.\Geos\triangle.h
# End Source File
# End Group
# Begin Group "Nongeos"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Nongeos\light.cpp
# End Source File
# Begin Source File

SOURCE=.\Nongeos\light.h
# End Source File
# Begin Source File

SOURCE=.\Nongeos\world_light.cpp
# End Source File
# Begin Source File

SOURCE=.\Nongeos\world_light.h
# End Source File
# End Group
# Begin Group "Manips"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Manips\accumrotate.cpp
# End Source File
# Begin Source File

SOURCE=.\Manips\accumrotate.h
# End Source File
# Begin Source File

SOURCE=.\Manips\alpha.cpp
# End Source File
# Begin Source File

SOURCE=.\Manips\alpha.h
# End Source File
# Begin Source File

SOURCE=.\Manips\ambient.cpp
# End Source File
# Begin Source File

SOURCE=.\Manips\ambient.h
# End Source File
# Begin Source File

SOURCE=.\Manips\ambientRGB.cpp
# End Source File
# Begin Source File

SOURCE=.\Manips\ambientRGB.h
# End Source File
# Begin Source File

SOURCE=.\Manips\color.cpp
# End Source File
# Begin Source File

SOURCE=.\Manips\color.h
# End Source File
# Begin Source File

SOURCE=.\Manips\colorRGB.cpp
# End Source File
# Begin Source File

SOURCE=.\Manips\colorRGB.h
# End Source File
# Begin Source File

SOURCE=.\Manips\depth.cpp
# End Source File
# Begin Source File

SOURCE=.\Manips\depth.h
# End Source File
# Begin Source File

SOURCE=.\Manips\diffuse.cpp
# End Source File
# Begin Source File

SOURCE=.\Manips\diffuse.h
# End Source File
# Begin Source File

SOURCE=.\Manips\diffuseRGB.cpp
# End Source File
# Begin Source File

SOURCE=.\Manips\diffuseRGB.h
# End Source File
# Begin Source File

SOURCE=.\Manips\emission.cpp
# End Source File
# Begin Source File

SOURCE=.\Manips\emission.h
# End Source File
# Begin Source File

SOURCE=.\Manips\emissionRGB.cpp
# End Source File
# Begin Source File

SOURCE=.\Manips\emissionRGB.h
# End Source File
# Begin Source File

SOURCE=.\Manips\linear_path.cpp
# End Source File
# Begin Source File

SOURCE=.\Manips\linear_path.h
# End Source File
# Begin Source File

SOURCE=.\Manips\ortho.cpp
# End Source File
# Begin Source File

SOURCE=.\Manips\ortho.h
# End Source File
# Begin Source File

SOURCE=.\Manips\rotate.cpp
# End Source File
# Begin Source File

SOURCE=.\Manips\rotate.h
# End Source File
# Begin Source File

SOURCE=.\Manips\rotateXYZ.cpp
# End Source File
# Begin Source File

SOURCE=.\Manips\rotateXYZ.h
# End Source File
# Begin Source File

SOURCE=.\Manips\scale.cpp
# End Source File
# Begin Source File

SOURCE=.\Manips\scale.h
# End Source File
# Begin Source File

SOURCE=.\Manips\scaleXYZ.cpp
# End Source File
# Begin Source File

SOURCE=.\Manips\scaleXYZ.h
# End Source File
# Begin Source File

SOURCE=.\Manips\separator.cpp
# End Source File
# Begin Source File

SOURCE=.\Manips\separator.h
# End Source File
# Begin Source File

SOURCE=.\Manips\shininess.cpp
# End Source File
# Begin Source File

SOURCE=.\Manips\shininess.h
# End Source File
# Begin Source File

SOURCE=.\Manips\specular.cpp
# End Source File
# Begin Source File

SOURCE=.\Manips\specular.h
# End Source File
# Begin Source File

SOURCE=.\Manips\specularRGB.cpp
# End Source File
# Begin Source File

SOURCE=.\Manips\specularRGB.h
# End Source File
# Begin Source File

SOURCE=.\Manips\spline_path.cpp
# End Source File
# Begin Source File

SOURCE=.\Manips\spline_path.h
# End Source File
# Begin Source File

SOURCE=.\Manips\translate.cpp
# End Source File
# Begin Source File

SOURCE=.\Manips\translate.h
# End Source File
# Begin Source File

SOURCE=.\Manips\translateXYZ.cpp
# End Source File
# Begin Source File

SOURCE=.\Manips\translateXYZ.h
# End Source File
# End Group
# Begin Group "Controls"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Controls\gemhead.cpp
# End Source File
# Begin Source File

SOURCE=.\Controls\gemhead.h
# End Source File
# Begin Source File

SOURCE=.\Controls\gemkeyboard.cpp
# End Source File
# Begin Source File

SOURCE=.\Controls\gemkeyboard.h
# End Source File
# Begin Source File

SOURCE=.\Controls\gemkeyname.cpp
# End Source File
# Begin Source File

SOURCE=.\Controls\gemkeyname.h
# End Source File
# Begin Source File

SOURCE=.\Controls\gemmouse.cpp
# End Source File
# Begin Source File

SOURCE=.\Controls\gemmouse.h
# End Source File
# Begin Source File

SOURCE=.\Controls\gemorb.cpp
# End Source File
# Begin Source File

SOURCE=.\Controls\gemorb.h
# End Source File
# Begin Source File

SOURCE=.\Controls\gemtablet.cpp
# End Source File
# Begin Source File

SOURCE=.\Controls\gemtablet.h
# End Source File
# Begin Source File

SOURCE=.\Controls\gemwin.cpp
# End Source File
# Begin Source File

SOURCE=.\Controls\gemwin.h
# End Source File
# Begin Source File

SOURCE=.\Controls\render_trigger.cpp
# End Source File
# Begin Source File

SOURCE=.\Controls\render_trigger.h
# End Source File
# End Group
# Begin Group "Pixes"

# PROP Default_Filter ""
# Begin Group "Processing"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Pixes\pix_2grey.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_2grey.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_a_2grey.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_a_2grey.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_aging.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_aging.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_alpha.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_alpha.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_bitmask.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_bitmask.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_color.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_color.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_coloralpha.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_coloralpha.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_colormatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_colormatrix.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_convolve.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_convolve.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_curve.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_curve.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_dot.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_dot.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_flip.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_flip.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_fx.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_fx.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_gain.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_gain.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_hsv2rgb.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_hsv2rgb.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_invert.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_invert.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_normalize.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_normalize.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_offset.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_offset.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_puzzle.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_puzzle.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_resize.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_resize.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_rgb2hsv.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_rgb2hsv.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_test.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_threshold.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_threshold.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_zoom.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_zoom.h
# End Source File
# End Group
# Begin Group "DualInput"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Pixes\pix_add.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_add.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_composite.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_composite.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_diff.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_diff.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_mask.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_mask.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_multiply.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_multiply.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_subtract.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_subtract.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_takealpha.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_takealpha.h
# End Source File
# End Group
# Begin Group "Control"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Pixes\pix_blob.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_blob.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_buf.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_buf.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_clearblock.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_clearblock.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_coordinate.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_coordinate.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_data.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_data.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_dump.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_dump.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_histo.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_histo.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_image.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_image.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_imageInPlace.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_imageInPlace.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_multiimage.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_multiimage.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_pix2sig.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_pix2sig.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_rgba.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_rgba.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_set.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_set.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_sig2pix.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_sig2pix.h
# End Source File
# End Group
# Begin Group "Render"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Pixes\pix_draw.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_draw.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_rectangle.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_rectangle.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_snap.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_snap.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_texture.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_texture.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_texture2.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_texture2.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_write.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_write.h
# End Source File
# End Group
# Begin Group "Video"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Pixes\pix_dv.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_dv.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_film.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_film.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_filmNT.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_filmNT.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_movie.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_movie.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_video.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_video.h
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_videoNT.cpp
# End Source File
# Begin Source File

SOURCE=.\Pixes\pix_videoNT.h
# End Source File
# End Group
# Begin Group "TV"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TV\tv_biquad.cpp
# End Source File
# Begin Source File

SOURCE=.\TV\tv_biquad.h
# End Source File
# Begin Source File

SOURCE=.\TV\tv_movement.cpp
# End Source File
# Begin Source File

SOURCE=.\TV\tv_movement.h
# End Source File
# Begin Source File

SOURCE=.\TV\tv_rtx.cpp
# End Source File
# Begin Source File

SOURCE=.\TV\tv_rtx.h
# End Source File
# Begin Source File

SOURCE=.\TV\tv_test.cpp
# End Source File
# Begin Source File

SOURCE=.\TV\tv_test.h
# End Source File
# End Group
# End Group
# Begin Group "Particles"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Particles\part_color.cpp
# End Source File
# Begin Source File

SOURCE=.\Particles\part_color.h
# End Source File
# Begin Source File

SOURCE=.\Particles\part_damp.cpp
# End Source File
# Begin Source File

SOURCE=.\Particles\part_damp.h
# End Source File
# Begin Source File

SOURCE=.\Particles\part_draw.cpp
# End Source File
# Begin Source File

SOURCE=.\Particles\part_draw.h
# End Source File
# Begin Source File

SOURCE=.\Particles\part_follow.cpp
# End Source File
# Begin Source File

SOURCE=.\Particles\part_follow.h
# End Source File
# Begin Source File

SOURCE=.\Particles\part_gravity.cpp
# End Source File
# Begin Source File

SOURCE=.\Particles\part_gravity.h
# End Source File
# Begin Source File

SOURCE=.\Particles\part_head.cpp
# End Source File
# Begin Source File

SOURCE=.\Particles\part_head.h
# End Source File
# Begin Source File

SOURCE=.\Particles\part_killold.cpp
# End Source File
# Begin Source File

SOURCE=.\Particles\part_killold.h
# End Source File
# Begin Source File

SOURCE=.\Particles\part_killslow.cpp
# End Source File
# Begin Source File

SOURCE=.\Particles\part_killslow.h
# End Source File
# Begin Source File

SOURCE=.\Particles\part_orbitpoint.cpp
# End Source File
# Begin Source File

SOURCE=.\Particles\part_orbitpoint.h
# End Source File
# Begin Source File

SOURCE=.\Particles\part_size.cpp
# End Source File
# Begin Source File

SOURCE=.\Particles\part_size.h
# End Source File
# Begin Source File

SOURCE=.\Particles\part_source.cpp
# End Source File
# Begin Source File

SOURCE=.\Particles\part_source.h
# End Source File
# Begin Source File

SOURCE=.\Particles\part_targetcolor.cpp
# End Source File
# Begin Source File

SOURCE=.\Particles\part_targetcolor.h
# End Source File
# Begin Source File

SOURCE=.\Particles\part_targetsize.cpp
# End Source File
# Begin Source File

SOURCE=.\Particles\part_targetsize.h
# End Source File
# Begin Source File

SOURCE=.\Particles\part_velcone.cpp
# End Source File
# Begin Source File

SOURCE=.\Particles\part_velcone.h
# End Source File
# Begin Source File

SOURCE=.\Particles\part_velsphere.cpp
# End Source File
# Begin Source File

SOURCE=.\Particles\part_velsphere.h
# End Source File
# End Group
# Begin Group "MarkEx"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MarkEx\abs.cpp
# End Source File
# Begin Source File

SOURCE=.\MarkEx\average.cpp
# End Source File
# Begin Source File

SOURCE=.\MarkEx\counter.cpp
# End Source File
# Begin Source File

SOURCE=.\MarkEx\hsvrgb.cpp
# End Source File
# Begin Source File

SOURCE=.\MarkEx\m_control.cpp
# End Source File
# Begin Source File

SOURCE=.\MarkEx\MarkExSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\MarkEx\multiselect.cpp
# End Source File
# Begin Source File

SOURCE=.\MarkEx\randomF.cpp
# End Source File
# Begin Source File

SOURCE=.\MarkEx\reson.cpp
# End Source File
# Begin Source File

SOURCE=.\MarkEx\strcat.cpp
# End Source File
# Begin Source File

SOURCE=.\MarkEx\tripleLine.cpp
# End Source File
# Begin Source File

SOURCE=.\MarkEx\vector.cpp
# End Source File
# End Group
# End Target
# End Project
