//
//  apparelMod_pedopathy.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 25/11/2014.
//
//

#include "apparelMod_pedopathy.h"


//--------------------------------------------------------------
apparelMod_pedopathy::apparelMod_pedopathy() : apparelMod("Pedopathy")
{

	m_displacement.set("Displacement", 30.0f, 0.0f, 50.0f);
	m_parameters.add(m_displacement);
	
	m_dispAxisX.set("DisplacementX",0.0f,-1.0f,1.0f);
	m_dispAxisY.set("DisplacementY",0.0f,-1.0f,1.0f);
	m_dispAxisZ.set("DisplacementZ",0.0f,-1.0f,1.0f);
	
	m_parameters.add(m_dispAxisX);
	m_parameters.add(m_dispAxisY);
	m_parameters.add(m_dispAxisZ);
}

//--------------------------------------------------------------
void apparelMod_pedopathy::loadModel()
{
	apparelMod::loadModel();
}

//--------------------------------------------------------------
void apparelMod_pedopathy::saveModel()
{
	apparelMod::saveModel();
}

//--------------------------------------------------------------
void apparelMod_pedopathy::onParameterChanged(ofAbstractParameter& parameter)
{
	//OFAPPLOG->begin("apparelMod_authoritopathy::onParameterChanged(\""+parameter.getName()+"\"");
	//apparelMod::parameterChanged(parameter);

	int nbVertices = m_indicesVertex.size();
	ofVec3f v,vo,n;
	
	for (int i=0; i<nbVertices ; i++)
	{
		vo = m_meshInput.getVertex( m_indicesVertex[i] );
		n = m_meshInput.getNormal( m_indicesVertex[i] );
		v = vo;
		v = vo+n*m_weight*m_displacement;
		
		m_model.mesh.setVertex(m_indicesVertex[i], v);
	}
	
	m_model.createMeshFaces();
	
	setChanged(true);
	//OFAPPLOG->end();
}