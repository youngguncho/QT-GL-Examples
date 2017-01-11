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
#include <QTimer>

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

    QColor clearColor;
    QPoint _lastMousePosition;
    QPoint lastPos;

    // shader and data
    QOpenGLShaderProgram _shaderProgram;
    QOpenGLShaderProgram _lightshaderProgram;

    int xRot;
    int yRot;
    int zRot;
    QOpenGLTexture *textures[6];
    QOpenGLTexture *textures_one;

    QVector<QVector3D> cubeVertices;
    QVector<QVector3D> cubeNormals;
    QVector<QVector2D> cubeTextureCoordinates;
    GLuint cubeTexture;
    QVector<QVector3D> spotlightVertices;
    QVector<QVector3D> spotlightColors;
    double lightAngle;

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
    void timeout();
};

#endif // OPENGLWIDGET_H
