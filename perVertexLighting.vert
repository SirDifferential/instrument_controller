// Based on the example at http://arunan.50webs.com/cs594final.html

varying float diffuse;
varying float spec;
uniform vec3 lightpos;
varying vec2 TexCoord;

void main()
{ 
    // eye position, normal, light vector, reflection vector and view vectors are
    // calculated for finding the specularity, which is the dot product of the
    // reflection vector and the view vector clamped between 0 and 1, raised 
    // to the power of 16 and diffuse values.

    vec3 eyeVec = vec3 (gl_ModelViewMatrix * gl_Vertex);
    vec3 normal = normalize(gl_NormalMatrix * gl_Normal);
    vec3 lightVec = normalize(lightpos - eyeVec);
    vec3 reflectVec = normalize(reflect(-lightVec, normal));
    vec3 viewVec = normalize(-eyeVec);

    diffuse = max(dot(lightVec, normal), 0.0);
    spec = max(dot(reflectVec, viewVec), 0.0);
    spec = pow(spec, 2.0);

    TexCoord = gl_MultiTexCoord0.st;
    gl_Position = ftransform(); 
}
