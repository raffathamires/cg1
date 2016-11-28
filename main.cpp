#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <GL/glut.h>

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

// Variᶥis gerais
GLint   lvl  = 1;
GLint   lvlRecorde = 1;
GLint   score  = 0;
GLint   scoreRecorde = 0;
GLint   speed = 200;
GLbyte  gameOver = true;
GLbyte  menu = true;

// Variᶥis do gato
GLint   catX        = 30;
GLint   catZ        = 30;
GLfloat catRotation = 90.0f;
GLbyte  direction   = 0;

// Variᶥis do teapot
GLint   teapotX = 0;
GLint   teapotZ = 0;

// Variᶥis da esfera
GLint   esferaX = -1000;
GLint   esferaZ = -1000;

//Variᶥis do ⮧ulo da c⭥ra
GLfloat angleX = 30.0f;
GLfloat angleY = 0.0f;
GLfloat zoom   = -980.0f;

//Inicializar ilumina磯
void Initialize(void)
{
    glEnable(GL_DEPTH_TEST); // Habilita o depth-buffering
    glEnable(GL_COLOR_MATERIAL); //Habilita cor
    glEnable(GL_LIGHTING); //Habilita luz
    glEnable(GL_LIGHT0); //Habilita luz #0
    glEnable(GL_LIGHT1); //Habilita luz #1
    glEnable(GL_NORMALIZE); //Normaliza normais

    //Ativa o uso da luz ambiente
    GLfloat luzAmbiente[4] = {0.2f, 0.2f, 0.2f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

    //Ativa o uso da reflex㯠difusa
    GLfloat luzDifusa[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat posicaoLuz[4] = {0.0f, 30.0f, 120.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);
}

//Rotaciona c⭥ra no eixo x e y
void rotaciona()
{
    glRotatef(angleX,1.0f,0.0f,0.0f);
    glRotatef(angleY,0.0f,1.0f,0.0f);
}

//Escrever na tela
void write(char *string)
{
    while(*string)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string++);
}

//Placar do jogo
void board()
{
    char level[20];
    char sScore[20];

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(30.0f, 0.0f);

    sprintf(level, "Level %d", lvl);
    write(level);

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(30.0f, -10.0f);

    sprintf(sScore, "Score %d", score);
    write(sScore);
}

//Desenha a mesa
void drawTable()
{
    //Desenha a tᢵa da mesa
    glPushMatrix();
    rotaciona();
    glColor3f(0.5f, 0.35f, 0.05f);
    glTranslatef(75.0f, -16.0f, 75.0f);
    glScalef(100.0f,2.0f,100.0f);
    glutSolidCube(1);
    glPopMatrix();

    //Desenha as pernas da mesa
    glPushMatrix();
    rotaciona();
    glColor3f(0.5f, 0.35f, 0.05f);
    glTranslatef(30.0f,-87.0f,30.0f);
    glScalef(1.0f,1.0f,1.0f);
    glRotatef(-90.0f,1.0f,0.0f,0.0f);
    glutSolidCylinder(5.0f,70.0f,20.0f,10.0f);
    glPopMatrix();

    glPushMatrix();
    rotaciona();
    glColor3f(0.5f, 0.35f, 0.05f);
    glTranslatef(120.0f,-87.0f,30.0f);
    glScalef(1.0f,1.0f,1.0f);
    glRotatef(-90.0f,1.0f,0.0f,0.0f);
    glutSolidCylinder(5.0f,70.0f,20.0f,10.0f);
    glPopMatrix();

    glPushMatrix();
    rotaciona();
    glColor3f(0.5f, 0.35f, 0.05f);
    glTranslatef(120.0f,-87.0f,120.0f);
    glScalef(1.0f,1.0f,1.0f);
    glRotatef(-90,1.0,0,0);
    glutSolidCylinder(5.0f,70.0f,20.0f,10.0f);
    glPopMatrix();

    glPushMatrix();
    rotaciona();
    glColor3f(0.5f, 0.35f, 0.05f);
    glTranslatef(30.0f,-87.0f,120.0f);
    glScalef(1.0f,1.0f,1.0f);
    glRotatef(-90.0f,1.0f,0.0f,0.0f);
    glutSolidCylinder(5.0f,70.0f,20.0f,10.0f);
    glPopMatrix();
}

//Desenha o gato
void drawCat()
{
    //Push
    glPushMatrix();

    //Gato
    rotaciona();
    glColor3f(0.5,0.5,0.4); //Cor do gato
    glTranslatef(catX,-11.0,catZ); //Posicao inicial do gato
    glScalef(0.5,0.5,0.5); //Escala (Cabe硠do gato)
    glutSolidSphere(8,16,16); //Esfera (Cabe硠do gato)
    glRotatef(catRotation, 0.0, 1.0, 0.0); //Rotacao do gato
    //Fucinho
    glColor3f(0.5,0.5,0.4); //Cor do fucinho
    glTranslatef(0,0.0,4.0); //Transla磯 do fucinho
    glScalef(0.9,0.8,0.8); //Escala (Fucinho do gato)
    glutSolidCone(8,9,16,16); //Cone (Fucinho do gato)
    //Olhos
    glColor3f(1,1,1); //Cor dos olhos do gato
    glTranslatef(-4.2,5.0,2.0); //Transla磯 do olho direito
    glScalef(0.3,0.3,0.3); //Escala dos olhos do gato
    glutSolidSphere(8,16,16); //Esfera (Olho direito do gato)
    glTranslatef(26.0,0.0,0.0); //Transla磯 do olho esquerdo
    glutSolidSphere(8,16,16); //Esfera (Olho esquerdo do gato)
    //Orelhas
    glColor3f(0.5,0.5,0.4); //Cor das orelhas
    glScalef(1.5,1.5,1.5); //Escala (Orelhas do gato)
    glRotatef(-80.0,1.0,0.0,0.0); //Rotacao da orelha esquerda do gato
    glRotatef(30,0.0,1.0,0.0); //Rotacao da orelha esquerda do gato
    glTranslatef(0.0,15.0,4.0); //Transla磯 da orelha esquerda do gato
    glutSolidCone(8,15,16,16); //Cone (Orelha esquerda do gato)
    glRotatef(-57.0,0.0,1.0,0.0); //Rotacao da orelha direita do gato
    glTranslatef(-18.0,0.0,9.0); //Transla磯 da orelha direita do gato
    glutSolidCone(8,15,16,16); //Cone (Orelha direita do gato)
    //Esfera nos Olhos
    glColor3f(0,0,0); //Cor dos olhos do gato
    glTranslatef(15.5,-18.0,-9.5); //Transla磯 do olho direito
    glScalef(0.3,0.3,0.3); //Escala dos olhos do gato
    glutSolidSphere(8,16,16); //Esfera (Olho direito do gato)
    glRotatef(80.0,1.0,0.0,0.0); //Rotacao da orelha direita do gato
    glRotatef(5.0,0.0,1.0,0.0); //Rotacao da orelha direita do gato
    glTranslatef(-52.0,26.0,0.5); //Transla磯 do olho esquerdo
    glutSolidSphere(8,16,16); //Esfera (Olho esquerdo do gato)

    //Pop
    glPopMatrix();
}

//Desenha um teapot
void drawTeapot()
{
    glPushMatrix();
    rotaciona();
    glTranslatef(teapotX,-11.0,teapotZ);
    glColor3f(1, 1, 0);
    glScalef(0.5,0.5,0.5);
    glutSolidTeapot(5);
    glPopMatrix();
}

//Desenha uma esfera
void drawSphere()
{
    glPushMatrix();
    rotaciona();
    glTranslatef(esferaX,-11.0,esferaZ);
    glColor3f(0.5, 0.1, 0.1);
    glScalef(0.5,0.5,0.5);
    glutSolidSphere(7,12,12);
    glPopMatrix();
}

//Gerar rand��o
int random(int up, int low)
{
    return (rand() % (up-low))+low;
}

//Gera novo TeaPot baseado no cᬣulo de rand��o
void newTeaPot()
{
    time_t seconds;
    time(&seconds);
    srand((unsigned int) seconds);
    teapotX = random(95, 40);
    teapotZ = random(95, 40);
}

//Gera nova esfera baseada no cᬣulo de rand��o
void newSphere()
{
    time_t seconds;
    time(&seconds);
    srand((unsigned int) seconds*13);
    esferaX = random(95, 40);
    esferaZ = random(95, 40);
}

//Ocorre quando um TeaPot ultrapassar a borda da mesa, quebrando ele
void breakTeapot()
{
    PlaySound("break.wav", NULL, SND_ASYNC|SND_FILENAME);
    score = score + 100;
    if(score%500 == 0)
    {
        PlaySound("SuperMarioBros_Mushroom.wav", NULL, SND_ASYNC|SND_FILENAME);
        lvl++;
        if (speed==50) speed-=10;
        else speed-=50;
        if (speed<2) speed = 1;
        newSphere();
    }
    newTeaPot();
}

//Chamado quando gato estiver empurrando TeaPot
void pushTeapot()
{
    switch(direction)
    {
    case RIGHT:
        teapotX += 5;
        if(teapotX > 125) breakTeapot();
        break;
    case LEFT:
        teapotX -= 5;
        if(teapotX < 25) breakTeapot();
        break;
    case UP:
        teapotZ += 5;
        if(teapotZ > 125) breakTeapot();
        break;
    case DOWN:
        teapotZ -= 5;
        if(teapotZ < 25) breakTeapot();
        break;
    }
}

//Reinicia o jogo
void restart()
{
    PlaySound("meow.wav", NULL, SND_ASYNC|SND_FILENAME);

    gameOver = true;

    if(lvl > lvlRecorde)
    {
        lvlRecorde = lvl;
    }

    if(score > scoreRecorde)
    {
        scoreRecorde = score;
    }

    catX = 30;
    catZ = 30;
    direction  = 0;
    angleX = 30.0f ;
    angleY = 0.0f ;
    catRotation = 90.0f;
    score = 0;
    speed = 200;
    lvl = 1;

    newTeaPot();
    esferaX = -1000;
    esferaZ = -1000;
}

//Trata o movimento do gato
void moveCat(int value)
{
    int i;

    switch(direction)
    {
    case RIGHT:
        catRotation =90;
        catX += 5;
        if(catX > 125) restart();
        break;
    case LEFT:
        catRotation =-90;
        catX -= 5;
        if(catX < 25) restart();
        break;
    case UP:
        catRotation =0;
        catZ += 5;
        if(catZ > 125) restart();
        break;
    case DOWN:
        catRotation =180;
        catZ -= 5;
        if(catZ < 25) restart();
        break;
    }

    //Testa colis㯠entre gato e teapot, se houver ent㯠empurra teapot
    if((catX >= teapotX - 6) && (catX <= teapotX + 6) && (catZ >= teapotZ - 6) && (catZ <= teapotZ + 6))
    {
        pushTeapot();
    }

    //Testa colis㯠entre gato e as esferas, se houver ent㯠reinicia o jogo
    if((catX >= esferaX - 5) && (catX <= esferaX + 5) & (catZ >= esferaZ - 5) && (catZ <= esferaZ + 5)) restart();

    //Define a velocidade do gato (speed 頴ratado em milisegundos, logo quanto menor mais rᰩdo chama moveCat)
    glutTimerFunc(speed, moveCat, 0);
}

//Exibe tudo na tela
void Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef (-80, 80, zoom);

    board();
    drawTeapot();
    drawSphere();
    drawCat();
    drawTable();

    if(gameOver && scoreRecorde > 0)
    {
        char tmp_str[40];

        glColor3f(1, 1, 0);
        glRasterPos2f(30, 10);

        sprintf(tmp_str, "Recorde: Level %d - Score %d", lvlRecorde, scoreRecorde);
        write(tmp_str);
    }

    char tmp_menu1[40];
    char tmp_menu2[40];
    char tmp_menu3[40];
    char tmp_menu4[40];
    char tmp_menu5[40];
    char tmp_menu6[40];
    char tmp_menu7[40];
    char tmp_menu8[40];
    char tmp_menu9[40];
    char tmp_menu10[40];
    char tmp_menu11[40];


    glColor3f(1.0, 0.0, 0.0);
    glRasterPos2f(0, 300);
    sprintf(tmp_menu1, "Creepy Cat");
    write(tmp_menu1);

    glColor3f(1, 1, 0);
    glRasterPos2f(-230, 270);
    sprintf(tmp_menu2, "- Objetivo: empurrar o teapot para fora dos limites da mesa");
    write(tmp_menu2);

    glColor3f(1, 1, 0);
    glRasterPos2f(-230, 240);
    sprintf(tmp_menu3, "- Cada teapot derrubado incrementa 100 pontos no score");
    write(tmp_menu3);

    glColor3f(1, 1, 0);
    glRasterPos2f(-230, 210);
    sprintf(tmp_menu4, "- A cada 500 pontos acumulados, o jogador sobe um level");
    write(tmp_menu4);

    glColor3f(1, 1, 0);
    glRasterPos2f(-230, 180);
    sprintf(tmp_menu5, "- A cada level, aumenta a velocidade do gato e podem surgir obstaculos");
    write(tmp_menu5);

    glColor3f(1, 1, 0);
    glRasterPos2f(-230, 150);
    sprintf(tmp_menu6, "- O jogador perde a partida se o gato ultrapassar os limites da mesa");
    write(tmp_menu6);

    glColor3f(1, 1, 0);
    glRasterPos2f(-230, 120);
    sprintf(tmp_menu7, "- Controle do gato pelas teclas direcionais (UP, DOWN, LEFT, RIGHT)");
    write(tmp_menu7);

    glColor3f(1, 1, 0);
    glRasterPos2f(-230, 90);
    sprintf(tmp_menu8, "- Controle da camera pelas teclas numericas (1, 2, 3, 4, 5, 6)");
    write(tmp_menu8);

    glColor3f(1, 1, 0);
    glRasterPos2f(-230, 60);
    sprintf(tmp_menu9, "- Para redefinir a camera pressione < E N T E R >");
    write(tmp_menu9);

    glColor3f(1, 1, 0);
    glRasterPos2f(-230, 30);
    sprintf(tmp_menu10, "- Para sair do jogo pressione < E S C > a qualquer momento");
    write(tmp_menu10);

    glColor3f(1, 1, 1);
    glRasterPos2f(-130, -200);
    sprintf(tmp_menu11, "Pressione < E N T E R >  para iniciar a partida" );
    write(tmp_menu11);

    char tmp_nomes[40];

    glColor3f(1.0, 0.0, 0.0);
    glRasterPos2f(-130, -400);
    sprintf(tmp_nomes, "Criado por: Luan De Nale e Raffaela Monteiro" );
    write(tmp_nomes);

    glutPostRedisplay();
    glutSwapBuffers();
}

