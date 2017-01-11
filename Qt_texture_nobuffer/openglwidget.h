#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QMouseEvent>
#include <QKeyEvent>

class OpenglWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit OpenglWidget(QWidget *parent = 0);
    ~OpenglWidget();

protected:
    // Opengl Default Drawer, mouse and key event
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    QSize sizeHint() const;

    void initShaders();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);

protected:


private:
    void makeObject();


    QPoint _lastMousePosition;

    // shader and data
    QOpenGLShaderProgram _shaderProgram;
    QOpenGLBuffer _vbo;
    QColor clearColor;
    QPoint lastPos;
    int xRot;
    int yRot;
    int zRot;
    QOpenGLTexture *textures[6];
    QOpenGLTexture *textures_one;
    QOpenGLShaderProgram *program;

    QVector<QVector3D> vertices;
    QVector<QVector2D> textureCoordinates;

    // Transformation matrix
    QMatrix4x4 _T;

    // camera motion
    QMatrix4x4 _pMatrix;
    QMatrix4x4 _mMatrix;
    QMatrix4x4 _vMatrix;

    // for lookat
    QVector3D _camPosition;
    QVector3D _camFront;
    QVector3D _camUpDirection;

    QVector3D _rot;

signals:

public slots:
};

#endif // OPENGLWIDGET_H
