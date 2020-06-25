

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <vector>
#include <math.h>
#include <limits> 
#include <iostream>   
#include <algorithm>   

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shaders.h"
#define G180 3.14159265358979323846
#define G90  1.57079632679489661923
int flagTecla = -1;
int mouseoldx, mouseoldy ;
float timeOffsetTecla;
int piezaSeleccionada = -1;
GLfloat radY = 0.75;
GLfloat radX = 0.75;
GLfloat rEye = 4.0;
GLfloat radYUp = 0;
GLfloat radXUp = G90;
GLfloat anguloUp = 0;
GLuint vertexshader, fragmentshader, shaderprogram ;
GLuint projectionPos, modelviewPos, objectPos;
glm::mat4 projection, modelview; 
glm::mat4 identity(1.0f);
/***************   BASIC SET UP FOR DRAWING OBJECTS ***********************/
glm::vec3 eye, center, up;
const int numobjects = 21 ; 
const int numperobj  = 3 ; // Vertices, colors, indices
GLuint VAOs[numobjects]; // A VAO for each object
GLuint buffers[numperobj*numobjects] ; // List of buffers for geometric data 
GLuint objects[numobjects] ; // For each object
GLenum PrimType[numobjects] ; // Primitive Type (quads, polygons etc.)
GLsizei NumElems[numobjects] ; // Number of geometric elements

std::vector <glm::mat4> modelviewStack;

// Floor Geometry is specified with a vertex array

enum { ejeX, ejeY, ejeZ };
enum {Vertices, Colors, Elements} ; // For arrays for object 
enum { BLANCO, ROJO, AZUL,NARANJA,VERDE,AMARILLO };
enum {CENTRO, ARISTA, VERTICE,
    ETIQUETA_BL1, ETIQUETA_RO1, ETIQUETA_AZ1, ETIQUETA_NA1, ETIQUETA_VE1, ETIQUETA_AM1,
    ETIQUETA_BL2, ETIQUETA_RO2, ETIQUETA_AZ2, ETIQUETA_NA2, ETIQUETA_VE2, ETIQUETA_AM2,
    ETIQUETA_BL3, ETIQUETA_RO3, ETIQUETA_AZ3, ETIQUETA_NA3, ETIQUETA_VE3, ETIQUETA_AM3} ; // For objects, for the floor





const GLfloat verticesCentro[12][3] = {
  {0.1, 0.1, 0.1}, {0.1, 0.1, -0.1}, {-0.1, 0.1, -0.1}, {-0.1, 0.1, 0.1},
  {0.1, -0.1, 0.1}, {0.1, -0.1, -0.1}, {-0.1, -0.1, -0.1}, {-0.1, -0.1, 0.1},
  //cara superior
  {0.09, 0.105, 0.09}, {0.09, 0.105, -0.09}, {-0.09, 0.105, -0.09}, {-0.09, 0.105, 0.09}
};
const GLfloat coloresCentro[16][3] = {
  {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2},
  {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2},
  {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}
};
const GLubyte indicesCentro[1][54] = { 
    {0,4,1,4,1,5, 1,5,2,5,2,6,  2,6,3,6,3,7,    3,7,0,7,0,4,   4,5,6,4,6,7, 
    0,8,1,8,1,9, 1,9,2,9,2,10, 2,10,3,10,3,11, 3,11,0,11,0,8} 
};

const GLfloat verticesArista[16][3] = {
  {0.1, 0.1, 0.1}, {0.1, 0.1, -0.1}, {-0.1, 0.1, -0.1}, {-0.1, 0.1, 0.1},
  {0.1, -0.1, 0.1}, {0.1, -0.1, -0.1}, {-0.1, -0.1, -0.1}, {-0.1, -0.1, 0.1},
  //cara superior
  {0.09, 0.105, 0.09}, {0.09, 0.105, -0.09}, {-0.09, 0.105, -0.09}, {-0.09, 0.105, 0.09},
  //cara frontal
  {0.09, 0.09, 0.105}, {0.09, -0.09, 0.105}, {-0.09, -0.09, 0.105}, {-0.09, 0.09, 0.105}
};
const GLfloat coloresArista[16][3] = {
  {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2},
  {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2},
  {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2},
  {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}
};
const GLubyte indicesArista[1][72] = {
    {0,4,1,4,1,5, 1,5,2,5,2,6,  2,6,3,6,3,7,   4,5,6,4,6,7,
    0,8,1,8,1,9, 1,9,2,9,2,10, 2,10,3,10,3,11, 3,11,0,11,0,8,
    0,12,4,12,4,13, 4,13,7,13,7,14, 7,14,3,14,3,15, 3,15,0,15,0,12}
};

const GLfloat verticesVertice[20][3] = {
  {0.1, 0.1, 0.1}, {0.1, 0.1, -0.1}, {-0.1, 0.1, -0.1}, {-0.1, 0.1, 0.1},
  {0.1, -0.1, 0.1}, {0.1, -0.1, -0.1}, {-0.1, -0.1, -0.1}, {-0.1, -0.1, 0.1},
  //cara superior
  {0.09, 0.105, 0.09}, {0.09, 0.105, -0.09}, {-0.09, 0.105, -0.09}, {-0.09, 0.105, 0.09},
  //cara frontal
  {0.09, 0.09, 0.105}, {0.09, -0.09, 0.105}, {-0.09, -0.09, 0.105}, {-0.09, 0.09, 0.105},
  //cara lateral
  {0.105, 0.09, 0.09}, {0.105, 0.09, -0.09}, {0.105,-0.09, -0.09}, {0.105, -0.09, 0.09}
};
const GLfloat coloresVertice[20][3] = {
  {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2},
  {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2},
  //cara superior
  {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2},
  //cara frontal
  {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2},
  //cara lateral
  {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}
};
const GLubyte indicesVertice[1][90] = {
    { 1,5,2,5,2,6,  2,6,3,6,3,7,   4,5,6,4,6,7,
    0,8,1,8,1,9, 1,9,2,9,2,10, 2,10,3,10,3,11, 3,11,0,11,0,8,
    0,12,4,12,4,13, 4,13,7,13,7,14, 7,14,3,14,3,15, 3,15,0,15,0,12,
    0,16,1,16,1,17, 1,17,5,17,5,18, 5,18,4,18,4,19, 4,19,0,19,0,16}
};

