#include "window.h"
#include <QDebug>
#include <QString>



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
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);  // greeen background

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
}

void Window::teardownGL()
{
    // Nothing now. related about GPU things.
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
