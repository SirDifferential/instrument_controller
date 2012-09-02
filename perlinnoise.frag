uniform sampler2D baseNoise;
uniform float persistence;
uniform float amplitude;
uniform int octaveSetter;
uniform float magicNumber1;
uniform float magicNumber2;
uniform int size_x;
uniform int size_y;
varying vec2 texCoord;
varying vec4 v_color;
uniform float time;

// Great explanation of this stuff: http://devmag.org.za/2009/04/25/perlin-noise/

float interpolate(float x0, float x1, float alpha)
{
    float interpolation = x0 * (1.0 - alpha) + alpha * x1;
    return interpolation;
}

float noise2(vec2 input_coords, int octave)
{
    float out1 = 0.0f;
    float i = input_coords.s;   // 0...1
    float j = input_coords.t;   // 0...1
    
    // Get noise location related to the native texture size instea of using OpenGL texture space range 0...1
    int i_resize = int(input_coords.s * size_x);    // 0...1024
    int j_resize = int(input_coords.t * size_y);    // 0...1024
    
    int samplePeriod = 1 << octave;         // 2^n, defines the step size, how large our gradients are
    float frequency = 1.0f / samplePeriod;  // 1/2^n
    
    // Starting point of our gradient
    // Note that that this is not the same as just i_resize due to flooring math
    // The produced value is the largest multiple if samplePeriod that is smaller than i_resize
    int sample_i0 = (i_resize / samplePeriod) * samplePeriod;
    // Ending point of the gradient. If > 1024, wrap around to 0
    int sample_i1 = (sample_i0 + samplePeriod) % size_x;
    
    // Defines the alpha for our linear interpolation on X-axis
    // If we are close to the left end of the gradient, alpha is close to 0
    // If we are close to the right end of the gradient, alpha is close to 1
    float horizontal_blend = (i_resize - sample_i0) * frequency;
    
    // The starting point of our gradient (Y-axis)
    int sample_j0 = (j_resize / samplePeriod) * samplePeriod;
    // Ending point (Y-axis)
    int sample_j1 = int(sample_j0 + samplePeriod) % size_y;
    
    // After we are done with our interpolation for top row and bottom row, this
    // value is used for interpolating on Y-axis
    float vertical_blend = (j_resize - sample_j0) * frequency;
    
    // Transform to OpenGL texture coordinate space 0...1
    float coord1 = float(sample_i0) / 1024;
    float coord2 = float(sample_i1) / 1024;
    float coord3 = float(sample_j0) / 1024;
    float coord4 = float(sample_j1) / 1024;
    
    // Make sure values are in range 0...1
    float top = interpolate(texture2D(baseNoise, vec2(coord1,coord3)).r, texture2D(baseNoise, vec2(coord2, coord3)).r, horizontal_blend);
    float bottom = interpolate(texture2D(baseNoise, vec2(coord1, coord4)).r, texture2D(baseNoise, vec2(coord2, coord4)).r, horizontal_blend);
    out1 = interpolate(top, bottom, vertical_blend);
    
    return out1;
}

float filterValue(float v, float filter)
{
    if (v >= filter)
        return v;
    else
        return 0.0f;
}

