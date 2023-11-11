#pragma once
#include "ofMain.h"

#include "ofxGui.h"

//------

// CONSTANTS

#define SURFING__PAD_TO_WINDOW_BORDERS 8

#define SURFING__PAD_OFXGUI_BETWEEN_PANELS 4

#define SURFING__STRING_BOX__DEFAULT_XPAD 20
#define SURFING__STRING_BOX__DEFAULT_YPAD 20
#define SURFING__STRING_BOX__DEFAULT_ROUND 5.f // 0.f to not rounded

//#define SURFING__STRING_BOX__INCLUDE_EXTRA_LAYOUTS // include mouse pos

//#define SURFING__STRING_BOX___USE_TTF_INSTEAD_OF_BITMAP_FONT___WIP//TODO

//--

// ofxGui

// Default font and sizes/colors will be used to customize ofxGui!
#define SURFING__OFXGUI__FONT_DEFAULT_SIZE 9
#define SURFING__OFXGUI__FONT_DEFAULT_SIZE_MINI 7
#define SURFING__OFXGUI__FONT_DEFAULT_PATH "assets/fonts/NotoSansMono-Regular.ttf"

//#define SURFING__OFXGUI__FONT_DEFAULT_SIZE 10
//#define SURFING__OFXGUI__FONT_DEFAULT_PATH "assets/fonts/Montserrat-Regular.ttf"

//#define SURFING__OFXGUI__FONT_DEFAULT_PATH "assets/fonts/Inter-Regular.ttf"
//#define SURFING__OFXGUI__FONT_DEFAULT_PATH "assets/fonts/JetBrainsMonoNL-ExtraBold.ttf"
//#define SURFING__OFXGUI__FONT_DEFAULT_PATH "assets/fonts/JetBrainsMonoNL-SemiBold.ttf"
//#define SURFING__OFXGUI__FONT_DEFAULT_PATH "assets/fonts/JetBrainsMono-Bold.ttf"

//------

