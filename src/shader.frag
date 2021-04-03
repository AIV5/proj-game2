#version 460

#define MAX_FACES 32

float pi = acos(-1);
float lambda = 2;
int vertexNumber = 4;

out vec4 fragColor;

uniform vec2 hr;
uniform vec4 playerP, playerR, playerU, playerF; // position, right, up, forward

uniform int   faceNumber;
uniform mat4  [MAX_FACES] faceIM; // inverse matrix
uniform vec4  [MAX_FACES] faceColor;
uniform vec4  [MAX_FACES] faceCenter;
uniform vec4  [MAX_FACES] faceStart;
uniform vec4  [MAX_FACES] facePoint;
uniform float [MAX_FACES] faceRad;

vec4 intersectionPoint;

float gauss (mat4 matrix) {
    int r, c, e, m; // row num, column num, element num, max index
    float tmp, mv;
    for (e = 0; e < 3; ++e) { // first 3 elements
        m = e; // m is column index (i.e. number of row)
        for (r = e+1; r < 4; ++r) // for rows: find max element in column
            if (abs(matrix[e][r]) > abs(matrix[e][m]))
                m = r;
        // now matrix[e][m] is max in the column
        mv = matrix[e][m];
        for (c = e; c < 4; ++c) { // swap and divide, r is row index (i.e. number of conumn)
            tmp = matrix[c][m];
            matrix[c][m] = matrix[c][e];
            matrix[c][e] = tmp / mv;
        }
        for (r = e+1; r < 4; ++r) // sub: r is column index (i.e. number of row)
            for (c = e+1; c < 4; ++c) // c is row index (i.e. number of column)
                matrix[c][r] -= matrix[c][e] * matrix[e][r];
    }
    return matrix[3][2];
}

float projDist (vec4 a, vec4 b) {
    float prod = abs(dot(a, b));
    if (prod > 1)
        return 0;
    return acos(prod);
}

float projAngle (vec4 center, vec4 a, vec4 b) {
    a = normalize(a - center * dot(center, a));
    b = normalize(b - center * dot(center, b));
    float prod = dot(a, b);
    if (abs(prod) > 1)
        return 0;
    return acos(prod);
}

float poligonRadCoef (float alpha) {
    float phi = 2 * pi / vertexNumber;
    while (alpha > phi)
        alpha -= phi;
    while (alpha < 0)
        alpha += phi;
    float c1 = cos(phi), s1 = sin(phi);
    float c2 = cos(alpha), s2 = sin(alpha);
    return s1 / (s2 + c2*s1 - s2*c1);
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

float intersection (vec4 look, int faceIndex) {
    intersectionPoint = look * (faceIM[faceIndex] * playerP).w - playerP * (faceIM[faceIndex] * look).w;
    if (length(intersectionPoint) == 0)
        return 2;
    vec4 p = intersectionPoint * gauss(mat4(faceStart[faceIndex] - faceCenter[faceIndex], facePoint[faceIndex] - faceCenter[faceIndex], intersectionPoint, faceCenter[faceIndex]));
    if (length(p) == 0)
        return 2;
    float poligonRad = faceRad[faceIndex] * poligonRadCoef(angle(faceCenter[faceIndex], p, faceStart[faceIndex]));
    intersectionPoint = normalize(intersectionPoint);
    return distance(p, faceCenter[faceIndex]) / poligonRad;
}

void main () {
    vec2 normCoord = (gl_FragCoord.xy - hr) / length(hr);
    float polarRad = length(normCoord);
    vec2 polarTrig = normCoord / polarRad;
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
