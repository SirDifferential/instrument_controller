varying vec2 texcoord;

void main()
{
	vec4 fragmentPos = gl_ModelViewMatrix * gl_Vertex;
	texcoord = gl_MultiTexCoord0.st;
	gl_Position = gl_ProjectionMatrix * fragmentPos;
}
