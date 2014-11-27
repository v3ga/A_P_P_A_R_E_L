//
//  apparelMod.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 27/05/2014.
//
//

#pragma once
#include "ofMain.h"
#include "apparelModel.h"
#include "ofxXmlSettings.h"
#include "oscSenderInterface.h"

class apparelMod
{
	public:
		apparelMod			(string id);
		virtual ~apparelMod	(){}
	
	
				void		createDirMod		();

		virtual	void		loadParameters		();
		virtual	void		loadParametersCustom(){};
		virtual	void		saveParameters		();

		virtual	void		readModel			();
		virtual	void		loadModel			();
		virtual	void		loadModel			(string& xml);
		virtual	void		saveModel			();
		virtual	string		getModelAsXMLString	();
	
		void				setConfiguration	(string name);

		string				getPathDocument		(string filename="");
		string				getPathResources	(string filename="");

		void				createWordsList		();

	
		ofParameterGroup&	getParamatersGroupRef	(){return m_parameters;}
		ofAbstractParameter&getParameter			(string name);
		virtual void		parameterChanged		(ofAbstractParameter & parameter);
		virtual	void		onParameterChanged		(ofAbstractParameter & parameter){}

		string				getPathRelative			(string filename="");
		string				getPathToolMods			(string filename){return "tools/mods/"+filename;}

		void				addFaceIndex			(int faceIndex);
		void				removeFaceIndex			(int faceIndex);
	
		void				addVertexIndex			(int vertexIndex);
		void				removeVertexIndex		(int vertexIndex);


		// STATE
		void				setChanged				(bool is=true){m_isChanged = is;}
		bool				isChanged				(){return m_isChanged;}
 		bool				m_isChanged;			// tells if mesh itself or selection (vertices, faces) changed

		// DRAWING
		virtual void		draw					(){};

		// OSC NETWORK
		void				setOscSender			(oscSenderInterface* p){mp_oscSender=p;}

		// INTERFACE FOR RECEIVING DATA FROM USER
		void				onNewText				(string text){}
		void				onNewWords				(vector<string>& text){}

		// INTERFACE FOR SAVING IN DATABASE
	
		// ID
		string				m_id;
		string				getId					(){return m_id;}

		// MODEL w/ DATA
		apparelModel*		mp_model;			// original model
		apparelModel		m_model;
	   	vector<int>			m_indicesFaces;		// selected faces on original model
		vector<int>			m_indicesVertex;	// selected vertices on original model

		virtual void		copyModelFrom		(const apparelModel& model);
		virtual void		clearSelection		();
		virtual void		apply				(){};

		// PARAMETERS CONFIGURATION
		string				m_configurationName;
	
		// PARAMETERS
		ofParameterGroup 	m_parameters;
		ofParameter<bool>	m_isActive;
		ofParameter<float>	m_weight;


		// SETTINGS
		ofxXmlSettings		m_settingsModel;
	
		// WORDS
		vector<string>		m_words;
		//ofBuffer buffer = ofBufferFromFile("someFile.txt");


		// OSC
		oscSenderInterface*	mp_oscSender;
};
