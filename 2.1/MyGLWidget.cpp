#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  scale = 1.0f;
  rotate_homer = 0.0f;
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
  
  m.load("/home/alteos98/Documents/UPC/IDI/models/HomerProves.obj");
  
  // Per saber els vèrtexs de l'objecte
  /*for (unsigned int i = 0; i < m.vertices().size(); i+=3) {
	  std::cout << "(x,y,z) = (" << m.vertices()[i] << ", "
								<< m.vertices()[i+1] << ", "
								<< m.vertices()[i+2] << ")" << std::endl;
  }*/

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffers();
}

void MyGLWidget::paintGL () 
{
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // Activem la càmera
  iniCamera();

  // Carreguem la transformació de model
  modelTransform_Homer();

  // Activem el VAO per a pintar la caseta 
  glBindVertexArray (VAO_Homer);

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size() * 3);

  modelTransform_Terra();
  glBindVertexArray(VAO_Terra);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindVertexArray (0);
}

void MyGLWidget::modelTransform_Homer() 
{
  // Matriu de transformació del Homer
  glm::mat4 transform_Homer (1.0f);
  transform_Homer = glm::rotate(transform_Homer, rotate_homer, glm::vec3(0,1,0));
  transform_Homer = glm::scale(transform_Homer, glm::vec3(scale));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform_Homer[0][0]);
}

void MyGLWidget::modelTransform_Terra() 
{
  // Matriu de transformació del terra
  glm::mat4 transform_terra (1.0f);
  transform_terra = glm::scale(transform_terra, glm::vec3(scale));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform_terra[0][0]);
}

void MyGLWidget::projectTransform() {
	glm::mat4 Proj = glm::perspective((float)M_PI/2.0f, 1.0f, 0.4f, 3.0f);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform() {
	glm::mat4 View = glm::lookAt(glm::vec3(0,0,1), glm::vec3(0,0,0), glm::vec3(0,1,0));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::iniCamera() {
	projectTransform();
	viewTransform();
}

void MyGLWidget::resizeGL (int w, int h) 
{
  glViewport(0, 0, w, h);
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
    case Qt::Key_R: { // rotar
      rotate_homer -= (float)M_PI/4;
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::createBuffers () 
{
  glm::vec3 Vertex_Terra[6];
  Vertex_Terra[0] = glm::vec3 (-1.0, -1.0, -1.0);
  Vertex_Terra[1] = glm::vec3 (-1.0, -1.0, 1.0);
  Vertex_Terra[2] = glm::vec3 (1.0, -1.0, 1.0);
  Vertex_Terra[3] = glm::vec3 (1.0, -1.0, 1.0);
  Vertex_Terra[4] = glm::vec3 (1.0, -1.0, -1.0);
  Vertex_Terra[5] = glm::vec3 (-1.0, -1.0, -1.0);

  glm::vec3 Colors_Terra[6];
  Colors_Terra[0] = glm::vec3 (1.0, 0.0, 0.0);
  Colors_Terra[1] = glm::vec3 (0.0, 1.0, 0.0);
  Colors_Terra[2] = glm::vec3 (0.0, 0.0, 1.0);
  Colors_Terra[3] = glm::vec3 (0.0, 0.0, 1.0);
  Colors_Terra[4] = glm::vec3 (0.0, 1.0, 1.0);
  Colors_Terra[5] = glm::vec3 (1.0, 0.0, 0.0);

  // VAO_Homer
  glGenVertexArrays(1, &VAO_Homer);
  glBindVertexArray(VAO_Homer);

  GLuint VBO_Homer[2];
  glGenBuffers(2, VBO_Homer);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m.faces().size() * 3 * 3, m.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m.faces().size() * 3 * 3, m.VBO_matdiff(), GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // VAO_Terra
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  GLuint VBO_Terra[2];
  glGenBuffers(2, VBO_Terra);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_Terra), Vertex_Terra, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors_Terra), Colors_Terra, GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
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

