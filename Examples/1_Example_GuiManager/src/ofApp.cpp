#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	parameters.setName("ofApp");
	parameters.add(vNextIndexScene);
	parameters.add(vPrevIndexScene);
	parameters.add(indexScene);
	parameters.add(nameScene);

#if 0
	// Optional workflow: 
	parameters.add(guiManager.paramsGuiManager);//populate some internal controls
	guiManager.setAutoAddInternalParamasToMainPanel(false);//disable population visible toggles
#endif

	paramsAnimate.setName("Animate");
	paramsAnimate.add(bAnimRotate);
	paramsAnimate.add(rotateSpeed);
	paramsAnimate.add(bAnimZoom);
	paramsAnimate.add(zoomSpeed);
	paramsAnimate.add(powZoom);

	paramsTransforms.setName("Transforms");
	paramsTransforms.add(scale);
	paramsTransforms.add(yPos);

	paramsScene.setName("Scene");
	paramsScene.add(indexScene);
	paramsScene.add(nameScene);
	paramsScene.add(vReset);
	paramsScene.add(vNextIndexScene);
	paramsScene.add(vPrevIndexScene);

	setupGui();
}

//--------------------------------------------------------------
void ofApp::setupGui() {
	//all the panels
	gui.setup(parameters);
	guiAnimate.setup(paramsAnimate);
	guiTransform.setup(paramsTransforms);
	guiScene.setup(paramsScene);

	//--

	//the panels manager
	guiManager.setup(&gui); //anchor main left panel

	//example for selecting different setup approaches
	const int APP_MODE = 0;
	if (APP_MODE == 0) {
		///passing only the panel. faster approach.
		guiManager.add(&gui);
		guiManager.add(&guiAnimate);
		guiManager.add(&guiTransform);
		guiManager.add(&guiScene);
	} else if (APP_MODE == 1) {
		//passing panel and each toggle if you have one.
		guiManager.add(&gui, bGui);
		guiManager.add(&guiAnimate, bGuiAnimate);
		guiManager.add(&guiTransform, bGuiTransform);
		guiManager.add(&guiScene, bGuiScene);
	} else if (APP_MODE == 2) {
		//passing panel, each toggle and each mode
		guiManager.add(&gui, bGui, ofxSurfing::SURFING__OFXGUI__MODE_FULL);
		guiManager.add(&guiAnimate, bGuiAnimate, ofxSurfing::SURFING__OFXGUI__MODE_FULL);
		guiManager.add(&guiTransform, bGuiTransform);
		guiManager.add(&guiScene, bGuiScene);
	}

	//guiManager.startup(true); //Optional. pass true to ignore the settings!
}

//--------------------------------------------------------------
void ofApp::update() {
	guiManager.refreshGui(); //will apply index layout when autoLayout enabled!
}

//--------------------------------------------------------------
void ofApp::draw() {
	guiManager.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == OF_KEY_BACKSPACE) guiManager.setGuiAnchorPositionToLayout(ofxSurfing::SURFING_LAYOUT_TOP_CENTER);
}
