//
//  apparelMod_selfopathy.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 03/02/2015.
//
//

#include "apparelMod_selfopathy.h"
#include "parameterGroupLowHigh.h"
#include "ofAppLog.h"

//--------------------------------------------------------------
apparelMod_selfopathy::apparelMod_selfopathy() : apparelMod("Selfopathy")
{
	mp_image = 0;

	m_bDoSudivision = true;
	m_bDoDisplacement = false;

	m_levelSubdiv = 2;
	setChanged();
}

//--------------------------------------------------------------
apparelMod_selfopathy::~apparelMod_selfopathy()
{

}

//--------------------------------------------------------------
void apparelMod_selfopathy::createParameters()
{
	apparelMod::createParameters();
	
	m_amplitude.set("Amplitude", 5.0f, 0.0f, 50.0f);
	m_parameters.add(m_amplitude);
	
/*	m_levelSubdiv.set("Subdivision", 1, 1, 2);
	m_parameters.add(m_levelSubdiv);
*/
	m_weightUI = true;
}

//--------------------------------------------------------------
void apparelMod_selfopathy::setImage(ofImage* pImage)
{
	OFAPPLOG->begin("apparelMod_selfopathy::setImage()");
//	m_bDoSudivision = false;
//	if (mp_image == 0)
		m_bDoSudivision = true;
	
	mp_image = pImage;
	setChanged();
	m_bDoDisplacement = true;
	OFAPPLOG->end();
}

//--------------------------------------------------------------
void apparelMod_selfopathy::apply()
{
		m_meshInputBoundingBox.calculateAABoundingBox( m_meshInput.getVertices() );
	
		if (m_bDoSudivision)
		{
			m_model.mesh = m_meshInput;
			m_newMesh.clear();

/*
			vector<ofIndexType>& indices = m_model.getMeshRef().getIndices();
			ofVec3f A,B,C,M,n;
			for (int i=0;i<indices.size(); i=i+3)
			{
				ofIndexType indexA = indices[i];
				ofIndexType indexB = indices[i+1];
				ofIndexType indexC = indices[i+2];

				A = m_model.getMeshRef().getVertex( indexA );
				B = m_model.getMeshRef().getVertex( indexB );
				C = m_model.getMeshRef().getVertex( indexC );

				M = (A+B+C) / 3.0f;

				

			}
*/

			
			// Go through all faces
			ofMesh meshSubdivided0;
			int nbFaces = (int)m_model.getMeshFacesRef().size();
			for (int i=0; i<nbFaces; i++)
			{
			
				// Select face to subdivide
			 	ofMeshFaceApparel* pFace = m_model.getMeshFacesRef()[ i ];
				ofVec3f faceNormal = pFace->getFaceNormal();

			 
			 
				 // For this face we create a mesh "meshSubdivived0"
				 meshSubdivided0.setMode(OF_PRIMITIVE_TRIANGLES);
				 meshSubdivided0.clear();


				 // Add vertices, normals and indices
				 meshSubdivided0.addVertex( pFace->getVertex(0) );
				 meshSubdivided0.addVertex( pFace->getVertex(1) );
				 meshSubdivided0.addVertex( pFace->getVertex(2) );


				 meshSubdivided0.addNormal( pFace->getNormal(0) );
				 meshSubdivided0.addNormal( pFace->getNormal(1) );
				 meshSubdivided0.addNormal( pFace->getNormal(2) );


				 meshSubdivided0.addIndex(0);
				 meshSubdivided0.addIndex(1);
				 meshSubdivided0.addIndex(2);
				 
				 // Subdivide now.
				 ofMesh meshSubdivided1 = m_butterfly.subdivideLinear(meshSubdivided0, (int)m_levelSubdiv );

				 // Add vertices to current model
				 // Index of the first vertices we are going to add
				 //ofIndexType indexVertModel = m_model.mesh.getNumVertices();
				 ofIndexType indexVertModel = m_newMesh.getNumVertices();
				 
				 // References to vertices of subdivided mesh
				 //vector<ofVec3f>& verticesAdd = meshSubdivided1.getVertices();
				 int nbVerticesAdd = (int)meshSubdivided1.getNumVertices();
				 
				 // Go
				 for (int v=0; v<nbVerticesAdd; v++)
				 {
					 m_newMesh.addVertex( meshSubdivided1.getVertex(v) );
					 m_newMesh.addNormal( faceNormal );
				 }
				 
				 
				 // Add indices (faces) to current model
				 vector<ofIndexType>& 		indicesAdd 	= meshSubdivided1.getIndices();
				 const vector<ofMeshFace>& 	facesAdd 	= meshSubdivided1.getUniqueFaces();

				 int nbFacesAdd = (int)facesAdd.size();
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
					 
					 m_newMesh.addIndex(indexA);
					 m_newMesh.addIndex(indexB);
					 m_newMesh.addIndex(indexC);

				}


			
				m_newMesh.mergeDuplicateVertices();



				m_model.mesh.clear();
				m_model.mesh.append(m_newMesh);

				m_meshRefDisplacement = m_model.mesh;
			}
			
		}

		if (m_bDoDisplacement)
		{
//			m_model.computeMeshFacesNormals();
			displaceVertices();
		}

		if (m_bDoSudivision || m_bDoDisplacement)
		{
			m_model.createMeshFaces();
		}



		// Reset flags
		if (m_bDoSudivision) 	m_bDoSudivision 	= false;
		if (m_bDoDisplacement) 	m_bDoDisplacement 	= false;
}

