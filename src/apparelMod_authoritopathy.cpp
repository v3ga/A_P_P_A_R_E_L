//
//  apparelMod_authoritopathy.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 02/06/2014.
//
//

#include "apparelMod_authoritopathy.h"
#include "ofAppLog.h"
//--------------------------------------------------------------
apparelMod_authoritopathy::apparelMod_authoritopathy() : apparelMod("Authoritopathy")
{
}


//--------------------------------------------------------------
void apparelMod_authoritopathy::apply()
{
}

//--------------------------------------------------------------
void apparelMod_authoritopathy::copyModelFrom(const apparelModel& model)
{
	apparelMod::copyModelFrom(model);
	m_meshOriginal = model.mesh;// keep a copy of the mesh to perform displacement
}

//--------------------------------------------------------------
void apparelMod_authoritopathy::onParameterChanged(ofAbstractParameter& parameter)
{
	OFAPPLOG->begin("apparelMod_authoritopathy::onParameterChanged(\""+parameter.getName()+"\"");
	//apparelMod::parameterChanged(parameter);

	int nbVertices = m_indicesVertex.size();
	ofVec3f v,vo;
	
	for (int i=0; i<nbVertices ; i++)
	{
		vo = m_meshOriginal.getVertex( m_indicesVertex[i] );

		v = vo;
		v.y = vo.y + m_weight*50.0f;
		
		m_model.mesh.setVertex(m_indicesVertex[i], v);
	}
	
	m_model.createMeshFaces();
	
	setChanged(true);
	OFAPPLOG->end();
}

