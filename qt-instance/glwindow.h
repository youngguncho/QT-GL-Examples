#ifndef GL_WINDOW_H
#define GL_WINDOW_H

#include <QExposeEvent>
#include <QSurfaceFormat>
#include <QWindow>

#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>


class GLWindow : public QWindow, protected QOpenGLFunctions
{
  Q_OBJECT
public:
  GLWindow(QWindow * = 0);
  virtual ~GLWindow();

  void initGL();
  void paintGL();
  void resizeGL(int, int);

protected:
  virtual void exposeEvent(QExposeEvent *);


private:

  void setupShaders();

  QOpenGLBuffer _vbo;
  QOpenGLBuffer _matbo;
  QOpenGLContext *_context;
  QOpenGLShaderProgram *_program;
  QOpenGLVertexArrayObject _vao;
  QOpenGLFunctions_3_3_Core *_func330;

  GLuint _positionAttr;
  GLuint _colourAttr;
  GLuint _matrixAttr;

  size_t _colourOffset;

} ; 

#endif