//--------------------------------------------------------------
void apparelMod_selfopathy::displaceVertices()
{
	if (mp_image == 0) return;


   vector<ofVec3f>& verticesRef = m_meshRefDisplacement.getVertices();

   vector<ofVec3f>& vertices = m_model.mesh.getVertices();
   vector<ofVec3f>& normals = m_model.mesh.getNormals();

   ofVec3f posBounding = m_meshInputBoundingBox.getPosition();
   ofColor c;
   for (int i=0; i<vertices.size(); i++ )
   {
	   // Displacement
	   ofVec3f vNorm
	   (
		   0.5f+(vertices[i].x-posBounding.x)/m_meshInputBoundingBox.getSize().x,
		   0.5f+(vertices[i].y-posBounding.y)/m_meshInputBoundingBox.getSize().y,
		   0.5f+(vertices[i].z-posBounding.z)/m_meshInputBoundingBox.getSize().z
	   );
	
		vNorm.x = min(max(vNorm.x,0.0f),1.0f);
		vNorm.y = min(max(vNorm.y,0.0f),1.0f);
		vNorm.z = 1.0f-min(max(vNorm.z,0.0f),1.0f); // invert here
	
	   float d = normals[i].dot( ofVec3f(0,1,0) ); // Z-axis
	   c = mp_image->getColor(vNorm.x*(mp_image->getWidth()-1),vNorm.z*(mp_image->getHeight()-1));

	   float lightness = c.getLightness() / c.limit();
	   float amplitude = abs(d)*lightness*m_amplitude;
	   if (abs(d)>0.65f)
	   {
		   vertices[i] = verticesRef[i] + m_weight*amplitude*normals[i];
	   
	   }
	   
   }
}

//--------------------------------------------------------------
void apparelMod_selfopathy::drawExtra()
{
	if (mp_image == 0) return;
	
	if (m_bDrawDebug)
	{
		ofSetColor(200,0,0,200);
		m_meshInputBoundingBox.draw();
		ofPushMatrix();
		mp_image->setAnchorPercent(0.5f,0.5f);
		ofTranslate(m_meshInputBoundingBox.getPosition().x, m_meshInputBoundingBox.getPosition().y+m_meshInputBoundingBox.getSize().y/2, m_meshInputBoundingBox.getPosition().z);
		ofRotateX(90);
		ofSetColor(255);
		mp_image->draw(0, 0, 0, m_meshInputBoundingBox.getSize().x, m_meshInputBoundingBox.getSize().z);
		ofPopMatrix();
	}
}

//--------------------------------------------------------------
void apparelMod_selfopathy::onParameterChanged(ofAbstractParameter& parameter)
{
	if (parameter.getName() == "Subdivision")
	{
		m_bDoDisplacement = true;
		//m_bDoSudivision = true;
		ofLog() << m_levelSubdiv;
	}
	else
	{
		m_bDoDisplacement = true;
	}
	setChanged();
}

//--------------------------------------------------------------
void apparelMod_selfopathy::onWeightChanged()
{
	m_bDoDisplacement = true;
}


//--------------------------------------------------------------
void apparelMod_selfopathy::update()
{
}
