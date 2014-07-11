#include "ofApp.h"
#include "data.h"

#include "tool3D.h"
#include "toolModel.h"
#include "toolMods.h"

#include "apparelMod_authority.h"
#include "apparelMod_debug.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofLog() << "ofApp::setup()";
	
	GLOBALS->setApp(this);
	GLOBALS->setOscSender(&oscSender);
	
	// OSC
	oscSender.setup("192.168.1.81", 12000);

	// DATA
	ofLog() << "  - loading data";
	DATA->load();

	// SETTINGS
	ofLog() << "  - loading configuration.xml";
    settings.loadFile("configuration.xml");
	string modelObjName = settings.getValue("apparel:model", "");
	
	
	// MODEL
	ofLog() << "  - loading " << "3d/" << modelObjName;
	apparelModel.load("3d/"+modelObjName);
	apparelModel.setPosition(0,0,0);
	apparelModel.setScale(1/1.83328,1/1.83328,1/1.83328); // TODO : why 1.83328 ?

	// CAM
	cam.setDistance(200);
	cam.setTranslationKey(' ');
	//cam.disableMouseInput();

	// TOOLS
	ofLog() << "  - creating tools";
	pTool3D = new tool3D(&toolManager, &apparelModel);
	pToolMods = new toolMods(&toolManager, &apparelModel);
	pToolMods->addMod( new apparelMod_debug() );
//	pToolMods->addMod( new apparelMod_authority() );
	
	toolManager.setLogo("ARicon_150x150.png");
	toolManager.addTool( pTool3D );
	toolManager.addTool( pToolMods );
	toolManager.createControls(ofVec2f(150,0), ofVec2f(400,100));

	ofLog() << "  - loading tools data";
	toolManager.loadData();
	ofLog() << "  - setup end";
}

//--------------------------------------------------------------
void ofApp::update()
{
	toolManager.update();
}

//--------------------------------------------------------------
void ofApp::exit()
{
	toolManager.saveData();
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

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