const GLfloat verticesEtiquetaUp[4][3] = {
  {0.1, 0.1, 0.1}, {0.1, 0.1, -0.1}, {-0.1, 0.1, -0.1}, {-0.1, 0.1, 0.1}
};
const GLfloat verticesEtiquetaFront[4][3] = {
  {0.1, 0.1, 0.1}, {0.1, -0.1, 0.1}, {-0.1, -0.1, 0.1}, {-0.1, 0.1, 0.1}
};
const GLfloat verticesEtiquetaLat[4][3] = {
  {0.1, 0.1, 0.1}, {0.1, 0.1, -0.1}, {0.1, -0.1, -0.1}, {0.1, -0.1, 0.1}
};
//blanco,rojo,azul,naranja,verde,amarillo
const GLfloat coloresEtiquetaBlanco[4][3] = {
  {1.0,1.0,1.0}, {1.0,1.0,1.0}, {1.0,1.0,1.0}, {1.0,1.0,1.0}
};
const GLfloat coloresEtiquetaRojo[4][3] = {
  {0.7255,0.0,0.0}, {0.7255,0.0,0.0}, {0.7255,0.0,0.0}, {0.7255,0.0,0.0}
};
const GLfloat coloresEtiquetaAzul[4][3] = {
  {0.0,0.2745,0.7255}, {0.0,0.2745,0.7255}, {0.0,0.2745,0.7255}, {0.0,0.2745,0.7255}
};
const GLfloat coloresEtiquetaNaranja[4][3] = {
  {1.0,0.3529,0.0}, {1.0,0.3529,0.0}, {1.0,0.3529,0.0}, {1.0,0.3529,0.0}
};
const GLfloat coloresEtiquetaVerde[4][3] = {
  {0.0,0.6078,0.2745}, {0.0,0.6078,0.2745}, {0.0,0.6078,0.2745}, {0.0,0.6078,0.2745}
};
const GLfloat coloresEtiquetaAmarillo[4][3] = {
  {1.0,0.8353,0.0}, {1.0,0.8353,0.0}, {1.0,0.8353,0.0}, {1.0,0.8353,0.0}
};
const GLubyte indicesEtiqueta[1][6] = {
    {0,1,2,0,2,3}
};


void pushMatrix(glm::mat4 mat) {
    modelviewStack.push_back(glm::mat4(mat));
}

// This function pops a matrix from the modelview stack and assigns that to the matrix passed in
void popMatrix(glm::mat4& mat) {
    if (modelviewStack.size()) {
        mat = glm::mat4(modelviewStack.back());
        modelviewStack.pop_back();
    }
    else { // Just to prevent an unwanted error
        mat = glm::mat4(1.0f);
    }
}

struct Piezas {
    short type;//0 centro, 1 arista, 3 vertice
    short color[3];//0 blanco,1 rojo,2 azul,3 naranja,4 verde,5 amarillo
    glm::mat4 transformation;
    Piezas(int = 0) {
        type = -1;
        color[0] = 0;
        color[1] = 0;
        color[2] = 0;
    }
    Piezas(short t,short i,short c1, short c2=0, short c3=0){
        type = t;
        color[0] = c1;
        color[1] = c2;
        color[2] = c3;
        if (t == 0) {
            transformation = glm::translate(identity, glm::vec3(0.0, 0.2, 0.0));
            if (i == 1)
                transformation = glm::rotate(identity, (float)G90, glm::vec3(1.0, 0.0, 0.0)) * transformation;
            else if (i == 2)
                transformation = glm::rotate(identity, (float)G90, glm::vec3(0.0, 0.0, -1.0)) * transformation;
            else if (i == 3)
                transformation = glm::rotate(identity, (float)G90, glm::vec3(0.0, 0.0, 1.0)) * transformation;
            else if(i == 4)
                transformation = glm::rotate(identity, (float)G90, glm::vec3(-1.0, 0.0, 0.0)) * transformation;
            else if(i == 5 )
                transformation = glm::rotate(identity, (float)G180, glm::vec3(0.0, 0.0, 1.0)) * transformation;
        }else if (t == 1) {
            transformation = glm::translate(identity, glm::vec3(0.0, 0.2, 0.2));
            int flag = 1;
            if (i > 7) {
                transformation = glm::rotate(identity, (float)G180, glm::vec3(0.0, 0.0, 1.0)) * transformation;
            }
            else if (i > 3) {
                flag = -1;
                transformation = glm::rotate(identity, (float)G90, glm::vec3(0.0, 0.0, -1.0)) * transformation;
            }
            if(i%4==1)
                transformation = glm::rotate(identity, (float)G90, glm::vec3(0.0, flag*1.0, 0.0)) * transformation;
            else if (i % 4 == 2)
                transformation = glm::rotate(identity, (float)G90, glm::vec3(0.0, flag * -1.0, 0.0)) * transformation;
            else if (i % 4 == 3)
                transformation = glm::rotate(identity, (float)G180, glm::vec3(0.0, 1.0, 0.0)) * transformation;
        }else if (t == 2) {
            transformation = glm::translate(identity, glm::vec3(0.2, 0.2, 0.2));
            if(i>3)
                transformation = glm::rotate(identity, (float)G90, glm::vec3(1.0, 0.0, 0.0)) * transformation;
            if (i % 4 == 1)
                transformation = glm::rotate(identity, (float)G90, glm::vec3(0.0, -1.0, 0.0)) * transformation;
            else if (i % 4 == 2)
                transformation = glm::rotate(identity, (float)G90, glm::vec3(0.0, 1.0, 0.0)) * transformation;
            else if (i % 4 == 3)
                transformation = glm::rotate(identity, (float)G180, glm::vec3(0.0, 1.0, 0.0)) * transformation;
        }
    }
    void draw(){
        if (type == -1)
            return;
        pushMatrix(modelview);
        modelview = modelview * transformation;
        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
        glBindVertexArray(VAOs[type]);
        glDrawElements(PrimType[type], NumElems[type], GL_UNSIGNED_BYTE, 0);

        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
        glBindVertexArray(VAOs[color[0]+3]);
        glDrawElements(PrimType[color[0] + 3], NumElems[color[0] + 3], GL_UNSIGNED_BYTE, 0);
        if (type) {
            glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
            glBindVertexArray(VAOs[color[1] + 9]);
            glDrawElements(PrimType[color[1] + 9], NumElems[color[1] + 9], GL_UNSIGNED_BYTE, 0);
            if (type == 2) {
                glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
                glBindVertexArray(VAOs[color[2] + 15]);
                glDrawElements(PrimType[color[2] + 15], NumElems[color[2] + 15], GL_UNSIGNED_BYTE, 0);
            }
        }
        glBindVertexArray(0);
        popMatrix(modelview);
    }
    void draw(glm::mat4 tAux) {
        if (type == -1)
            return;
        pushMatrix(modelview);
        modelview = modelview * tAux * transformation;
        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
        glBindVertexArray(VAOs[type]);
        glDrawElements(PrimType[type], NumElems[type], GL_UNSIGNED_BYTE, 0);

        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
        glBindVertexArray(VAOs[color[0] + 3]);
        glDrawElements(PrimType[color[0] + 3], NumElems[color[0] + 3], GL_UNSIGNED_BYTE, 0);
        if (type) {
            glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
            glBindVertexArray(VAOs[color[1] + 9]);
            glDrawElements(PrimType[color[1] + 9], NumElems[color[1] + 9], GL_UNSIGNED_BYTE, 0);
            if (type == 2) {
                glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
                glBindVertexArray(VAOs[color[2] + 15]);
                glDrawElements(PrimType[color[2] + 15], NumElems[color[2] + 15], GL_UNSIGNED_BYTE, 0);
            }
        }
        glBindVertexArray(0);
        popMatrix(modelview);
    }
};

