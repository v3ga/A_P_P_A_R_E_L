#include "ofApp.h"
#include "ofAppLog.h"
#include "data.h"

#include "tool3D.h"
#include "toolModel.h"
#include "toolMods.h"
#include "toolNetwork.h"
#include "toolUser.h"
#include "toolCalibration.h"
#include "toolSound.h"

#include "apparelMod_include.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	OFAPPLOG->begin( "ofApp::setup()" );
	
	GLOBALS->setApp(this);
	GLOBALS->setOscSender(&oscSender);
	GLOBALS->setUser(&user);

	// SETTINGS
	OFAPPLOG->println("  - loading configuration.xml");
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
	int soundDeviceId	= settings.getValue("apparel:soundInput:id", 0);
	int soundChannels	= settings.getValue("apparel:soundInput:channels", 1);
	
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
	user.setTemplate(true);
	user.setModManager(&apparelModManager);
	user.connect();
 
	// MODEL
	apparelModel.load(modelObjName);
	
	// POST PROCESSING
	sceneBuffer.allocate(ofGetWidth(),ofGetHeight());
	sceneFxBlur.allocate(ofGetWidth()/10,ofGetHeight()/10);

	// MODS
	apparelModManager.constructMods(&apparelModel);

	user.loadConfiguration();
	m_imageSelfopathy.loadImage(user.getServicePropertyString("twitter_image"));
	if (GLOBALS->mp_modSelfopathy)
		GLOBALS->mp_modSelfopathy->setImage(&m_imageSelfopathy);


	// USER WORDS
	// this will initialize words count for each mod for this user
	apparelModManager.countUserWords(&user,true);

	// SOUND
	m_soundInput.setup(soundDeviceId, soundChannels);

	// CAM
	cam.setDistance(200);
	cam.setTranslationKey(' ');
	//cam.disableMouseInput();

	// TOOLS
	OFAPPLOG->println("- creating tools");
//	pTool3D 		= new tool3D(&toolManager);
	pToolMods 		= new toolMods(&toolManager, &apparelModManager);
	pToolNetwork 	= new toolNetwork(&toolManager);
	pToolUser		= new toolUser(&toolManager, &user);
	pToolCalibration= new toolCalibration(&toolManager);
	pToolSound		= new toolSound(&toolManager);

	pToolSound->setSoundInput(&m_soundInput);
	
	toolManager.setLogo("ARicon_150x150.png");
	toolManager.setFontName("fonts/LetterGothic.ttf");

	//toolManager.addTool( pTool3D );
	toolManager.addTool( pToolMods );
	toolManager.addTool( pToolNetwork );
	toolManager.addTool( pToolUser );
	toolManager.addTool( pToolCalibration );


	toolManager.addTool( pToolSound );
	
   
	toolManager.createControls(ofVec2f(400,100));

	OFAPPLOG->println("- loading tools data");
	toolManager.loadData();


//	apparelModManager.applyModChain();

	OFAPPLOG->end();


	m_bSaveframe = false;
}



//--------------------------------------------------------------
void ofApp::update()
{
	float dt = (float) ofGetLastFrameTime();

	toolManager.update();
	user.update(dt);
	
//	ofLog() << ">>>>" << apparelModel.getMeshRef().getNumVertices();
}

//--------------------------------------------------------------
void ofApp::exit()
{
	OFAPPLOG->begin( "ofApp::exit()" );
	user.saveServicesData();
	toolManager.saveData();
	toolManager.exit();
	OFAPPLOG->end();
	m_soundInput.stop();
}

//--------------------------------------------------------------
void ofApp::draw()
{
		// render to offscreen
		if (this->pToolMods->isPostProcessEnabled())
		{
			sceneBuffer.begin();
 			ofClear(0,0,0,0);
		}
		else
		{
			ofBackgroundGradient(ofColor(64,255), ofColor(0,255));
		}

  		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		ofEnableDepthTest();
	
		cam.begin();
		toolManager.draw();
		cam.end();

		ofDisableDepthTest();
    	ofDisableBlendMode();
		
		if (this->pToolMods->isPostProcessEnabled())
		{
			sceneBuffer.end();

			// Filters
			sceneFxBlur.setTexture(sceneBuffer.getTextureReference());
			sceneFxBlur.update();

		    //ofEnableBlendMode(OF_BLENDMODE_ALPHA);
			ofDisableDepthTest();
			ofBackgroundGradient(ofColor(64,255), ofColor(0,255));
			ofSetColor(255);
 
			ofEnableBlendMode(OF_BLENDMODE_ALPHA);
			sceneFxBlur.draw(0,0);
			ofDisableBlendMode();
		}

	// Screenshot
	if (m_bSaveframe)
	{
		m_bSaveframe = false;
		ofSaveScreen("export.png");
	}

	// Interface
	toolManager.drawUI();

//	 sceneFxBlur.fboNoise.draw(0,ofGetHeight()-20,sceneFxBlur.fboNoise.getWidth(),20);
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels)
{
	m_soundInput.audioIn(input,bufferSize,nChannels);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	bool isToolHandleKey = toolManager.keyPressed(key);
	if (isToolHandleKey == false)
	{
		if (key == 's')
		{
			m_bSaveframe = true;
		}
		if (key == OF_KEY_UP)
			cam.setDistance( cam.getDistance() + 10.0);
		else if (key == OF_KEY_DOWN)
			cam.setDistance( cam.getDistance() - 10.0);
	}
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
	if (!toolManager.isHit(x, y)){
		toolManager.mouseDragged(x, y, button);
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (toolManager.isHit(x, y)){
		cam.disableMouseInput();
	}
	else
		toolManager.mousePressed(x, y, button);

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
