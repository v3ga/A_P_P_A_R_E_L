//
//  apparelMod_kawaiopathy.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 25/05/2015.
//
//

#include "apparelMod_kawaiopathy.h"
#include "ofAppLog.h"



//--------------------------------------------------------------
kawai::kawai(apparelMod_kawaiopathy* p)
{
	mp_parent = p;
}

//--------------------------------------------------------------
bool kawai::readXml(ofxXmlSettings& settings, int which)
{
	OFAPPLOG->begin("kawai::readXml()");
	settings.pushTag("kawai", which);

	m_meshName = settings.getValue("mesh", "");
	if (m_meshName==""){
		OFAPPLOG->end();
		settings.popTag();
		return false;
	}

	OFAPPLOG->println("- meshName="+m_meshName);
	
	
	m_scale = settings.getValue("scale", 0.75f, which);
	OFAPPLOG->println("- scale="+ofToString(m_scale));
	
	m_pos.x = settings.getAttribute("position", "x", 0.0f);
	m_pos.y = settings.getAttribute("position", "y", 0.0f);
	m_pos.z = settings.getAttribute("position", "z", 0.0f);

	OFAPPLOG->println("- pos="+ofToString(m_pos));

	float angle = settings.getAttribute("rotation", "angle", 0.0f);
	float xaxis = settings.getAttribute("rotation", "x", 1.0f);
	float yaxis = settings.getAttribute("rotation", "y", 0.0f);
	float zaxis = settings.getAttribute("rotation", "z", 0.0f);

	m_rot.makeRotate(angle, xaxis,yaxis,zaxis);

	OFAPPLOG->println("- rot="+ofToString(m_rot));
	
	m_weight = settings.getValue("weight", 0.0f);
	if (m_weight == 0.0f){
		OFAPPLOG->end();
		settings.popTag();
		return false;
	}

	OFAPPLOG->println("- weight="+ofToString(m_weight));

	OFAPPLOG->end();
		settings.popTag();

	return true;
}

//--------------------------------------------------------------
void kawai::loadData()
{
	OFAPPLOG->begin("kawai::loadData()");
	
	
	if (mp_parent)
	{
		string pathModel = mp_parent->getPathResources(m_meshName);
		if ( m_model.loadModel( pathModel ) )
		{
			OFAPPLOG->println("- OK loaded '"+pathModel+"'");

			if (m_model.getMeshCount()>0)
			{
				m_mesh = m_model.getMesh(0);

				ofMatrix4x4 transformation;
				transformation.glRotate(m_rot);
				transformation.glScale( m_scale,m_scale,m_scale );
				int nbVertices = m_mesh.getVertices().size();
				for (int i=0;i<nbVertices;i++)
				{
				 ofVec3f v = m_mesh.getVertex(i);
				 m_mesh.setVertex(i, transformation*v+m_pos);
				}
			}
		}
		else
		{
			OFAPPLOG->println("- ERROR loading '"+pathModel+"'");
		}
	}
	OFAPPLOG->end();
}


//--------------------------------------------------------------
apparelMod_kawaiopathy::apparelMod_kawaiopathy() : apparelMod("Kawaiopathy")
{
	OFAPPLOG->begin("apparelMod_kawaiopathy::apparelMod_kawaiopathy()");
	OFAPPLOG->end();
}

//--------------------------------------------------------------
apparelMod_kawaiopathy::~apparelMod_kawaiopathy()
{

}

//--------------------------------------------------------------
void apparelMod_kawaiopathy::readModel()
{
	OFAPPLOG->begin("apparelMod_kawaiopathy::readModel()");
	apparelMod::readModel();
	
	// Load xml
	ofxXmlSettings settings;
	if (settings.load(getPathResources("distribution.xml")))
	{
		OFAPPLOG->println(" - OK loaded 'distribution.xml'");
		
		int nbKawais = settings.getNumTags("kawai");
		OFAPPLOG->println(" - found "+ofToString(nbKawais)+" kawai(s)");
		for (int i=0; i<nbKawais; i++)
		{
			string kwType = settings.getAttribute("kawai", "type", "???", i);
			if (kwType == "mesh")
			{
				kawai* pKawai = new kawai(this);
				if (pKawai->readXml(settings, i))
				{
					pKawai->loadData();
					m_kawais.push_back(pKawai);
				}
				else
				{
					delete pKawai;
				}
			}
		}
	}
	else
	{
		OFAPPLOG->println(" - ERROR loading 'distribution.xml'");
	}
	
/*
	models[0].loadModel( getPathResources("bird.3ds") );
	models[1].loadModel( getPathResources("cat.3ds") );
	models[2].loadModel( getPathResources("dog.3ds") );
	models[3].loadModel( getPathResources("ears.3ds") );
*/

	OFAPPLOG->end();
}
/*
void apparelMod_kawaiopathy::drawFaces()
{
}
*/

//--------------------------------------------------------------
void apparelMod_kawaiopathy::apply()
{
	if (isChanged())
	{
		m_model.mesh = m_meshInput;
	
		int nbKwais = m_kawais.size();
		for (int i=0;i<nbKwais;i++)
		{
			kawai* pKawai = m_kawais[i];
			if (m_weight>=pKawai->m_weight && pKawai->m_model.getMeshCount()>0)
			{
				m_model.getMeshRef().append( pKawai->m_mesh );
			}
		
		}
	
	}
}

//--------------------------------------------------------------
void apparelMod_kawaiopathy::drawExtra()
{
/*
	if (models[0].getMeshCount()>0)
	{
		ofPushMatrix();
//		ofTranslate(-20,20);
//		ofRotateZ(180);
		ofScale(0.4f,0.4f,0.4f);
		models[0].getMesh(0).drawWireframe();
		ofPopMatrix();
	}
*/

}
