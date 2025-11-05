#pragma once

#include "ofMain.h"

#include "SurfingFilesManager.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	SurfingFilesManager filesManager;
	ofParameter<std::string> path_folder;

	ofxPanel gui;
};
