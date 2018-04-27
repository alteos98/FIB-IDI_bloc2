#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  scale = 1.0f;
  rotate = 0.0f;
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);
  
  m.load("/home/alteos98/Documents/UPC/IDI/models/Patricio.obj");

  calculCaixaEscena();
  calculCaixaPatricio();
  
  iniCamera();

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffers();
}

void MyGLWidget::paintGL () 
{
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  calculCamera();

  // PATRICIO
  modelTransformPatricio();
  glBindVertexArray (VAO_Patricio);
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size() * 3);
  
  modelTransformPatricio2();
  glBindVertexArray (VAO_Patricio);
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size() * 3);
  
  modelTransformPatricio3();
  glBindVertexArray (VAO_Patricio);
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size() * 3);

  // TERRA
  modelTransformTerra();
  glBindVertexArray(VAO_Terra);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindVertexArray (0);
}

void MyGLWidget::resizeGL (int w, int h) 
{
  glViewport(0, 0, w, h);
  float raV = float (w) / float (h);
  ra = raV;
}

void MyGLWidget::createBuffers () 
{
  glm::vec3 Vertex_Terra[6];
  Vertex_Terra[0] = glm::vec3 (-2.5, 0, -2.5);
  Vertex_Terra[1] = glm::vec3 (-2.5, 0, 2.5);
  Vertex_Terra[2] = glm::vec3 (2.5, 0, -2.5);
  Vertex_Terra[3] = glm::vec3 (2.5, 0, -2.5);
  Vertex_Terra[4] = glm::vec3 (-2.5, 0, 2.5);
  Vertex_Terra[5] = glm::vec3 (2.5, 0, 2.5);

  glm::vec3 Colors_Terra[6];
  Colors_Terra[0] = glm::vec3 (1.0, 0.0, 0.0);
  Colors_Terra[1] = glm::vec3 (1.0, 0.0, 0.0);
  Colors_Terra[2] = glm::vec3 (1.0, 0.0, 0.0);
  Colors_Terra[3] = glm::vec3 (1.0, 0.0, 0.0);
  Colors_Terra[4] = glm::vec3 (1.0, 0.0, 0.0);
  Colors_Terra[5] = glm::vec3 (1.0, 0.0, 0.0);


  /*// VAO_Homer
  glGenVertexArrays(1, &VAO_Homer);
  glBindVertexArray(VAO_Homer);

  GLuint VBO_Homer[2];
  glGenBuffers(2, VBO_Homer);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m.faces().size() * 3 * 3, m.VBO_vertices(), GL_STATIC_DRAW);

  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m.faces().size() * 3 * 3, m.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);*/


  // VAO_Patricio
  glGenVertexArrays(1, &VAO_Patricio);
  glBindVertexArray(VAO_Patricio);

  GLuint VBO_Patricio[2];
  glGenBuffers(2, VBO_Patricio);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Patricio[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m.faces().size() * 3 * 3, m.VBO_vertices(), GL_STATIC_DRAW);

  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Patricio[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m.faces().size() * 3 * 3, m.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);


  // VAO_Terra
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  GLuint VBO_Terra[2];
  glGenBuffers(2, VBO_Terra);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_Terra), Vertex_Terra, GL_STATIC_DRAW);

  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors_Terra), Colors_Terra, GL_STATIC_DRAW);

  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);


  glBindVertexArray (0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/fragshad.frag");
  vs.compileSourceFile("shaders/vertshad.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  // Obtenim identificador per a l'atribut “color” del vertex shader
  colorLoc = glGetAttribLocation (program->programId(), "color");
  // Uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");
  projLoc = glGetUniformLocation (program->programId(), "proj");
  viewLoc = glGetUniformLocation (program->programId(), "view");
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_S: { // escalar a més gran
      scale += 0.05;
      break;
    }
    case Qt::Key_D: { // escalar a més petit
      scale -= 0.05;
      break;
    }
    case Qt::Key_W: { // rotar dreta
      rotate += (float)M_PI/4;
      break;
    }
    case Qt::Key_Q: { // rotar esquerra
      rotate -= (float)M_PI/4;
      break;
    }
    case Qt::Key_Z: { // zoom-in (FOV)
      FOV -= (float)M_PI/10;
      if (FOV <= 0) FOV += (float)M_PI/10;
      break;
    }
    case Qt::Key_X: { // zoom-out (FOV)
      FOV += (float)M_PI/10;
      if (FOV >= M_PI) FOV -= (float)M_PI/10;
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e) {
  newX = e->x();
  newY = e->y();

  makeCurrent();

  float difX = newX - oldX;
  float difY = newY - oldY;

  rotX += (difX/width())*10;
  rotY += (difY/height())*10;

  viewTransform();
  update();

  oldX = newX;
  oldY = newY;
}


/* ================== */
/* LES MEVES FUNCIONS */
/* ================== */

void MyGLWidget::modelTransformTerra() 
{
  // Matriu de transformació del terra
  glm::mat4 transform_terra (1.0f);
  transform_terra = glm::scale(transform_terra, glm::vec3(scale));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform_terra[0][0]);
}

void MyGLWidget::modelTransformPatricio() 
{
	int alturaFinal = 1; //altura desitjada del patricio
	
	// Matriu de transformació del Patricio
	glm::mat4 transform_Patricio (1.0f);
	transform_Patricio = glm::rotate(transform_Patricio, rotate, glm::vec3(0,1,0));
	
	transform_Patricio = glm::translate(transform_Patricio, glm::vec3(2,(alturaModel/2)*(alturaFinal/alturaModel),2));
	transform_Patricio = glm::scale(transform_Patricio, glm::vec3(alturaFinal/alturaModel));
	transform_Patricio = glm::translate(transform_Patricio, -centrePatricio);
	glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform_Patricio[0][0]);
}

