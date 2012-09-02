uniform sampler2D colorMap;
varying float diffuse;
varying vec2 TexCoord;
varying float spec;

void main (void)
{
    float diff = diffuse * 0.95;  // constant values for the contribution are used.
    float sp = spec * 0.05;
    vec3 lightcolor = vec3(texture2D(colorMap, TexCoord).rgb * (diff + sp));
    gl_FragColor = vec4(lightcolor , 1.0);
}