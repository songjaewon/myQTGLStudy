#include "window.h"
#include <QDebug>
#include <QString>
#include <QOpenGLShaderProgram>
#include "vertex.h"

// Create a colored triangle
static const Vertex sg_vertexes[] = {
  Vertex( QVector3D( 0.00f,  0.75f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f) ),
  Vertex( QVector3D(-0.75f, -0.75f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f) ),
  Vertex( QVector3D( 0.75f, -0.75f, 1.0f), QVector3D(0.0f, 0.0f, 1.0f) )
};



Window::~Window()
{
    makeCurrent();
    teardownGL();
}

void Window::initializeGL()
{
    // initialize OpenGL backend
    initializeOpenGLFunctions();    // perform initialization current OpenGL context
    printContextInformation();      // our function for printing the OpenGL information on the console window

    //See the global information
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // greeen background

    // Application - specific initialization
    {
        // Create Shader (Do not release until VAO is created)
        m_program = new QOpenGLShaderProgram();
        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vert");
        m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.frag");
        m_program->link();
        m_program->bind();

        // Create Buffer (Do not release until VAO is created)
        m_vertex.create();
        m_vertex.bind();
        m_vertex.setUsagePattern(QOpenGLBuffer::StaticDraw);
        m_vertex.allocate(sg_vertexes, sizeof(sg_vertexes));

        // Create Vertex Array Object
        m_object.create();
        m_object.bind();
        m_program->enableAttributeArray(0);
        m_program->enableAttributeArray(1);
        m_program->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
        m_program->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

        // Release (unbind) all
        m_object.release();
        m_vertex.release();
        m_program->release();

    }

}

void Window::resizeGL(int width, int height)
{
    // Nothing now
    (void)width;
    (void)height;
}

void Window::paintGL()
{
    //clear
    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();
    {
        m_object.bind();
        glDrawArrays(GL_TRIANGLES, 0, sizeof(sg_vertexes)/sizeof(sg_vertexes[0]));
        m_object.release();
    }
    m_program->release();
}

void Window::teardownGL()
{
    // Nothing now. related about GPU things.
    m_object.destroy();
    m_vertex.destroy();
    delete m_program;
}

/****************************
 * private helpers
 ****************************/

void Window::printContextInformation()
{
    QString glType;
    QString glVersion;
    QString glProfile;

    //get version information
    glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

    //get profile information
#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
    switch (format().profile())
    {
        CASE(NoProfile);
        CASE(CoreProfile);
        CASE(CompatibilityProfile);
    }
#undef CASE

    qDebug() << "Jaewon Song's Debug function." << endl;
    qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}
