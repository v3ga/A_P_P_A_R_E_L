void main()
{
		vec4 vertexEye 	= gl_ModelViewMatrix * gl_Vertex;
		vec3 normalEye 	= normalize(gl_NormalMatrix * gl_Normal);


		gl_Position = gl_ProjectionMatrix * vertexEye;
		float g = dot(normalEye,normalize(-vertexEye.xyz));
		gl_FrontColor = vec4( g,g,g, 1.0);
		gl_BackColor = gl_FrontColor;
}