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
    qDebug() << "Start GL";

}

OpenglWidget::~OpenglWidget()
{

}

void OpenglWidget::initializeGL()
{
    qDebug() << "initial GL";
    initializeOpenGLFunctions();

    makeObject();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0, 0, 0, 1);


    initShaders();

    qDebug() << "initial GL done";

}

void OpenglWidget::initShaders()
{
    qDebug() << "init Shader";


    _shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/vertexShader.vsh");
    _shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/fragmentShader.fsh");
    _shaderProgram.link();

    _shaderProgram.bind();
    _shaderProgram.setUniformValue("texture", 0);

    qDebug() << "Done shader";

}

void OpenglWidget::resizeGL(int width, int height)
{
    qDebug() << "resizeGL GL";

    // Calculate aspect ratio
    qreal aspect = qreal(width) / qreal(height ? height : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
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

    _shaderProgram.setUniformValue("mvpMatrix", _T);
    _shaderProgram.setAttributeArray("vertex", vertices.constData());
    _shaderProgram.enableAttributeArray("vertex");

    _shaderProgram.setAttributeArray("textureCoordinate", textureCoordinates.constData());
    _shaderProgram.enableAttributeArray("textureCoordinate");

    textures_one->bind();

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    _shaderProgram.disableAttributeArray("vertex");

    _shaderProgram.disableAttributeArray("textureCoordinate");

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

void OpenglWidget::makeObject()
{
    vertices << QVector3D(-0.5, -0.5,  0.5) << QVector3D( 0.5, -0.5,  0.5) << QVector3D( 0.5,  0.5,  0.5) // Front
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
    //! [3]
    textureCoordinates << QVector2D(0, 0) << QVector2D(1, 0) << QVector2D(1, 1) // Front
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

    static const int coords[6][4][3] = {
        { { +1, -1, -1 }, { -1, -1, -1 }, { -1, +1, -1 }, { +1, +1, -1 } },
        { { +1, +1, -1 }, { -1, +1, -1 }, { -1, +1, +1 }, { +1, +1, +1 } },
        { { +1, -1, +1 }, { +1, -1, -1 }, { +1, +1, -1 }, { +1, +1, +1 } },
        { { -1, -1, -1 }, { -1, -1, +1 }, { -1, +1, +1 }, { -1, +1, -1 } },
        { { +1, -1, +1 }, { -1, -1, +1 }, { -1, -1, -1 }, { +1, -1, -1 } },
        { { -1, -1, +1 }, { +1, -1, +1 }, { +1, +1, +1 }, { -1, +1, +1 } }
    };

    // load texture
    for (int i=0; i<6; ++i)
        textures[i] = new QOpenGLTexture(QImage(QString(":resources/texture.png")).mirrored());

    textures_one =new QOpenGLTexture(QImage(QString(":resources/texture.png")));


    // set vertex and texture coordinate
    QVector<GLfloat> vertData;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 4; ++j) {
            // vertex position
            vertData.append(0.2 * coords[i][j][0]);
            vertData.append(0.2 * coords[i][j][1]);
            vertData.append(0.2 * coords[i][j][2]);
            // texture coordinate
            vertData.append(j == 0 || j == 3);
            vertData.append(j == 0 || j == 1);
        }
    }

//    // create buffer
//    vbo.create();
//    vbo.bind();
//    vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));


}

