#include "tinyxml2.h"
#include "Submarino.h"
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <cstdio>
#include <math.h>
#include <list>
#include <string>
#define numTorpedos 250
#define numMisseis 100

using namespace tinyxml2;

//XML
const char* nome;
const char* tipo;
const char* arquivo;
GLfloat raioUser, 
        tpx[numTorpedos], 
        tpy[numTorpedos], 
        thetaTiro[numTorpedos], 
        misselX[numMisseis], 
        misselY[numMisseis], 
        misselOrigemX[numMisseis], 
        misselOrigemY[numMisseis], 
        miraX, 
        miraY,
        dpx[numMisseis],
        dpy[numMisseis],
        dp[numMisseis],
        theta[numMisseis],
        raio[numMisseis];

int keyStatus[256]; //Armazena as teclas pressionadas e soltas
char svgURL[200], arg[200];

int bt, st;


typedef struct circulos{
    float cx, cy, r; //centro
    int id;
    const char* fill;
    bool status;
} Circulos;


// Variáveis de tempo para animação de submergir/emergir
GLdouble tempo;
GLdouble prtime;
GLdouble timeDiff;
GLdouble runtime;

Circulos arena;
Submarino sub;
std::list<Circulos> elementos;

//Funções
void leXML(char* url);
void leSVG();
void keyPress (unsigned char key, int x, int y);
void keyUp (unsigned char key, int x, int y);
void idle (void);
void display(void);
void init(void);
void mouse(int button, int state, int x, int y);
void arrastar(int x, int y);
bool temColisao(float x, float y);
bool temColisaoSub(float x, float y);
bool temColisaoMissel(int index);
bool temColisaoTorpedo(int index);
void desenhaArena(void);
void desenhaElementos(void);
//void desenhaUser();