struct Cubo {
    typedef bool (Cubo::*fptr)(float);
    Piezas* pieza[3][3][3];
    fptr movimientos[18];
    int ejeAnimacion;
    int iAnimacion;
    float gradoAnimacion;
    int iSeleccionado;
    int jSeleccionado;
    int kSeleccionado;
    int caraSeleccionada;
    Cubo(int i) {
        ejeAnimacion = -1;
        iAnimacion = 0;
        gradoAnimacion = 0.0;
        iSeleccionado =-1;
        jSeleccionado = -1;
        kSeleccionado = -1;
        caraSeleccionada =-1;
        movimientos[0] = &Cubo::U;
        movimientos[1] = &Cubo::E;
        movimientos[2] = &Cubo::D;
        movimientos[3] = &Cubo::F;
        movimientos[4] = &Cubo::S;
        movimientos[5] = &Cubo::B;
        movimientos[6] = &Cubo::R;
        movimientos[7] = &Cubo::M;
        movimientos[8] = &Cubo::L;
        movimientos[9] = &Cubo::Uprima;
        movimientos[10] = &Cubo::Eprima;
        movimientos[11] = &Cubo::Dprima;
        movimientos[12] = &Cubo::Fprima;
        movimientos[13] = &Cubo::Sprima;
        movimientos[14] = &Cubo::Bprima;
        movimientos[15] = &Cubo::Rprima;
        movimientos[16] = &Cubo::Mprima;
        movimientos[17] = &Cubo::Lprima;
        pieza[0][0][0] = new Piezas(VERTICE, 0, BLANCO, ROJO, AZUL);
        pieza[0][0][1] = new Piezas(ARISTA, 0, BLANCO, ROJO);
        pieza[0][0][2] = new Piezas(VERTICE, 1, BLANCO, VERDE, ROJO);
        pieza[0][1][0] = new Piezas(ARISTA, 1, BLANCO, AZUL);
        pieza[0][1][1] = new Piezas(CENTRO, 0, BLANCO);
        pieza[0][1][2] = new Piezas(ARISTA, 2, BLANCO, VERDE);
        pieza[0][2][0] = new Piezas(VERTICE, 2, BLANCO, AZUL, NARANJA);
        pieza[0][2][1] = new Piezas(ARISTA, 3, BLANCO, NARANJA);
        pieza[0][2][2] = new Piezas(VERTICE, 3, BLANCO, NARANJA, VERDE);

        pieza[1][0][0] = new Piezas(ARISTA, 4, AZUL, ROJO);
        pieza[1][0][1] = new Piezas(CENTRO, 1, ROJO);
        pieza[1][0][2] = new Piezas(ARISTA, 5, ROJO, VERDE);
        pieza[1][1][0] = new Piezas(CENTRO, 2, AZUL);
        pieza[1][1][1] = new Piezas(-1);
        pieza[1][1][2] = new Piezas(CENTRO, 3, VERDE);
        pieza[1][2][0] = new Piezas(ARISTA, 6, NARANJA, AZUL);
        pieza[1][2][1] = new Piezas(CENTRO, 4, NARANJA);
        pieza[1][2][2] = new Piezas(ARISTA, 7, VERDE, NARANJA);

        pieza[2][0][0] = new Piezas(VERTICE, 4, ROJO, AMARILLO, AZUL);
        pieza[2][0][1] = new Piezas(ARISTA, 8, AMARILLO, ROJO);
        pieza[2][0][2] = new Piezas(VERTICE, 5, VERDE, AMARILLO, ROJO);
        pieza[2][1][0] = new Piezas(ARISTA, 9, AMARILLO, AZUL);
        pieza[2][1][1] = new Piezas(CENTRO, 5, AMARILLO);
        pieza[2][1][2] = new Piezas(ARISTA, 10, AMARILLO, VERDE);
        pieza[2][2][0] = new Piezas(VERTICE, 6, AZUL, AMARILLO, NARANJA);
        pieza[2][2][1] = new Piezas(ARISTA, 11, AMARILLO, NARANJA);
        pieza[2][2][2] = new Piezas(VERTICE, 7, NARANJA, AMARILLO, VERDE);
    }
    void draw() {
        if (ejeAnimacion == ejeX) {
            //tercera
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    for (int k = 0; k < 3; ++k)
                        if(k==iAnimacion)
                            pieza[i][j][k]->draw(glm::rotate(identity, gradoAnimacion, glm::vec3(1.0, 0.0, 0.0)) );
                        else
                            pieza[i][j][k]->draw();
        }
        else if (ejeAnimacion == ejeY) {
            //primera
            for (int i = 0; i < 3; ++i) 
                if (i == iAnimacion)
                    for (int j = 0; j < 3; ++j)
                        for (int k = 0; k < 3; ++k)
                            pieza[i][j][k]->draw(glm::rotate(identity, gradoAnimacion, glm::vec3(0.0, 1.0, 0.0)));
                else
                    for (int j = 0; j < 3; ++j)
                        for (int k = 0; k < 3; ++k)
                            pieza[i][j][k]->draw();
        }
        else if (ejeAnimacion == ejeZ) {
            //segunda
            for (int i = 0; i < 3; ++i)
                 for (int j = 0; j < 3; ++j)
                    if (j == iAnimacion)
                        for (int k = 0; k < 3; ++k)
                            pieza[i][j][k]->draw(glm::rotate(identity, gradoAnimacion, glm::vec3(0.0, 0.0, 1.0)));
                    else
                        for (int k = 0; k < 3; ++k)
                            pieza[i][j][k]->draw();
        }
        else{
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    for (int k = 0; k < 3; ++k)
                        pieza[i][j][k]->draw();
        }
    }
    void rotar(Piezas*& p1, Piezas*& p2, Piezas*& p3, Piezas*& p4) {
        Piezas* aux = p1;
        p1 = p2;
        p2 = p3;
        p3 = p4;
        p4 = aux;
    }
    bool U(float g) {
        ejeAnimacion = ejeY;
        iAnimacion = 0;
        gradoAnimacion = -g;
        if (g >= G90) {
            ejeAnimacion = -1;
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    pieza[0][i][j]->transformation = glm::rotate(identity, (float)G90, glm::vec3(0.0, -1.0, 0.0)) * pieza[0][i][j]->transformation;
            rotar(pieza[0][0][0], pieza[0][2][0], pieza[0][2][2], pieza[0][0][2]);
            rotar(pieza[0][0][1], pieza[0][1][0], pieza[0][2][1], pieza[0][1][2]);
            return true;
        }
        return false;
    }
    bool Uprima(float g) {
        ejeAnimacion = ejeY;
        iAnimacion = 0;
        gradoAnimacion = g;
        if (g >= G90) {
            ejeAnimacion = -1;
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    pieza[0][i][j]->transformation = glm::rotate(identity, (float)G90, glm::vec3(0.0, 1.0, 0.0)) * pieza[0][i][j]->transformation;
            rotar(pieza[0][0][0], pieza[0][0][2], pieza[0][2][2], pieza[0][2][0]);
            rotar(pieza[0][0][1], pieza[0][1][2], pieza[0][2][1], pieza[0][1][0]);
            return true;
        }
        return false;
    }
    bool E(float g) {
        if (g >= G90) {
            return true;
        }
        return false;
    }
    bool Eprima(float g) {
        if (g >= G90) {
            return true;
        }
        return false;
    }
    bool D(float g) {
        ejeAnimacion = ejeY;
        iAnimacion = 2;
        gradoAnimacion = g;
        if (g >= G90) {
            ejeAnimacion = -1;
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    pieza[2][i][j]->transformation = glm::rotate(identity, (float)G90, glm::vec3(0.0, 1.0, 0.0)) * pieza[2][i][j]->transformation;
            rotar(pieza[2][0][0], pieza[2][0][2], pieza[2][2][2], pieza[2][2][0]);
            rotar(pieza[2][0][1], pieza[2][1][2], pieza[2][2][1], pieza[2][1][0]);
            return true;
        }
        return false;
    }
    bool Dprima(float g) {
        ejeAnimacion = ejeY;
        iAnimacion = 2;
        gradoAnimacion = -g;
        if (g >= G90) {
            ejeAnimacion = -1;
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    pieza[2][i][j]->transformation = glm::rotate(identity, (float)G90, glm::vec3(0.0, -1.0, 0.0)) * pieza[2][i][j]->transformation;
            rotar(pieza[2][0][0], pieza[2][2][0], pieza[2][2][2], pieza[2][0][2]);
            rotar(pieza[2][0][1], pieza[2][1][0], pieza[2][2][1], pieza[2][1][2]);
            return true;
        }
        return false;
    }
    bool F(float g) {
        ejeAnimacion = ejeZ;
        iAnimacion = 0;
        gradoAnimacion = -g;
        if (g >= G90) {
            ejeAnimacion = -1;
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    pieza[i][0][j]->transformation = glm::rotate(identity, (float)G90, glm::vec3(0.0, 0.0, -1.0)) * pieza[i][0][j]->transformation;
            rotar(pieza[0][0][0], pieza[0][0][2], pieza[2][0][2], pieza[2][0][0]);
            rotar(pieza[0][0][1], pieza[1][0][2], pieza[2][0][1], pieza[1][0][0]);
            return true;
        }
        return false;
    }
    bool Fprima(float g) {
        ejeAnimacion = ejeZ;
        iAnimacion = 0;
        gradoAnimacion = g;
        if (g >= G90) {
            ejeAnimacion = -1;
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    pieza[i][0][j]->transformation = glm::rotate(identity, (float)G90, glm::vec3(0.0, 0.0, 1.0)) * pieza[i][0][j]->transformation;
            rotar(pieza[0][0][0], pieza[2][0][0], pieza[2][0][2], pieza[0][0][2]);
            rotar(pieza[0][0][1], pieza[1][0][0], pieza[2][0][1], pieza[1][0][2]);
            return true;
        }
        return false;
    }
    bool S(float g) {
        if (g >= G90) {
            return true;
        }
        return false;
    }
    bool Sprima(float g) {
        if (g >= G90) {
            return true;
        }
        return false;
    }
    bool B(float g) {
        ejeAnimacion = ejeZ;
        iAnimacion = 2;
        gradoAnimacion = g;
        if (g >= G90) {
            ejeAnimacion = -1;
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    pieza[i][2][j]->transformation = glm::rotate(identity, (float)G90, glm::vec3(0.0, 0.0, 1.0)) * pieza[i][2][j]->transformation;
            rotar(pieza[0][2][0], pieza[2][2][0], pieza[2][2][2], pieza[0][2][2]);
            rotar(pieza[0][2][1], pieza[1][2][0], pieza[2][2][1], pieza[1][2][2]);
            return true;
        }
        return false;
    }
    bool Bprima(float g) {
        ejeAnimacion = ejeZ;
        iAnimacion = 2;
        gradoAnimacion = -g;
        if (g >= G90) {
            ejeAnimacion = -1;
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    pieza[i][2][j]->transformation = glm::rotate(identity, (float)G90, glm::vec3(0.0, 0.0, -1.0)) * pieza[i][2][j]->transformation;
            rotar(pieza[0][2][0], pieza[0][2][2], pieza[2][2][2], pieza[2][2][0]);
            rotar(pieza[0][2][1], pieza[1][2][2], pieza[2][2][1], pieza[1][2][0]);
            return true;
        }
        return false;
    }
    bool R(float g) {
        ejeAnimacion = ejeX;
        iAnimacion = 0;
        gradoAnimacion = -g;
        if (g >= G90) {
            ejeAnimacion = -1;
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    pieza[i][j][0]->transformation = glm::rotate(identity, (float)G90, glm::vec3(-1.0, 0.0, 0.0)) * pieza[i][j][0]->transformation;
            rotar(pieza[0][0][0], pieza[2][0][0], pieza[2][2][0], pieza[0][2][0]);
            rotar(pieza[0][1][0], pieza[1][0][0], pieza[2][1][0], pieza[1][2][0]);
            return true;
        }
        return false;
    }
    bool Rprima(float g) {
        ejeAnimacion = ejeX;
        iAnimacion = 0;
        gradoAnimacion = g;
        if (g >= G90) {
            ejeAnimacion = -1;
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    pieza[i][j][0]->transformation = glm::rotate(identity, (float)G90, glm::vec3(1.0, 0.0, 0.0)) * pieza[i][j][0]->transformation;
            rotar(pieza[0][0][0], pieza[0][2][0], pieza[2][2][0], pieza[2][0][0]);
            rotar(pieza[0][1][0], pieza[1][2][0], pieza[2][1][0], pieza[1][0][0]);
            return true;
        }
        return false;
    }
    bool M(float g) {
        if (g >= G90) {
            return true;
        }
        return false;
    }
    bool Mprima(float g) {
        if (g >= G90) {
            return true;
        }
        return false;
    }
    bool L(float g) {
        ejeAnimacion = ejeX;
        iAnimacion = 2;
        gradoAnimacion = g;
        if (g >= G90) {
            ejeAnimacion = -1;
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    pieza[i][j][2]->transformation = glm::rotate(identity, (float)G90, glm::vec3(1.0, 0.0, 0.0)) * pieza[i][j][2]->transformation;
            rotar(pieza[0][0][2], pieza[0][2][2], pieza[2][2][2], pieza[2][0][2]);
            rotar(pieza[0][1][2], pieza[1][2][2], pieza[2][1][2], pieza[1][0][2]);
            return true;
        }
        return false;
    }
    bool Lprima(float g) {
        ejeAnimacion = ejeX;
        iAnimacion = 2;
        gradoAnimacion = -g;
        if (g >= G90) {
            ejeAnimacion = -1;
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    pieza[i][j][2]->transformation = glm::rotate(identity, (float)G90, glm::vec3(-1.0, 0.0, 0.0)) * pieza[i][j][2]->transformation;
            rotar(pieza[0][0][2], pieza[2][0][2], pieza[2][2][2], pieza[0][2][2]);
            rotar(pieza[0][1][2], pieza[1][0][2], pieza[2][1][2], pieza[1][2][2]);
            return true;
        }
        return false;
    }
    bool movimiento(int i, float g) {
        return (this->*movimientos[i])(g);
    }
    void scramble(int n) {
        for (int i = 0; i < n; ++i) {
            (this->*movimientos[rand()%18])(G90+1);
        }
    }
    int seleccionarMejor(glm::vec3 pos, glm::vec3 direccion) {
        float escalarX = (abs(pos.x) - 0.3) / abs(direccion.x);
        float escalarY = (abs(pos.y) - 0.3) / abs(direccion.y);
        float escalarZ = (abs(pos.z) - 0.3) / abs(direccion.z);
        glm::vec3 posMod = pos + escalarX * direccion;
        int seleccionadoInt = -1;
        caraSeleccionada = -1;
        iSeleccionado = -1;
        jSeleccionado = -1;
        kSeleccionado = -1;
        if (posMod.y<0.301 && posMod.y > -0.301 && posMod.z<0.301 && posMod.z > -0.301) {
            caraSeleccionada = ejeX;
            iSeleccionado = (int)((0.301 - posMod.y) / 0.2);
            jSeleccionado = (int)((0.301 - posMod.z) / 0.2);
            if (direccion.x < 0)
                kSeleccionado = 0;
            else
                kSeleccionado = 2;
            seleccionadoInt = kSeleccionado + 9 * iSeleccionado + jSeleccionado * 3;
            return seleccionadoInt;
        }
        posMod = pos + escalarY * direccion;
        if (posMod.x<0.301 && posMod.x > -0.301 && posMod.z<0.301 && posMod.z > -0.301){
            caraSeleccionada = ejeY;
            jSeleccionado = (int)((0.301 - posMod.z) / 0.2);
            kSeleccionado = (int)((0.301 - posMod.x) / 0.2);
            if (direccion.y < 0)
                iSeleccionado = 0;
            else
                iSeleccionado = 2;
            seleccionadoInt = kSeleccionado + 9 * iSeleccionado + jSeleccionado * 3;
            return seleccionadoInt;
        }
        posMod = pos + escalarZ * direccion;
        if (posMod.y<0.301 && posMod.y > -0.301 && posMod.x<0.301 && posMod.x > -0.301){
            caraSeleccionada = ejeZ;
            iSeleccionado = (int)((0.301 - posMod.y) / 0.2);
            kSeleccionado = (int)((0.301 - posMod.x) / 0.2);
            if (direccion.z < 0)
                jSeleccionado = 0;
            else
                jSeleccionado = 2;
            seleccionadoInt = kSeleccionado + 9 * iSeleccionado + jSeleccionado * 3;
            return seleccionadoInt;
        }
        return-1;
    }
    void moverMouse(glm::vec3 direccion) {
        gradoAnimacion = 0;
        if (caraSeleccionada == ejeX) {
            if (abs(direccion.y) > abs(direccion.z) && abs(direccion.y)>0.05) {
                //std::cout << "FSB" << std::endl;
                if (jSeleccionado == 0)
                    (this->*movimientos[3 + jSeleccionado + (9 * !((direccion.y > 0) ^ !kSeleccionado))])(std::min(1.5f, 4 * abs(direccion.y)));
                else
                    (this->*movimientos[3+ jSeleccionado + (9 * ((direccion.y > 0) ^ !kSeleccionado))])(std::min(1.5f, 4 * abs(direccion.y)));
            }
            else if(abs(direccion.z) > 0.05){
                //std::cout << "UED" << std::endl;
                if(iSeleccionado==0)
                    (this->*movimientos[iSeleccionado +(9*((direccion.z>0) ^ !kSeleccionado))])(std::min(1.5f,4*abs(direccion.z)) );
                else
                    (this->*movimientos[iSeleccionado + (9 * !((direccion.z > 0) ^ !kSeleccionado))])(std::min(1.5f, 4 * abs(direccion.z)));
            }
        }
        else if (caraSeleccionada == ejeY) {
            if (abs(direccion.x) > abs(direccion.z)){
                //std::cout << "FSB" << std::endl;
            }
            else {
                //std::cout << "LMR" << std::endl;
            }
        }
        else {
            if (abs(direccion.y) > abs(direccion.x)){
                //std::cout << "LMR" << std::endl;
            }
            else {
                //std::cout << "UED" << std::endl;
                if (iSeleccionado == 0)
                    (this->*movimientos[iSeleccionado + (9 * ((direccion.x > 0) ^ (bool)jSeleccionado) )])(std::min(1.5f, 4 * abs(direccion.x)));
                else
                    (this->*movimientos[iSeleccionado + (9 * !((direccion.x > 0) ^ (bool)jSeleccionado))])(std::min(1.5f, 4 * abs(direccion.x)));
            }
        }
    }
    void terminarGiro() {
        if (ejeAnimacion > -1 && iSeleccionado > -1 && abs(gradoAnimacion) > 0.8f) {
            if(ejeAnimacion==ejeX)
                (this->*movimientos[6+ iAnimacion + (9 * !(gradoAnimacion > 0 ^ iAnimacion==0))])(G90+1);
            else if (ejeAnimacion == ejeY)
                (this->*movimientos[iAnimacion + (9 * !(gradoAnimacion > 0 ^ iAnimacion == 0))])(G90 + 1);
            else
                (this->*movimientos[3+ iAnimacion + (9 * !(gradoAnimacion > 0 ^ iAnimacion == 0))])(G90 + 1);
        }
        ejeAnimacion = -1;
        iSeleccionado = -1;
        jSeleccionado = -1;
        kSeleccionado = -1;
        gradoAnimacion = 0.0;
    }
    ~Cubo() {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                for (int k = 0; k < 3; ++k)
                    delete pieza[i][j][k];
    }
};

