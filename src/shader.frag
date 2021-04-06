#version 460

const int MAX_FACES = 32;
const float pi = acos(-1);
const float lambda = 2;
const int vertexNumber = 4;

out vec4 fragColor;

uniform vec2 hr;
uniform vec4 playerP, playerR, playerU, playerF; // position, right, up, forward

uniform int   faceNumber;
uniform vec4  [MAX_FACES] faceColor;
uniform vec4  [MAX_FACES] faceCenter;
uniform vec4  [MAX_FACES] faceStart;
uniform vec4  [MAX_FACES] facePoint;
uniform float [MAX_FACES] faceRad;

vec4 intersectionPoint;

float projDist (vec4 a, vec4 b) {
    float prod = abs(dot(a, b));
    if (prod > 1)
        return 0;
    return acos(prod);
}

float angle (vec4 center, vec4 a, vec4 b) {
    a -= center;
    if (length(a) == 0)
        return 0;
    a = normalize(a);
    b -= center;
    if (length(b) == 0)
        return 0;
    b = normalize(b);
    float prod = dot(a, b);
    if (abs(prod) > 1)
        return 0;
    return acos(prod);
}

float poligonRadCoef (float alpha) {
    float phi = 2 * pi / vertexNumber;
    while (alpha > phi)
        alpha -= phi;
    float c1 = cos(phi), s1 = sin(phi);
    float c2 = cos(alpha), s2 = sin(alpha);
    return s1 / (s2 + c2*s1 - s2*c1);
}

float intersection (vec4 look, int faceIndex) {
    mat4 m = mat4(faceCenter[faceIndex] - faceStart[faceIndex], faceCenter[faceIndex] - facePoint[faceIndex], playerP, look);
    if (determinant(m) == 0)
        return 2;
    vec4 sol = inverse(m) * faceCenter[faceIndex];
    vec4 p = playerP * sol.z + look * sol.w;
    if (length(p) == 0)
        return 2;
    intersectionPoint = normalize(p);
    float poligonRad = faceRad[faceIndex] * poligonRadCoef(angle(faceCenter[faceIndex], p, faceStart[faceIndex]));
    return distance(p, faceCenter[faceIndex]) / poligonRad;
}

void main () {
    vec2 normCoord = (gl_FragCoord.xy - hr) / length(hr);
    float polarRad = length(normCoord);
    vec2 polarTrig = polarRad == 0 ? vec2(0, 0) : normCoord / polarRad;
    vec4 look = playerF * cos(lambda * polarRad) + (polarTrig.x * playerR + polarTrig.y * playerU) * sin(lambda * polarRad);
    if (dot(look, playerF) < 0) {
        fragColor = vec4(0, 0, 0, 1);
        return;
    }
    float dMin = 2 * pi, dCur, IRCur, IRMin;
    int indexMin;
    for (int faceIndex = 0; faceIndex < faceNumber; ++faceIndex) {
        IRCur = intersection(look, faceIndex);
        if (IRCur > 1)
            continue;
        dCur = projDist(playerP, intersectionPoint);
        if (dot(playerP, intersectionPoint) * dot(look, intersectionPoint) < 0)
                dCur = pi - dCur;
        if (dCur < dMin) {
            dMin = dCur;
            IRMin = IRCur;
            indexMin = faceIndex;
        }
    }
    if (dMin < 2 * pi) {
        fragColor = faceColor[indexMin] - .25 * IRMin;
    } else {
        fragColor = vec4(.25, .25, .25, 1);
    }
}