void leXML(char* url){

    XMLDocument* doc = new XMLDocument();
    doc->LoadFile( url );
    
    XMLElement* nomeElement = doc->FirstChildElement( "aplicacao" )->FirstChildElement( "arquivoDaArena" )->FirstChildElement( "nome" );
    nome = nomeElement->GetText();

    XMLElement* tipoElement = doc->FirstChildElement( "aplicacao" )->FirstChildElement( "arquivoDaArena" )->FirstChildElement( "tipo" );
    tipo = tipoElement->GetText();

    XMLElement* arquivoElement = doc->FirstChildElement( "aplicacao" )->FirstChildElement( "arquivoDaArena" )->FirstChildElement( "caminho" );
    arquivo = arquivoElement->GetText();

    XMLElement* atributo = doc->FirstChildElement( "aplicacao" )->FirstChildElement( "jogador" );
    const XMLAttribute* tiro = atributo->FindAttribute("velTiro");
    sub.setvelTiro(tiro->FloatValue());
    printf("Velocidade do Tiro: %f\n", sub.getvelTiro());

    const XMLAttribute* velocidade = atributo->FindAttribute("vel");
    sub.setvelUser(velocidade->FloatValue());
    printf("Velocidade do Jogador: %f\n", sub.getvelUser());

    char vet[200];
    char aux[200];

    strcat (svgURL, arg);
    
    if(strcmp(arquivo, "./")!=0){
        strcpy(vet, arquivo);
        if(vet[0] == '.' && vet[1]=='/'){
                        
            for(int i=0; i<strlen(vet)-2; i++){
                aux[i] = vet[i+2];
            }
            
        }
        strcat (svgURL, aux);
    }
    
	strcat (svgURL, nome);
	strcat (svgURL, ".");
	strcat (svgURL, tipo);

    printf("Caminho relativo ao XML do SVG: %s\n", aux);
    printf("Caminho relativo ao main do SVG: %s\n", svgURL);
            
    leSVG();

}
void leSVG(){

        XMLDocument* doc = new XMLDocument();
        doc->LoadFile( svgURL);
        XMLElement* atributo = doc->FirstChildElement("svg")->FirstChildElement("circle");

        for(XMLElement* e = atributo; e != NULL; e = e->NextSiblingElement("circle")) {
                Circulos c;
                const XMLAttribute* fill = e->FindAttribute("fill");
                c.fill = fill->Value();

            if(strcmp(c.fill, "green")!=0){

                if(strcmp(c.fill, "blue")==0){
                    const XMLAttribute* r = e->FindAttribute("r");
                    arena.r = r->FloatValue();
                    c.r = arena.r;

                    const XMLAttribute* cx = e->FindAttribute("cx");
                    arena.cx = cx->FloatValue();
                    c.cx = 0.0;

                    const XMLAttribute* cy = e->FindAttribute("cy");
                    arena.cy = cy->FloatValue();
                    c.cy = 0.0;

                    const XMLAttribute* id = e->FindAttribute("id");
                    c.id = id->IntValue();

                    c.status = true;
                
                }else{
                    const XMLAttribute* cx = e->FindAttribute("cx");                    
                    c.cx = (cx->FloatValue()- arena.cx);
                    
                    const XMLAttribute* cy = e->FindAttribute("cy");
                    c.cy = -(cy->FloatValue())+arena.cy;

                    const XMLAttribute* r = e->FindAttribute("r");
                    c.r = r->FloatValue();

                    const XMLAttribute* id = e->FindAttribute("id");
                    c.id = id->IntValue();

                    c.status = true;
                
                    elementos.push_back(c);
                }
                
            }else{
                const XMLAttribute* cx = e->FindAttribute("cx");
                sub.setcx(cx->FloatValue() - arena.cx);
                
                const XMLAttribute* cy = e->FindAttribute("cy");
                sub.setcy(-cy->FloatValue() + arena.cy);

                const XMLAttribute* r = e->FindAttribute("r");
                sub.setraio(r->FloatValue());
                raioUser = sub.getraio();
                
            }
                
        }                        
        
}
void imprimeElementos(){
    for(std::list<Circulos>::iterator it = elementos.begin(); it != elementos.end(); it++){
            printf("cx: %f\n", it->cx);
            printf("cy: %f\n", it->cy);
            printf("r: %f\n", it->r);
            printf("fill: %s\n", it->fill);
            printf("id: %d\n\n", it->id);
            printf("status: %d\n\n", it->status);
               
    }
    printf("cx: %f\n", arena.cx);
    printf("cy: %f\n", arena.cy);
    printf("r: %f\n", arena.r);
}
void display(void){
    int i=0;
    int j=0;
    glClear(GL_COLOR_BUFFER_BIT);


    desenhaArena();
    sub.Desenha();
        
        
    while(tpx[i]){
        if(!temColisaoTorpedo(i) && tpx[numTorpedos]==0){
            sub.desenhaTorpedo(tpx[i], tpy[i], thetaTiro[i]);
            tpy[i] += sub.getvelTiro()*cos(thetaTiro[i]/60);
            tpx[i] += sub.getvelTiro()*sin(thetaTiro[i]/60);
        }
        i++;    
    }

    desenhaElementos();

    for (j=0; j<numMisseis; j++){
        if(misselOrigemX[j] ){

            sub.desenhaMissel(misselOrigemX[j], misselOrigemY[j], raio[j], theta[j]);
            
        }
    }
    


    if(bt==2 && st==0 && !sub.getsubmerso()){   
        sub.desenhaMira(miraX, miraY, raioUser);

    }

    glutSwapBuffers();
}
void mouse(int button, int state, int x, int y){ 
    int i=0, j=0;
    bt = button;
    st = state;

    if(state==0 && sub.getsubmerso() && button==0){
        while(thetaTiro[i] !=0.0){
            i++;
        }
        thetaTiro[i] = sub.getthetaSub();
        tpx[i] = sub.getcx();
        tpy[i] =  sub.getcy();  

    }else if(!sub.getsubmerso() && button==2 && state==0){

        miraX = -(arena.r-x);
        miraY = arena.r-y;

    }else if(!sub.getsubmerso() && button==2 && state==1){

        for(j=0; j<numMisseis; j++){
            if(misselOrigemX[j]==0 && misselOrigemY[j]==0){
                misselX[j] = miraX; 
                misselY[j] = miraY; 

                misselOrigemX[j] = sub.getcx();
                misselOrigemY[j] = sub.getcy();
                theta[j] = sub.getthetaSub();
                dp[j] = sqrt( pow(misselOrigemX[j] - misselX[j], 2) + pow(misselOrigemY[j] - misselY[j],2) );
                dpy[j] = misselY[j] - misselOrigemY[j];
                dpx[j] = misselX[j] - misselOrigemX[j];
                raio[j] = raioUser/10;
                
                break;
            }
            
        }
        
        
    }
        
    glutPostRedisplay(); 

}
void arrastar(int x, int y){
    if(bt==2 && !sub.getsubmerso()){
        miraX = -(arena.r-x);
        miraY = arena.r-y;
    }
    

    glutPostRedisplay();
}
void idle (void){

    GLfloat velocidadeUser = sub.getvelUser();
    GLfloat thetaS = sub.getthetaSub();
    GLfloat raioMissel = raioUser/10;

    tempo = glutGet(GLUT_ELAPSED_TIME);
    timeDiff = tempo - prtime; 
    prtime = tempo;
    runtime = timeDiff / 10.0;

    for (int j=0; j<numMisseis; j++){
        if(misselOrigemX[j]!=0 || misselOrigemY[j]!=0){
            if(!temColisaoMissel(j)){
                GLfloat dist = sqrt( pow(misselOrigemX[j] - misselX[j] ,2) + pow(misselOrigemY[j] - misselY[j] ,2) );
                if(dist>0){
                    misselOrigemX[j] += sub.getvelTiro()*(dpx[j]/dp[j])*runtime*0.5;
                    misselOrigemY[j] += sub.getvelTiro()*(dpy[j]/dp[j])*runtime*0.5;
                    
                    if((dp[j] - dist) < dp[j]/2.0){
                        //Aumenta
                        if(raio[j]<raioMissel*5){
                            raio[j] += raioMissel*runtime*(dist - dp[j]/2)*0.0001;
                        }else{
                            raio[j] = raioMissel*5.0;
                        }
                    }else{
                        //Diminui
                        if(raio[j]>raioMissel){
                            raio[j] -= raioMissel*runtime*(dp[j]/2 - dist)*0.0001;
                        }else{
                            raio[j] = raioMissel;
                        }

                    }
                    
                }
                

            }
                
        }
    }


    int velHelice=10;
    sub.setthetaHelice(velHelice);

    if((keyStatus[(int)('a')] == 1) ){
        sub.rodaLeme(keyStatus[(int)('a')], 0, runtime);
    }

    if((keyStatus[(int)('d')] == 1)){
        sub.rodaLeme(0, keyStatus[(int)('d')], runtime);
    }
    


    if(keyStatus[(int)('w')] == 1){

        sub.setthetaHelice(20);
        sub.rodaSub(1,0);
        
        
        if(!temColisao(sub.getvelUser()*sin(sub.getthetaSub()/60)*runtime*0.5, sub.getvelUser()*cos(sub.getthetaSub()/60)*runtime*0.5)){
            sub.move(sub.getvelUser()*sin(sub.getthetaSub()/60)*runtime*0.5, sub.getvelUser()*cos(sub.getthetaSub()/60)*runtime*0.5);
        }
        
    }else if (keyStatus[(int)('s')] == 1){
        sub.setthetaHelice(-40);

        sub.rodaSub(0,1);
        
        if(!temColisao(-sub.getvelUser()*sin(sub.getthetaSub()/60)*runtime*0.5, -sub.getvelUser()*cos(sub.getthetaSub()/60)*runtime*0.5)){
            sub.move(-sub.getvelUser()*sin(sub.getthetaSub()/60)*runtime*0.5, -sub.getvelUser()*cos(sub.getthetaSub()/60)*runtime*0.5);
        }
    }
       

    if(keyStatus[(int)('u')] == 1){
        sub.submerge(raioUser/2.0, runtime); 
    }else{
        sub.emerge(raioUser, runtime);
    }


    glutPostRedisplay();
}
void init(void){
    glClearColor(0, 0, 0, 0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-arena.r, arena.r, -arena.r, arena.r, -arena.r, arena.r);
}
void keyPress (unsigned char key, int x, int y){
    switch(key){
        case 'w':
        case 'W':
            keyStatus[(int)('w')] = 1;
            break;
        case 's':
        case 'S':
            keyStatus[(int)('s')] = 1;
            break;
        case 'a':
        case 'A':
            keyStatus[(int)('a')] = 1;
            break;
        case 'd':
        case 'D':
            keyStatus[(int)('d')] = 1;
            break;
        case 'u':
        case 'U':

            if(sub.getsubmerso() && !temColisaoSub(0.0, 0.0)){
                keyStatus[(int)('u')] = 0;    
            }else{
                keyStatus[(int)('u')] = 1;
            }
                        
            break;
    }
    glutPostRedisplay();
}
void keyUp (unsigned char key, int x, int y){
    switch(key){
        case 'w':
        case 'W':
            keyStatus[(int)('w')] = 0;
            break;
        case 's':
        case 'S':
            keyStatus[(int)('s')] = 0;
            break;
        case 'a':
        case 'A':
            keyStatus[(int)('a')] = 0;
            break;
        case 'd':
        case 'D':
            keyStatus[(int)('d')] = 0;
            break;
       
    }
    glutPostRedisplay();

}
bool temColisaoSub(float x, float y){
    float d, da;

    for(std::list<Circulos>::iterator it = elementos.begin(); it != elementos.end(); it++){
        if(it->status){
            d = sqrt( pow(((sub.getcx()+x) - it->cx), 2) + pow(((sub.getcy()+y) - it->cy), 2) );
            if(d<=(raioUser+it->r)){
                return true;
            }
        }
        
    }
    return false;
}
bool temColisaoTorpedo(int index){
    float d, da;
        
    for(std::list<Circulos>::iterator it = elementos.begin(); it != elementos.end(); it++){
        if(it->status){
            d = sqrt( pow((tpx[index]+sub.getvelTiro()*cos(thetaTiro[index]/60) - it->cx), 2) + pow((tpy[index]+sub.getvelTiro()*sin(thetaTiro[index]/60) - it->cy), 2) );
            if(strcmp(it->fill, "black")==0){ //ilha
                if(d<=((torpedoHeight/2)+it->r)){
                    tpx[index] = arena.r*2;
                    tpy[index] = arena.r*2;
                    //it->status = false;
                    return true;
                }
            }else if(strcmp(it->fill, "red")==0){
                    //d = sqrt( pow((tpx[index] - it->cx), 2) + pow((tpy[index] - it->cy), 2) );
                    if(d<=((torpedoHeight/2)+it->r)){
                        tpx[index] = arena.r*2;
                        tpy[index] = arena.r*2;
                        //it->status = false;
                        return true;
                    }
                }   

        }
    }

    
    da = sqrt( pow((tpx[index] - 0.0), 2) + pow((tpy[index] - 0.0), 2) );
    if(da > arena.r+torpedoHeight/2 ){
        return true;
    }
    return false;

}
bool temColisao(float x, float y){
    float d, da;    
    if(!sub.getsubmerso()){
        for(std::list<Circulos>::iterator it = elementos.begin(); it != elementos.end(); it++){
            if(it->status){
                d = sqrt( pow(((sub.getcx()+x) - it->cx), 2) + pow(((sub.getcy()+y) - it->cy), 2) );
                if(d<=(raioUser+it->r)){
                    return true;
                }
            }
            
        }
    }else {
        for(std::list<Circulos>::iterator it = elementos.begin(); it != elementos.end(); it++){
            if(it->status){
                if(strcmp(it->fill, "black")==0){ //ilha
                    d = sqrt( pow(((sub.getcx()+x) - it->cx), 2) + pow(((sub.getcy()+y) - it->cy), 2) );
                    if(d<=(raioUser+it->r)){
                        return true;
                    }
                }
            }
            

        }

    }  

    da = sqrt( pow(((sub.getcx()+x) - 0.0), 2) + pow(((sub.getcy()+y) - 0.0), 2) );
    if((da+raioUser)>arena.r ){
        return true;
    }
    return false;
}
bool temColisaoMissel(int index){
    float d, da;    

    da = sqrt( pow((misselOrigemX[index] - misselX[index]), 2) + pow((misselOrigemY[index] - misselY[index]), 2) );
    
    if(da<=raio[index]){
        for(std::list<Circulos>::iterator it = elementos.begin(); it != elementos.end(); it++){
            if(it->status){
                d = sqrt( pow((misselOrigemX[index] - it->cx), 2) + pow((misselOrigemY[index] - it->cy), 2) );
                if(strcmp(it->fill, "black")==0){ 
                    //d = sqrt( pow((misselOrigemX[index] - it->cx), 2) + pow((misselOrigemY[index] - it->cy), 2) );
                    if(d<=(it->r)){
                        misselX[index] = 0;
                        misselY[index] = 0;
                        misselOrigemX[index] = 0;
                        misselOrigemY[index] = 0;
                        //it->status = false;
                        return true;

                    }
                }

                if(strcmp(it->fill, "red")==0){
                    //d = sqrt( pow((misselOrigemX[index] - it->cx), 2) + pow((misselOrigemY[index] - it->cy), 2) );
                    if(d<=(it->r)){
                        misselX[index] = 0;
                        misselY[index] = 0;
                        misselOrigemX[index] = 0;
                        misselOrigemY[index] = 0;
                        //it->status = false;
                        return true;
                    }
                }   

            }
        }
        misselX[index] = 0;
        misselY[index] = 0;
        misselOrigemX[index] = 0;
        misselOrigemY[index] = 0;
        return true;
    }
    
    return false;
}
void desenhaArena(){
    //Arena
    glColor3f(0, 0, 1.0);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0.0, 0.0); //centrado na origem
        for (int i=0; i<=numTriangulos; i++){
            glVertex2f((arena.r * cos(i *  (2*M_PI) / numTriangulos)), (arena.r * sin(i *  (2*M_PI) / numTriangulos)));
        }        
    glEnd();
}
void desenhaElementos(){
    for(std::list<Circulos>::iterator it = elementos.begin(); it != elementos.end(); it++){
        if(it->status){
            if(strcmp (it->fill,"black") == 0 ){
                glColor3f(0, 0, 0);
                glBegin(GL_TRIANGLE_FAN);
                    glVertex2f(it->cx, it->cy); //centrado na origem
                    for (int i=0; i<=numTriangulos; i++){
                        glVertex2f(it->cx + (it->r * cos(i *  (2*M_PI) / numTriangulos)), it->cy + (it->r * sin(i *  (2*M_PI) / numTriangulos)));
                    }        
                glEnd();
            }

            if(strcmp (it->fill,"red") == 0 ){
                glColor3f(1.0, 0, 0);
                glBegin(GL_TRIANGLE_FAN);
                    glVertex2f(it->cx, it->cy); //centrado na origem
                    for (int i=0; i<=numTriangulos; i++){
                        glVertex2f(it->cx + (it->r * cos(i *  (2*M_PI) / numTriangulos)), it->cy + (it->r * sin(i *  (2*M_PI) / numTriangulos)));
                    }        
                glEnd();
            }
        }
        
    }
}