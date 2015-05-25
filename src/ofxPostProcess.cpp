//
//  ofxPostProcess.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 28/08/2014.
//
//

#include "ofxPostProcess.h"


ofxPostProcess::ofxPostProcess()  : ofxFXObject()
{
        internalFormat = GL_RGBA;

//		fboNoise.allocate(ofGetHeight(),10,GL_RGB);
     
        gl2FragmentShader = STRINGIFY(

/*
								   uniform sampler2DRect 	tex0;
								   uniform sampler2DRect 	tex1;
								   uniform float    		time;
                                   uniform float    		waves;
                                   
                                   void main()
								   {
                                       vec2 st          = gl_TexCoord[0].st;
                                       vec2 st1         = gl_TexCoord[1].st;

										vec2 nnn1		= texture2DRect(tex1, st.yx).xy;
										vec2 nnn2		= texture2DRect(tex1, st.yx+vec2(0.1,0)).xy;
										vec2 nnn3		= texture2DRect(tex1, st.yx+vec2(0.8,0)).xy;

									   float t1 = nnn1.x*60.0;
									   float t2 = nnn2.x*60.0;
									   float t3 = nnn3.x*60.0;

                                       vec4 c1 = texture2DRect(tex0, st+vec2(t1*0.9,0.0) );
                                       vec4 c2 = texture2DRect(tex0, st+vec2(t2*0.95,0.0) );
                                       vec4 c3 = texture2DRect(tex0, st+vec2(t3*0.8,0.0) );

                                       vec3 finalCol	= vec3( c1.r, c2.g, c3.b );
                                    
                                       gl_FragColor.rgb = finalCol;
                                       gl_FragColor.a   = 0.5;
                                   }
*/


uniform sampler2DRect tex0;
uniform float scale;

float find_closest(int x, int  y, float c0)
{
	mat4 dither = mat4(
		1.0,  33.0,  9.0, 41.0,
		49.0, 17.0, 57.0, 25.0,
		13.0, 45.0,  5.0, 37.0,
		61.0, 29.0, 53.0, 21.0 );
	
	float limit = 0.0;
	if(x < 4) {
		if(y >= 4) {
			limit = (dither[x][y-4]+3.0)/65.0;
		} else {
			limit = (dither[x][y])/65.0;
		}
	}
		
	if(x >= 4) {
		if(y >= 4)
			limit = (dither[x-4][y-4]+1.0)/65.0;
		else
			limit = (dither[x-4][y]+2.0)/65.0;
	}
		
	if(c0 < limit)
		return 0.0;
	
	return 1.0;
}

void main(void)
{
	float grayscale = dot( texture2DRect(tex0, gl_TexCoord[0].st), vec4(0.299, 0.587, 0.114, 0));
	vec2 xy = gl_FragCoord.xy * scale;
	
	int x = int(mod(xy.x, 8.0));
	int y = int(mod(xy.y, 8.0));
	float final = find_closest(x, y, grayscale);
	if (final == 1.0)
		gl_FragColor = vec4(final, final, final, 1.0);
	else
		gl_FragColor = vec4(0.0,0.0,0.0, 0.0);
}




                                   );

		glESVertexShader = STRINGIFY(

					attribute vec4 position;
					attribute vec4 color;
					attribute vec4 normal;
					attribute vec2 texcoord;

					uniform mat4 modelViewMatrix;
					uniform mat4 projectionMatrix;


						varying vec2 texCoordVarying;

                        void main()
						{
							texCoordVarying = texcoord;
                            gl_Position = projectionMatrix * modelViewMatrix * position;
						}
									);


        glESFragmentShader = STRINGIFY(
								   precision highp float;

								   uniform sampler2D	 	tex0;
								   uniform sampler2D	 	tex1;
								   
								   varying vec2 texCoordVarying;

                                   void main()

								   {

                                       gl_FragColor.x = texCoordVarying.x/960.0;
                                       gl_FragColor.y = texCoordVarying.y/640.0;
/*                                       gl_FragColor.x = texCoordVarying.x/960.0;
                                       gl_FragColor.y = texCoordVarying.y/640.0;
	                                   gl_FragColor.z = 0.0;
	                                   gl_FragColor.a = 1.0;
*/
	                                   gl_FragColor = texture2D(tex0,vec2(texCoordVarying.x/640.0,texCoordVarying.y/960.0));

										   }
                                   );


	

}

void ofxPostProcess::update()
{

	{
		// Time
		float time = ofGetElapsedTimef();
	
        pingPong.dst->begin();
        
        ofClear(0);
        shader.begin();
        
//        shader.setUniformTexture( "tex0" , textures[0].getTextureReference(), 0 );
//        shader.setUniformTexture( "tex1" , fboNoise.getTextureReference(), 1 );
//        shader.setUniform1f		( "time" , time );
//        shader.setUniform1f		( "waves" , 3.0f );

        shader.setUniformTexture( "tex0" , textures[0].getTextureReference(), 0 );
        shader.setUniform1f( "scale" , 1.0 );
        renderFrame();
        
        shader.end();
        
        pingPong.dst->end();
    };
 


}