Cubo rubik(1);

void mostrarMenu() {
    std::cout << "######################################################################" << std::endl;
    std::cout << "              Sistema de coordenadas de la camara" << std::endl;
    std::cout << "                                      Estudiante: Guido Luis Tapia Ore" << std::endl;
    std::cout << "######################################################################" << std::endl << std::endl;
    std::cout << "Teclas validas ----- Efecto Sobre la camara" << std::endl;
    std::cout << "     1         ----- Permite rotar la posicion de la camara(eye) en" << std::endl;
    std::cout << "                     torno al centro actual, usando el mouse" << std::endl;
    std::cout << "     2         ----- Permite mover la camara (eye y center) usando el " << std::endl;
    std::cout << "                     mouse" << std::endl;
    std::cout << "     3         ----- Apunta la camara hacia la piramide" << std::endl;
    std::cout << "     4         ----- Apunta y sigue con la camara al cubo" << std::endl;
    std::cout << "     5         ----- Apunta y sigue con la camara al octaedro" << std::endl;
    std::cout << "   UP & DOWN   ----- Zoom de camara" << std::endl ;
    std::cout << " LEFT & RIGHT  ----- Rotar la camara en su propio eje (up)" << std::endl << std::endl;
    std::cout << "Uso del mouse  ----- Efecto Sobre la camara" << std::endl;
    std::cout << "clic derecho   ----- Reinicia la posicion de la camara a la inicial" << std::endl;
    std::cout << "clic izquierdo ----- Dependiendo de la tecla presionada antes, permite" << std::endl;
    std::cout << "                     que al mover el cursor se modifique la camara" << std::endl;
    std::cout << "                     *si previamente se presiono la tecla 2 el efecto" << std::endl;
    std::cout << "                      sera de translacion; caso contrario, de rotacion" << std::endl << std::endl << std::endl << std::endl;
}


