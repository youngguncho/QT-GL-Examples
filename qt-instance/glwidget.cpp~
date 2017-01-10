#include "glwindow.h"

#include <QColor>
#include <QMatrix4x4>
#include <QVector>
#include <QVector3D>
#include <QVector4D>

#include <QDebug>


GLWindow::GLWindow(QWindow *parent) 
  : QWindow(parent)
  , _vbo(QOpenGLBuffer::VertexBuffer)
  , _matbo(QOpenGLBuffer::VertexBuffer)
  , _context(0)
{
  setSurfaceType(QWindow::OpenGLSurface);
}

GLWindow::~GLWindow()
{}

void GLWindow::initGL()
{  
  setupShaders();
  _program->bind();
  _positionAttr = _program->attributeLocation("position");
  _colourAttr = _program->attributeLocation("colour");
  _matrixAttr = _program->attributeLocation("matrix");

  QVector<QVector3D> triangles;
  triangles << QVector3D(-0.5, 0.5, 1) << QVector3D(-0.5, -0.5, 1) << QVector3D(0.5, -0.5, 1);
  triangles << QVector3D(0.5, 0.5, 0.5) << QVector3D(-0.5, -0.5, 0.5) << QVector3D(0.5, -0.5, 0.5);

  QVector<QVector3D> colours;
  colours << QVector3D(1, 0, 0) << QVector3D(0, 1, 0) << QVector3D(0, 0, 1);
  colours << QVector3D(1, 1, 1) << QVector3D(1, 1, 1) << QVector3D(1, 1, 1);

  _vao.create();
  _vao.bind();

  _vbo.create();
  _vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
  _vbo.bind();

  size_t positionSize = triangles.size() * sizeof(QVector3D);
  size_t colourSize = colours.size() * sizeof(QVector3D);
  _vbo.allocate(positionSize + colourSize);
  _vbo.bind();
  _vbo.write(0, triangles.constData(), positionSize);
  _vbo.write(positionSize, colours.constData(), colourSize);
  _colourOffset = positionSize;

  _program->setAttributeBuffer(_positionAttr, GL_FLOAT, 0, 3, 0);
  _program->setAttributeBuffer(_colourAttr, GL_FLOAT, _colourOffset, 3, 0);

  _program->enableAttributeArray(_positionAttr);  
  _program->enableAttributeArray(_colourAttr);


  _matbo.create();
  _matbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
  _matbo.bind();

  _matbo.allocate(4 * sizeof(QVector4D));

  // This is completely wrong
  /*_program->setAttributeBuffer(_matrixAttr, GL_FLOAT, 0, 4, 4 * sizeof(QVector4D));
  _program->enableAttributeArray(_matrixAttr);

  _func330->glVertexAttribDivisor(_matrixAttr, 1);
  */      

  // The right way to set up a mat4 attribute for instancing
  for (unsigned i = 0; i < 4; i++)
  {
    _program->setAttributeBuffer(_matrixAttr + i, GL_FLOAT, i * sizeof(QVector4D), 4, 4 * sizeof(QVector4D));
    _program->enableAttributeArray(_matrixAttr + i);
    _func330->glVertexAttribDivisor(_matrixAttr + i, 1);
  }

  _matbo.release();
  _vao.release();

  _program->release();
  resizeGL(width(), height());
}

void GLWindow::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);
}

void GLWindow::paintGL()
{
  if (! _context) // not yet initialized
    return;

  _context->makeCurrent(this);
  QColor background(Qt::black);

  glClearColor(background.redF(), background.greenF(), background.blueF(), 1.0f);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  QMatrix4x4 matrix;
  matrix.perspective(60, 4.0/3.0, 0.1, 100.0);
  matrix.translate(0, 0, -2);

  _program->bind();
  _vao.bind();

  _matbo.bind();
  _matbo.write(0, matrix.constData(), 4 * sizeof(QVector4D));


  glEnable(GL_DEPTH_TEST);
  _func330->glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 1);

  _vao.release();

  _program->release();

  _context->swapBuffers(this);
  _context->doneCurrent();

}

void GLWindow::setupShaders()
{

  QString vShaderSrc("#version 330\n"
    "layout(location = 0) in vec4 position;\n"
    "layout(location = 1) in vec4 colour;\n"
    "layout(location = 2) in mat4 matrix;\n"
    "smooth out vec4 col;\n"
    "void main() {\n"
    "   col = colour;\n"
    "   gl_Position = matrix * position;\n"
    "}\n");

  QString fShaderSrc("#version 330\n"
    "smooth in vec4 col;\n"
    "void main() {\n"
    "   gl_FragColor = col;\n"
    "}\n");


  _program = new QOpenGLShaderProgram(this);
  _program->addShaderFromSourceCode(QOpenGLShader::Vertex, vShaderSrc);
  _program->addShaderFromSourceCode(QOpenGLShader::Fragment, fShaderSrc);
  _program->link();


}

void GLWindow::exposeEvent(QExposeEvent *event)
{
  Q_UNUSED(event);

  if (isExposed())
  {
    if (! _context)
    {
      _context = new QOpenGLContext(this);
      QSurfaceFormat format(requestedFormat());
      format.setVersion(3,3);
      format.setDepthBufferSize(24);

      _context->setFormat(format);
      _context->create();

      _context->makeCurrent(this);
      initializeOpenGLFunctions();

      _func330 = _context->versionFunctions<QOpenGLFunctions_3_3_Core>();
      if (_func330)
        _func330->initializeOpenGLFunctions();
      else
      {
        qWarning() << "Could not obtain required OpenGL context version";
        exit(1);
      }

      initGL();
    }

    paintGL();
  }
}
