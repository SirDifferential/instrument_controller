varying vec2 texcoord;
uniform float time;
uniform int sizex;
uniform int sizey;

void main()
{
	float i = texcoord.s;
	float j = texcoord.t;
	vec4 color;
	color.r =	(
					abs(sin(1 / sqrt( (i-0.5*abs(sin(time*0.01)))*(i-0.5*abs(sin(time*0.01))) + (j-0.5*abs(sin(time*0.03)))*(j-0.5*abs(sin(time*0.03))) ) + (time*0.1) ))-0.25
					+
					abs(sin(1 / sqrt( (i-0.75*abs(cos(time*0.01)))*(i-0.75*abs(cos(time*0.01))) + (j-0.75*abs(sin(time*0.03)))*(j-0.75*abs(sin(time*0.03))) ) + (time*0.1) ))-0.25
					*
					abs(sin(1 / sqrt( (i-0.75*abs(cos(time*0.01))*abs(sin(i*time*0.02)))*(i-0.75*abs(cos(time*0.01))*abs(sin(i*time*0.02))) + (j-0.75*abs(sin(time*0.03))*(abs(sin(time*j*0.02))))*(j-0.75*abs(sin(time*0.03))*abs(sin(time*j*0.02))) ) + (time*0.1) ))-0.25
					
				);
	color.g =	(
					color.r
					-
					sqrt((i*i)+(j*j)) * sin(time*0.01)
				);
	color.b =	(
					abs(sin(color.r * color.g)) * cos(time*0.02)
				);
	gl_FragColor = color;
}