void deleteBuffers() 
{
	glDeleteVertexArrays(numobjects, VAOs);
	glDeleteBuffers(numperobj*numobjects, buffers);
}


void initobject(GLuint object, GLfloat * vert, GLint sizevert, GLfloat * col, GLint sizecol, GLubyte * inds, GLint sizeind, GLenum type)
{
  int offset = object * numperobj;
  glBindVertexArray(VAOs[object]);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertices+offset]) ; 
  glBufferData(GL_ARRAY_BUFFER, sizevert, vert,GL_STATIC_DRAW);
  // Use layout location 0 for the vertices
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[Colors+offset]) ; 
  glBufferData(GL_ARRAY_BUFFER, sizecol, col,GL_STATIC_DRAW);
  // Use layout location 1 for the colors
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[Elements+offset]) ; 
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeind, inds,GL_STATIC_DRAW);
  PrimType[object] = type;
  NumElems[object] = sizeind;
  // Prevent further modification of this VAO by unbinding it
  glBindVertexArray(0);
}

void drawobject(GLuint object) 
{
  glBindVertexArray(VAOs[object]);
  glDrawElements(PrimType[object], NumElems[object], GL_UNSIGNED_BYTE, 0); 
  glBindVertexArray(0);
}


/****************   BASIC SETUP FOR DRAWING OBJECTS ***********************/
void display(GLFWwindow* window)
{
  /* Clear all pixels  */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  modelview = glm::lookAt(glm::vec3(sin(radY) * cos(radX), cos(radY), sin(radY) * sin(radX)) * rEye, 
      glm::vec3(0.0,0.0,0.0), glm::vec3(sin(anguloUp), cos(anguloUp), 0));
  if (flagTecla >= 0 && rubik.movimiento(flagTecla, (glfwGetTime() - timeOffsetTecla) * 2)) {
      std::cout << flagTecla;
      flagTecla = -1;
  }
  rubik.draw();
}