namespace ofxSurfing {

//------

/*
* Settings Management
* 
	Settings de/serializers for ofParameterGroup.
*/

// Load
//--------------------------------------------------------------
inline bool loadSettings(ofParameterGroup & parameters, string path) {
	//verify that the file exist
	ofFile f;
	bool b = f.doesFileExist(path);
	if (b)
		ofLogNotice("ofxSurfing") << "Found file: " << path;
	else
		ofLogError("ofxSurfing") << "File " << path
								 << " for ofParameterGroup " << parameters.getName() << " not found!";
	//load if exist
	if (b) {
		ofJson settings;
		settings = ofLoadJson(path);
		ofDeserialize(settings, parameters);
	}

	return b;
}

// Save
//--------------------------------------------------------------
inline bool saveSettings(ofParameterGroup & parameters, string path) {
	//if the destination folder do not exists, will be created.
	if (!ofDirectory::doesDirectoryExist(ofFilePath::getEnclosingDirectory(path))) {
		if (ofFilePath::createEnclosingDirectory(path))
			ofLogWarning("ofxSurfing") << "Created enclosing folder for: " << path;
		else
			ofLogError("ofxSurfing") << "Unable to create enclosing folder for: " << path;
	}
	//try to save the file
	ofJson settings;
	ofSerialize(settings, parameters);
	bool b = ofSavePrettyJson(path, settings);
	if (b)
		ofLogNotice("ofxSurfing") << "Saved ofParameterGroup: " << parameters.getName() << " to " << path;
	else
		ofLogError("ofxSurfing") << "Error saving: " << parameters.getName() << " to " << path;

	return b;
}

// Create if folder not found
//--------------------------------------------------------------
inline void checkFolderOrCreate(string path) {
	if (!ofDirectory::doesDirectoryExist(ofFilePath::getEnclosingDirectory(path))) {
		if (ofFilePath::createEnclosingDirectory(path))
			ofLogWarning("ofxSurfing") << "Created enclosing folder for: " << path;
		else
			ofLogError("ofxSurfing") << "Unable to create enclosing folder for: " << path;
	}
}

// LEGACY
// for ofxSurfingHelpers
inline bool loadGroup(ofParameterGroup & parameters, string path) {
	return loadSettings(parameters, path);
}
inline bool saveGroup(ofParameterGroup & parameters, string path) {
	return saveSettings(parameters, path);
}

//------

/*
* App Window
* 
	App window tools to handle position, settings or name.
*/

// Get the OF project file path name and set to the window title.
//--------------------------------------------------------------
inline string getProjectName() {
	of::filesystem::path path = ofToDataPath("", true);
	string s = (path.parent_path().parent_path().filename()).string();
	return s;
}

//--------------------------------------------------------------
inline void setWindowTitleAsProjectName() {
	ofSetWindowTitle(getProjectName());
}

// WARNING!
// For my personal setup!
// I have three FHD (1920x1080) monitors:
// 2nd monitor (central) is settled as the main system display.
// Central and Left monitor are landscape.
// Right monitor is portrait.
// Examples:
//	ofxSurfing::setWindowAtMonitor(); // Center monitor landscape
//	ofxSurfing::setWindowAtMonitor(-1); // Move to left monitor landscape
//	ofxSurfing::setWindowAtMonitor(1, true); // Move to right monitor portrait
//	ofxSurfing::setWindowAtMonitor(-1, true); // Left monitor portrait
//	ofxSurfing::setWindowAtMonitor(-1, false); // Left monitor landscape
//	ofxSurfing::setWindowAtMonitor(1, false , 120, true); // Right monitor landscape, 60fps, vSync on
//--------------------------------------------------------------
inline void setWindowAtMonitor(int pos = 0, bool bPortrait = false, int fps = 60, bool vSync = false) {

	ofSetFrameRate(fps);
	ofSetVerticalSync(vSync);

	// My PERSONAL displays setup
	// pos monitors
	// -1: FHD, landscape
	//  0: FHD, landscape / main display
	//  1: FHD, portrait
	// My main display
	int wMain = 1920;
	int hMain = 1080;

	//fix window bar height on WIN_32.
	int hBar = 32;
	int yPos = hBar; //move a bit down to move it inside the window.
	int hOffset = hBar; //make the window height smaller.

	int x, y, w, h;

	x = 0;
	y = 0;
	y += yPos;

	w = bPortrait ? hMain : wMain;
	h = bPortrait ? wMain : hMain;

	h -= hOffset;

	if (pos == -1)
		x = bPortrait ? -hMain : -wMain;
	else if (pos == 1)
		x = wMain;

	ofSetWindowPosition(x, y);
	ofSetWindowShape(w, h);
}

//--------------------------------------------------------------
inline string setWindowShapeSquared(int sz = 800) {
	string s = "Squared " + ofToString(sz) + "x" + ofToString(sz) + " (1:1)";
	ofSetWindowShape(sz, sz);

	return s;
}

//--------------------------------------------------------------
inline string setWindowShapeForInstagram(size_t i = 0, bool bForcePos = false) {
	/*
	* Instagram Social media 
	* Image type sizes
	* Dimensions in pixels
	*/

	vector<string> names = {
		"IGTV Cover Photo: \n420x654 1:1.55",
		"IG Landscape Photo: \n1080x566 1.91:1",
		"IG Portrait: \n1080x1350 4:5",
		"IG Story: \n1080x1920 9:16",
		"IG Square: \n1080x1080 1:1",
	};

	i = ofClamp(i, 0, names.size() - 1);

	//if (bForcePos) ofSetWindowPosition(0, 0);

	if (i == 0) ofSetWindowShape(420, 654); // IGTV Cover
	if (i == 1) ofSetWindowShape(1080, 566); // IGLandscape
	if (i == 2) ofSetWindowShape(1080, 1350); // IG Portrait
	if (i == 3) ofSetWindowShape(1080, 1920); // IG Story
	if (i == 4) ofSetWindowShape(1080, 1080); // IG Square

	if (bForcePos) ofSetWindowPosition(0, 0); //fix force

	return names[i];

	//// TODO:
	//// allow resize on runtime
	//// set instagram size
	//case OF_KEY_F6: {
	//	int w, h;
	//	w = 864;
	//	h = 1080 + 19;
	//	ofSetWindowShape(w, h);
	//	windowResized(w, h);
	//	cap_w = w;
	//	cap_h = h;
	//	buildAllocateFbo();
	//} break;
}

//------

/*
*  ofDrawBitmapStringBox
* 
	A text box widget with 12 layout positions management.
*/

//--------------------------------------------------------------
inline void ofDrawBitmapStringBox(string s, int x, int y, float round = SURFING__STRING_BOX__DEFAULT_ROUND) {
	bool bdebug = 0;

#ifdef SURFING__STRING_BOX___USE_TTF_INSTEAD_OF_BITMAP_FONT___WIP
	static bool bDone = 0;
	static ofTrueTypeFont font;
	if (!bDone) {
		bDone = 1;
		string path = SURFING__OFXGUI__FONT_DEFAULT_PATH;
		int sz = SURFING__OFXGUI__FONT_DEFAULT_SIZE;
		font.loadFont(path, sz);
	}
#endif

	int a1 = 255;
	int a2 = 200;
	ofColor c1 = ofColor(ofColor::white, a1);
	ofColor c2 = ofColor(ofColor::black, a2);

	int xpad = SURFING__STRING_BOX__DEFAULT_XPAD;
	int ypad = SURFING__STRING_BOX__DEFAULT_YPAD;

	x += xpad;
	y += ypad;

	ofBitmapFont bf;
	auto bb = bf.getBoundingBox(s, 0, 0);

	//fix
	int xoffset = 0;
	int yoffset = 0;

	//xoffset = 0; //TODO
	yoffset = 11; //TODO

	ofRectangle bb1;
	bb1 = { (float)x, (float)y, bb.width, bb.height };

	ofRectangle bb2;
	bb2 = { (float)x - xpad, (float)y - ypad, bb1.width + 2 * xpad, bb1.height + 2 * ypad };

	{
		ofPushStyle();
		ofFill();
		ofSetColor(c2);

		if (round == 0)
			ofDrawRectangle(bb2);
		else
			ofDrawRectRounded(bb2, round);

		ofPopStyle();
	}

#if (bdebug)
	ofPushStyle();
	ofNoFill();
	ofSetLineWidth(2);
	ofSetColor(ofColor::yellow);

	ofDrawRectangle(bb1);

	ofSetColor(ofColor::red);
	ofDrawRectangle(bb2);

	ofPopStyle();
#endif

	ofPushStyle();
	ofSetColor(c1);

#ifdef SURFING__STRING_BOX___USE_TTF_INSTEAD_OF_BITMAP_FONT___WIP
	if (font.isLoaded())
		font.drawString(s, x + xoffset, y + yoffset);
	else
		ofDrawBitmapString(s, x + xoffset, y + yoffset);
#else
	ofDrawBitmapString(s, x + xoffset, y + yoffset);
#endif

	ofPopStyle();
}

//--------------------------------------------------------------
inline ofRectangle getBBBitmapStringBox(string s, int x = 0, int y = 0) {
	int xpad = SURFING__STRING_BOX__DEFAULT_XPAD;
	int ypad = SURFING__STRING_BOX__DEFAULT_YPAD;

	x += xpad;
	y += ypad;

	ofRectangle bb;

#ifdef SURFING__STRING_BOX___USE_TTF_INSTEAD_OF_BITMAP_FONT___WIP
	static bool bDone = 0;
	static ofTrueTypeFont font;
	if (!bDone) {
		bDone = 1;
		string path = SURFING__OFXGUI__FONT_DEFAULT_PATH;
		int sz = SURFING__OFXGUI__FONT_DEFAULT_SIZE;
		font.loadFont(path, sz);
	}
	if (font.isLoaded())
		bb = font.getStringBoundingBox(s, 0, 0);
	else {
		ofBitmapFont bf;
		bb = bf.getBoundingBox(s, 0, 0);
	}
#else
	ofBitmapFont bf;
	bb = bf.getBoundingBox(s, 0, 0);
#endif

	ofRectangle bb1;
	bb1 = { (float)x, (float)y, bb.width, bb.height };

	ofRectangle bb2;
	bb2 = { (float)x - xpad, (float)y - ypad, bb1.width + 2 * xpad, bb1.height + 2 * ypad };

	return bb2;
}

//--------------------------------------------------------------
inline glm::vec2 getBitmapStringBoxPosToCenterLeft(string s, int xpad = SURFING__PAD_TO_WINDOW_BORDERS) {
	auto bb = getBBBitmapStringBox(s, 0, 0);
	int x = xpad;
	int y = ofGetHeight() / 2 - bb.getHeight() / 2;
	glm::vec2 p { x, y };
	return p;
}

//--------------------------------------------------------------
inline glm::vec2 getBitmapStringBoxPosToCenter(string s) {
	auto bb = getBBBitmapStringBox(s, 0, 0);
	int x = ofGetWidth() / 2 - bb.getWidth() / 2;
	int y = ofGetHeight() / 2 - bb.getHeight() / 2;
	glm::vec2 p { x, y };
	return p;
}

//--------------------------------------------------------------
inline glm::vec2 getBitmapStringBoxPosToCenterRight(string s, int xpad = SURFING__PAD_TO_WINDOW_BORDERS) {
	auto bb = getBBBitmapStringBox(s, 0, 0);
	int x = ofGetWidth() - bb.getWidth() - xpad;
	int y = ofGetHeight() / 2 - bb.getHeight() / 2;
	glm::vec2 p { x, y };
	return p;
}

//--------------------------------------------------------------
inline glm::vec2 getBitmapStringBoxPosToTopCenter(string s, int ypad = SURFING__PAD_TO_WINDOW_BORDERS) {
	auto bb = getBBBitmapStringBox(s, 0, 0);
	int x = ofGetWidth() / 2 - bb.getWidth() / 2;
	int y = ypad;
	glm::vec2 p { x, y };
	return p;
}

//--------------------------------------------------------------
inline glm::vec2 getBitmapStringBoxPosToTopLeft(string s, int xpad = SURFING__PAD_TO_WINDOW_BORDERS, int ypad = SURFING__PAD_TO_WINDOW_BORDERS) {
	auto bb = getBBBitmapStringBox(s, 0, 0);
	int x = xpad;
	int y = ypad;
	glm::vec2 p { x, y };
	return p;
}

//--------------------------------------------------------------
inline glm::vec2 getBitmapStringBoxPosToTopRight(string s, int xpad = SURFING__PAD_TO_WINDOW_BORDERS, int ypad = SURFING__PAD_TO_WINDOW_BORDERS) {
	auto bb = getBBBitmapStringBox(s, 0, 0);
	int x = ofGetWidth() - bb.getWidth() - xpad;
	int y = ypad;
	glm::vec2 p { x, y };
	return p;
}

//--------------------------------------------------------------
inline glm::vec2 getBitmapStringBoxPosToBottomLeft(string s, int xpad = SURFING__PAD_TO_WINDOW_BORDERS, int ypad = SURFING__PAD_TO_WINDOW_BORDERS) {
	auto bb = getBBBitmapStringBox(s, 0, 0);
	int x = xpad;
	int y = ofGetHeight() - bb.getHeight() - ypad;
	glm::vec2 p { x, y };
	return p;
}

//--------------------------------------------------------------
inline glm::vec2 getBitmapStringBoxPosToBottomCenter(string s, int ypad = SURFING__PAD_TO_WINDOW_BORDERS) {
	auto bb = getBBBitmapStringBox(s, 0, 0);
	int x = ofGetWidth() / 2 - bb.getWidth() / 2;
	int y = ofGetHeight() - bb.getHeight() - ypad;
	glm::vec2 p { x, y };
	return p;
}

//--------------------------------------------------------------
inline glm::vec2 getBitmapStringBoxPosToBottomRight(string s, int xpad = SURFING__PAD_TO_WINDOW_BORDERS, int ypad = SURFING__PAD_TO_WINDOW_BORDERS) {
	auto bb = getBBBitmapStringBox(s, 0, 0);
	int x = ofGetWidth() - bb.getWidth() - xpad;
	int y = ofGetHeight() - bb.getHeight() - ypad;
	glm::vec2 p { x, y };
	return p;
}

enum SURFING_LAYOUT {
	SURFING_LAYOUT_TOP_LEFT = 0,
	SURFING_LAYOUT_TOP_CENTER,
	SURFING_LAYOUT_TOP_RIGHT,

