#include "openglwidget.h"

OpenglWidget::OpenglWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    memset(textures, 0, sizeof(textures));

    //// Set Initial Gl variables
    _camPosition = QVector3D(0, 0, 10);
    _camFront = QVector3D(0, 0, -1);
    _camUpDirection = QVector3D(0, 1, 0);
    _rot = QVector3D(0, 0, 0);

    _pMatrix.setToIdentity();
    _vMatrix.setToIdentity();
    _mMatrix.setToIdentity();
    setFocusPolicy(Qt::StrongFocus);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer->start(20);

    qDebug() << "Start GL";

}

OpenglWidget::~OpenglWidget()
{

}

void OpenglWidget::initializeGL()
{
    qDebug() << "initial GL";
    initializeOpenGLFunctions();

    // make objects (cube and light model)
    makeObject();

    // initialize shader program
    initShaders();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0, 0, 0, 1);

    qDebug() << "initial GL done";

}


void OpenglWidget::makeObject()
{
    // main cube object
    cubeVertices << QVector3D(-0.5, -0.5,  0.5) << QVector3D( 0.5, -0.5,  0.5) << QVector3D( 0.5,  0.5,  0.5) // Front
                     << QVector3D( 0.5,  0.5,  0.5) << QVector3D(-0.5,  0.5,  0.5) << QVector3D(-0.5, -0.5,  0.5)
                     << QVector3D( 0.5, -0.5, -0.5) << QVector3D(-0.5, -0.5, -0.5) << QVector3D(-0.5,  0.5, -0.5) // Back
                     << QVector3D(-0.5,  0.5, -0.5) << QVector3D( 0.5,  0.5, -0.5) << QVector3D( 0.5, -0.5, -0.5)
                     << QVector3D(-0.5, -0.5, -0.5) << QVector3D(-0.5, -0.5,  0.5) << QVector3D(-0.5,  0.5,  0.5) // Left
                     << QVector3D(-0.5,  0.5,  0.5) << QVector3D(-0.5,  0.5, -0.5) << QVector3D(-0.5, -0.5, -0.5)
                     << QVector3D( 0.5, -0.5,  0.5) << QVector3D( 0.5, -0.5, -0.5) << QVector3D( 0.5,  0.5, -0.5) // Right
                     << QVector3D( 0.5,  0.5, -0.5) << QVector3D( 0.5,  0.5,  0.5) << QVector3D( 0.5, -0.5,  0.5)
                     << QVector3D(-0.5,  0.5,  0.5) << QVector3D( 0.5,  0.5,  0.5) << QVector3D( 0.5,  0.5, -0.5) // Top
                     << QVector3D( 0.5,  0.5, -0.5) << QVector3D(-0.5,  0.5, -0.5) << QVector3D(-0.5,  0.5,  0.5)
                     << QVector3D(-0.5, -0.5, -0.5) << QVector3D( 0.5, -0.5, -0.5) << QVector3D( 0.5, -0.5,  0.5) // Bottom
                     << QVector3D( 0.5, -0.5,  0.5) << QVector3D(-0.5, -0.5,  0.5) << QVector3D(-0.5, -0.5, -0.5);
    cubeNormals << QVector3D( 0,  0,  1) << QVector3D( 0,  0,  1) << QVector3D( 0,  0,  1) // Front
                << QVector3D( 0,  0,  1) << QVector3D( 0,  0,  1) << QVector3D( 0,  0,  1)
                << QVector3D( 0,  0, -1) << QVector3D( 0,  0, -1) << QVector3D( 0,  0, -1) // Back
                << QVector3D( 0,  0, -1) << QVector3D( 0,  0, -1) << QVector3D( 0,  0, -1)
                << QVector3D(-1,  0,  0) << QVector3D(-1,  0,  0) << QVector3D(-1,  0,  0) // Left
                << QVector3D(-1,  0,  0) << QVector3D(-1,  0,  0) << QVector3D(-1,  0,  0)
                << QVector3D( 1,  0,  0) << QVector3D( 1,  0,  0) << QVector3D( 1,  0,  0) // Right
                << QVector3D( 1,  0,  0) << QVector3D( 1,  0,  0) << QVector3D( 1,  0,  0)
                << QVector3D( 0,  1,  0) << QVector3D( 0,  1,  0) << QVector3D( 0,  1,  0) // Top
                << QVector3D( 0,  1,  0) << QVector3D( 0,  1,  0) << QVector3D( 0,  1,  0)
                << QVector3D( 0, -1,  0) << QVector3D( 0, -1,  0) << QVector3D( 0, -1,  0) // Bottom
                << QVector3D( 0, -1,  0) << QVector3D( 0, -1,  0) << QVector3D( 0, -1,  0);
    cubeTextureCoordinates << QVector2D(0, 0) << QVector2D(1, 0) << QVector2D(1, 1) // Front
                           << QVector2D(1, 1) << QVector2D(0, 1) << QVector2D(0, 0)
                           << QVector2D(0, 0) << QVector2D(1, 0) << QVector2D(1, 1) // Back
                           << QVector2D(1, 1) << QVector2D(0, 1) << QVector2D(0, 0)
                           << QVector2D(0, 0) << QVector2D(1, 0) << QVector2D(1, 1) // Left
                           << QVector2D(1, 1) << QVector2D(0, 1) << QVector2D(0, 0)
                           << QVector2D(0, 0) << QVector2D(1, 0) << QVector2D(1, 1) // Right
                           << QVector2D(1, 1) << QVector2D(0, 1) << QVector2D(0, 0)
                           << QVector2D(0, 0) << QVector2D(1, 0) << QVector2D(1, 1) // Top
                           << QVector2D(1, 1) << QVector2D(0, 1) << QVector2D(0, 0)
                           << QVector2D(0, 0) << QVector2D(1, 0) << QVector2D(1, 1) // Bottom
                           << QVector2D(1, 1) << QVector2D(0, 1) << QVector2D(0, 0);

    // light source object
    spotlightVertices << QVector3D(   0,    1,    0) << QVector3D(-0.5,    0,  0.5) << QVector3D( 0.5,    0,  0.5) // Front
                          << QVector3D(   0,    1,    0) << QVector3D( 0.5,    0, -0.5) << QVector3D(-0.5,    0, -0.5) // Back
                          << QVector3D(   0,    1,    0) << QVector3D(-0.5,    0, -0.5) << QVector3D(-0.5,    0,  0.5) // Left
                          << QVector3D(   0,    1,    0) << QVector3D( 0.5,    0,  0.5) << QVector3D( 0.5,    0, -0.5) // Right
                          << QVector3D(-0.5,    0, -0.5) << QVector3D( 0.5,    0, -0.5) << QVector3D( 0.5,    0,  0.5) // Bottom
                          << QVector3D( 0.5,    0,  0.5) << QVector3D(-0.5,    0,  0.5) << QVector3D(-0.5,    0, -0.5);
    spotlightColors << QVector3D(0.2, 0.2, 0.2) << QVector3D(0.2, 0.2, 0.2) << QVector3D(0.2, 0.2, 0.2) // Front
                    << QVector3D(0.2, 0.2, 0.2) << QVector3D(0.2, 0.2, 0.2) << QVector3D(0.2, 0.2, 0.2) // Back
                    << QVector3D(0.2, 0.2, 0.2) << QVector3D(0.2, 0.2, 0.2) << QVector3D(0.2, 0.2, 0.2) // Left
                    << QVector3D(0.2, 0.2, 0.2) << QVector3D(0.2, 0.2, 0.2) << QVector3D(0.2, 0.2, 0.2) // Right
                    << QVector3D(  1,   1,   1) << QVector3D(  1,   1,   1) << QVector3D(  1,   1,   1) // Bottom
                    << QVector3D(  1,   1,   1) << QVector3D(  1,   1,   1) << QVector3D(  1,   1,   1);


    // load texture
    for (int i=0; i<6; ++i)
        textures[i] = new QOpenGLTexture(QImage(QString(":resources/texture.png")).mirrored());

    textures_one =new QOpenGLTexture(QImage(QString(":resources/texture.png")));

}