/* Defines a Mouse callback to zoom in and out */
/* This is done by modifying gluLookAt         */
/* The actual motion is in mousedrag           */
/* mouse simply sets state for mousedrag       */


/* Defines what to do when various mouse */
void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_LEFT)
  {
    if (action == GLFW_RELEASE)
	{
      piezaSeleccionada = -1;
      rubik.terminarGiro();
      // Do Nothing ;
    }
    else if (action == GLFW_PRESS)
	{
      anguloUp = 0.0;
	  double x, y;
      
	  glfwGetCursorPos(window, &x, &y);
      
      float gradoX = ((float)x * 3 / 50)-15.0;
      float gradoY = ((float)y * 3 / 50)-15.0;
      //std::cout << gradoX << ' ' << gradoY << std::endl;
      
      glm::vec3 v(sin(G180 - radY) * cos(radX + G180), cos(G180 - radY), sin(G180 - radY) * sin(radX + G180));
      glm::vec3 u(sin(G90 - radY) * cos(radX + G180), cos(G90 - radY), sin(G90 - radY) * sin(radX + G180));
      glm::vec3 w(cos(radX -G90),0, sin(radX - G90));

      /*std::cout << std::endl << "Vectores direccion" << std::endl;
      std::cout << v.x << ' ' << v.y << ' ' << v.z << ' ' << std::endl;
      std::cout << u.x << ' ' << u.y << ' ' << u.z << ' ' << std::endl;
      std::cout << w.x << ' ' << w.y << ' ' << w.z << ' ' << std::endl;
      std::cout  << "Productos" << std::endl;
      std::cout << glm::dot(v,u) << std::endl;
      std::cout << glm::dot(v, w) << std::endl;
      std::cout << glm::dot(w, u) << std::endl;*/
      gradoY = ((90.0+gradoY)*G180)/180;
      gradoX = (gradoX * G180) / 180;
      piezaSeleccionada=rubik.seleccionarMejor(glm::vec3(sin(radY) * cos(radX)*rEye , cos(radY) * rEye, sin(radY) * sin(radX) * rEye),
          v*sin(gradoY) * cos(gradoX) + u * cos(gradoY) + w* sin(gradoY) * sin(gradoX));
      mouseoldx = (int)x ; 
	  mouseoldy = (int)y ; // so we can move wrt x , y 
    }
  }
  else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS )
  { 
	// Reset gluLookAt
    radY = 1.5 ;
    radX = 1.5;
    anguloUp = 0.0;
    center.x = 0;
    center.y = 0;
    center.z = 0;
    rEye = 4;
	// Send the updated matrix to the shader
	glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);
  }
}

/* Defines what to do when various mousedrag */
void mousedrag_callback(GLFWwindow* window, double x, double y)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		int yloc = (int)y - mouseoldy;	// We will use the y coord to zoom in/out
        int xloc = (int)x - mouseoldx;
        if (piezaSeleccionada == -1) {
            radX += 0.005f * xloc;
            radY -= 0.005f * yloc;		// Where do we look from
            radY = std::max(std::min(radY, 3.14f), 0.001f);
            mouseoldy = (int)y;
            mouseoldx = (int)x;
        }
        else {
            glm::vec3 v(sin(G180 - radY) * cos(radX + G180), cos(G180 - radY), sin(G180 - radY) * sin(radX + G180));
            glm::vec3 u(sin(G90 - radY) * cos(radX + G180), cos(G90 - radY), sin(G90 - radY) * sin(radX + G180));
            glm::vec3 w(cos(radX - G90), 0, sin(radX - G90));
            float gradoY = G90 + (float)(y- mouseoldy) * G180 / 3000;
            float gradoX = (float)(x - mouseoldx) * G180 / 3000;
            v= glm::vec3(sin(radY) * cos(radX) * rEye, cos(radY) * rEye, sin(radY) * sin(radX) * rEye) +
                (v* sin(gradoY)* cos(gradoX) + u * cos(gradoY) + w * sin(gradoY) * sin(gradoX) )*4.0f;
            //std::cout << gradoX << ' ' << gradoY << std::endl;
            //std::cout << v.x << ' '<<v.y << ' ' << v.z << std::endl;
            rubik.moverMouse(v);
        }
        //std::cout << sin(radY) << ' ' << cos(radY) << std::endl;
		//mouseoldy = (int)y;
        //mouseoldx = (int)x;

		// Send the updated matrix over to the shader
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);
	}
}

