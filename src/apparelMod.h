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

class apparelModel;
class apparelMod
{
	public:
		apparelMod			(string id);
		virtual ~apparelMod	(){}
	
		virtual	void		loadParameters	();
		virtual	void		saveParameters	();
	
		ofParameterGroup&	getParamatersGroupRef(){return m_parameters;}
		string				getPathMod(string filename){return "mods/"+m_id+"/"+filename;}
	
		void				addFaceIndex	(int faceIndex);
		void				removeFaceIndex	(int faceIndex);

		string				m_id;
		apparelModel*		mp_model;
	   	vector<int>			m_indicesFaces;
		vector<int>			m_indicesVertex;
		ofParameterGroup 	m_parameters;

		ofxXmlSettings		m_settingsModel;
};
