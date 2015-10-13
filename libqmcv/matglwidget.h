#ifndef MATGLWIDGET_H
#define MATGLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>
#include <opencv2/core/core.hpp>
#include <QMutex>

class MatGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit MatGLWidget( QWidget *parent = 0 );
    ~MatGLWidget();
    QSize minimumSizeHint();
    QSize sizeHint();
    void initializeGL();
    void resizeGL( int width, int height );
    bool setImage( cv::Mat &image );

signals:

public slots:

protected:
    void paintGL();
    void drawTexture();

private:
    unsigned int NextLargerPowerOfTwo( unsigned int n );

    GLuint _texture_id;
    cv::Mat _image;
    QMutex _image_mutex;
    cv::Size _texture_size;
    cv::Point2f _textureUL;
    cv::Point2f _textureLR;

};

#endif // MATGLWIDGET_H