	SURFING_LAYOUT_CENTER_LEFT,
	SURFING_LAYOUT_CENTER,
	SURFING_LAYOUT_CENTER_RIGHT,

	SURFING_LAYOUT_BOTTOM_LEFT,
	SURFING_LAYOUT_BOTTOM_CENTER,
	SURFING_LAYOUT_BOTTOM_RIGHT,

//--

#ifdef SURFING__STRING_BOX__INCLUDE_EXTRA_LAYOUTS
	SURFING_LAYOUT_MOUSE_POS,
	SURFING_LAYOUT_MOUSE_POS_CENTER,
#endif

	SURFING_LAYOUT_AMOUNT,
};

//--------------------------------------------------------------
inline string getLayoutName(SURFING_LAYOUT layout) {
	string s;
	switch (layout) {

	case SURFING_LAYOUT_TOP_LEFT:
		s = "TOP LEFT";
		break;
	case SURFING_LAYOUT_TOP_CENTER:
		s = "TOP CENTER";
		break;
	case SURFING_LAYOUT_TOP_RIGHT:
		s = "TOP RIGHT";
		break;

	case SURFING_LAYOUT_CENTER_LEFT:
		s = "CENTER LEFT";
		break;
	case SURFING_LAYOUT_CENTER:
		s = "CENTER";
		break;
	case SURFING_LAYOUT_CENTER_RIGHT:
		s = "CENTER RIGHT";
		break;

	case SURFING_LAYOUT_BOTTOM_LEFT:
		s = "BOTTOM LEFT";
		break;
	case SURFING_LAYOUT_BOTTOM_CENTER:
		s = "BOTTOM CENTER";
		break;
	case SURFING_LAYOUT_BOTTOM_RIGHT:
		s = "BOTTOM RIGHT";
		break;

		//--

#ifdef SURFING__STRING_BOX__INCLUDE_EXTRA_LAYOUTS
	case SURFING_LAYOUT_MOUSE_POS:
		s = "MOUSE POS";
		break;
	case SURFING_LAYOUT_MOUSE_POS_CENTER:
		s = "MOUSE POS C";
		break;
#endif

	default:
		s = "NONE";
		break;
	}
	return s;
}
//--------------------------------------------------------------
inline string getLayoutName(int layout) {
	return getLayoutName((SURFING_LAYOUT)layout);
}

//--------------------------------------------------------------
inline glm::vec2 getBitmapStringBoxPosToLayout(string s, SURFING_LAYOUT layout) {
	glm::vec2 p { 0, 0 };

	if (layout == SURFING_LAYOUT_CENTER_LEFT) {
		// center
		p = getBitmapStringBoxPosToCenterLeft(s);
	} else if (layout == SURFING_LAYOUT_CENTER) {
		// center
		p = getBitmapStringBoxPosToCenter(s);
	} else if (layout == SURFING_LAYOUT_CENTER_RIGHT) {
		// center
		p = getBitmapStringBoxPosToCenterRight(s);
	}

	else if (layout == SURFING_LAYOUT_TOP_LEFT) {
		// top-left
		p = getBitmapStringBoxPosToTopLeft(s);
	} else if (layout == SURFING_LAYOUT_TOP_CENTER) {
		// top-center
		p = getBitmapStringBoxPosToTopCenter(s);
	} else if (layout == SURFING_LAYOUT_TOP_RIGHT) {
		// top-right
		p = getBitmapStringBoxPosToTopRight(s);
	}

	else if (layout == SURFING_LAYOUT_BOTTOM_LEFT) {
		// bottom-left
		p = getBitmapStringBoxPosToBottomLeft(s);
	} else if (layout == SURFING_LAYOUT_BOTTOM_CENTER) {
		// bottom-center
		p = getBitmapStringBoxPosToBottomCenter(s);
	} else if (layout == SURFING_LAYOUT_BOTTOM_RIGHT) {
		// bottom-right
		p = getBitmapStringBoxPosToBottomRight(s);
	}

	//--

#ifdef SURFING__STRING_BOX__INCLUDE_EXTRA_LAYOUTS
	else if (layout == SURFING_LAYOUT_MOUSE_POS) {
		// mouse pos clamped inside the window
		auto bb = getBBBitmapStringBox(s);
		int xm = ofGetMouseX();
		int ym = ofGetMouseY();
		int x = ofClamp(xm, 0, ofGetWidth() - bb.width);
		int y = ofClamp(ym, 0, ofGetHeight() - bb.height);
		p = { x, y };
	} else if (layout == SURFING_LAYOUT_MOUSE_POS_CENTER) {
		// mouse pos centered clamped inside the window
		auto bb = getBBBitmapStringBox(s);
		int xm = ofGetMouseX() - bb.width / 2;
		int ym = ofGetMouseY() - bb.height / 2;
		int x = ofClamp(xm, 0, ofGetWidth() - bb.width);
		int y = ofClamp(ym, 0, ofGetHeight() - bb.height);
		p = { x, y };
	}
#endif

	return p;
}

//--------------------------------------------------------------
inline ofRectangle getBBBitmapStringBoxToLayout(string s, SURFING_LAYOUT layout) {
	glm::vec2 p = getBitmapStringBoxPosToLayout(s, layout);
	return getBBBitmapStringBox(s, p.x, p.y);
}
//--------------------------------------------------------------
inline ofRectangle getBBBitmapStringBoxToLayout(string s, int layout) {
	return getBBBitmapStringBoxToLayout(s, (SURFING_LAYOUT)layout);
}

//--------------------------------------------------------------
inline glm::vec2 getBitmapStringBoxPosToLayout(string s, int layout) {
	return getBitmapStringBoxPosToLayout(s, (SURFING_LAYOUT)layout);
}

//--------------------------------------------------------------
inline void ofDrawBitmapStringBox(string s, SURFING_LAYOUT layout = SURFING_LAYOUT_CENTER) {
	glm::vec2 p = ofxSurfing::getBitmapStringBoxPosToLayout(s, layout);
	ofxSurfing::ofDrawBitmapStringBox(s, p.x, p.y);
}
//--------------------------------------------------------------
inline void ofDrawBitmapStringBox(string s, int layout /* = 0*/) {
	ofDrawBitmapStringBox(s, (SURFING_LAYOUT)layout);
}

/*
	EXAMPLES

	// simple
	{
		ofxSurfing::ofDrawBitmapStringBox(s, layout);//centered
	}
	
	// debug fps
	{
		float fps = ofGetFrameRate();
		string s = ofToString(fps, 1);
		s += " FPS";
		ofxSurfing::ofDrawBitmapStringBox(s, ofxSurfing::SURFING_LAYOUT_BOTTOM_RIGHT);
	}

	// browse layouts
	{
		keyPressed(int key) 
		if (key == 'l') {
			helpLayout++;
			helpLayout = helpLayout % ((int)ofxSurfing::SURFING_LAYOUT_AMOUNT + 1);
			buildHelp();
		}
	}
*/

//------

/*
* ofxGui
* 
	ofxGui Helpers
*/

//--------------------------------------------------------------
inline void setGuiPositionToLayout(ofxPanel & gui, int layout = 0) {
	//TODO: Could add other layout positions and use above enum..

	// Move gui panel window to:
	glm::vec2 p;
	int pad = SURFING__PAD_TO_WINDOW_BORDERS; //to borders

	// bottom-center
	if (layout == 0) {
		int x = ofGetWindowWidth() / 2 - gui.getShape().getWidth() / 2;
		int y = ofGetWindowHeight() - gui.getShape().getHeight() - pad;
		p = glm::vec2(x, y);
	}

	// top-center
	else if (layout == 1) {
		int x = ofGetWindowWidth() / 2 - gui.getShape().getWidth() / 2;
		int y = pad;
		p = glm::vec2(x, y);
	}

	else if (layout == 2)
	// bottom-right
	{
		glm::vec2 p1 = glm::vec2(ofGetWindowWidth(), ofGetHeight());
		glm::vec2 p2 = glm::vec2(gui.getShape().getWidth() + pad, gui.getShape().getHeight() + pad);
		p = p1 - p2;
	}

	else if (layout == 3)
	// bottom-left
	{
		p = glm::vec2(pad, ofGetHeight() - gui.getShape().getHeight() - pad);
	}

	gui.setPosition(p.x, p.y);
}

// Set position of gui1 at the window bottom and centered
// (gui2 must be externally linked to gui1 with the correct padding).
//TODO: other layouts
//--------------------------------------------------------------
inline void setGuiPositionToLayoutBoth(ofxPanel & gui1, ofxPanel & gui2, int layout = 1) {
	//TODO: ADD OTHER LAYOUTS
	if (layout == 0) { //bottom-center
		int gw = gui1.getShape().getWidth() + gui2.getShape().getWidth() + SURFING__PAD_OFXGUI_BETWEEN_PANELS;
		int gh = MAX(gui1.getShape().getHeight(), gui2.getShape().getHeight());
		gh += SURFING__PAD_TO_WINDOW_BORDERS;
		int x = ofGetWidth() / 2 - gw / 2;
		int y = ofGetHeight() - gh;
		gui1.setPosition(x, y);
	} else if (layout == 1) { //top center
		int gw = gui1.getShape().getWidth() + gui2.getShape().getWidth() + SURFING__PAD_OFXGUI_BETWEEN_PANELS;
		int x = ofGetWidth() / 2 - gw / 2;
		int y = SURFING__PAD_TO_WINDOW_BORDERS;
		gui1.setPosition(x, y);
	}
}
/*

TODO
		// put bb above gui panel
		// not sure if there is some bug on OF getBoundingBox..
		//auto bbG = gui.getShape();
		//auto bbH = ofxSurfing::getBBBitmapStringBox(sHelp);
		//int x = bbG.getTopLeft().x + bbG.getWidth() / 2;
		//int y = bbG.getTopLeft().y - bbG.getHeight();
		//x -= bbH.getWidth() / 2;
		//y -= 20;
		//glm::vec2 p { x, y };
		//ofxSurfing::ofDrawBitmapStringBox(sHelp, p.x, p.y);
*/

//--------------------------------------------------------------
inline void setOfxGuiTheme(bool bMini = 0, std::string pathFont = "") {

	bool bDebugDefault = 0;

	bool bColors = 1;
	bool bSizes = 1;

	bool bFont = 1;
	int size = bMini ? SURFING__OFXGUI__FONT_DEFAULT_SIZE_MINI : SURFING__OFXGUI__FONT_DEFAULT_SIZE;

	if (pathFont == "") pathFont = SURFING__OFXGUI__FONT_DEFAULT_PATH;

	if (bFont) {
		ofFile file(pathFont);
		bool b = file.exists();
		if (b) {
			ofxGuiSetFont(pathFont, size);
		} else {
			ofLogError(__FUNCTION__) << "Font file " + pathFont + " not found!";
			ofLogError(__FUNCTION__) << "Unable to customize the ofxGui theme font.";
		}
	}

	if (bColors) {
		ofColor cHeadBg;
		ofColor cBg;
		ofColor cBorder;
		ofColor cText;
		ofColor cFill;

#if bDebugDefault
		//Default
		/*
		ofxBaseGui::headerBackgroundColor(80),
		ofxBaseGui::backgroundColor(0),
		ofxBaseGui::borderColor(120, 100),
		ofxBaseGui::textColor(255),
		ofxBaseGui::fillColor(128);
		*/
		cHeadBg = ofColor(80);
		cBg = ofColor(0);
		cBorder = ofColor(120, 100);
		cText = ofColor(255);
		cFill = ofColor(128);
#else
	//TODO
	#if 0
		//Default
		cHeadBg = ofColor(80);
		cBg = ofColor(0);
		cBorder = ofColor(120, 100);
		cText = ofColor(255);
		cFill = ofColor(128);
	#else
		cHeadBg = ofColor(80);
		cBg = ofColor(50, 210);
		cBorder = ofColor(120, 100);
		cText = ofColor(255);
		cFill = ofColor(120, 210);
	#endif
#endif
		ofxGuiSetHeaderColor(cHeadBg);
		ofxGuiSetBackgroundColor(cBg);
		ofxGuiSetBorderColor(cBorder);
		ofxGuiSetFillColor(cFill);
		ofxGuiSetTextColor(cText);
	}

	if (bSizes) {
		int textPadding;
		int defaultWidth;
		int defaultHeight;

#if bDebugDefault
		//Default
		/*
		int ofxBaseGui::textPadding = 4;
		int ofxBaseGui::defaultWidth = 200;
		int ofxBaseGui::defaultHeight = 18;
		*/
		textPadding = 4;
		defaultWidth = 200;
		defaultHeight = 18;
#else
		if (bMini) {
			textPadding = 6;
			defaultWidth = 135;
			defaultHeight = 15;
		} else {
			textPadding = 6;
			defaultWidth = 200;
			defaultHeight = 18;
		}
#endif
		ofxGuiSetTextPadding(textPadding);
		ofxGuiSetDefaultWidth(defaultWidth);
		ofxGuiSetDefaultHeight(defaultHeight);
	}
}
};

