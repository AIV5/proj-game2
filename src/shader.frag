#version 460

#define MAX_OBJECTS 32
#define FULL 0
#define LIMITED 1
#define BOUND 2


float pi = acos(0);
float lambda = 1;
float eps = 0.005;

out vec4 fragColor;

uniform vec2 hr;
uniform vec4 playerP, playerR, playerU, playerF; // position, right, up, forward

uniform int   objNumber;
uniform int   [MAX_OBJECTS] objType;
uniform mat4  [MAX_OBJECTS] objPM; // projection matrix
uniform vec4  [MAX_OBJECTS] objColor;
uniform vec4  [MAX_OBJECTS] objCenter;
uniform float [MAX_OBJECTS] objRad;

float projDist1v (vec4 v) {
    float len = length(v);
    if (len > 1)
        return 0;
    return acos(len);
}

float projDist2v (vec4 a, vec4 b) {
    float prod = abs(dot(a, b));
    if (prod > 1)
        return 0;
    return acos(prod);
}

float projDist (vec4 v, int objIndex) {
    vec4 p = objPM[objIndex] * v;   //  projection
    if (objType[objIndex] == FULL)
        return projDist1v(p);
    if (length(p) == 0)
        return pi / 2;
    vec4 n = normalize(p);          //  normalized projection
    /*
    if (objType[objIndex] == LIMITED && projDist2v(n, objCenter[objIndex]) < objRad[objIndex])  //  if got inside the object
        return projDist1v(p);
    */
    n = n - objCenter[objIndex] * dot(n, objCenter[objIndex]);
    /*
    if (length(n) < 0.001)
        return pi/2;
    */
    n = normalize(n);   // orthonormal addition to the center of the figure
    n = objCenter[objIndex] * cos(objRad[objIndex]) + n * sin(objRad[objIndex]);    //  boundary point
    return projDist2v(v, n);
}

void main () {
    vec2 normCoord = (gl_FragCoord.xy - hr) / length(hr);
    float polarRad = length(normCoord);
    vec2 polarTrig = normCoord / polarRad;
    float alpha = 0, dMin = 2 * pi, dCur = 0;
    vec4 look = playerF * cos(lambda * polarRad) + (polarTrig.x * playerR + polarTrig.y * playerU) * sin(lambda * polarRad);

    if (dot(look, playerF) < 0) {
        fragColor = vec4(0, 0, 0, 1);
        return;
    }

    vec4 explorer;
    for (int k = 0; k < 250; ++k) {
        explorer = cos(alpha) * playerP + sin(alpha) * look;
        for (int objIndex = 0; objIndex < objNumber; ++objIndex) {
            dCur = projDist(explorer, objIndex);
            if (dCur < eps) {

                float k = alpha <= pi ? 0 : alpha / pi - 1;
                fragColor = objColor[objIndex] * (1 - k) + k * vec4(.25, .25, .25, 1);

                /*
                if (f == 0)
                    fragColor = vec4(0,0,0,1);
                if (f == 1)
                    fragColor = vec4(1,0,0,1);
                if (f == 2)
                    fragColor = vec4(0,1,0,1);
                if (f == 3)
                    fragColor = vec4(0,0,1,1);
                if (f == 4)
                    fragColor = vec4(1,1,0,1);
                if (f == 5)
                    fragColor = vec4(0,1,1,1);
                */
                return;
            }
            if (dCur < dMin)
                dMin = dCur;
        }
        alpha += dMin;
        if (alpha >= 2 * pi) {
            fragColor = vec4(.25, .25, .25, 1);
            return;
        }
    }
    fragColor = vec4(1, 1, 1, 1);
}
