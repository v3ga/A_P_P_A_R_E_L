#include "ofApp.h"
#include "ofAppLog.h"
#include "data.h"

#include "tool3D.h"
#include "toolModel.h"
#include "toolMods.h"
#include "toolNetwork.h"
#include "toolUser.h"
#include "toolCalibration.h"

#include "apparelMod_include.h"


//--------------------------------------------------------------
void ofApp::setup()
{
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
	string userId		= settings.getValue("apparel:user", "creativeclaude");
	
	// DATA
	DATA->load();
	OFAPPLOG->println("- loading data");

	// OSC
	if (oscActivate)
	{
		OFAPPLOG->println("- setting OSC sender @ " + oscIP + ":" + ofToString(oscPort));
		oscSender.setup(oscIP, oscPort);
	}

	// USER
	OFAPPLOG->println("- user is @"+userId);
	user.setId(userId);
	user.setModManager(&apparelModManager);
	user.loadConfiguration();
	
	// MODEL
   	OFAPPLOG->println("- loading 3d/"+modelObjName);
	apparelModel.load("3d/"+modelObjName);
	apparelModel.setPosition(0,0,0);

	// MODS
//	apparelModManager.addMod( new apparelMod_debug() );
	apparelModManager.addMod( new apparelMod_authority() );
	apparelModManager.addMod( new apparelMod_girliness() );
	apparelModManager.addMod( new apparelMod_narcissism() );
	apparelModManager.addMod( new apparelMod_vulgaropathy() );
	apparelModManager.addMod( new apparelMod_pretentiousness() );

	apparelModManager.setModel(&apparelModel);

	// CAM
	cam.setDistance(200);
	cam.setTranslationKey(' ');
	//cam.disableMouseInput();

	// TOOLS
	OFAPPLOG->println("- creating tools");
	pTool3D 		= new tool3D(&toolManager, &apparelModel);
	pToolMods 		= new toolMods(&toolManager, &apparelModManager, &apparelModel);
	pToolNetwork 	= new toolNetwork(&toolManager);
	pToolUser		= new toolUser(&toolManager, &user);
	pToolCalibration= new toolCalibration(&toolManager);
	
	toolManager.setLogo("ARicon_150x150.png");
	toolManager.setFontName("fonts/LetterGothic.ttf");

	toolManager.addTool( pTool3D );
	toolManager.addTool( pToolMods );
	toolManager.addTool( pToolNetwork );
	toolManager.addTool( pToolUser );
	toolManager.addTool( pToolCalibration );
	

	toolManager.createControls(ofVec2f(400,100));

	OFAPPLOG->println("- loading tools data");
	toolManager.loadData();

	OFAPPLOG->end();
}

//--------------------------------------------------------------
void ofApp::update()
{
	float dt = (float) ofGetLastFrameTime();

	toolManager.update();
	user.update(dt);
}

//--------------------------------------------------------------
void ofApp::exit()
{
	OFAPPLOG->begin( "ofApp::exit()" );
	user.saveServicesData();
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


	// Wireframe
    ofSetColor(255);
	glEnable(GL_POLYGON_OFFSET_LINE);
    glPolygonOffset(-1,-1);
	apparelModel.drawWireframe();

	// Filled
    ofSetColor(0);
	apparelModel.drawFaces();
	
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