/* Defines what to do when various keys are pressed */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS )
  {
	  deleteBuffers();
	  glfwSetWindowShouldClose(window, true);
  }
  else if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
      if (flagTecla==-1) {
          timeOffsetTecla = glfwGetTime();
          flagTecla = 0;
      }
  }
  else if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
      if (flagTecla == -1) {
          timeOffsetTecla = glfwGetTime();
          flagTecla = 2;
      }
  }
  else if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
      if (flagTecla == -1) {
          timeOffsetTecla = glfwGetTime();
          flagTecla = 3;
      }
  }
  else if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
      if (flagTecla == -1) {
          timeOffsetTecla = glfwGetTime();
          flagTecla = 5;
      }
  }
  else if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
      if (flagTecla == -1) {
          timeOffsetTecla = glfwGetTime();
          flagTecla = 6;
      }
  }
  else if (key == GLFW_KEY_6 && action == GLFW_PRESS) {
      if (flagTecla == -1) {
          timeOffsetTecla = glfwGetTime();
          flagTecla = 8;
      }
  }/* 
   else if (key == GLFW_KEY_7 && action == GLFW_PRESS) {
      if (flagTecla==-1) {
          timeOffsetTecla = glfwGetTime();
          flagTecla = 7;
      }
  }
  else if (key == GLFW_KEY_8 && action == GLFW_PRESS) {
      if (flagTecla==-1) {
          timeOffsetTecla = glfwGetTime();
          flagTecla = 8;
      }
  }
  else if (key == GLFW_KEY_9 && action == GLFW_PRESS) {
      if (flagTecla==-1) {
          timeOffsetTecla = glfwGetTime();
          flagTecla = 9;
      }
  }*/
  else if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
      rubik.scramble(30);
  }
}

/* Reshapes the window appropriately */
void reshape_callback(GLFWwindow* window, int w, int h)
{
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  
  // Think about the rationale for this choice
  // What would happen if you changed near and far planes? 
  // Note that the field of view takes in a radian angle!
  
  if (h > 0)
  {
	  projection = glm::perspective(30.0f / 180.0f * glm::pi<float>(), (GLfloat)w / (GLfloat)h, 1.0f, 100.0f);
	  glUniformMatrix4fv(projectionPos, 1, GL_FALSE, &projection[0][0]);
  }
 
}

