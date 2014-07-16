#include "ofApp.h"
#include "ofAppLog.h"
#include "data.h"

#include "tool3D.h"
#include "toolModel.h"
#include "toolMods.h"

#include "apparelMod_authority.h"
#include "apparelMod_debug.h"


//--------------------------------------------------------------
void ofApp::setup(){
	// ofLog() << "ofApp::setup()";
	OFAPPLOG->begin( "ofApp::setup()" );
	
	GLOBALS->setApp(this);
	GLOBALS->setOscSender(&oscSender);

	// SETTINGS
	ofLog() << "  - loading configuration.xml";
    if ( settings.loadFile("configuration.xml") == false)
	{
		OFAPPLOG->println("- unable to load configuration.xml");
		exit();
	}

	string modelObjName = settings.getValue("apparel:model", "");
	bool oscActivate 	= settings.getAttribute("apparel:osc", "activated", 1) > 0 ? true : false;
	string oscIP 		= settings.getValue("apparel:osc:ip","");
	int oscPort 		= settings.getValue("apparel:osc:port",1235);

	
	// DATA
	// ofLog() << "  - loading data";
	
	DATA->load();
	OFAPPLOG->println("- loading data");
	// OSC
	if (oscActivate)
	{
		OFAPPLOG->println("- setting OSC sender @ " + oscIP + ":" + ofToString(oscPort));
		oscSender.setup(oscIP, oscPort);
	}
	
	// MODEL
   	OFAPPLOG->println("- loading 3d/"+modelObjName);
	apparelModel.load("3d/"+modelObjName);
	apparelModel.setPosition(0,0,0);
	apparelModel.setScale(1/1.83328,1/1.83328,1/1.83328); // TODO : why 1.83328 ?

	// CAM
	cam.setDistance(200);
	cam.setTranslationKey(' ');
	//cam.disableMouseInput();

	// TOOLS
	OFAPPLOG->println("- creating tools");
	pTool3D = new tool3D(&toolManager, &apparelModel);
	pToolMods = new toolMods(&toolManager, &apparelModel);
	pToolMods->addMod( new apparelMod_debug() );
	pToolMods->addMod( new apparelMod_authority() );
	
	toolManager.setLogo("ARicon_150x150.png");
	toolManager.setFontName("fonts/LetterGothic.ttf");

	toolManager.addTool( pTool3D );
	toolManager.addTool( pToolMods );
	toolManager.createControls(ofVec2f(150,0), ofVec2f(400,100));

	OFAPPLOG->println("- loading tools data");
	toolManager.loadData();

	OFAPPLOG->end();
}

//--------------------------------------------------------------
void ofApp::update()
{
	toolManager.update();
}

//--------------------------------------------------------------
void ofApp::exit()
{
	OFAPPLOG->begin( "ofApp::exit()" );
	toolManager.saveData();
	toolManager.exit();
	OFAPPLOG->end();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackgroundGradient(ofColor(64), ofColor(0));

    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofEnableDepthTest();


	cam.begin();

    ofSetColor(0);
	ofPushMatrix();
	ofScale(0.999, 0.999,0.999);
	apparelModel.drawFaces();
	ofPopMatrix();

    ofSetColor(255);
	apparelModel.drawWireframe();

	toolManager.draw();
	
	cam.end();
	ofDisableDepthTest();


	toolManager.drawUI();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == OF_KEY_UP)
		cam.setDistance( cam.getDistance() + 10.0);
	else if (key == OF_KEY_DOWN)
		cam.setDistance( cam.getDistance() - 10.0);

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	toolManager.mousePressed(x, y, button);
	if (toolManager.isHit(x, y)){
		cam.disableMouseInput();
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	cam.enableMouseInput();
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
