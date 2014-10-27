//
//  toolCalibration.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 23/07/2014.
//
//

#pragma once
#include "tool.h"
#include "ofxOsc.h"

class ofxAssimpModelLoader;

class toolCalibrationData
{
	public:
		toolCalibrationData		()
		{
			m_modelScale = 0.0f;
			m_modelPositionSensitivity	= 0.1f;
 			m_modelScaleSensitivity		= 0.1f;
		}
 
	   	void					reset(){
									 m_modelPosition.set(0.0f);
									 m_modelScale = 0.0f;
								}
 

 		void					setPositionXY(float x, float y){m_modelPosition.x = x*m_modelPositionSensitivity;m_modelPosition.y = y*m_modelPositionSensitivity;}
 
 		void					setPositionXY(ofVec2f& p){setPositionXY(p.x,p.y);}
		void					setPositionZ(float z){m_modelPosition.z = z;}
		void					setScale(float s){m_modelScale = s*m_modelScaleSensitivity;}

 		void					toOSC(ofxOscMessage& oscMessage){
									oscMessage.addFloatArg(m_modelPosition.x);
									oscMessage.addFloatArg(m_modelPosition.y);
									oscMessage.addFloatArg(m_modelPosition.z);
									oscMessage.addFloatArg(m_modelScale);
								}
	
 		ofVec3f					m_modelPosition;
		float					m_modelScale;

 		float					m_modelPositionSensitivity;
 		float					m_modelScaleSensitivity;
 
 		string					toString()
								{
									string s = "position "+ofToString(m_modelPosition)+"; scale="+ofToString(m_modelScale);
									return s;
								}
};


class toolCalibration : public tool
{
	public:
		toolCalibration			(toolManager* parent);

		void					setup					();
		void					createControlsCustom	();
		void					handleEvents			(ofxUIEventArgs& e);
		void					handleEventPropertyModel();

 		void					mousePressed			(int x, int y, int button);
		void					mouseDragged			(int x, int y, int button);
		void					mouseReleased			(int x, int y, int button);
		bool					keyPressed				(int key);

		ofVec2f					m_mousePositionPressed;
		ofVec2f					m_mousePositionDelta;

		enum{
			EDIT_APPAREL		= 0,
			EDIT_MANIKIN		= 1
		};

		int						m_editModel;
		string					m_editModelPropertyName;


	private:
		ofxUIRadio*				mp_radioApparel;
		ofxUIRadio*				mp_radioManikin;
 
 
	    toolCalibrationData		m_apparelData;
	    toolCalibrationData		m_manikinData;
 
		toolCalibrationData*	mp_modelData;

 		float					m_modelPositionSensitivity;
 		float					m_modelScaleSensitivity;
 
 };


