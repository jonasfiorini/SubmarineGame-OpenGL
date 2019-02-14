#include "main.h"

int main(int argc, char** argv) {
    
    if (argc > 1){
        strcpy(arg, argv[1]);
        char* file = argv[1];
        strcat(file, "config.xml");
        leXML(file);
        
    }else{
        printf("Erro ao ler o xml! Não foi especificado o nome/localização do arquivo\n\n");
        exit(0);
    }
    //imprimeElementos();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(arena.r*2.0, arena.r*2.0);
    //glutInitWindowPosition(500, 500);
    glutCreateWindow(nome);
    init();
    glutMouseFunc(mouse);
    glutMotionFunc(arrastar);
    glutKeyboardFunc(keyPress);
    glutKeyboardUpFunc(keyUp);
    glutIdleFunc(idle);
    glutDisplayFunc(display);
    
    glutMainLoop();

    return 0;
}

