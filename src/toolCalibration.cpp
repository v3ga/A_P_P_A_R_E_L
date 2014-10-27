//
//  toolCalibration.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 23/07/2014.
//
//

#include "toolCalibration.h"
#include "globals.h"
#include "oscDefs.h"
#include "oscSender.h"
#include "ofApp.h"

//--------------------------------------------------------------
toolCalibration::toolCalibration(toolManager* parent) : tool("Calibration", parent)
{
	mp_radioApparel = 0;
	mp_radioManikin = 0;
	
	m_editModel = EDIT_APPAREL;
	mp_modelData = 0;

	m_modelPositionSensitivity 	= 0.2f;
	m_modelScaleSensitivity 	= 0.2f;
}

//--------------------------------------------------------------
void toolCalibration::setup()
{
}

//--------------------------------------------------------------
void toolCalibration::createControlsCustom()
{
	if (mp_canvas==0) return;

	mp_canvas->addLabel("Calibration");
	mp_canvas->addSpacer(300,1);


	vector<string> radioModelNames;
	radioModelNames.push_back("Scale");
	radioModelNames.push_back("Position XY");
	radioModelNames.push_back("Position Z");

	mp_canvas->addLabel("APPAREL");
	mp_radioApparel = mp_canvas->addRadio("apparelProperties", radioModelNames, OFX_UI_ORIENTATION_HORIZONTAL);

    mp_canvas->addSpacer(300,1);

	mp_canvas->addLabel("Manikin");
	mp_radioManikin = mp_canvas->addRadio("manikinProperties", radioModelNames, OFX_UI_ORIENTATION_HORIZONTAL);


	mp_canvas->addLabel("Precision");
    mp_canvas->addSpacer(300,1);

	mp_canvas->addWidgetDown( new ofxUISlider("positionPrecision", 		0.1f, 0.5f, &m_modelPositionSensitivity, 300,16 ));
	mp_canvas->addWidgetDown( new ofxUISlider("scalePrecision", 		0.1f, 0.5f, &m_modelScaleSensitivity, 300,16));

	mp_canvas->addWidgetDown( new ofxUILabelButton("savePropertiesDevice","save on device", 140, 16,0,0,OFX_UI_FONT_SMALL));
	mp_canvas->addWidgetRight( new ofxUILabelButton("resetPropertiesDevice","reset", 140, 16,0,0,OFX_UI_FONT_SMALL));


	mp_canvas->autoSizeToFitWidgets();
}

//--------------------------------------------------------------
void toolCalibration::handleEvents(ofxUIEventArgs& e)
{
	string name = e.getName();
	OFAPPLOG->begin("toolCalibration::handleEvents('"+name+"')");
	OFAPPLOG->println("- kind="+ofToString(e.getKind()));
	
	if (e.getKind() == OFX_UI_WIDGET_TOGGLE)
	{

	 ofxUIToggle* pToggle = e.getToggle();
	 if (pToggle->getValue())
	 {

		 mp_modelData = 0;
		 m_editModelPropertyName = name;
		 if (pToggle->getParent() == mp_radioApparel)
		 {
			 m_editModel = EDIT_APPAREL;
			 mp_modelData = &m_apparelData;
		 }
		 else
		 if (pToggle->getParent() == mp_radioManikin)
		 {
			 m_editModel = EDIT_MANIKIN;
			 mp_modelData = &m_manikinData;
		 }

		 if (mp_modelData)
		 {
			 mp_modelData->reset();
			 mp_modelData->m_modelPositionSensitivity = m_modelPositionSensitivity;
			 mp_modelData->m_modelScaleSensitivity = m_modelScaleSensitivity;
		 }

	 }

	}
	else if (e.getKind() == OFX_UI_WIDGET_LABELBUTTON)
	{
		if (name == "savePropertiesDevice" && e.getButton()->getValue()==1)
		{
			GLOBALS->getApp()->apparelModel.saveProperties();

			OSC_SENDER->saveCalibration();
		}
	}

	OFAPPLOG->end();
}

