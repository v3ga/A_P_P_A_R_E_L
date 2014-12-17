#pragma once

#include "ofMain.h"
#include "globals.h"
#include "apparelModel.h"
#include "apparelModManager.h"
#include "tool.h"
#include "oscSender.h"
#include "user.h"
#include "ofxBlur.h"
#include "ofxChromaGlitch.h"
#include "ofxGaussianBlur.h"
#include "ofxGlow.h"
#include "ofxPostProcess.h"

class tool3D;
class toolMods;
class toolNetwork;
class toolUser;
class toolCalibration;

class ofApp : public ofBaseApp
{

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
	
		// SETTINGS
		ofxXmlSettings		settings;

		// SCENE
		ofEasyCam 			cam;
		apparelModel		apparelModel;
		ofFbo				sceneBuffer;

		// POST EFFECT
		ofxPostProcess		   sceneFxBlur;

		// USER
		user				user;

		// MODS
		apparelModManager	apparelModManager;

		// NETWORK
		oscSender			oscSender;

		// EXPORT TO IMAGE
		bool 				m_bSaveframe;

	private:
		toolManager			toolManager;
		tool3D*				pTool3D;
		toolMods*			pToolMods;
		toolNetwork*		pToolNetwork;
		toolUser*			pToolUser;
		toolCalibration*	pToolCalibration;
};
