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
	OFAPPLOG->begin("apparelMod_selfopathy::apparelMod_selfopathy()");
	bool ok = m_image.loadImage("images/testTexture.png");
	OFAPPLOG->println("- loaded images/testTexture.png "+ofToString(ok ? "OK":"NO..."));
	OFAPPLOG->end();

	m_amplitude.set("Amplitude", 30.0f, 0.0f, 50.0f);
	m_levelSubdiv.set("Subdivision",2, 1,4);

	m_parameters.add(m_amplitude);
	m_parameters.add(m_levelSubdiv);

//	parameterGroupLowHigh::create(m_amplitudeRndFactor, "AmplitudeRndFactor", 0.0,0.7,1.0,1.0);
//	m_parameters.add(m_amplitudeRndFactor);
	
}

//--------------------------------------------------------------
apparelMod_selfopathy::~apparelMod_selfopathy()
{

}

//--------------------------------------------------------------
void apparelMod_selfopathy::apply()
{
	if (isChanged())
	{
		// OFAPPLOG->begin("apparelMood_porcupinopathy::apply()");
	
		newMesh.clear();
		
		m_meshInputBoundingBox.calculateAABoundingBox( m_meshInput.getVertices() );
	
		int nbFacesSelected = m_indicesFaces.size();
		
		int indexVerticesStart = m_model.mesh.getVertices().size();
		//if (nbFacesSelected>0)
		{
		
			int nbFaces = m_model.getMeshFacesRef().size();
			
			// Go through all faces
			for (int i=0; i<nbFaces/*nbFacesSelected*/; i++)
			{
			
				// Select face to subdivide
			 	//ofMeshFaceApparel* pFace = m_model.getMeshFacesRef()[ m_indicesFaces[i] ];
			 	ofMeshFaceApparel* pFace = m_model.getMeshFacesRef()[ i ];
				ofVec3f faceNormal = pFace->getFaceNormal();

				float d = faceNormal.dot( ofVec3f(0,1,0) );
				//if (abs(d)<=0.7)
				{
			 
			 
				 // For this face we create a mesh "meshSubdivived0"
				 ofMesh meshSubdivided0;
				 meshSubdivided0.setMode(OF_PRIMITIVE_TRIANGLES);


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

					 // Compute "middle" point of triangle
/*					 ofVec3f faceMiddle = (faceAdd.getVertex(0) + faceAdd.getVertex(1) + faceAdd.getVertex(2))/3.0f;


					 // Extrude
					 ofVec3f M = faceMiddle;// + faceNormal*m_weight*m_amplitude;

					 // Add to model
					 ofIndexType indexM = newMesh.getNumVertices();
					 newMesh.addVertex(M);
					 newMesh.addNormal(faceNormal);

					 // Add indices of extruded faces now


					  newMesh.addIndex(indexA);
					  newMesh.addIndex(indexB);
					  newMesh.addIndex(indexM);

					  newMesh.addIndex(indexB);
					  newMesh.addIndex(indexC);
					  newMesh.addIndex(indexM);

					  newMesh.addIndex(indexC);
					  newMesh.addIndex(indexA);
					  newMesh.addIndex(indexM);
*/
					  // Save in our list to retrieve in update without updating model
					  //m_extrusionData.push_back( new porcuVertexData(indexM,faceNormal,faceMiddle, m_amplitudeRndFactor.getFloat("low"), m_amplitudeRndFactor.getFloat("high") ));
				 }
				}
			}
			
			newMesh.mergeDuplicateVertices();

			m_model.mesh.clear();
			m_model.mesh.append(newMesh);
		}
		
		displaceVertices();
		m_model.createMeshFaces(); // useful for selection

//		OFAPPLOG->end();
	}
}

//--------------------------------------------------------------
void apparelMod_selfopathy::displaceVertices()
{
   vector<ofVec3f>& vertices = m_model.mesh.getVertices();
   vector<ofVec3f>& normals = m_model.mesh.getNormals();

   ofVec3f posBounding = m_meshInputBoundingBox.getPosition();
   for (int i=0; i<vertices.size(); i++ )
   {
	
	   // Displacement
	   ofVec3f vNorm(
		   0.5f+(vertices[i].x-posBounding.x)/m_meshInputBoundingBox.getSize().x,
		   0.5f+(vertices[i].y-posBounding.y)/m_meshInputBoundingBox.getSize().y,
		   0.5f+(vertices[i].z-posBounding.z)/m_meshInputBoundingBox.getSize().z);
	
	   float d = normals[i].dot( ofVec3f(0,1,0) ); // Z-axis
	   ofColor c = m_image.getColor(vNorm.x*(m_image.getWidth()-1),vNorm.z*(m_image.getHeight()-1));

	   float lightness = c.getLightness() / c.limit();
	   float amplitude = abs(d)*lightness*m_weight*m_amplitude;
	   if (abs(d)>0.65f)
	   {
		   vertices[i] += amplitude*normals[i];
	   
	   }
	   
   }
}

//--------------------------------------------------------------
void apparelMod_selfopathy::drawExtra()
{
	//m_meshInput.draw();
	ofSetColor(200,0,0,200);
	m_meshInputBoundingBox.draw();
	ofPushMatrix();
	m_image.setAnchorPercent(0.5f,0.5f);
	ofTranslate(m_meshInputBoundingBox.getPosition().x, m_meshInputBoundingBox.getPosition().y+m_meshInputBoundingBox.getSize().y/2, m_meshInputBoundingBox.getPosition().z);
	ofRotateX(90);
	ofSetColor(255);
	m_image.draw(0, 0, 0, m_meshInputBoundingBox.getSize().x, m_meshInputBoundingBox.getSize().z);
	ofPopMatrix();
}

//--------------------------------------------------------------
void apparelMod_selfopathy::onParameterChanged(ofAbstractParameter& parameter)
{
	// Only recompute Mesh if subdivision is changed
//	if (parameter.getName() == "Subdivision")
		setChanged(true);
//	if (parameter.getName() == "Subdivision")
}

//--------------------------------------------------------------
void apparelMod_selfopathy::onWeightChanged()
{
	//displaceVertices();
}



//--------------------------------------------------------------
void apparelMod_selfopathy::update()
{
	// Get amplitude of sound
	//m_weight = GLOBALS->getSoundInputVolume(); // ofMap(GLOBALS->getSoundInputVolume(),0.0f,0.2f, 0.1f,1.0f);
	
	//int nbVerticesExtruded = m_extrusionData.size();

	// Compute points
	float amplitude = m_weight*m_amplitude;
	//for (int i=0;i<nbVerticesExtruded;i++)
	{
//		porcuVertexData* pData = m_extrusionData[i];

		// Update
		// pData->m_amplitude = m_weight * pData->m_amplitudeMax;
//		pData->m_amplitude += (pData->m_amplitudeFactor*amplitude-pData->m_amplitude)*pData->m_amplitudeSpeed;

		// Get index of the vertex
//		ofIndexType indexVertex = pData->m_index;

		// ... and compute new extruded vertex from middl point of face
		//m_model.mesh.setVertex(indexVertex, pData->m_middleFace+pData->m_normal*pData->m_amplitude);
	}
	
	// compute normals (as vertices moved)
	//m_model.
	
}


//--------------------------------------------------------------
/*
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
*/

/*

[TWTRLogInButton buttonWithLogInCompletion:^(TWTRSession *session, NSError *error) {
  [[[Twitter sharedInstance] APIClient] loadUserWithID:session.userID completion:^(TWTRUser *user, NSError *error) {
       NSLog(@"User image %@", user.profileImageURL);
  }];
}];

*/
