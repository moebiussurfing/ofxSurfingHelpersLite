#pragma once

#include "ofMain.h"
#include "ofxSurfingHelpersLite.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void setupGui();
	void update();
	void draw();

	void keyPressed(int key);

	ofxPanel gui;//main anchor panel
	SurfingOfxGuiPanelsManager guiManager;
	ofParameterGroup parameters;
	ofParameter<bool> bGui { "myUI ofApp", true };//optional when APP_MODE = 2

	ofxPanel guiTransform;
	ofParameterGroup paramsTransforms;
	ofParameter<float> scale { "Scale", 0, -1.f, 1.f };
	ofParameter<float> yPos { "Pos y", 0, -1.f, 1.f };
	ofParameter<bool> bGuiTransform { "myUI Transform", true };//optional when APP_MODE = 2

	ofxPanel guiAnimate;
	ofParameterGroup paramsAnimate;
	ofParameter<bool> bAnimRotate { "Rotate Anim", false };
	ofParameter<float> rotateSpeed { "Rotate Speed", 0.5f, 0, 1 };
	ofParameter<bool> bAnimZoom { "Zoom Anim", false };
	ofParameter<float> zoomSpeed { "Zoom Speed", 0.5f, 0, 1 };
	ofParameter<float> powZoom { "Zoom Pow", 0.5f, 0, 1 };
	ofParameter<bool> bGuiAnimate { "myUI Animate", true };//optional when APP_MODE = 2

	ofxPanel guiScene;
	ofParameterGroup paramsScene;
	ofParameter<int> indexScene { "SCENE", 0, 0, 2 };
	ofParameter<string> nameScene { "Name", "" };
	ofParameter<void> vReset { "Reset Scene" };
	ofParameter<void> vNextIndexScene { "Next" };
	ofParameter<void> vPrevIndexScene { "Previous" };
	ofParameter<bool> bGuiScene { "myUI Scene", true };//optional when APP_MODE = 2
};
