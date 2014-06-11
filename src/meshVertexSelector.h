//
//  meshVertexSelector.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 31/05/2014.
//
//

#pragma once

#include "meshFaceSelector.h"

class meshVertexSelector/* : public meshFaceSelector*/
{
	public:
		meshVertexSelector					();

		meshFaceSelectorResult*		select				(const ofMesh&, const ofCamera& cam, const ofVec2f& point);
	//	virtual	void				onMeshFaceSelected	(ofMeshFace* face, const ofVec3f pointInFace);
	
		float						m_thDistVertex;
		meshFaceSelectorResult		result;
};