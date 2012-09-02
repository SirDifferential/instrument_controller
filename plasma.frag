varying vec2 textureCoord;
uniform float time;

void main(void)
{
    float editedTime = time;
    editedTime *= 100;
    float i = textureCoord.s;
    float j = textureCoord.t;
    vec4 col;

    col.r = (
            abs(sin(1 / sqrt( (i-0.25*abs(cos(editedTime*0.01))*abs(sin(i*editedTime*0.02)))*(i-0.25*abs(cos(editedTime*0.01))*abs(sin(i*editedTime*0.02))) + (j-0.25*abs(sin(editedTime*0.03))*(abs(sin(editedTime*j*0.02))))*(j-0.25*abs(sin(editedTime*0.03))*abs(sin(editedTime*j*0.02))) ) + (editedTime*0.1) ))-0.25
            );

    col.g = (
            abs(sin(1 / sqrt( (i-0.55*abs(cos(time*0.01))*abs(sin(i*time*0.02)))*(i-0.55*abs(cos(time*0.01))*abs(sin(i*time*0.02))) + (j-0.55*abs(sin(time*0.03))*(abs(sin(time*j*0.02))))*(j-0.55*abs(sin(time*0.03))*abs(sin(time*j*0.02))) ) + (time*0.1) ))-0.25
            );
    
    col.b = (
             abs(sin(1 / sqrt( (i-0.75*abs(cos(editedTime*0.01))*abs(sin(i*editedTime*0.02)))*(i-0.75*abs(cos(editedTime*0.01))*abs(sin(i*editedTime*0.02))) + (j-0.75*abs(sin(editedTime*0.03))*(abs(sin(editedTime*j*0.02))))*(j-0.75*abs(sin(editedTime*0.03))*abs(sin(editedTime*j*0.02))) ) + (editedTime*0.1) ))-0.25
            );

   // col.g = col.r * col.b;
   // col.b = col.r * col.r;

    gl_FragColor = col;
}

