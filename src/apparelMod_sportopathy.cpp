//
//  apparelMod_sportopathy.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 25/11/2014.
//
//

#include "apparelMod_sportopathy.h"


//--------------------------------------------------------------
apparelMod_sportopathy::apparelMod_sportopathy() : apparelMod("Sportopathy")
{

}


//--------------------------------------------------------------
void apparelMod_sportopathy::apply()
{
	if (isChanged())
	{
		//m_model.clearMeshFaces();
	
		vector<ofVec3f>& 	vertices = m_model.getVerticesRef();
		ofMesh& modelMesh = m_model.getMeshRef();
		
		// Extrude selected faces
		for (int i=0; i<m_indicesFaces.size();i++)
		{
			int faceSelectedIndex = m_indicesFaces[i];
			ofMeshFaceApparel* pFaceSelected = m_model.getMeshFacesRef()[faceSelectedIndex];
			if (pFaceSelected)
			{
				ofIndexType		 index0	  = pFaceSelected->getVertexIndex(0);
				ofIndexType		 index1	  = pFaceSelected->getVertexIndex(1);
				ofIndexType		 index2	  = pFaceSelected->getVertexIndex(2);
			
				ofVec3f* pVertex0 = pFaceSelected->getVertexPointer(0);
				ofVec3f* pVertex1 = pFaceSelected->getVertexPointer(1);
				ofVec3f* pVertex2 = pFaceSelected->getVertexPointer(2);
			
				ofVec3f	 faceNormal = pFaceSelected->getFaceNormal();

				ofVec3f	vertex0_extruded = ofVec3f(*pVertex0) + faceNormal*20;
				ofVec3f	vertex1_extruded = ofVec3f(*pVertex1) + faceNormal*20;
				ofVec3f	vertex2_extruded = ofVec3f(*pVertex2) + faceNormal*20;

				int index0_extruded = vertices.size();
				modelMesh.addVertex(vertex0_extruded);
				modelMesh.addNormal( faceNormal );

				int index1_extruded = vertices.size();
				modelMesh.addVertex(vertex1_extruded);
				modelMesh.addNormal( faceNormal );

				int index2_extruded = vertices.size();
				modelMesh.addVertex(vertex2_extruded);
				modelMesh.addNormal( faceNormal );

				// Facet for drawing
				modelMesh.addIndex( index0_extruded );
				modelMesh.addIndex( index1_extruded );
				modelMesh.addIndex( index2_extruded );
				
				modelMesh.addIndex( index0 );
				modelMesh.addIndex( index1 );
				modelMesh.addIndex( index0_extruded );

				modelMesh.addIndex( index0_extruded );
				modelMesh.addIndex( index1 );
				modelMesh.addIndex( index1_extruded );
				
				modelMesh.addIndex( index1 );
				modelMesh.addIndex( index2_extruded );
				modelMesh.addIndex( index1_extruded );

				modelMesh.addIndex( index2 );
				modelMesh.addIndex( index2_extruded );
				modelMesh.addIndex( index1 );

				modelMesh.addIndex( index2 );
				modelMesh.addIndex( index0_extruded );
				modelMesh.addIndex( index2_extruded );

				modelMesh.addIndex( index0 );
				modelMesh.addIndex( index0_extruded );
				modelMesh.addIndex( index2 );


				
			}
		}

		m_model.createMeshFaces();
	}
	
   }

//--------------------------------------------------------------
void apparelMod_sportopathy::onParameterChanged(ofAbstractParameter& parameter)
{
	if (parameter.getName() == "weight")
	{
		
	}
}

