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
	m_bDoSudivision = false;
	if (mp_image == 0)
		m_bDoSudivision = true;
	
	mp_image = pImage;
	setChanged();
	m_bDoDisplacement = true;
}

//--------------------------------------------------------------
void apparelMod_selfopathy::apply()
{
		m_meshInputBoundingBox.calculateAABoundingBox( m_meshInput.getVertices() );

/*
		if (m_bDoSudivision && false)
		{
			m_meshInput.mergeDuplicateVertices();
			ofMesh meshSubdivided1 = butterfly.subdivideLinear(m_meshInput, (int)m_levelSubdiv );
			m_model.mesh = meshSubdivided1;
			m_meshRefDisplacement = m_model.mesh;
		}
*/
	
		if (m_bDoSudivision)
		{
			m_model.mesh = m_meshInput;

			newMesh.clear();
		
			int nbFaces = m_model.getMeshFacesRef().size();
			
			// Go through all faces
			ofMesh meshSubdivided0;
			for (int i=0; i<nbFaces; i++)
			{
			
				// Select face to subdivide
			 	ofMeshFaceApparel* pFace = m_model.getMeshFacesRef()[ i ];
				ofVec3f faceNormal = pFace->getFaceNormal();

				//float d = faceNormal.dot( ofVec3f(0,1,0) );
				//if (abs(d)<=0.7)
				{
			 
			 
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
				 ofMesh meshSubdivided1 = butterfly.subdivideLinear(meshSubdivided0, (int)m_levelSubdiv );

				 // Add vertices to current model
				 // Index of the first vertices we are going to add
				 //ofIndexType indexVertModel = m_model.mesh.getNumVertices();
				 ofIndexType indexVertModel = newMesh.getNumVertices();
				 
				 // References to vertices of subdivided mesh
				 //vector<ofVec3f>& verticesAdd = meshSubdivided1.getVertices();
				 int nbVerticesAdd = meshSubdivided1.getNumVertices();
				 
				 // Go
				 for (int v=0; v<nbVerticesAdd; v++)
				 {
					 newMesh.addVertex( meshSubdivided1.getVertex(v) );
					 newMesh.addNormal( faceNormal );
				 }
				 
				 
				 // Add indices (faces) to current model
				 vector<ofIndexType>& 		indicesAdd 	= meshSubdivided1.getIndices();
				 const vector<ofMeshFace>& 	facesAdd 	= meshSubdivided1.getUniqueFaces();

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
					 
					 newMesh.addIndex(indexA);
					 newMesh.addIndex(indexB);
					 newMesh.addIndex(indexC);

				 }
				}
			
				newMesh.mergeDuplicateVertices();

				m_model.mesh.clear();
				m_model.mesh.append(newMesh);

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
		vNorm.z = min(max(vNorm.z,0.0f),1.0f);
	
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
