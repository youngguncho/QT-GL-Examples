#include "openglwidget.h"

OpenglWidget::OpenglWidget(QWidget *parent) : QOpenGLWidget(parent)
{

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

    // set up the rendering context

    makeObject();

    initShaders();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_5_Core>();
    f->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    qDebug() << "initial GL done";

}


void OpenglWidget::makeObject()
{
    vertices << QVector3D(1, 0, 0) << QVector3D(0, 1, 0) << QVector3D(0, 0, 1);
    colors << QVector4D(1, 0, 0, 1) << QVector4D(0, 1, 1, 1) << QVector4D(0, 0, 1, 1);
}



void OpenglWidget::initShaders()
{
    qDebug() << "init Shader";

    QOpenGLShader *v_shader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    QOpenGLShader *f_shader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    QOpenGLShader *tcs_shader = new QOpenGLShader(QOpenGLShader::TessellationControl, this);
    QOpenGLShader *tes_shader = new QOpenGLShader(QOpenGLShader::TessellationEvaluation, this);

    static const char * vs_source =
        "#version 450 core                                                 \n"
        "                                                                  \n"
        "void main(void)                                                   \n"
        "{                                                                 \n"
        "    const vec4 vertices[] = vec4[](vec4( 0.25, -0.25, 0.5, 1.0),  \n"
        "                                   vec4(-0.25, -0.25, 0.5, 1.0),  \n"
        "                                   vec4( 0.25,  0.25, 0.5, 1.0)); \n"
        "                                                                  \n"
        "    gl_Position = vertices[gl_VertexID];                          \n"
        "}                                                                 \n";

    static const char * tcs_source =
        "#version 450 core                                                                 \n"
        "                                                                                  \n"
        "layout (vertices = 3) out;                                                        \n"
        "                                                                                  \n"
        "void main(void)                                                                   \n"
        "{                                                                                 \n"
        "    if (gl_InvocationID == 0)                                                     \n"
        "    {                                                                             \n"
        "        gl_TessLevelInner[0] = 10.0;                                               \n"
        "        gl_TessLevelOuter[0] = 5.0;                                               \n"
        "        gl_TessLevelOuter[1] = 10.0;                                               \n"
        "        gl_TessLevelOuter[2] = 5.0;                                               \n"
        "    }                                                                             \n"
        "    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;     \n"
        "}                                                                                 \n";

    static const char * tes_source =
        "#version 450 core                                                                 \n"
        "                                                                                  \n"
        "layout (triangles, equal_spacing, cw) in;                                         \n"
        "                                                                                  \n"
        "void main(void)                                                                   \n"
        "{                                                                                 \n"
        "    gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position) +                       \n"
        "                  (gl_TessCoord.y * gl_in[1].gl_Position) +                       \n"
        "                  (gl_TessCoord.z * gl_in[2].gl_Position);                        \n"
        "}                                                                                 \n";

    static const char * fs_source =
        "#version 450 core                                                 \n"
        "                                                                  \n"
        "out vec4 color;                                                   \n"
        "                                                                  \n"
        "void main(void)                                                   \n"
        "{                                                                 \n"
        "    color = vec4(0.0, 0.8, 1.0, 1.0);                             \n"
        "}                                                                 \n";

    v_shader->compileSourceCode(vs_source);
    f_shader->compileSourceCode(fs_source);
    tes_shader->compileSourceCode(tes_source);
    tcs_shader->compileSourceCode(tcs_source);

    _shaderProgram.addShader(v_shader);
    _shaderProgram.addShader(f_shader);
    _shaderProgram.addShader(tes_shader);
    _shaderProgram.addShader(tcs_shader);
    _shaderProgram.link();

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

    QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_5_Core>();


    //// prepare camera
    _mMatrix.setToIdentity();
    _vMatrix.setToIdentity();
    _vMatrix.lookAt(_camPosition, _camPosition+_camFront, _camUpDirection);
    _T = _pMatrix * _vMatrix * _mMatrix;

    _shaderProgram.bind();

    f->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    f->glDrawArrays(GL_PATCHES, 0, 3);

    _shaderProgram.release();


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
