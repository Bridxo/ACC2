//#version 400 core
////Tessellation Evaluation Shader

//layout(triangles, equal_spacing, ccw) in ;

//in vec3[] vertcoords_es;

//uniform mat4 modelviewmatrix;
//uniform mat4 projectionmatrix;
//uniform mat3 normalmatrix;


//void main() {
//    float u = gl_TessCoord.x;
//    float v = gl_TessCoord.y;
//    float w = gl_TessCoord.z;
//    vec3 F0,F1,F2;

//    F0 = (w * vertcoords_es[10] + v * vertcoords_es[9]) / (v + w);
//    F1 = (u * vertcoords_es[12] + w * vertcoords_es[11]) / (u + w);
//    F2 = (v * vertcoords_es[14] + u * vertcoords_es[13]) / (v + u);

//    if (u==1.0 && v==0.0 && w==0.0) {
//        vec3 position = vertcoords_es[1];
//    } else if (u==0.0 && v==1.0 && w==0.0){
//        vec3 position = vertcoords_es[2];
//    } else if ((u==0.0 && v==0.0 && w==1.0)){
//        vec3 position = vertcoords_es[0];
//    }
//    else{
//        //    // 0  1  2  3   4   5   6   7   8   9  10   11  12  13  14
//        //    // p0 p1 p2 e0+ e0- e1+ e1- e2+ e2- f0+ f0- f1+ f1- f2+ f2-
//        vec3 position = pow(u,3.0) * vertcoords_es[1] + pow(v,3.0) * vertcoords_es[2] + pow(w,3.0) * vertcoords_es[0] +
//                    3.0 * u * v * (u + v) * (u * vertcoords_es[3] + v * vertcoords_es[6]) +
//                    3.0 * v * w * (v + w) * (v * vertcoords_es[5] + v * vertcoords_es[8]) +
//                    3.0 * u * v * (w + u) * (w * vertcoords_es[7] + v * vertcoords_es[4]) +
//                    12.0 * u * v * w * (u * F0 + v * F1 + w * F2);
//    }
//    gl_Position = projectionmatrix * modelviewmatrix * vec4(position, 1.0);
//}

#version 400 core
//Tessellation Evaluation Shader (Triangles)

// 0  1  2  3   4   5   6   7   8   9   10  11  12  13  14
// p0 p1 p2 e0+ e0- e1+ e1- e2+ e2- f0+ f0- f1+ f1- f2+ f2-

layout(triangles, equal_spacing, ccw) in ;

in vec3[] vertcoords_es;

uniform mat4 modelviewmatrix;
uniform mat4 projectionmatrix;
uniform mat3 normalmatrix;

vec3 calcPos(float u, float v, float w) {

    //if u,v,w is in a corner (to avoid zero division)

    if (u==0.0 && v==1.0 && w==0.0){
        return vertcoords_es[0];
    } else if (u==0.0 && v==0.0 && w==1.0){
        return vertcoords_es[1];
    } else if (u==1.0 && v==1.0 && w==0.0){
        return vertcoords_es[2];
    }
    else{

        vec3 F0 = (w * vertcoords_es[10] + v * vertcoords_es[9]) / (v + w);
        vec3 F1 = (u * vertcoords_es[12] + w * vertcoords_es[11]) / (u + w);
        vec3 F2 = (v * vertcoords_es[14] + u * vertcoords_es[13]) / (v + u);

        return pow(u,3.0) * vertcoords_es[0] + pow(v,3.0) * vertcoords_es[1] + pow(w,3.0) * vertcoords_es[2] +
                        3.0 * u * v * (u + v) * (u * vertcoords_es[3] + v * vertcoords_es[6]) +
                        3.0 * v * w * (v + w) * (v * vertcoords_es[5] + v * vertcoords_es[8]) +
                        3.0 * u * v * (w + u) * (w * vertcoords_es[7] + v * vertcoords_es[4]) +
                        12.0 * u * v * w * (u * F0 + v * F1 + w * F2);
    }

}

vec3 calcPos2(float u, float v, float w) {

    //if u,v,w is in a corner (to avoid zero division)

        vec3 F0 = (w * vertcoords_es[10] + v * vertcoords_es[9]) / (v + w);
        vec3 F1 = (u * vertcoords_es[12] + w * vertcoords_es[11]) / (u + w);
        vec3 F2 = (v * vertcoords_es[14] + u * vertcoords_es[13]) / (v + u);

        if (u==1.0 && v==0.0 && w==0.0){
            return vertcoords_es[0];
        }
        else if (u==0.0 && v==1.0 && w==0.0){
            return vertcoords_es[1];
        }
        else if (u==0.0 && v==0.0 && w==1.0){
            return vertcoords_es[2];
        }
        else{
            return pow(u,3.0) * vertcoords_es[0] + pow(v,3.0) * vertcoords_es[1] + pow(w,3.0) * vertcoords_es[2] +
                            3.0 * u * v * (u + v) * (u * vertcoords_es[3] + v * vertcoords_es[6]) +
                            3.0 * v * w * (v + w) * (v * vertcoords_es[5] + w * vertcoords_es[8]) +
                            3.0 * u * w * (w + u) * (w * vertcoords_es[7] + u * vertcoords_es[4]) +
                            12.0 * u * v * w * (u * F0 + v * F1 + w * F2);
        }
}

vec3 calcPos3(float u, float v, float w) {
    return  u * vertcoords_es[0] +
            v * vertcoords_es[1] +
            w * vertcoords_es[2];
}


void main() {


    //vec3 position = calcPos(gl_TessCoord.x, gl_TessCoord.y, gl_TessCoord.z);
    vec3 position = calcPos2(gl_TessCoord.x, gl_TessCoord.y, gl_TessCoord.z);
//    vec3 position = calcPos3(gl_TessCoord.x, gl_TessCoord.y, gl_TessCoord.z);

    gl_Position = projectionmatrix * modelviewmatrix * vec4(position, 1.0);
}
