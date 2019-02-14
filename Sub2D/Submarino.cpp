#include "Submarino.h"

//Getters and Setters
void Submarino::setvelTiro(GLfloat v){
    velTiro = v;
}
void Submarino::setvelUser(GLfloat v){
    velUser = v;
}
void Submarino::setthetaLeme(GLfloat theta){
    thetaLeme = theta;
} 
void Submarino::setthetaSub(GLfloat theta){
    thetaSub = theta;
} 
void Submarino::setthetaHelice(GLfloat theta){
    thetaHelice += theta;
}
void Submarino::setcx(GLfloat x){
    cx = x;
} 
void Submarino::setcy(GLfloat y){
    cy = y;
}
void Submarino::setraio(GLfloat r){
    raio = r;
}
void Submarino::setcolorR(GLfloat R){
    colorR = R;
} 
void Submarino::setcolorG(GLfloat G){
    colorG = G;
} 
void Submarino::setcolorB(GLfloat B){
    colorB = B;
}
void Submarino::setsubmerso(bool sub){
    submerso = sub;
}
GLfloat Submarino::getvelTiro(){
    return velTiro;
}
GLfloat Submarino::getvelUser(){
    return velUser;
}
GLfloat Submarino::getthetaLeme(){
    return thetaLeme;
}
GLfloat Submarino::getthetaSub(){
    return thetaSub;
} 
GLfloat Submarino::getthetaHelice(){
    return thetaHelice;
}
GLfloat Submarino::getcx(){
    return cx;
} 
GLfloat Submarino::getcy(){
    return cy;
}
GLfloat Submarino::getraio(){
    return raio;
}
GLfloat Submarino::getcolorR(){
    return colorR;
} 
GLfloat Submarino::getcolorG(){
    return colorG;
} 
GLfloat Submarino::getcolorB(){
    return colorB;
}
bool Submarino::getsubmerso(){
    return submerso;
}

//private
void Submarino::desenhaRect( GLfloat height, GLfloat width, GLfloat R, GLfloat G, GLfloat B){

	glColor3f(R,G,B);
	glBegin(GL_POLYGON);
		glVertex3f(-width/2.0,0.0,0.0);
		glVertex3f(width/2.0,0.0,0.0);
		glVertex3f(width/2.0,height,0.0);
		glVertex3f(-width/2.0,height,0.0);
	glEnd();

}
void Submarino::desenhaLeme( GLfloat h, GLfloat w, GLfloat theta1, GLfloat B){
	glPushMatrix();

	glRotatef(theta1, 0, 0, 1);
	desenhaRect(h , w, 0.0, 0.0, B/2);

	glPopMatrix();
}
void Submarino::desenhaHelices(GLfloat v2x, GLfloat v2y, GLfloat v3x, GLfloat v3y,  GLfloat R, GLfloat G, GLfloat B){
    glPushMatrix();
    glRotatef(thetaHelice, 0,1,0);
    desenhaTriangulo(v2x, v2y, v3x, v3y, R, G, B);
    desenhaTriangulo(-v2x, v2y, -v3x, v3y, R, G, B);
    glPopMatrix();
}
void Submarino::desenhaCirc(GLfloat x, GLfloat y,GLfloat radius, GLfloat R, GLfloat G, GLfloat B){

    glColor3f(R, G, B);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y); 
        for (int i=0; i<=numTriangulos; i++){
            glVertex2f((radius * cos(i *  (2*M_PI) / numTriangulos)), (radius * sin(i *  (2*M_PI) / numTriangulos)));
        }        
    glEnd();
}
void Submarino::desenhaElipse(GLfloat r, GLfloat R, GLfloat G, GLfloat B){
    
    int i;
    
    glColor3f(R, G, B);
    glBegin(GL_TRIANGLE_FAN);

    for (i = 0; i <= numTriangulos; i++) {
        glVertex2f(((r/2.0 * sin(i * (2.0*M_PI) / numTriangulos))), 2.0   * ((r/2.0 * cos(i *  (2.0*M_PI) / numTriangulos))));
    }

    glEnd();

}
void Submarino::desenhaTriangulo(GLfloat v2x, GLfloat v2y, GLfloat v3x, GLfloat v3y, GLfloat R, GLfloat G, GLfloat B){
    glColor3f(R,G,B);
    glBegin(GL_TRIANGLES);
        glVertex2f(0,0);
        glVertex2f(v2x,v2y);
        glVertex2f(v3x,v3y);
    glEnd();    
}
void Submarino::desenhaSubmarino(GLfloat x, GLfloat y, GLfloat rUser, GLfloat R, GLfloat G, GLfloat B){
    glPushMatrix();
    //desenhaUser();

    glTranslatef(x, y, 0);
    glRotatef(-thetaSub, 0,0,1);

    //Corpo
    desenhaElipse(raio, R, G, B);

    //Buraco torpedo
    glTranslatef(0, rUser-rUser*0.2, 0);
    desenhaCirc(0.0, 0.0, rUser*0.15, 0.0, 0.0, B/2);
    glTranslatef(0, -rUser+rUser*0.2, 0);


    glTranslatef(-x, -y, 0);


    //Asas
    glTranslatef(x-rUser/1.5, y-rUser/3.5, 0);
    desenhaRect(rUser/2.0, rUser/2.5, 0.0, 0.0, B/2);
    glTranslatef(rUser+rUser/3.0, 0, 0);
    desenhaRect(rUser/2.0, rUser/2.5, 0.0, 0.0, B/2);

    //Buraco míssel
    glTranslatef(-rUser/3.0, 0, 0);
    desenhaCirc(0.0, 0.0, rUser*0.15, 1.0, 0.0, B);

    //Hélices
    glTranslatef(-rUser/3.0, -rUser/1.5, 0);
    desenhaHelices(rUser/2, -rUser/10, rUser/2, rUser/4.0, 1,1,B);
   
    //Leme
    desenhaLeme(-rUser/2.0, rUser/5.0, thetaLeme, 0.0);
    

    glPopMatrix();

}

