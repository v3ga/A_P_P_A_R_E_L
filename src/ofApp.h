#pragma once

#include "ofMain.h"
#include "globals.h"
#include "apparelModel.h"

#include "tool.h"

class tool3D;
class toolMods;

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
	
	
		ofxXmlSettings	settings;
		ofEasyCam 		cam;
		apparelModel	apparelModel;


	private:
		toolManager		toolManager;
		tool3D*			pTool3D;
		toolMods*		pToolMods;
};