//------

/*
* SurfingAutoSaver
* 
	A class to auto save an ofParameterGroup.
	settings are saved one second 
	after any change to avoid overflow o file savings.

	EXAMPLE

	//.h
	SurfingAutoSaver autoSaver;

	//setup()
	callback_t f = std::bind(&ofxSurfingPBR::save, this);
	autoSaver.setFunctionSaver(f);
	//autoSaver.setName("ofApp");//for debug
	internalParams.add(autoSaver.bEnable);

	void ofxSurfingPBR::Changed(ofAbstractParameter & e) {
		// ...	
		autoSaver.saveSoon();
	}

	void load() {
		autoSaver.pause();//stop saving
		// load //
		autoSaver.start();//start
	}
*/

#include <functional>
using callback_t = std::function<void()>;

//--------------------------------------------------------------
class SurfingAutoSaver {
public:
	SurfingAutoSaver();
	~SurfingAutoSaver();

	// auto saver workflow
	// we will auto save after every param change,
	// but after waiting some ms. reducing saving overflow.
	// we will save also when app exit.
	ofParameter<bool> bEnable { "Auto Save", true };

	void saveSoon(bool b = true) {
		if (b) {
			// flag to save delayed
			if (bEnable) {
				//only log once/first call
				if (!bFlagSave) ofLogVerbose("SurfingAutoSaver") << "saveSoon() " << name;

				bFlagSave = true;
				timeLastChange = ofGetElapsedTimeMillis();
			}
		}
	}