//--------------------------------------------------------------
void toolCalibration::handleEventPropertyModel()
{
	if (mp_modelData == 0) return;
	
	if (m_editModelPropertyName == "Position XY")
	{
		mp_modelData->setPositionXY(m_mousePositionDelta.x, -m_mousePositionDelta.y); // because of Y axis on 3D model
	}
	else if (m_editModelPropertyName == "Position Z")
	{
		mp_modelData->setPositionZ(m_mousePositionDelta.y);
	}
	else if (m_editModelPropertyName == "Scale")
	{
		mp_modelData->setScale(m_mousePositionDelta.x);
	}
	
	ofxOscMessage m;
	m.setAddress(OSC_MODEL_SET_CALIBRATION);
	mp_modelData->toOSC(m);
	OSC_SENDER->send(m);
	
	// OFAPPLOG->println( mp_modelData->toString() );
}

//--------------------------------------------------------------
void toolCalibration::mousePressed(int x, int y, int button)
{
	if (mp_modelData == 0) return;

	m_mousePositionPressed.set((float)x, (float)y);
	m_mousePositionDelta.set(0.0f,0.0f);
	OSC_SENDER->sendBeginCalibration();
}

//--------------------------------------------------------------
void toolCalibration::mouseDragged(int x, int y, int button)
{
	if (mp_modelData == 0) return;

	m_mousePositionDelta = ofVec2f((float)x, (float)y) - m_mousePositionPressed;
	handleEventPropertyModel();
}

//--------------------------------------------------------------
void toolCalibration::mouseReleased(int x, int y, int button)
{
	OSC_SENDER->sendEndCalibration();
}

//--------------------------------------------------------------
bool toolCalibration::keyPressed(int key)
{
	if (mp_modelData == 0) return;

	OFAPPLOG->begin("toolCalibration::keyPressed");
	OFAPPLOG->println("m_editModelPropertyName="+m_editModelPropertyName);

	float positionIncrement = 10.0f;
	float scaleIncrement = 10.0f;

	bool isKeyHandled = false;
	if (m_editModelPropertyName == "Position XY")
	{
		if (key == OF_KEY_RIGHT)	{mp_modelData->setPositionXY(positionIncrement,0);isKeyHandled=true;}
		if (key == OF_KEY_LEFT)		{mp_modelData->setPositionXY(-positionIncrement,0);isKeyHandled=true;}
		if (key == OF_KEY_UP)		{mp_modelData->setPositionXY(0,positionIncrement);isKeyHandled=true;}
		if (key == OF_KEY_DOWN)		{mp_modelData->setPositionXY(0,-positionIncrement);isKeyHandled=true;}
	}
	else if (m_editModelPropertyName == "Position Z")
	{
		if (key == OF_KEY_UP)		{mp_modelData->setPositionZ(-positionIncrement);isKeyHandled=true;}
		if (key == OF_KEY_DOWN)		{mp_modelData->setPositionZ(positionIncrement);isKeyHandled=true;}
	}
	else if (m_editModelPropertyName == "Scale")
	{
		if (key == OF_KEY_LEFT)		{mp_modelData->setScale(-scaleIncrement);isKeyHandled=true;}
		if (key == OF_KEY_RIGHT)	{mp_modelData->setScale(scaleIncrement);isKeyHandled=true;}
	}

	if (isKeyHandled)
	{
		OFAPPLOG->println("properties="+mp_modelData->toString());
	
		OSC_SENDER->sendBeginCalibration();

			ofxOscMessage m;
			m.setAddress(OSC_MODEL_SET_CALIBRATION);
			mp_modelData->toOSC(m);
			OSC_SENDER->send(m);
		
		OSC_SENDER->sendEndCalibration();
	
		OFAPPLOG->end();
		return true;
	}

	OFAPPLOG->end();

	return false;
}