//Trata teclas especiais, como direcionais
void special(int key, int catX, int catY)
{
    if(!menu)
    {
        switch(key)
        {
        //Controlar gato
        case GLUT_KEY_RIGHT     :
            gameOver = false;
            if(direction != LEFT)
                direction = RIGHT;
            break;
        case GLUT_KEY_LEFT      :
            gameOver = false;
            if(direction != RIGHT)
                direction = LEFT;
            break;
        case GLUT_KEY_UP        :
            gameOver = false;
            if(direction != UP)
                direction = DOWN;
            break;
        case GLUT_KEY_DOWN      :
            gameOver = false;
            if(direction != DOWN)
                direction = UP;
            break;
        }
    }
}

//Trata teclas normais
void keyboard (unsigned char key, int catX, int catY)
{
    // Sair do jogo
    if(key == char(27))
    {
        exit(0);
    }

    if(key == char(13))
    {
        zoom = -300;
        angleX = 30.0f;
        angleY = 0.0f;
        menu = false;
    }

    if(!menu){
        switch (key)
        {
        //Mudar camera
        case '1':
            angleX -= 10;
            break;
        case '2':
            angleX += 10;
            break;
        case '3':
            angleY -= 10;
            break;
        case '4':
            angleY += 10;
            break;
        case '5':
            if(zoom > -400)
            {
                zoom -= 10;
            }
            break;
        case '6':
            if(zoom < -150)
            {
                zoom += 10;
            }
            break;
        default:
            break;
        }
    }
}

int main(void)
{
    int argc = 0;
    char *argv[] = { (char *)"gl", 0 };

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1000,700);
    glutInitWindowPosition(80,80);
    glutCreateWindow("Creepy Cat");
    glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(Display);
    glViewport(0, 0, 1000, 700);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)1000 / (double)700, 1, 1000.0);

    newTeaPot();
    moveCat(0);

    PlaySound("Psicose.wav", NULL, SND_ASYNC|SND_FILENAME);

    Initialize();
    glutMainLoop();
}


