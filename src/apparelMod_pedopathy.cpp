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
	m_pointInfluence.set(0.0f,0.0f,-20.0);

	m_displacement.set("Displacement", 30.0f, 0.0f, 50.0f);
	m_parameters.add(m_displacement);
}

//--------------------------------------------------------------
void apparelMod_pedopathy::onParameterChanged(ofAbstractParameter& parameter)
{
	//OFAPPLOG->begin("apparelMod_authoritopathy::onParameterChanged(\""+parameter.getName()+"\"");
	//apparelMod::parameterChanged(parameter);

	int nbVertices = m_indicesVertex.size();
	ofVec3f v,vo;
	
	for (int i=0; i<nbVertices ; i++)
	{
		vo = m_meshInput.getVertex( m_indicesVertex[i] );

		v = vo;
		m_pointInfluence.set(0,vo.y,vo.z);
		v = vo+(vo-m_pointInfluence).normalized()*m_weight*m_displacement;
//		v.x = vo.x+m_weight*m_displacement;
		
		m_model.mesh.setVertex(m_indicesVertex[i], v);
	}
	
	m_model.createMeshFaces();
	
	setChanged(true);
	//OFAPPLOG->end();
}