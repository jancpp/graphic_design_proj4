#version 410 core

// phong.fsh - a fragment shader that implements a Phong Lighting model.

in PVA
{
    vec3 ecPosition; // Q
    vec3 ecUnitNormal;
    vec2 texCoords;
} pvaIn;

uniform sampler2D textureMap;
uniform int usingTextureMap = 0;
uniform int sceneHasTranslucentObjects = 0;
uniform int drawingOpaqueObjects = 1;

// For lighing model:
const int MAX_NUM_LIGHTS = 4;
uniform mat4 ec_lds; // so projection type and hence vHat can be determined

// Phong material properties (RGB reflectances);
uniform vec3 ka = vec3(0.8, 0.0, 0.0); // default: darkish red
uniform vec3 kd = vec3(0.8, 0.0, 0.0); // default: darkish red
uniform vec3 ks = vec3(0.8, 0.0, 0.0); // default: darkish red

// Lighting environment
uniform vec3 La = vec3(0.25, 0.25, 0.25); // RGB strength of assumed ambient light:
uniform vec3 lightStrength[MAX_NUM_LIGHTS];

// a lighting model attenuation function: fi(Q) = cScale / (c0 + c1d + c2d2)
float atten(vec3 Q, vec3 Li) {
    return (5.0/((distance(Q, Li)*0.08 + 3.5)));
}

uniform vec4 p_ecLightPos[MAX_NUM_LIGHTS];
vec3 riHat;
uniform float m = 1.0; // shininess
uniform float alpha = 1.0;
uniform int actualNumLights = 3;

// output color from the lighting model:
out vec4 fragColor;

vec4 evaluateLightingModel()
{
    vec3 liHat  = vec3(0.0, 0.0, 1.0);  // directional light in EC at eye (a flashlight)
    vec3 vHat   = vec3(0.0, 0.0, 1.0);  // unit towards the eye
    vec3 riHat  = vec3(0.0, 0.0, 0.0);
    
    vec3 Ia     = ka * La;              // ambient reflection
    vec3 Id     = vec3(0.0, 0.0, 0.0);  // diffuse reflection
    vec3 Is     = vec3(0.0, 0.0, 0.0);  // specular reflection
    
    if (ec_lds[3].w == 0) {
        vHat = normalize(-pvaIn.ecPosition);
    } else {
        // oblique or orthogonal, m[col][row]!!!
        double dx = -ec_lds[2][0]/ec_lds[0][0];
        double dy = -ec_lds[2][1]/ec_lds[1][1];
        vHat = normalize(vec3(dx, dy, 1.0));
    }
    
    vec3 nHat = pvaIn.ecUnitNormal;
    if (dot(vHat, nHat) < 0){
        nHat = -nHat;
    }
    
    for (int i=0; i<actualNumLights; i++) {
        // 4D position/direction, Li, xyzw = (xi, yi, zi, wi):
        vec3 Li = vec3(p_ecLightPos[i].xyz);
        float wi = p_ecLightPos[i].w;
        float af = 1.0;
        if (wi == 0.0) {
            // a directional source
            liHat = normalize(Li);
        } else {
            // a local source, wi == 1.0
            af = atten(pvaIn.ecPosition, Li) ;
            liHat = normalize(Li - pvaIn.ecPosition);
        }
        
        if (dot(liHat, nHat) > 0) {
            Id += af * lightStrength[i] * kd * dot(liHat,nHat);
            
            riHat = normalize(reflect(-liHat, nHat));
            
            if( dot(riHat, vHat) > 0)
            {
                Is += af * lightStrength[i] * ks * pow(dot(riHat,vHat),m);
            }
        }
    }
    return vec4(Ia + Id + Is, alpha);
}

vec4 composeColor(vec4 lmColor, vec4 tColor)
{
    vec4 combinedColor = (tColor * lmColor);
    
    return combinedColor;
}

void main ()
{
    vec4 color = evaluateLightingModel();
    if (usingTextureMap == 1) {
        vec4 textureColor = texture(textureMap, pvaIn.texCoords);
        fragColor = composeColor(color, textureColor);
    }
    else if (sceneHasTranslucentObjects == 1) {
        if (drawingOpaqueObjects == 1)
            if (color.a < 1.0)
                discard;
            else
                fragColor = color;
            else if (color.a < 1)
                fragColor = color;
            else
                discard;
    }
    else
        fragColor = color;
}

