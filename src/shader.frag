#version 460

const int MAX_FACES = 32;
const int MAX_VERT = 10;
const float pi = acos(-1);
const float lambda = 2;

out vec4 fragColor;

uniform vec2 hr;
uniform vec4 playerP, playerR, playerU, playerF;// position, right, up, forward

uniform int   faceNumber;
uniform vec4  [MAX_FACES] faceColor;
uniform int   [MAX_FACES] faceVertNum;
uniform vec4  [MAX_FACES][MAX_VERT] faceVert;

vec4 intersectionPoint;
bool signal = false;

float projDist (vec4 a, vec4 b) {
    float prod = abs(dot(a, b));
    if (prod > 1) {
        return 0;
    }
    return acos(prod);
}

float angle (vec4 center, vec4 a, vec4 b) {
    a -= center;
    if (length(a) == 0) {
        return 0;
    }
    a = normalize(a);
    b -= center;
    if (length(b) == 0) {
        return 0;
    }
    b = normalize(b);
    float prod = dot(a, b);
    if (abs(prod) > 1) {
        return 0;
    }
    return acos(prod);
}

bool is_inside (vec4 left, vec4 test, vec4 right) {
    float a = projDist(left, test);
    float b = projDist(test, right);
    float c = projDist(left, right);
    float ab = abs(a + b - c);
    float ac = abs(a - b + c);
    float bc = abs(-a + b + c);
    float lp = abs(a + b + c - pi);
    if (ab > .01 && ac > .01 && bc > .01 && lp > .01) {
        if (lp < .01) {
            fragColor = vec4(1, 1, 0, 1);
        } else {
            fragColor = vec4(1, 0, 0, 1);
        }
//        fragColor = vec4(abs(a + b + c - pi), abs(a + b - c), 0, 1);
        signal = true;
        return true;
    }
    return ab < 1e-3; //ab < ac && ab < bc && ab < lp;
}

bool intersection (vec4 look, int faceIndex) {
    mat4 m = mat4(faceVert[faceIndex][0], faceVert[faceIndex][1], faceVert[faceIndex][2], playerP);
    if (determinant(m) == 0) {
        return false;
    }
    vec4 sol = inverse(m) * look;
    vec4 p = look - playerP * sol.w;
    if (length(p) == 0) {
        return false;
    }
    intersectionPoint = normalize(p);
    vec4 linesPoint;
    for (int i = 1; i < faceVertNum[faceIndex] - 1; ++i) {
        m = mat4(faceVert[faceIndex][0], faceVert[faceIndex][i], faceVert[faceIndex][i+1], playerP);
        if (determinant(m) == 0) {
            fragColor = vec4(1, 0, 0, 1);
            signal = true;
            return false;
        }
        sol = inverse(m) * intersectionPoint;
        linesPoint = sol.y * faceVert[faceIndex][i] + sol.z * faceVert[faceIndex][i+1];
        if (length(linesPoint) == 0) {
            return false;
        }
        linesPoint = normalize(linesPoint);
        if (is_inside(faceVert[faceIndex][i], linesPoint, faceVert[faceIndex][i+1])) {
            return is_inside(faceVert[faceIndex][0], intersectionPoint, linesPoint);
        }
    }
    return false;
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
    float dMin = 2 * pi, dCur;
    int indexMin;
    if (faceNumber == 0) {
        fragColor = vec4(.5, .2, .7, 1);
        return;
    }
    for (int faceIndex = 0; faceIndex < faceNumber; ++faceIndex) {
        bool intersect = intersection(look, faceIndex);
        if (signal) {
            return;
        }
        if (!intersect) {
            continue;
        }
        dCur = projDist(playerP, intersectionPoint);
        if (dot(playerP, intersectionPoint) * dot(look, intersectionPoint) < 0) {
            dCur = pi - dCur;
        }
        if (dCur < dMin) {
            dMin = dCur;
            indexMin = faceIndex;
        }
    }
    if (dMin < 2 * pi) {
        fragColor = faceColor[indexMin];// - .1 * dMin;
    } else {
        fragColor = vec4(.25, .25, .25, 1);
    }
}
