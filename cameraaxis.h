#ifndef CAMERAAXIS_H
#define CAMERAAXIS_H

#include <QMainWindow>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QFileDialog>
#include<QtCore>

/**
 * Our interface
 */
namespace Ui {
class CameraAxis;
}

/**
 * @brief The CameraAxis class
 * this class will contain slots and
 * objects we need to make our application function
 */
class CameraAxis : public QMainWindow
{
    Q_OBJECT

public:
    explicit CameraAxis(QWidget *parent = 0);
    ~CameraAxis();
public slots:
    //These two slots will be the most used in the project
    void updatePicture();
    void recordPicture();
private slots:
    //slots used in setting up the interface
    void initialize();
    void convertImg();
    void setDimension();
    void writeVideo();
    void setButton();
    //slots  we connect to the interface
    void on_record_clicked();
    void on_stop_clicked();
    void on_play_clicked();
    void on_fichier_clicked();
private:
    //our interface
    Ui::CameraAxis *ui;
    //Our Camera object
    cv::VideoCapture camera;
    //A timer for slots
    QTimer * timer;
    //Our object to write videos
    cv::VideoWriter writter;
    //LabelSize to adapt the image to the interface
    QSize labelSize;
    //Qt objects to show image on interface
    QPixmap pixmap;
    QImage qImage;
    //opencv image to get image on disk
    cv::Mat image;
    //Strings for the values of our button
    QString IP;
    QString Username;
    QString Password;
    QString Name;
    QString format;
    //We had an issue with the Qfilevideo opening when
    //we press "save" so we have a boolean saying if it opened once
    //that's it
    bool trick;
};

#endif // CAMERAAXIS_H