	const string getName() {
		return name;
	}

	/// <summary>
	/// not mandatory but for debug
	/// </summary>
	/// <param name="n"></param>
	void setName(string n) {
		name = n;
	}

	void pause() {
		// Pause auto save
		// disables auto save
		// to avoid save after loading the settings,
		// as the params will change and would trig the auto save!
		bAutoSave_ = bEnable; //store state
		if (bEnable) {
			bEnable.setWithoutEventNotifications(false);
		}
	}

	void start() {
		if (bAutoSave_) {
			bEnable.setWithoutEventNotifications(true); //restore state
		}
	}

private:
	uint64_t timeLastChange = 0;
	int timeSaveDelay = 1000; //save delayed x milliseconds.
	bool bFlagSave = false;

	bool bAutoSave_ = false;
	string name = "";

private:
	callback_t f_Saver = nullptr;

public:
	void setFunctionSaver(callback_t f = nullptr) {
		f_Saver = f;
	}

private:
	void update(ofEventArgs & args) {
		update();
	}

public:
	/// <summary>
	///  Warning. Should be private! Expected to be called automaticaly as a listener to ofEvents().update() is registered..
	/// </summary>
	void update() {
		if (bEnable) {
			auto t = ofGetElapsedTimeMillis() - timeLastChange;
			if (bFlagSave && t > timeSaveDelay) {
				bFlagSave = false;

				save();
			}
		}
	}

public:
	void save() {
		if (f_Saver != nullptr) f_Saver();

		ofLogVerbose("SurfingAutoSaver") << "Save() " << name;
	}
};