// Calculates a fire effect by using the perlin noise value at
// these texture coordinates as a temperature, then looking up
// the corresponding RGB values from the tables below
vec4 getFire(float t)
{
/*
                    Values taken from:
                Colour Rendering of Spectra

                       by John Walker
                  http://www.fourmilab.ch/
                  
    Temperature       x      y      z       R     G     B
    -----------    ------ ------ ------   ----- ----- -----
       1000 K      0.6528 0.3444 0.0028   1.000 0.007 0.000 (Approximation)
       1500 K      0.5857 0.3931 0.0212   1.000 0.126 0.000 (Approximation)
       2000 K      0.5267 0.4133 0.0600   1.000 0.234 0.010
       2500 K      0.4770 0.4137 0.1093   1.000 0.349 0.067
       3000 K      0.4369 0.4041 0.1590   1.000 0.454 0.151
       3500 K      0.4053 0.3907 0.2040   1.000 0.549 0.254
       4000 K      0.3805 0.3768 0.2428   1.000 0.635 0.370
       4500 K      0.3608 0.3636 0.2756   1.000 0.710 0.493
       5000 K      0.3451 0.3516 0.3032   1.000 0.778 0.620
       5500 K      0.3325 0.3411 0.3265   1.000 0.837 0.746
       6000 K      0.3221 0.3318 0.3461   1.000 0.890 0.869
       6500 K      0.3135 0.3237 0.3628   1.000 0.937 0.988
       7000 K      0.3064 0.3166 0.3770   0.907 0.888 1.000
       7500 K      0.3004 0.3103 0.3893   0.827 0.839 1.000
       8000 K      0.2952 0.3048 0.4000   0.762 0.800 1.000
       8500 K      0.2908 0.3000 0.4093   0.711 0.766 1.000
       9000 K      0.2869 0.2956 0.4174   0.668 0.738 1.000
       9500 K      0.2836 0.2918 0.4246   0.632 0.714 1.000
      10000 K      0.2807 0.2884 0.4310   0.602 0.693 1.000
*/

    // Here I have three arrays that contain RGB values for some kelvin values

    float blue[] = 
    {
        0.0f,
        0.0f,
        0.010f,
        0.067f,
        0.151f,
        0.254f,
        0.370f,
        0.493f,
        0.620f,
        0.746f,
        0.869f,
        0.988f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f
    };

    float green[] = 
    {
        0.007f,
        0.126f,
        0.234f,
        0.349f,
        0.454f,
        0.549f,
        0.635f,
        0.710f,
        0.778f,
        0.837f,
        0.890f,
        0.937f,
        0.888f,
        0.839f,
        0.766f,
        0.738f,
        0.714f,
        0.693f
    };

    float red[] =
    {
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        0.907f,
        0.827f,
        0.762f,
        0.668f,
        0.632f,
        0.602f
    };

    vec4 fireOutput;
    
    // Search the temperature array for the closest match, then do interpolation
    // with the two closest values. That's our RGB representation of temperature t
    
    // convert t into range 0 ... 16 so that we can look up the correct indices in the RGB arrays above
    float temperature = 16.0f*t;
    
    int colour_index = int(floor(temperature));
    
    // Interpolate each value
    float distance = temperature - float(colour_index);   // Produces the distance to the lower value
    float r = interpolate(red[colour_index], red[colour_index+1], distance);
    float g = interpolate(green[colour_index], green[colour_index+1], distance);
    float b = interpolate(blue[colour_index], blue[colour_index+1], distance);
    fireOutput.r = r;
    fireOutput.g = g;
    fireOutput.b = b;
    fireOutput.a = 1.0f;
    
    /*
    // These vectors contain some key RGB values for defining the fire colour
    vec3 highest_temperature = vec3(1.0f, 1.0f, 1.0f);
    vec3 high_temperature= vec3(0.95f, 0.93f, 0.125f);
    vec3 middle_high_temperature_begin = vec3(0.945f, 0.443f, 0.157f);
    vec3 middle_low_temperature_begin = vec3(0.384, 0.0f, 0.0f);
    vec3 lowest_temperature = vec3(0.0f, 0.0f, 0.0f);
    */
    
    return fireOutput;
}

void main()
{
    float noiseOctaves[20];
    float persistence_copy = persistence;
    //float amplitude_copy = amplitude;
    // For fun, use time. Looks cool with the temperature stuff
    float amplitude_copy = sin(time/10);
    float totalAmplitude = 0.0f;
    float n;
    float perlinNoise = 0.0f;
    vec4 col;
  
    for (int i = 0; i < octaveSetter && i < 20; i++)
    {
        noiseOctaves[i] = noise2(texCoord.st, i);
    }
    
    for (int i = octaveSetter-1; i >= 0; i--)
    {
        amplitude_copy *= persistence_copy;
        totalAmplitude += amplitude_copy;
        perlinNoise += (noiseOctaves[i] * amplitude_copy);
    }
    
    // Normalize the noise
    //perlinNoise = perlinNoise / totalAmplitude;
    
    col = getFire(perlinNoise);
    gl_FragColor = v_color * col;
}
