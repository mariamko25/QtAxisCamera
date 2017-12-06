#include "cameraaxis.h"
#include "ui_cameraaxis.h"

using namespace cv;
using namespace std;

/**
 * @brief CameraAxis::CameraAxis
 * @param parent
 * our class our to connect interface
 * to buttons
 */
CameraAxis::CameraAxis(QWidget *parent) :  QMainWindow(parent), ui(new Ui::CameraAxis)
{
    //setting up interface
    ui->setupUi(this);
    //setting up buttons
    setButton();
    //launch our programs every button sets actions
    connect(ui->play,SIGNAL(clicked()),this,SLOT(on_play_clicked()));
    connect(ui->record,SIGNAL(clicked()),this,SLOT(on_record_clicked()));
    connect(ui->stop,SIGNAL(clicked()),this,SLOT(on_stop_clicked()));
    connect(ui->fichier,SIGNAL(clicked()),this,SLOT(on_fichier_clicked()));

}

/**
 * @brief CameraAxis::~CameraAxis
 * destrector, frees memory used by pointers
 */
CameraAxis::~CameraAxis()
{
    delete ui;
    delete timer;
}

/**
 * @brief CameraAxis::setButton
 * we set our buttons and boxes
 * add icons to our buttons and
 * lists to our combobox and enable
 * saving the file
 */
void CameraAxis::setButton()
{
    //we didn't find a QStyle icon for record button
    //So we took an image changed it into QIcon
    QPixmap pxmp(":/record/record1.png");
    QIcon icon(pxmp);
    ui->record->setIcon(icon);
    //Play and Stop icons already existed
    ui->play->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->stop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    //List of video formats we use in our app
    QStringList fonts;
    fonts << "MJPEG" << "MPEG1";
    ui->Format->addItems(fonts);
    //boolean to allow "saving file"
    trick=true;
}


/**
 * @brief CameraAxis::initialize
 * we use this function to get information
 * written on the interface
 * to be able to open our camera
 */
void CameraAxis::initialize()
{
    //We use a QString to append the
    //information we need to connect to the camera
    //Our videoStreamAddress should ressemble something like this:
    //----->http://usrname:password@IPaddress/mjpg/video.mjpg
    QString videoStreamAddress = "http://";
    videoStreamAddress+=Username;
    videoStreamAddress+=":";
    videoStreamAddress+=Password;
    videoStreamAddress+="@";
    videoStreamAddress+=IP;
    videoStreamAddress+="/mjpg/video.mjpg";
    //Opencv videocapture only knows stdStrings so we need to
    //Convert our QString to std String to open it
    //camera.open(videoStreamAddress.toStdString());
    camera.open(0);
    //setting up timer
    timer = new QTimer(this);
}

/**
 * @brief CameraAxis::on_record_clicked
 * slot launching when we click record
 */
void CameraAxis::on_record_clicked()
{
    //it unlocks saving button
    trick=true;
    //it gets all the information needed to open de camera and recording
    IP=ui->textIP->toPlainText();
    Username=ui->textLogin->toPlainText();
    Password=ui->textPassword->toPlainText();
    Name=ui->textFile->toPlainText();
    //if all the information needed isn't given to us, we don't record
    if(IP.isEmpty()==0 && Username.isEmpty()==0 && Password.isEmpty()==0  && Name.isEmpty()==0 )
    {
        //before initializing we check wether or not our camera has
        //already been initialized so opened
        if(!camera.isOpened())
        {
            initialize();
        }

        //then we get the size of the image to be able to save it on the disk
        Mat frame;
        camera >> frame;
        int frame_width=   camera.get(CV_CAP_PROP_FRAME_WIDTH);
        int frame_height=   camera.get(CV_CAP_PROP_FRAME_HEIGHT);
        //we get the format of the video before saving it
        format=ui->Format->currentText();
        //creating an opencv videoWriter to write the video according to the format
        if(format=="MJPEG")
        {
            //Video writer takes:Paths of the file, format, speed, size and color=true
            writter=cv::VideoWriter(Name.toStdString(),CV_FOURCC('M','J','P','G'),24, Size(frame_width,frame_height),true);

        }
        else
        {
            //Video writer takes:Paths of the file, format, speed, size and color=true
            writter=cv::VideoWriter(Name.toStdString(),CV_FOURCC('P','I','M','1'),24, Size(frame_width,frame_height),true);

        }

        //When everything is ready we connect it to the slot recordPicture
        connect(timer, SIGNAL(timeout()), this, SLOT(recordPicture()));
        timer->start(20);
    }
}

