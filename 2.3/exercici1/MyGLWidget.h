#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "model.h"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    // keyPressEvent - És cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);
    // mouseMoveEvent - És cridat quan clickem el ratolí
    virtual void mouseMoveEvent (QMouseEvent *e);

  private:
    void createBuffers();
    void carregaShaders();
    void modelTransformHomer();
    void modelTransformTerra();
    void modelTransformPatricio();
    void projectTransform();
    void viewTransform();
    void iniCamera();
    void calculCamera();
    void calculCaixaEscena();
    void calculCaixaPatricio();
    void calculRadiCentre();
    
    Model m;

    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniform locations
    GLuint transLoc, projLoc, viewLoc;
    // VAO i VBO names
    GLuint VAO_Homer, VAO_Terra, VAO_Patricio;
    // Program
    QOpenGLShaderProgram *program;
    // Internal vars
    float scale, rotate_homer;
    glm::vec3 pos;

    glm::vec3 Pmin, Pmax, centreEsfera; //punt màxim i mínim de l'escena
    glm::vec3 PminPat, PmaxPat, centrePatricio; //punt màxim i mínim del patricio
    glm::vec3 OBS, VRP, UP;
    float radi_esfera;
    float d; //dist(OBS, VRP)
    float FOV, zN, zF;
    float wi, he;
    float ra;
    float alturaModel;

    float oldX, oldY, newX, newY, rotX, rotY; //coordenades rotació càmera angles d'Euler
};

