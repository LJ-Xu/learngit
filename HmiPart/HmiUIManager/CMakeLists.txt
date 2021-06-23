#aux_source_directory(./src src_files)
#add_library(Project STATIC ${src_files})

add_library(HmiUIManager STATIC 
	src/UIManager.cpp
	src/BaseGControl.cpp
	src/BaseControl.cpp
	src/IControlFactory.cpp
	src/UIThread.cpp
	src/Resource/IResourceService.cpp
	src/Resource/ResourceService.cpp
	src/Secure/IPermission.cpp
	src/Secure/UsrPermission.cpp
	src/Tools/BinBitmap.cpp
	src/HMIPage.cpp
	src/GHMIPage.cpp
	src/HMIWindow.cpp
	src/IPageFactory.cpp
	src/Macro/XJMacro.cpp
	src/Data/UIDataService.cpp
	src/ModalPage.cpp

	)
