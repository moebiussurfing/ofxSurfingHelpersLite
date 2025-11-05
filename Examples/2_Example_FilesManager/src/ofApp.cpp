#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	// Custom callbacks for files manager

	// Notify ofApp when a folder is selected in the manager
	filesManager.setFolderSelectedCallback([this](const std::string & folder) {
		path_folder = folder; // store selected folder
		ofLogNotice("ofApp") << "Folder selected: " << folder;
		// TODO: act accordingly (e.g., enable export button, refresh state, etc.)
	});

	// Setup export callback (debug example): writes a text file into selected folder
	filesManager.setExportCallback([](const std::string & folder) -> std::string {
		// Ensure folder exists
		ofDirectory dir(folder);
		if (!dir.exists()) dir.create(true);

		// Compose file name and full path
		std::string filename = "export_debug_" + ofGetTimestampString("%Y-%m-%d_%H-%M-%S") + ".txt";
		std::string fullpath = ofFilePath::join(folder, filename);

		// Prepare content
		ofBuffer buf;
		buf.append("Debug export from ofApp\n");
		buf.append("Saved at: " + fullpath + "\n");

		// Write and return path if success
		if (ofBufferToFile(fullpath, buf)) {
			return fullpath;
		}
		return std::string();
	});

	//--

	// Setup files manager

	filesManager.setName("myOfAppExporter");//-> customize the default name "Export"

	filesManager.setup();

	//filesManager.setup(true);//-> to hide export trigger in case you dont need it

	//--

	// Gui
	gui.setup(filesManager.params);
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {
	gui.draw();
}
