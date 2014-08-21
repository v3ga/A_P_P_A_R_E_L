//
//  apparelMod.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 27/05/2014.
//
//

#pragma once
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "oscSender.h"

class apparelModel;
class apparelMod
{
	public:
		apparelMod			(string id);
		virtual ~apparelMod	(){}
	
		virtual	void		loadParameters		();
		virtual	void		loadParametersCustom(){};
		virtual	void		saveParameters		();
		void				setConfiguration	(string name);
	
		ofParameterGroup&	getParamatersGroupRef	(){return m_parameters;}
		virtual void		parameterChanged		(ofAbstractParameter & parameter);
		string				getPathMod				(string filename="");
		string				getPathToolMods			(string filename){return "tools/mods/"+filename;}

		void				addFaceIndex			(int faceIndex);
		void				removeFaceIndex			(int faceIndex);
	
		void				addVertexIndex			(int vertexIndex);
		void				removeVertexIndex		(int vertexIndex);

		void				setOscSender			(oscSenderInterface* p){mp_oscSender=p;}

		// INTERFACE FOR RECEIVING DATA FROM USER
		void				onNewText				(string text){}
		void				onNewWords				(vector<string>& text){}

		// INTERFACE FOR SAVING IN DATABASE
	
		// ID
		string				m_id;

		// MODEL w/ DATA
		apparelModel*		mp_model;
	   	vector<int>			m_indicesFaces;
		vector<int>			m_indicesVertex;

		// PARAMETERS CONFIGURATION
		string				m_configurationName;
	
		// PARAMETERS
		ofParameterGroup 	m_parameters;
		ofParameter<bool>	m_isActive;
		ofParameter<float>	m_weight;

		// SETTINGS
		ofxXmlSettings		m_settingsModel;

		// OSC
		oscSenderInterface*	mp_oscSender;
};
