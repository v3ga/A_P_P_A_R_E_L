//
//  apparelMod_selfopathy.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 03/02/2015.
//
//

#include "apparelMod_selfopathy.h"
#include "ofAppLog.h"

apparelMod_selfopathy::apparelMod_selfopathy() : apparelMod("Selfopathy")
{
	OFAPPLOG->begin("apparelMod_selfopathy::apparelMod_selfopathy()");
	bool ok = m_image.loadImage("images/testTexture.png");
	OFAPPLOG->println("- loaded images/testTexture.png "+ofToString(ok ? "OK":"NO..."));
	OFAPPLOG->end();
}

apparelMod_selfopathy::~apparelMod_selfopathy()
{

}

void apparelMod_selfopathy::drawFaces()
{
//	m_model.mesh.enableTextures();


	int nbVertices = m_model.mesh.getIndices().size();
	for (int i=0;i<nbVertices;i++)
	{
		//m_model.mesh.setTexCoord(m_model.mesh.getIndices()[i], ofVec2f(ofRandom(0,m_image.getWidth()),ofRandom(0,m_image.getHeight())));
	}
	//	m_model.createMeshFaces();
	

	ofPushMatrix();
	ofMultMatrix(m_model.getModelMatrix());
//	m_image.getTextureReference().bind();
	m_model.getMeshRef().drawFaces();
//	m_image.getTextureReference().unbind();
	ofPopMatrix();
	
}