/**
 * @brief CameraAxis::convertImg
 * on our interface we show QImages
 * However we record and save opencv images
 * so we need to convert opencv images to Qimage
 * in order to be able to show it on the interface
 */
void CameraAxis::convertImg()
{
    //we convert our object image into a matcp (other image) but with qt colors
    Mat matcp;
    //opencv and Qt don't use the same colors
    cvtColor(image, matcp,CV_BGR2RGB);
    //Now that both images have the same setting we convert the matcp into QImage so ui can show it
    qImage=QImage((uchar*)matcp.data,matcp.cols,matcp.rows,matcp.step,QImage::Format_RGB888);
    pixmap=QPixmap(QPixmap::fromImage(qImage));
}

/**
 * @brief CameraAxis::setDimension
 * Setting the size for our interface's video
 */
void CameraAxis::setDimension()
{
    labelSize=ui->label->size();
    pixmap = pixmap.scaled(labelSize, Qt::IgnoreAspectRatio, Qt::FastTransformation);
}

/**
 * @brief CameraAxis::writeVideo
 * class to save our video to disk
 */
void CameraAxis::writeVideo()
{
    //opencvVideoWriter
    writter.write(image);
}

/**
 * @brief CameraAxis::updatePicture
 * class to get cameras opencv image
 * convert it to Qimage
 * fit the image to our interface's size
 * then shows it
 */
void CameraAxis::updatePicture()
{
   //opencv Mat image takes 1 image from camera
   camera >> image;
   //we convert image to QImage
   convertImg();
   //set up pixmap dimensions
   setDimension();
   //show image on interface
   ui->label->setPixmap(pixmap);
}

/**
 * @brief CameraAxis::recordPicture
 * this slot enables us to updatePicture
 * Meaning to show the image we are recording
 * in real time
 */
void CameraAxis::recordPicture()
{
   updatePicture();
   writeVideo();
}

/**
 * @brief CameraAxis::on_stop_clicked
 * enables us to stop recording
 */
void CameraAxis::on_stop_clicked()
{
    //enables saving button
    trick=true;
    //checks if we're saving a video
    if(writter.isOpened())
    {
        //then he stops it
        writter.release();
        //and shows image nontheless
        on_play_clicked();
    }
}

/**
 * @brief CameraAxis::on_play_clicked
 * this function just shows the image of the camera
 * but it's not recording
 */
void CameraAxis::on_play_clicked()
{
    ///it unlocks saving button
    trick=true;
    //it gets all the information needed to open de camera and recording
    IP=ui->textIP->toPlainText();
    Username=ui->textLogin->toPlainText();
    Password=ui->textPassword->toPlainText();
    //if all the information needed isn't given to us, we don't record
    if(IP.isEmpty()==false && Username.isEmpty()==false && Password.isEmpty()==false)
    {
        //before initializing we check wether or not our camera has
        //already been initialized so opened
        if(!camera.isOpened())
        {
            initialize();
        }
        //then we just show the camera image without recording it
        connect(timer, SIGNAL(timeout()), this, SLOT(updatePicture()));
        timer->start(20);
    }
}

/**
 * @brief CameraAxis::on_fichier_clicked
 * this function helps us save the file on the disk
 */
void CameraAxis::on_fichier_clicked()
{
    //we check first if it hasn't opened
    if(trick==true)
    {
        //we open and set ui->textFile to the name written
        ui->textFile->setText(QFileDialog::getSaveFileName(this, tr("Save File"), QString(),tr("All files (*.*)")));
    }
    //then we disable opening until we record
    trick=false;
}