void MyGLWidget::modelTransformPatricio2() 
{
	int alturaFinal = 1; //altura desitjada del patricio
  // Matriu de transformació del Patricio
  glm::mat4 transform_Patricio (1.0f);
  transform_Patricio = glm::rotate(transform_Patricio, rotate, glm::vec3(0,1,0));

  transform_Patricio = glm::translate(transform_Patricio, glm::vec3(0,(alturaModel/2)*(alturaFinal/alturaModel),0));
  transform_Patricio = glm::rotate(transform_Patricio, (float)M_PI/2, glm::vec3(0,1,0));
  transform_Patricio = glm::scale(transform_Patricio, glm::vec3(alturaFinal/alturaModel));
  transform_Patricio = glm::translate(transform_Patricio, -centrePatricio);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform_Patricio[0][0]);
}

void MyGLWidget::modelTransformPatricio3() 
{
	int alturaFinal = 1; //altura desitjada del patricio
  // Matriu de transformació del Patricio
  glm::mat4 transform_Patricio (1.0f);
  transform_Patricio = glm::rotate(transform_Patricio, rotate, glm::vec3(0,1,0));

  transform_Patricio = glm::translate(transform_Patricio, glm::vec3(0-2,(alturaModel/2)*(alturaFinal/alturaModel),-2));
  transform_Patricio = glm::rotate(transform_Patricio, (float)M_PI, glm::vec3(0,1,0));
  transform_Patricio = glm::scale(transform_Patricio, glm::vec3(alturaFinal/alturaModel));
  transform_Patricio = glm::translate(transform_Patricio, -centrePatricio);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform_Patricio[0][0]);
}

void MyGLWidget::projectTransform() {
	glm::mat4 Proj = glm::perspective(FOV, ra, zN, zF);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform() {

  glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,-(radi_esfera*2)));
  View = glm::translate(View, -VRP);
  View= glm::rotate(View,rotX,glm::vec3(0,1,0));
  View = glm::rotate(View,rotY,glm::vec3(1,0,0));

  //glm::mat4 View = glm::lookAt(OBS, VRP, UP);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::iniCamera() {
	FOV = 2 * asin(radi_esfera/d);
	FOVi = FOV;
	zN = d - radi_esfera;
	zF = d + radi_esfera;
	
	OBS = centreEsfera + d * glm::vec3 (0,1,1);
	VRP = centreEsfera;
	UP = glm::vec3(0,1,0);
	
  	ra = 1.0;

	projectTransform();
	viewTransform();
}

void MyGLWidget::calculCamera() {
	projectTransform();
	viewTransform();
}

void MyGLWidget::calculRadiCentre() {
  centreEsfera = glm::vec3 ((Pmin.x+Pmax.x)/2,(Pmin.y+Pmax.y)/2,(Pmin.z+Pmax.z)/2);
  radi_esfera = sqrt((Pmax.x - Pmin.x)*(Pmax.x - Pmin.x) + (Pmax.y - Pmin.y)*(Pmax.y - Pmin.y) + (Pmax.z - Pmin.z)*(Pmax.z - Pmin.z)) / 2;
  d = 2 * radi_esfera;
}

void MyGLWidget::calculCaixaEscena() {
	Pmin = glm::vec3(-2.5,0,-2.5);
	Pmax = glm::vec3(2.5,1,2.5);
	
	calculRadiCentre();
}

void MyGLWidget::calculCaixaPatricio() {
  PminPat = glm::vec3(m.vertices()[0], m.vertices()[1], m.vertices()[2]);
  PmaxPat = glm::vec3(m.vertices()[0], m.vertices()[1], m.vertices()[2]);

  for (unsigned int i = 0; i < m.vertices().size(); i+=3) {
	if (m.vertices()[i] < PminPat.x) PminPat.x = m.vertices()[i];
    if (m.vertices()[i] > PmaxPat.x) PmaxPat.x = m.vertices()[i];
    if (m.vertices()[i+1] < PminPat.y) PminPat.y = m.vertices()[i+1];
    if (m.vertices()[i+1] > PmaxPat.y) PmaxPat.y = m.vertices()[i+1];
    if (m.vertices()[i+2] < PminPat.z) PminPat.z = m.vertices()[i+2];
    if (m.vertices()[i+2] > PmaxPat.z) PmaxPat.z = m.vertices()[i+2];
  }

	centrePatricio = glm::vec3 ((PminPat.x+PmaxPat.x)/2,(PminPat.y+PmaxPat.y)/2,(PminPat.z+PmaxPat.z)/2);
	centreBasePatricio = glm::vec3 (centrePatricio.x, centrePatricio.y - alturaModel/2, centrePatricio.z);
	alturaModel = PmaxPat.y - PminPat.y;
}
