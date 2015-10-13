#include "matglwidget.h"
#include <iostream>

MatGLWidget::MatGLWidget( QWidget *parent ) : QGLWidget(parent)
{

}

MatGLWidget::~MatGLWidget()
{
    glDeleteTextures( 1, &_texture_id );
}

QSize MatGLWidget::minimumSizeHint()
{
    return QSize( 400, 300 );
}

QSize MatGLWidget::sizeHint()
{
    return QSize( _image.cols, _image.rows );
}

void MatGLWidget::initializeGL()
{
    glColor3f( 0.0f, 0.0f, 0.0f );			// Let OpenGL clear to black
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glShadeModel( GL_FLAT );			// Set the shading model
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glGenTextures(1, &_texture_id );
}

void MatGLWidget::paintGL()
{
    glColor3f( 1.0f, 1.0f, 1.0f );
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    if(!_image.data )
        return;

    glDisable( GL_DEPTH_TEST );
    drawTexture();
    glEnable( GL_DEPTH_TEST );
}

void MatGLWidget::drawTexture()
{
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, _texture_id );

    GLint format = -1;
    switch( _image.channels() )
    {
        case 1:
            format = GL_LUMINANCE;
            break;
        case 3:
            format = GL_BGR;
            break;
        case 4:
            format = GL_BGRA;
            break;
        default:
            glDisable( GL_TEXTURE_2D );
            return;
    }

    glTexSubImage2D( GL_TEXTURE_2D, 0, _texture_size.width - _image.cols, _texture_size.height - _image.rows, _image.cols,
                     _image.rows, format, GL_UNSIGNED_BYTE, _image.data );

    glLoadIdentity();					// Reset The Modelview Matrix
    glBegin(GL_QUADS);

    glTexCoord2f( _textureUL.x, _textureUL.y ); glVertex2f( -1.0f, 1.0f );
    glTexCoord2f( _textureUL.x, _textureLR.y ); glVertex2f( -1.0f, -1.0f );
    glTexCoord2f( _textureLR.x, _textureLR.y ); glVertex2f(  1.0f, -1.0f );
    glTexCoord2f( _textureLR.x, _textureUL.y ); glVertex2f(  1.0f, 1.0f );

    glEnd();
    glDisable( GL_TEXTURE_2D );

    glTranslatef( -1.0f, 1.0f, 0.0f );
    glScalef( 2.0f / _image.cols, 2.0f / _image.rows, 0.0 );
}

void MatGLWidget::resizeGL( int width, int height )
{
    QMutexLocker locker( &_image_mutex );

    if( _image.rows <= 0 || _image.cols <= 0 )
        return;

    //Scale viewport to keep aspect ratio
    float scaleh = (float)width / (float)_image.cols;
    float scalev = (float)height / (float)_image.rows;
    float scale = scaleh < scalev ? scaleh : scalev;
    GLsizei gl_width = _image.cols * scale;
    GLsizei gl_height = _image.rows * scale;

    //Translate viewport to keep image centered
    GLint gl_xtrans = (width - gl_width) / 2;
    GLint gl_ytrans = (height - gl_height) / 2;

    glViewport( gl_xtrans, gl_ytrans, gl_width, gl_height );
}

unsigned int MatGLWidget::NextLargerPowerOfTwo( unsigned int n )
{
    while( (n & (n - 1)) != 0 )
        ++n;
    return n;
}

bool MatGLWidget::setImage( cv::Mat &image )
{
    QMutexLocker locker( &_image_mutex );

    image.copyTo( _image );

    if( !_image.data || (_image.channels() != 1 && _image.channels() != 3 && _image.channels() != 4) )
        return false;

    glEnable( GL_TEXTURE_2D );					// Enable Texture Mapping
    // calculate texture size
    _texture_size.width = NextLargerPowerOfTwo( _image.cols );
    _texture_size.height = NextLargerPowerOfTwo( _image.rows );

    // calculate texture coordinates for image
    _textureUL.x = (float)(_texture_size.width - _image.cols) / (float)(_texture_size.width);
    _textureUL.y = (float)(_texture_size.height - _image.rows) / (float)(_texture_size.height);
    _textureLR.x = 1.0f;
    _textureLR.y = 1.0f;

    // tell OpenGL which texture "id" we will be working with.:
    glBindTexture( GL_TEXTURE_2D, _texture_id );
    // tell OpenGL that the pixel data which is going to be passed to it is aligned in byte order:
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    // set the various parameters for the current OpenGL texture:
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    // tell OpenGL how the texture will act when it is rendered into a scene:
    // The GL_MODULATE attribute allows you to apply effects such as lighting
    // and coloring to your texture. If you do not want lighting and coloring to effect
    // your texture and you would like to display the texture unchanged when coloring
    // is applied replace GL_MODULATE with GL_DECAL
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    GLint intformat = -1;
    GLint format = -1;
    switch( _image.channels() )
    {
        case 1:
            format = GL_LUMINANCE;
            intformat = GL_LUMINANCE;
            break;
        case 3:
            format = GL_BGR;
            intformat = GL_RGB;
            break;
        case 4:
            format = GL_BGRA;
            intformat = GL_RGBA;
            break;
        default:
            glDisable( GL_TEXTURE_2D );
            return false;
    }

    glTexImage2D( GL_TEXTURE_2D, 0, intformat, _texture_size.width, _texture_size.height, 0,  format, GL_UNSIGNED_BYTE, nullptr );

    glDisable( GL_TEXTURE_2D );

    updateGL();

    return true;
}
