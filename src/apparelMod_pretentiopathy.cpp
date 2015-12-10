//
//  apparelMod_pretentiopathy.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 05/12/2014.
//
//

#include "apparelMod_pretentiopathy.h"
#include "parameterGroupLowHigh.h"

//--------------------------------------------------------------
apparelMod_pretentiopathy::apparelMod_pretentiopathy() : apparelMod("Pretentiopathy")
{
	m_headScale.set("scale", 0.0f, 0.0f, 60.0f);
	//m_headPositionZ.set("position", 0.0f, 0.0f, 50.0f);

	parameterGroupLowHigh::create(m_headPositionZ, "position", 0.0f, 0.0f, 60.0f, 100.0f);
	m_parameters.add(m_headScale);
	m_parameters.add(m_headPositionZ);

}

//--------------------------------------------------------------
void apparelMod_pretentiopathy::apply()
{
	m_model.mesh = m_meshInput;
	m_head = ofMesh::sphere(m_weight*m_headScale,5,OF_PRIMITIVE_TRIANGLES);
	m_head.enableNormals();

	int nbVerticesHead = m_head.getVertices().size();
	for (int i=0; i<nbVerticesHead ; i++)
	{
		ofVec3f& vHead = m_head.getVertices()[i];
		vHead.y += 10;
		vHead.z += ofMap(m_weight, 0.0f, 1.0f, m_headPositionZ.getFloat("low"),m_headPositionZ.getFloat("high"));

		ofVec3f& normalHead = m_head.getNormals()[i];
//			normalHead = -normalHead;
	}
	m_model.mesh.append(m_head);
//	m_model.createMeshFaces();
}

//--------------------------------------------------------------
void apparelMod_pretentiopathy::onParameterChanged(ofAbstractParameter& parameter)
{
	setChanged();
}

//--------------------------------------------------------------
void apparelMod_pretentiopathy::setHeadPosition(float x, float y, float z)
{
	m_headPosition.set(x,y,z);
	setChanged();
}

