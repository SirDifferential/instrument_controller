varying vec2 texCoord;
varying vec4 v_color;
uniform float time;

void main()
{
    texCoord = gl_MultiTexCoord0.xy;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    v_color = gl_Color;
}
