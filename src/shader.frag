#version 460

const int MAX_VERT = 256;
const int MAX_FACES = 32;
const int MAX_SPHERE = 16;
const float pi = acos(-1);
const float lambda = 2;

out vec4 fragColor;

uniform vec2 hr;
uniform vec4 playerP, playerR, playerU, playerF;// position, right, up, forward

uniform int   faceNumber;
uniform vec4  [MAX_FACES] faceColor;
uniform int   [MAX_FACES] faceVertStart;
uniform int   [MAX_FACES] faceVertNum;
uniform vec4  [MAX_VERT]  Vert;

uniform int   sphereNumber;
uniform vec4  [MAX_SPHERE] sphereColor;
uniform vec4  [MAX_SPHERE] sphereCenter;
uniform float [MAX_SPHERE] sphereRad;

vec4 intersectionPoint, look;
bool signal = false;

// the angle between linear spaces: span(a) and span(b)
float projDist (vec4 a, vec4 b) {
    float prod = abs(dot(a, b));
    if (prod > 1) {
        return 0;
    }
    return acos(prod);
}

// phi in [0, pi] such that: +-dpoint = playerP * cos(phi) + look * sin(phi)
float lookDist (vec4 point) {
    float prod = sign(dot(point, look)) * dot(point, playerP);
    if (prod > 1) {
        return 0;
    } else if (prod < -1) {
        return pi;
    } else {
        return acos(prod);
    }
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
//        return ab < 1e-3;
    return ab < ac && ab < bc && ab < lp;
}

bool intersection (int faceIndex) {
    mat4 m = mat4(Vert[faceVertStart[faceIndex]], Vert[faceVertStart[faceIndex] + 1], Vert[faceVertStart[faceIndex] + 2], playerP);
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
        m = mat4(Vert[faceVertStart[faceIndex]], Vert[faceVertStart[faceIndex] + i], Vert[faceVertStart[faceIndex] + i+1], playerP);
        if (determinant(m) == 0) {
            fragColor = vec4(1, 0, 0, 1);
            signal = true;
            return false;
        }
        sol = inverse(m) * intersectionPoint;
        linesPoint = sol.y * Vert[faceVertStart[faceIndex] + i] + sol.z * Vert[faceVertStart[faceIndex] + i+1];
        if (length(linesPoint) == 0) {
            return false;
        }
        linesPoint = normalize(linesPoint);
        if (is_inside(Vert[faceVertStart[faceIndex] + i], linesPoint, Vert[faceVertStart[faceIndex] + i+1])) {
            return is_inside(Vert[faceVertStart[faceIndex]], intersectionPoint, linesPoint);
        }
    }
    return false;
}

bool sphere_intersection(int sphereIndex) {
    float a = dot(playerP, sphereCenter[sphereIndex]);
    float b = dot(look, sphereCenter[sphereIndex]);
    float c = sqrt(a * a + b * b);
    float d = cos(sphereRad[sphereIndex]) / c;
    if (d > 1) {
        return false;
    }
    float phi1 = asin(d) - atan(a, b);
    vec4 point1 = playerP * cos(phi1) + look * sin(phi1);
    float d1 = lookDist(point1);
    float phi2 = -asin(d) - atan(a, b);
    vec4 point2 = playerP * cos(phi2) + look * sin(phi2);
    float d2 = lookDist(point2);
    if (d1 < d2) {
        intersectionPoint = point1;
    } else {
        intersectionPoint = point2;
    }
    return true;
}

void main() {
    vec2 normCoord = (gl_FragCoord.xy - hr) / length(hr);
    float polarRad = length(normCoord);
    vec2 polarTrig = polarRad == 0 ? vec2(0, 0) : normCoord / polarRad;
    look = playerF * cos(lambda * polarRad) + (polarTrig.x * playerR + polarTrig.y * playerU) * sin(lambda * polarRad);
    if (dot(look, playerF) < 0) {
        fragColor = vec4(0, 0, 0, 1);
        return;
    }
    float dMin = 2 * pi, dCur;
    int indexMin, type = -1;
    for (int faceIndex = 0; faceIndex < faceNumber; ++faceIndex) {
        bool intersect = intersection(faceIndex);
        if (signal) {
            return;
        }
        if (!intersect) {
            continue;
        }
        dCur = lookDist(intersectionPoint);
        if (dCur < dMin) {
            dMin = dCur;
            indexMin = faceIndex;
            type = 1;
        }
    }
    for (int sphereIndex = 0; sphereIndex < sphereNumber; ++sphereIndex) {
        bool intersect = sphere_intersection(sphereIndex);
        if (signal) {
            return;
        }
        if (!intersect) {
            continue;
        }
        dCur = lookDist(intersectionPoint);
        if (dCur < dMin) {
            dMin = dCur;
            indexMin = sphereIndex;
            type = 2;
        }
    }
    if (type == 1) {
        fragColor = faceColor[indexMin] - .3 * dMin;
    } else if (type == 2) {
         fragColor = sphereColor[indexMin] - .3 * dMin;
    } else {
        fragColor = vec4(.25, .25, .25, 1);
    }
}
