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

#define USER_LOAD_CONFIGURATION true

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

	if (USER_LOAD_CONFIGURATION)
		user.loadConfiguration();
	
	// MODEL
	apparelModel.load(modelObjName);
//	apparelModel.setPosition(0,0,0);
	
	sceneBuffer.allocate(ofGetWidth(),ofGetHeight());
	sceneFxBlur.allocate(ofGetWidth()/10,ofGetHeight()/10);
	

	// MODS
	apparelModManager.addMod( new apparelMod_authoritopathy() );
	apparelModManager.addMod( new apparelMod_pedopathy() );
	apparelModManager.addMod( new apparelMod_sportopathy() );
	apparelModManager.addMod( new apparelMod_pretentiopathy() );
	apparelModManager.addMod( new apparelMod_meteopathy() );
	apparelModManager.addMod( new apparelMood_porcupinopathy() );
	apparelModManager.addMod( new apparelMod_selfopathy() );
	apparelModManager.addMod( new apparelMood_noisopathy() );
	

	// **Copy** the model to every mod in the chain
	apparelModManager.copyModelToMods(apparelModel);
	apparelModManager.loadModData();

	// USER WORDS
	// this will initialize words count for each mod for this user
	apparelModManager.countUserWords(&user);

	// SOUND
	soundStreamInput.listDevices();
//	int deviceSoundInputId = m_settings.getValue("simulator:soundInput:device", -1);
	int deviceSoundInputId = 0;
	int nbChannels = 2;
	if (deviceSoundInputId>=0)
	{
		soundStreamInput.setDeviceID(deviceSoundInputId);
		soundStreamInput.setup(ofGetAppPtr(), 0, nbChannels, 44100, 256, 4);
		soundStreamInput.start();

		setupAudioBuffers(nbChannels);
	}


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
	
	toolManager.setLogo("ARicon_150x150.png");
	toolManager.setFontName("fonts/LetterGothic.ttf");

	//toolManager.addTool( pTool3D );
	toolManager.addTool( pToolMods );
	toolManager.addTool( pToolNetwork );
	toolManager.addTool( pToolUser );
	toolManager.addTool( pToolCalibration );
	
	
//	pTool3D->setSceneFbo(&sceneBuffer);

	toolManager.createControls(ofVec2f(400,100));

	OFAPPLOG->println("- loading tools data");
	toolManager.loadData();
	
	
	apparelModManager.applyModChain();

	OFAPPLOG->end();


	m_bSaveframe = false;
}


//--------------------------------------------------------------
void ofApp::setupAudioBuffers(int nbChannels)
{
	m_soundBufferSize = 256;
    if (nbChannels == 1)
    {
        m_soundMono.assign(m_soundBufferSize, 0.0f);
    }
    else
    if (nbChannels == 2)
    {
        m_soundLeft.assign(m_soundBufferSize, 0.0f);
        m_soundRight.assign(m_soundBufferSize, 0.0f);
    }
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
	soundStreamInput.stop();
}

//--------------------------------------------------------------
void ofApp::draw()
{

	// render to offscreen
if (this->pToolMods->isPostProcessEnabled())
{
	sceneBuffer.begin();
 	ofClear(0,0,0,0);
}else
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
    //sceneBuffer.draw(0,0);
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
	float curVol = 0.0;
    int numCounted = 0;

    if (nChannels==1)
    {
		for (int i = 0; i < bufferSize; i++){
			m_soundMono[i] = input[i];
			curVol += m_soundMono[i]*m_soundMono[i];
			numCounted++;
		}

        //this is how we get the mean of rms :)
        curVol /= (float)numCounted;
        
        // this is how we get the root of rms :)
        curVol = sqrt( curVol );

		GLOBALS->setSoundInputVolume( curVol );

    }
    else
    if (nChannels==2)
    {
        // samples are "interleaved"
        
        for (int i = 0; i < bufferSize; i++)
        {
            m_soundLeft[i]	= input[i*2]*0.5;
            m_soundRight[i]	= input[i*2+1]*0.5;
            
            curVol += m_soundLeft[i] * m_soundLeft[i];
            curVol += m_soundRight[i] * m_soundRight[i];
            numCounted+=2;
        }
        
        //this is how we get the mean of rms :)
        curVol /= (float)numCounted;
        
        // this is how we get the root of rms :)
        curVol = sqrt( curVol );
        
		GLOBALS->setSoundInputVolume( curVol );
	 
        //bufferCounter++;
    }
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