void OpenglWidget::initShaders()
{
    qDebug() << "init Shader";


    _shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/coloringVertexShader.vsh");
    _shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/coloringFragmentShader.fsh");
    _shaderProgram.link();

    _lightshaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":resources/lightingVertexShader.vsh");
    _lightshaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":resources/lightingFragmentShader.fsh");
    _lightshaderProgram.link();


    qDebug() << "Done shader";

}

void OpenglWidget::resizeGL(int width, int height)
{
    qDebug() << "resizeGL GL";

    // Calculate aspect ratio
    qreal aspect = qreal(width) / qreal(height ? height : 1);

    const qreal zNear = 0.001, zFar = 1000, fov = 60.0;

    // Reset projection
    _pMatrix.setToIdentity();

    // Set perspective projection
    _pMatrix.perspective(fov, aspect, zNear, zFar);

}

void OpenglWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    qDebug() << "Camera Pose " << _camPosition;

    //// prepare camera
    _mMatrix.setToIdentity();
    _vMatrix.setToIdentity();
    _vMatrix.lookAt(_camPosition, _camPosition+_camFront, _camUpDirection);
    _T = _pMatrix * _vMatrix * _mMatrix;

    QMatrix4x4 mvMatrix;
    mvMatrix = _vMatrix * _mMatrix;
    QMatrix3x3 normalMatrix;
    normalMatrix = mvMatrix.normalMatrix();

    QMatrix4x4 lightTransformation;
    lightTransformation.rotate(lightAngle, 0, 1, 0);

    QVector3D lightPosition = lightTransformation * QVector3D(0, 1, 1);

    _lightshaderProgram.bind();
    _lightshaderProgram.setUniformValue("mvpMatrix", _T);
    _lightshaderProgram.setUniformValue("mvMatrix", mvMatrix);
    _lightshaderProgram.setUniformValue("normalMatrix", normalMatrix);
    _lightshaderProgram.setUniformValue("lightPosition", _vMatrix * lightPosition);

    _lightshaderProgram.setUniformValue("ambientColor", QColor(70, 80, 128));
    _lightshaderProgram.setUniformValue("diffuseColor", QColor(128, 128, 128));
    _lightshaderProgram.setUniformValue("specularColor", QColor(255, 255, 255));
    _lightshaderProgram.setUniformValue("ambientReflection", (GLfloat) 1.0);
    _lightshaderProgram.setUniformValue("diffuseReflection", (GLfloat) 1.0);
    _lightshaderProgram.setUniformValue("specularReflection", (GLfloat) 1.0);
    _lightshaderProgram.setUniformValue("shininess", (GLfloat) 100.0);

    _lightshaderProgram.setAttributeArray("vertex", cubeVertices.constData());
    _lightshaderProgram.enableAttributeArray("vertex");
    _lightshaderProgram.setAttributeArray("normal", cubeNormals.constData());
    _lightshaderProgram.enableAttributeArray("normal");
    _lightshaderProgram.setAttributeArray("textureCoordinate", cubeTextureCoordinates.constData());
    _lightshaderProgram.enableAttributeArray("textureCoordinate");

    _lightshaderProgram.setUniformValue("texture", 0);
    textures_one->bind();

    glDrawArrays(GL_TRIANGLES, 0, cubeVertices.size());

    _lightshaderProgram.disableAttributeArray("vertex");
    _lightshaderProgram.disableAttributeArray("normal");
    _lightshaderProgram.disableAttributeArray("textureCoordinate");
    _lightshaderProgram.release();


    _mMatrix.setToIdentity();
    _mMatrix.translate(lightPosition);
    _mMatrix.rotate(lightAngle, 0, 1, 0);
    _mMatrix.rotate(45, 1, 0, 0);
    _mMatrix.scale(0.1);

    _shaderProgram.bind();

    _shaderProgram.setUniformValue("mvpMatrix", _pMatrix * _vMatrix * _mMatrix);

    _shaderProgram.setAttributeArray("vertex", spotlightVertices.constData());
    _shaderProgram.enableAttributeArray("vertex");

    _shaderProgram.setAttributeArray("color", spotlightColors.constData());
    _shaderProgram.enableAttributeArray("color");

    glDrawArrays(GL_TRIANGLES, 0, spotlightVertices.size());

    _shaderProgram.disableAttributeArray("vertex");

    _shaderProgram.disableAttributeArray("color");

    _shaderProgram.release();


