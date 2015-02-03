//
//  apparelMode_porcupinopathy.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 05/12/2014.
//
//

#include "apparelMood_porcupinopathy.h"
#include "ofAppLog.h"
#include "globals.h"

//--------------------------------------------------------------
apparelMood_porcupinopathy::apparelMood_porcupinopathy() : apparelMod("Porcupinopathy")
{
	m_amplitude.set("Amplitude", 30.0f, 0.0f, 50.0f);
	m_levelSubdiv.set("Subdivision",2, 1,4);

	m_parameters.add(m_amplitude);
	m_parameters.add(m_levelSubdiv);
}

//--------------------------------------------------------------
apparelMood_porcupinopathy::~apparelMood_porcupinopathy()
{
}


//--------------------------------------------------------------
void apparelMood_porcupinopathy::apply()
{
	if (isChanged())
	{
		OFAPPLOG->begin("apparelMood_porcupinopathy::apply()");
	
		m_model.mesh = m_meshInput;
		m_indexVerticesExtruded.clear();
		m_normalVerticesExtruded.clear();
		m_middleFaceVertices.clear();
	

		int nbFacesSelected = m_indicesFaces.size();
		if (nbFacesSelected>0)
		{
		
			for (int i=0; i<nbFacesSelected; i++)
			{
				// Select face to subdivide
			 	ofMeshFaceApparel* pFace = m_model.getMeshFacesRef()[ m_indicesFaces[i] ];
				ofVec3f faceNormal = pFace->getFaceNormal();
			
			
				// For this face we create a mesh "meshSubdivived0"
				ofMesh meshSubdivided0;
				meshSubdivided0.setMode(OF_PRIMITIVE_TRIANGLES);


				// Add vertices, normals and indices
				meshSubdivided0.addVertex( pFace->getVertex(0) );
				meshSubdivided0.addVertex( pFace->getVertex(1) );
				meshSubdivided0.addVertex( pFace->getVertex(2) );


				meshSubdivided0.addNormal( pFace->getNormal(0) );
				meshSubdivided0.addNormal( pFace->getNormal(1) );
				meshSubdivided0.addVertex( pFace->getNormal(2) );


				meshSubdivided0.addIndex(0);
				meshSubdivided0.addIndex(1);
				meshSubdivided0.addIndex(2);
				
				// Subdivide now.
				ofMesh meshSubdivided1 = butterfly.subdivideLinear(meshSubdivided0, (int)m_levelSubdiv );

				// Add vertices to current model
				// Index of the first vertices we are going to add
				ofIndexType indexVertModel = m_model.mesh.getNumVertices();
				
				// References to vertices of subdivided mesh
				vector<ofVec3f>& verticesAdd = meshSubdivided1.getVertices();
				int nbVerticesAdd = meshSubdivided1.getNumVertices();
				
				// Go
				for (int v=0; v<nbVerticesAdd; v++)
				{
					m_model.mesh.addVertex( meshSubdivided1.getVertex(v) );
				}
				
				// Add indices (faces) to current model
				vector<ofIndexType>& indicesAdd = meshSubdivided1.getIndices();
				const vector<ofMeshFace>& facesAdd = meshSubdivided1.getUniqueFaces();
				int nbFacesAdd = facesAdd.size();
				for (int f=0;f<nbFacesAdd;f++)
				{
					// Get face
					const ofMeshFace& faceAdd = facesAdd[f];
					
					// Compare orientation of subdivided triangles to parent triangle
					bool bOrientationCorrect = faceAdd.getFaceNormal().dot(faceNormal) > 0;

					// Add indices to current mesh model
					ofIndexType indexA,indexB,indexC;
					if (bOrientationCorrect)
					{
						indexA = indexVertModel + indicesAdd[f*3+0];
						indexB = indexVertModel + indicesAdd[f*3+1];
						indexC = indexVertModel + indicesAdd[f*3+2];
					}
					else
					{
						indexA = indexVertModel + indicesAdd[f*3+0];
						indexB = indexVertModel + indicesAdd[f*3+2];
						indexC = indexVertModel + indicesAdd[f*3+1];
					}
					
					m_model.mesh.addIndex(indexA);
					m_model.mesh.addIndex(indexB);
					m_model.mesh.addIndex(indexC);

					// Compute "middle" point of triangle
					ofVec3f faceMiddleOpposite = faceAdd.getVertex(1) + ofRandom(0.1,0.9)*(faceAdd.getVertex(2)-faceAdd.getVertex(1));
//					ofVec3f faceMiddle = (faceAdd.getVertex(0)+faceAdd.getVertex(1)+faceAdd.getVertex(2))/3;
					ofVec3f faceMiddle = faceAdd.getVertex(0) + ofRandom(0.1,0.9)*(faceMiddleOpposite-faceAdd.getVertex(0));


					// Extrude
					ofVec3f M = faceMiddle + faceNormal*m_weight*m_amplitude;

					// Add to model
					ofIndexType indexM = m_model.mesh.getNumVertices();
					m_model.mesh.addVertex( M );

					// Add indices of extruded faces now
					 m_model.mesh.addIndex(indexA);
					 m_model.mesh.addIndex(indexB);
					 m_model.mesh.addIndex(indexM);

					 m_model.mesh.addIndex(indexB);
					 m_model.mesh.addIndex(indexC);
					 m_model.mesh.addIndex(indexM);

					 m_model.mesh.addIndex(indexC);
					 m_model.mesh.addIndex(indexA);
					 m_model.mesh.addIndex(indexM);
				 
					 // Save in our list to retrieve in update without updating model
					 m_indexVerticesExtruded.push_back(indexM);
					 m_normalVerticesExtruded.push_back(faceNormal);
					 m_middleFaceVertices.push_back(faceMiddle);
				}
			}

			m_model.createMeshFaces();
		}

		OFAPPLOG->end();
	}
}

//--------------------------------------------------------------
void apparelMood_porcupinopathy::onParameterChanged(ofAbstractParameter& parameter)
{
	// Only recompute Mesh if subdivision is changed
	if (parameter.getName() == "Subdivision")
		setChanged(true);
}


//--------------------------------------------------------------
void apparelMood_porcupinopathy::update()
{
	// Get amplitude of sound
	m_weight = ofMap(GLOBALS->getSoundInputVolume(),0.0f,0.5f, 0.5f,1.0f);
	int nbVerticesExtruded = m_indexVerticesExtruded.size();

printf("m_weight=%0.4f", m_weight.get());
printf("nbVerticesExtruded=%d",nbVerticesExtruded);


	// Compute points
	for (int i=0;i<nbVerticesExtruded;i++)
	{
		// Get index of the vertex
		ofIndexType indexVertex = m_indexVerticesExtruded[i];
		// Get normal...
		ofVec3f& normal = m_normalVerticesExtruded[i];
		// ... and compute new extruded vertex from middl point of face
		m_model.mesh.setVertex(indexVertex, m_middleFaceVertices[i] + normal*m_weight*m_amplitude);
	}
//	m_model.createMeshFaces();
}