void init (void) 
{
  /* select clearing color 	*/
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  /* initialize viewing values  */
  projection = glm::mat4(1.0f); // The identity matrix
  center.x = 0;
  center.y = 0;
  center.z = 0;
  // Think about this.  Why is the up vector not normalized?
  glm::vec3 pos(sin(radY) * cos(radX), cos(radY), sin(radY) * sin(radX));
  modelview = glm::lookAt(pos * 4.0f, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

  // Now create the buffer objects to be used in the scene later
  // Remember to delete all the VAOs and VBOs that you create when the program terminates!
  glGenVertexArrays(numobjects, VAOs);
  glGenBuffers(numperobj*numobjects, buffers);

  // Initialize the floors
  //initobject(FLOOR, (GLfloat *) floorverts, sizeof(floorverts), (GLfloat *) floorcol, sizeof (floorcol), (GLubyte *) floorinds, sizeof (floorinds), GL_TRIANGLES) ; 
  //initobject(FLOOR2, (GLfloat *) floorverts2, sizeof(floorverts2), (GLfloat *) floorcol2, sizeof (floorcol2), (GLubyte *) floorinds2, sizeof (floorinds2), GL_TRIANGLES) ;
  initobject(CENTRO, (GLfloat*)verticesCentro, sizeof(verticesCentro), (GLfloat*)coloresCentro, sizeof(coloresCentro), (GLubyte*)indicesCentro, sizeof(indicesCentro), GL_TRIANGLES);
  initobject(ARISTA, (GLfloat*)verticesArista, sizeof(verticesArista), (GLfloat*)coloresArista, sizeof(coloresArista), (GLubyte*)indicesArista, sizeof(indicesArista), GL_TRIANGLES);
  initobject(VERTICE, (GLfloat*)verticesVertice, sizeof(verticesVertice), (GLfloat*)coloresVertice, sizeof(coloresVertice), (GLubyte*)indicesVertice, sizeof(indicesVertice), GL_TRIANGLES);
  initobject(ETIQUETA_BL1, (GLfloat*)verticesEtiquetaUp, sizeof(verticesEtiquetaUp), (GLfloat*)coloresEtiquetaBlanco, sizeof(coloresEtiquetaBlanco), (GLubyte*)indicesEtiqueta, sizeof(indicesEtiqueta), GL_TRIANGLES);
  initobject(ETIQUETA_RO1, (GLfloat*)verticesEtiquetaUp, sizeof(verticesEtiquetaUp), (GLfloat*)coloresEtiquetaRojo, sizeof(coloresEtiquetaRojo), (GLubyte*)indicesEtiqueta, sizeof(indicesEtiqueta), GL_TRIANGLES);
  initobject(ETIQUETA_AZ1, (GLfloat*)verticesEtiquetaUp, sizeof(verticesEtiquetaUp), (GLfloat*)coloresEtiquetaAzul, sizeof(coloresEtiquetaAzul), (GLubyte*)indicesEtiqueta, sizeof(indicesEtiqueta), GL_TRIANGLES);
  initobject(ETIQUETA_NA1, (GLfloat*)verticesEtiquetaUp, sizeof(verticesEtiquetaUp), (GLfloat*)coloresEtiquetaNaranja, sizeof(coloresEtiquetaNaranja), (GLubyte*)indicesEtiqueta, sizeof(indicesEtiqueta), GL_TRIANGLES);
  initobject(ETIQUETA_VE1, (GLfloat*)verticesEtiquetaUp, sizeof(verticesEtiquetaUp), (GLfloat*)coloresEtiquetaVerde, sizeof(coloresEtiquetaVerde), (GLubyte*)indicesEtiqueta, sizeof(indicesEtiqueta), GL_TRIANGLES);
  initobject(ETIQUETA_AM1, (GLfloat*)verticesEtiquetaUp, sizeof(verticesEtiquetaUp), (GLfloat*)coloresEtiquetaAmarillo, sizeof(coloresEtiquetaAmarillo), (GLubyte*)indicesEtiqueta, sizeof(indicesEtiqueta), GL_TRIANGLES);
  
  initobject(ETIQUETA_BL2, (GLfloat*)verticesEtiquetaFront, sizeof(verticesEtiquetaFront), (GLfloat*)coloresEtiquetaBlanco, sizeof(coloresEtiquetaBlanco), (GLubyte*)indicesEtiqueta, sizeof(indicesEtiqueta), GL_TRIANGLES);
  initobject(ETIQUETA_RO2, (GLfloat*)verticesEtiquetaFront, sizeof(verticesEtiquetaFront), (GLfloat*)coloresEtiquetaRojo, sizeof(coloresEtiquetaRojo), (GLubyte*)indicesEtiqueta, sizeof(indicesEtiqueta), GL_TRIANGLES);
  initobject(ETIQUETA_AZ2, (GLfloat*)verticesEtiquetaFront, sizeof(verticesEtiquetaFront), (GLfloat*)coloresEtiquetaAzul, sizeof(coloresEtiquetaAzul), (GLubyte*)indicesEtiqueta, sizeof(indicesEtiqueta), GL_TRIANGLES);
  initobject(ETIQUETA_NA2, (GLfloat*)verticesEtiquetaFront, sizeof(verticesEtiquetaFront), (GLfloat*)coloresEtiquetaNaranja, sizeof(coloresEtiquetaNaranja), (GLubyte*)indicesEtiqueta, sizeof(indicesEtiqueta), GL_TRIANGLES);
  initobject(ETIQUETA_VE2, (GLfloat*)verticesEtiquetaFront, sizeof(verticesEtiquetaFront), (GLfloat*)coloresEtiquetaVerde, sizeof(coloresEtiquetaVerde), (GLubyte*)indicesEtiqueta, sizeof(indicesEtiqueta), GL_TRIANGLES);
  initobject(ETIQUETA_AM2, (GLfloat*)verticesEtiquetaFront, sizeof(verticesEtiquetaFront), (GLfloat*)coloresEtiquetaAmarillo, sizeof(coloresEtiquetaAmarillo), (GLubyte*)indicesEtiqueta, sizeof(indicesEtiqueta), GL_TRIANGLES);
  
  initobject(ETIQUETA_BL3, (GLfloat*)verticesEtiquetaLat, sizeof(verticesEtiquetaLat), (GLfloat*)coloresEtiquetaBlanco, sizeof(coloresEtiquetaBlanco), (GLubyte*)indicesEtiqueta, sizeof(indicesEtiqueta), GL_TRIANGLES);
  initobject(ETIQUETA_RO3, (GLfloat*)verticesEtiquetaLat, sizeof(verticesEtiquetaLat), (GLfloat*)coloresEtiquetaRojo, sizeof(coloresEtiquetaRojo), (GLubyte*)indicesEtiqueta, sizeof(indicesEtiqueta), GL_TRIANGLES);
  initobject(ETIQUETA_AZ3, (GLfloat*)verticesEtiquetaLat, sizeof(verticesEtiquetaLat), (GLfloat*)coloresEtiquetaAzul, sizeof(coloresEtiquetaAzul), (GLubyte*)indicesEtiqueta, sizeof(indicesEtiqueta), GL_TRIANGLES);
  initobject(ETIQUETA_NA3, (GLfloat*)verticesEtiquetaLat, sizeof(verticesEtiquetaLat), (GLfloat*)coloresEtiquetaNaranja, sizeof(coloresEtiquetaNaranja), (GLubyte*)indicesEtiqueta, sizeof(indicesEtiqueta), GL_TRIANGLES);
  initobject(ETIQUETA_VE3, (GLfloat*)verticesEtiquetaLat, sizeof(verticesEtiquetaLat), (GLfloat*)coloresEtiquetaVerde, sizeof(coloresEtiquetaVerde), (GLubyte*)indicesEtiqueta, sizeof(indicesEtiqueta), GL_TRIANGLES);
  initobject(ETIQUETA_AM3, (GLfloat*)verticesEtiquetaLat, sizeof(verticesEtiquetaLat), (GLfloat*)coloresEtiquetaAmarillo, sizeof(coloresEtiquetaAmarillo), (GLubyte*)indicesEtiqueta, sizeof(indicesEtiqueta), GL_TRIANGLES);

  std::string vertex_shader_path;
  std::string fragment_shader_path;

#ifdef __unix__         
  vertex_shader_path = "//home//manuel//Documents//Projects//OpenGL//GLFW_GLAD_GLUT_GLEW_cmake_project//src//mytest1_glut//shaders//nop.vert";
  fragment_shader_path = "//home//manuel//Documents//Projects//OpenGL//GLFW_GLAD_GLUT_GLEW_cmake_project//src//mytest1_glut//shaders//nop.frag";

#elif defined(_WIN32) || defined(WIN32) 
  vertex_shader_path = "D://UCSP 2020//Compu Grafica//OpenGLRoot//plantilla Manuel//GLFW_GLAD_GLUT_GLEW_cmake_project//src//mytest1_glfw//shaders//nop.vert";
  fragment_shader_path = "D://UCSP 2020//Compu Grafica//OpenGLRoot//plantilla Manuel//GLFW_GLAD_GLUT_GLEW_cmake_project//src//mytest1_glfw//shaders//nop.frag";
#endif


  // Initialize the shader program
  vertexshader = initshaders(GL_VERTEX_SHADER, vertex_shader_path.c_str());
  fragmentshader = initshaders(GL_FRAGMENT_SHADER, fragment_shader_path.c_str());

  shaderprogram = initprogram(vertexshader, fragmentshader) ;

  // Get the positions of the uniform variables
  projectionPos = glGetUniformLocation(shaderprogram, "projection");
  modelviewPos = glGetUniformLocation(shaderprogram, "modelview");
  // Pass the projection and modelview matrices to the shader
  glUniformMatrix4fv(projectionPos, 1, GL_FALSE, &projection[0][0]);
  glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);

}

int main(int argc, char** argv)
{
    mostrarMenu();
  // Requests the type of buffers (Single, RGB).
  // Think about what buffers you would need...

  glfwInit(); //glutInit(&argc, argv);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // glfw window creation
  // --------------------
  GLFWwindow* window = glfwCreateWindow(500, 500, "Simple Demo with Shaders", NULL, NULL);
  
  if (window == NULL)
  {
	  std::cout << "Failed to create GLFW window" << std::endl;
	  glfwTerminate();
	  return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetWindowPos(window, 100, 100);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
	  std::cout << "Failed to initialize GLAD" << std::endl;
	  return -1;
  }

  init (); // Always initialize first

  glfwSetFramebufferSizeCallback(window, reshape_callback);	//glutReshapeFunc(reshape) ;
  glfwSetKeyCallback(window, key_callback);					//glutKeyboardFunc(keyboard);
  glfwSetMouseButtonCallback(window, mouse_callback);		//glutMouseFunc(mouse) ;
  glfwSetCursorPosCallback(window, mousedrag_callback);		//glutMotionFunc(mousedrag) ;

  // First scene render
  reshape_callback(window, 500, 500);
  display(window); //glutDisplayFunc(display); 
  

  // render loop
  // Start the main code
  while (!glfwWindowShouldClose(window)) //glutMainLoop(); 
  {
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	  display(window); //glutDisplayFunc(display); 

	  // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	  // -------------------------------------------------------------------------------
	  glfwSwapBuffers(window);
	  glfwPollEvents();

	  //glFlush();
  }
  
  deleteBuffers(); // Program termination. Delete the buffers generated in init().
  
  glfwDestroyWindow(window);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  
  return 0;   /* ANSI C requires main to return int. */

}
