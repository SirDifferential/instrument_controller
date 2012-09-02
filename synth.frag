uniform int synth1;
uniform int downNotes[240];
uniform int downTimes[240];
uniform float time;
varying vec2 texcoord;

// downNotes[240] contains 240 notes with their state: either 0 if up, or 1 if pressed down
// downTimes[240] contains the time at which that particular key was last modified. 0 if never

float interpolate(float x0, float x1, float alpha)
{
	float interpolation = x0 * (1.0 - alpha) + alpha * x1;
	return interpolation;
}

void main()
{
	vec4 col;
	float sum = 0;

	// Gives a nice frequency based sine pattern
	// col.r += sin(texcoord.s*i);

	col.r = texcoord.s;
	col.g = texcoord.t / texcoord.s;
	col.b = texcoord.s*texcoord.t;

	int i = 0;
	for (i = 0; i < 240; i++)
	{
		if (downNotes[i] == 1)
		{
			col.r += sin(texcoord.s*i) + (sin(texcoord.s*time)*0.5);
			col.g += sin(texcoord.t*i) + (cos(texcoord.t*time)*0.5);
			col.b = sin(texcoord.s*texcoord.s*i) * cos(texcoord.t*texcoord.t*i) * sin(time*0.0001*i);
		}
	}
	
	col.r = col.g * col.b;
	col.g = col.r - col.b * sin(col.b);
	col.b = col.r * col.r;
	col.a = 1.0f;
	gl_FragColor = col;
}