//public
void Submarino::desenhaCruz(GLfloat x, GLfloat y, GLfloat R, GLfloat G, GLfloat B){

    glPushMatrix();
    desenhaRect(10, 2, R,G,B);
    glRotatef(90, 0, 0, 1);
    desenhaRect(10,2,R,G,B);
    glRotatef(90, 0, 0, 1);
    desenhaRect(10,2,R,G,B);
    glRotatef(90, 0, 0, 1);
    desenhaRect(10,2,R,G,B);

    glPopMatrix();

}
void Submarino::desenhaCircPontilhado(GLfloat x, GLfloat y, GLfloat radius, GLfloat R, GLfloat G, GLfloat B){

    //int i;
	//GLfloat x, y;
	glColor3f(R, G, B);
	glPointSize(3);
	glBegin(GL_POINTS);
		for(int i=0; i<360; i+=10){
			x = radius * cos(M_PI*i/180);
			y = radius * sin(M_PI*i/180);
			glVertex3f(x, y, 0);
		}
	glEnd();
}
void Submarino::desenhaMira(GLfloat misselX, GLfloat misselY, GLfloat raio){
    glPushMatrix();
    glTranslatef(misselX, misselY, 0);
    desenhaCircPontilhado(misselX,misselY,raio,0,0,0);
    desenhaCruz(misselX,misselY,0,0,0);
    glPopMatrix();
}
void Submarino::desenhaMissel(GLfloat misselX, GLfloat misselY, GLfloat r, GLfloat theta){
    glPushMatrix();
    glTranslatef(misselX, misselY, 0);
    glRotatef(-theta,0,0,1);
    glTranslatef(raio/3.0, -raio/3.5, 0);
    
    desenhaCirc(0, 0, r, 0, 0, 0);
    glPopMatrix();
}
void Submarino::move(GLfloat dx, GLfloat dy){
    cx += dx;
    cy += dy;
}
void Submarino::submerge(GLfloat raioMin, GLdouble runtime){
    if(raio > (raioMin)){
            raio -= raio*runtime*0.01;
        }else{
            raio = raioMin;
        }

        colorR = 0.0;
        colorG = 1.0;

        if(colorB<1.0){
            colorB+= runtime*0.03;
        }else{
            colorB = 1.0;
        }

        if(raio==raioMin){
            submerso = true;
        }
}
void Submarino::emerge(GLfloat raioMax, GLdouble runtime){
    if(raio < raioMax){
            raio += raio*runtime*0.01;
        }else{
            raio = raioMax;
        }

        colorR = 0.0;
        colorG = 1.0;

        if(colorB>0.0){
            colorB-= runtime*0.03;
        }else{
            colorB = 0.0;
        }

        if(raio==raioMax) {
            submerso = false;
        }
}
void Submarino::rodaSub(int w, int s){
    if(w==1){
        if(thetaLeme>0){
            if(thetaSub<360){
                thetaSub += thetaLeme/60;
            }else{
                thetaSub = 0;
            }
        }else{
            if(thetaSub>-360){
                thetaSub += thetaLeme/60;
            }else{
                thetaSub = 0;
            }
        }
    
    } else if(s==1){
        if(thetaLeme<0){
            if(thetaSub<360){
                thetaSub -= thetaLeme/45;
            }else{
                thetaSub = 0;
            }
        }else{
            if(thetaSub>-360){
                thetaSub -= thetaLeme/45;
            }else{
                thetaSub = 0;
            }
        }
    } 
        
    
}
void Submarino::rodaLeme(int a, int d, GLdouble runtime){
    if(a==1){
        if(thetaLeme>-45){
            thetaLeme -= 1*runtime*0.5;
        }else{
            thetaLeme=-45;
        }
    }
    if(d==1){
        if(thetaLeme<45){
            thetaLeme += 1*runtime*0.5;
        }else{
            thetaLeme=45;
        }
    }
}
void Submarino::desenhaTorpedo(GLfloat x, GLfloat y, GLfloat theta){
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(-theta,0,0,1);
    glTranslatef(0, ((raio/2.0)+torpedoHeight/2), 0);    
    desenhaRect(torpedoHeight, torpedoHeight/2, 0, 0, colorB/2);
    glPopMatrix();
}