// TODO: if uncomment release, texture won't be mappping
//    _shaderProgram.release();


}

QSize OpenglWidget::sizeHint() const
{
    return QSize(640, 480);
}

void OpenglWidget::mousePressEvent(QMouseEvent *event)
{
    _lastMousePosition = event->pos();
    event->accept();
}

void OpenglWidget::mouseMoveEvent(QMouseEvent *event)
{
    /* X->
     *  --------
     *  |
     *  |
     * */
    float deltaX = event->x() - _lastMousePosition.x();
    float deltaY = _lastMousePosition.y() - event->y();
    float sensitivity = 0.1;

    if (event->buttons() & Qt::LeftButton) { // translation motion
        QVector3D left = QVector3D::normal(_camUpDirection, _camFront);
        _camPosition = _camPosition - (_camPosition.z()*0.01*sensitivity)*deltaY*_camUpDirection;
        _camPosition = _camPosition + (_camPosition.z()*0.01*sensitivity)*deltaX*left;
    }
    if (event->buttons() & Qt::RightButton) { // rotation motion
        deltaX *= sensitivity;
        deltaY *= sensitivity;

        QMatrix4x4 rotation;
        rotation.setToIdentity();
        QVector3D right = QVector3D::normal(_camFront, _camUpDirection);

        rotation.rotate(deltaY, right);
        rotation.rotate(-deltaX, 0, 0, 1);
        _camFront = rotation*_camFront;
        _camUpDirection = rotation*_camUpDirection;

    }

    _lastMousePosition = event->pos();
    event->accept();
    update();

}

void OpenglWidget::wheelEvent(QWheelEvent *event)
{
    int delta = (float)event->delta();
    if (event->orientation() == Qt::Vertical) {
        if (delta < 0) {
            _camPosition -= 1.8 * _camFront;
        } else if (delta > 0) {
            _camPosition += 0.8 * _camFront;
        }
        update();
    }

}

void OpenglWidget::keyPressEvent(QKeyEvent *event)
{
    float cameraSpeed = 0.05f;
    if (event->key() == Qt::Key_W) {
        _camPosition += cameraSpeed * _camFront;
    }
    if (event->key() == Qt::Key_S) {
        _camPosition -= cameraSpeed * _camFront;
    }
    if (event->key() == Qt::Key_A) {
        _camPosition -= QVector3D::crossProduct(_camFront,_camUpDirection).normalized() * cameraSpeed;
    }
    if (event->key() == Qt::Key_D) {
        _camPosition += QVector3D::crossProduct(_camFront,_camUpDirection).normalized() * cameraSpeed;
    }

    update();
}

void OpenglWidget::timeout()
{
    lightAngle += 1;
    while (lightAngle >= 360) {
        lightAngle -= 360;
    }

    update();
}