SurfingAutoSaver::SurfingAutoSaver() {
	ofAddListener(ofEvents().update, this, &SurfingAutoSaver::update);
}

SurfingAutoSaver::~SurfingAutoSaver() {
	ofRemoveListener(ofEvents().update, this, &SurfingAutoSaver::update);
}

//------

namespace ofxSurfing {

//---
/*
* String helpers
*/

// original code copied from: ofxFilikaUtils.h
#define SECS_PER_MIN 60
#define SECS_PER_HOUR 3600
//--------------------------------------------------------------
inline std::string calculateTime(float _time) {

	int seconds;
	int minutes;
	int mins_left;
	int secs_left;

	seconds = (/*gameTimeSeconds - */ int(_time));
	minutes = (/*gameTimeSeconds - */ int(_time)) / SECS_PER_MIN;
	mins_left = minutes % SECS_PER_MIN;
	secs_left = seconds % SECS_PER_MIN;

	std::string mins;
	std::string secs;

	if (mins_left < 10) {
		mins = "0" + ofToString(mins_left);
	} else {
		mins = ofToString(mins_left);
	}

	if (secs_left < 10) {
		secs = "0" + ofToString(secs_left);
	} else {
		secs = ofToString(secs_left);
	}

	//cout << ofGetElapsedTimeMillis() / 1000 << endl;
	//cout << "remaining time : " << mins_left << " : " <<  secs_left << endl;
	//cout << "remaining time : " << mins << " : " <<  secs << endl;

	if (mins_left < 0 || secs_left < 0)
		return "00:00";
	else
		return (mins + ":" + secs);
}

//------

/*
* Push/Pop ArbTex state
*/

//--------------------------------------------------------------
inline bool pushSetArbTex(bool bEnable) {
	bool b = ofGetUsingArbTex(); //return pre state

	if (bEnable) {
		if (!ofGetUsingArbTex())
			ofEnableArbTex();
	} else {
		if (ofGetUsingArbTex())
			ofDisableArbTex();
	}

	return b;
}
//--------------------------------------------------------------
inline void popSetArbTex(bool bPrePush) {

	if (bPrePush != ofGetUsingArbTex()) {
		if (bPrePush)
			ofEnableArbTex();
		else
			ofDisableArbTex();
	}
}

}