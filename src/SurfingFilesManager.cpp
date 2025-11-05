// file: SurfingFilesManager.cpp
#include "SurfingFilesManager.h"
#include "ofxSurfingSerializers.h" // For settings save/load

//--------------------------------------------------------------
SurfingFilesManager::SurfingFilesManager() {
}

//--------------------------------------------------------------
SurfingFilesManager::~SurfingFilesManager() {
	if (!bDoneExit) exit();
}

//--------------------------------------------------------------
void SurfingFilesManager::setup(bool bHideExportTrigger_) {
	ofLogNotice("SurfingFilesManager") << "setup( ) bHideExportTrigger_:" << bHideExportTrigger_;

	bHideExportTrigger = bHideExportTrigger_;
	setup();
}

//--------------------------------------------------------------
void SurfingFilesManager::setup() {
	ofLogNotice("SurfingFilesManager") << "setup()";

	setupParams();
	setupCallbacks();

	// Load saved settings
	ofxSurfing::loadSettings(params);
}

//--------------------------------------------------------------
void SurfingFilesManager::setupParams() {
	ofLogNotice("SurfingFilesManager") << "setupParams()";

	params.setName(name);
	params.add(vChoosePathFolder.set("Set Folder"));
	params.add(pathFolder.set("Folder", ""));
	params.add(vOpenPathFolder.set("Open Folder"));
	if (!bHideExportTrigger)
		params.add(vExport.set("Export"));
}

//--------------------------------------------------------------
void SurfingFilesManager::setupCallbacks() {
	ofLogNotice("SurfingFilesManager") << "setupCallbacks()";
	e_vChooseFolder = vChoosePathFolder.newListener([this](const void * sender) {
		doChooseFolder();
	});

	e_vOpenExportFolder = vOpenPathFolder.newListener([this](const void * sender) {
		doOpenExportFolder();
	});

	e_vExport = vExport.newListener([this](const void * sender) {
		doExport();
	});
}

//--------------------------------------------------------------
void SurfingFilesManager::doExport() {
	ofLogNotice("SurfingFilesManager") << "doExport()";

	if (!exportCallback) {
		ofLogWarning("SurfingFilesManager") << "No export callback set!";
		ofLogWarning("SurfingFilesManager") << "Must be setled in ofApp!";
		return;
	}

	std::string folder = (pathFolder.get().empty())
		? ofToDataPath("", true)
		: pathFolder.get();

	std::string savedFile = exportCallback(folder);

	//--

	if (!savedFile.empty()) {
		ofLogNotice("SurfingFilesManager") << "Exported to: " << savedFile;

		// Auto-open the file (cross-platform)
		std::string quotedPath = "\"" + ofFilePath::getAbsolutePath(savedFile) + "\"";

#ifdef TARGET_OSX
		ofSystem("open " + quotedPath);
#elif defined(TARGET_WIN32)
		ofSystem("start \"\" " + quotedPath);
#elif defined(TARGET_LINUX)
		ofSystem("xdg-open " + quotedPath);
#endif
	}
}

//--------------------------------------------------------------
void SurfingFilesManager::doChooseFolder() {
	ofLogNotice("SurfingFilesManager") << "doChooseFolder()";

	// Determine default folder: if not set yet, use the app's data folder
	//std::string defaultFolder = pathFolder.get().empty() ? ofToDataPath("", true) : pathFolder.get();
	std::string defaultFolder = pathFolder.get().empty() ? ofToDataPath("", true) : ofToDataPath(pathFolder.get(),true);

	ofFileDialogResult result = ofSystemLoadDialog("Select output folder", true, defaultFolder);

	if (result.bSuccess) {
		pathFolder = result.getPath();
		ofLogNotice("SurfingFilesManager") << "Selected folder: " << pathFolder.get();

		// Notify external listener if provided
		if (folderSelectedCallback) {
			folderSelectedCallback(pathFolder.get());
		}

		// Save
		ofxSurfing::saveSettings(params);
	}
}

//--------------------------------------------------------------
void SurfingFilesManager::doOpenExportFolder() {
	ofLogNotice("SurfingFilesManager") << "doOpenExportFolder()";

	std::string folderStr = (pathFolder.get().empty())
		? ofToDataPath("", true)
		: pathFolder.get();

	ofFile folder(folderStr);
	if (!folder.isDirectory()) {
		folderStr = folder.getEnclosingDirectory();
	}
	folderStr = folder.getAbsolutePath();

	ofLogNotice("SurfingFilesManager") << "Opening folder: " << folderStr;

#ifdef TARGET_OSX
	std::string cmd = "open \"" + folderStr + "\"";
	system(cmd.c_str());
#elif defined(TARGET_LINUX)
	std::string cmd = "xdg-open \"" + folderStr + "\"";
	system(cmd.c_str());
#elif defined(_WIN32)
	std::string cmd = "start \"\" \"" + folderStr + "\"";
	system(cmd.c_str());
#endif
}

//--------------------------------------------------------------
void SurfingFilesManager::exit() {
	ofLogNotice("SurfingFilesManager") << "exit()";

	ofxSurfing::saveSettings(params);
	bDoneExit = true;
}
