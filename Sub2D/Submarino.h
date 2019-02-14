#ifndef SUBMARINO_H
#define SUBMARINO_H
#include <GL/freeglut.h>
#include <GL/glut.h>
#include<math.h> 
#include <cstdio>
#include <math.h>
#include <list>
#include <string.h>

#define torpedoHeight 8
//#define numTorpedos 250
#define numTriangulos 50


class Submarino{
    GLfloat 
        velTiro, 
        velUser,
        thetaLeme, 
        thetaSub, 
        thetaHelice, 
        cx, cy, raio,
        colorR, colorG, colorB;
    bool submerso;

    private:
        void desenhaRect( GLfloat height, GLfloat width, GLfloat R, GLfloat G, GLfloat B);
        void desenhaLeme( GLfloat h, GLfloat w, GLfloat theta1, GLfloat B);
        void desenhaCirc(GLfloat x, GLfloat y,GLfloat radius, GLfloat R, GLfloat G, GLfloat B);
        void desenhaElipse(GLfloat r, GLfloat R, GLfloat G, GLfloat B);
        void desenhaTriangulo(GLfloat v2x, GLfloat v2y, GLfloat v3x, GLfloat v3y, GLfloat R, GLfloat G, GLfloat B);
        void desenhaHelices(GLfloat v2x, GLfloat v2y, GLfloat v3x, GLfloat v3y,  GLfloat R, GLfloat G, GLfloat B);
        void desenhaSubmarino(GLfloat x, GLfloat y, GLfloat rUser, GLfloat R, GLfloat G, GLfloat B);

    public:
        Submarino(){
            velTiro=0; 
            velUser=0; 
            thetaLeme=0; 
            thetaSub=0;
            thetaHelice=0; 
            cx=0;
            cy=0;
            raio; 
            colorR=0; colorG=1; colorB=0;
            submerso=false;
        }

        void Desenha(){
            desenhaSubmarino(cx, cy, raio, 0, 0.8, colorB);
            
        }

        void move(GLfloat dx, GLfloat dy);
        void submerge(GLfloat raioMin, GLdouble runtime);
        void emerge(GLfloat raioMax, GLdouble runtime);
        void rodaSub(int w, int s);
        void rodaLeme(int a, int d, GLdouble runtime);
        void desenhaTorpedo(GLfloat x, GLfloat y, GLfloat theta);
        void desenhaCruz(GLfloat x, GLfloat y, GLfloat R, GLfloat G, GLfloat B);
        void desenhaCircPontilhado(GLfloat x, GLfloat y, GLfloat radius, GLfloat R, GLfloat G, GLfloat B);
        void desenhaMira(GLfloat misselX, GLfloat misselY, GLfloat raio);
        void desenhaMissel(GLfloat misselX, GLfloat misselY, GLfloat r, GLfloat theta);
       
        void setmisselX(GLfloat x);
        void setmisselY(GLfloat y);
        void setvelTiro(GLfloat v);
        void setvelUser(GLfloat v);
        void setthetaLeme(GLfloat theta); 
        void setthetaSub(GLfloat theta); 
        void setthetaHelice(GLfloat theta);

        void setcx(GLfloat x); 
        void setcy(GLfloat y);
        void setraio(GLfloat r);
        void setcolorR(GLfloat R); 
        void setcolorG(GLfloat G); 
        void setcolorB(GLfloat B);
        void setsubmerso(bool submerso);

        GLfloat getmisselX();
        GLfloat getmisselY();
        GLfloat getvelTiro();
        GLfloat getvelUser();
        GLfloat getthetaLeme(); 
        GLfloat getthetaSub(); 
        GLfloat getthetaHelice();
        GLfloat getcx(); 
        GLfloat getcy();
        GLfloat getraio();
        GLfloat getcolorR(); 
        GLfloat getcolorG(); 
        GLfloat getcolorB();
        bool getsubmerso();

};


#endif