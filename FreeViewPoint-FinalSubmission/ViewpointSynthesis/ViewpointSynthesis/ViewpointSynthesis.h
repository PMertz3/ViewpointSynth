#pragma once
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/xfeatures2d/nonfree.hpp"
#include <opencv2/videoio.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>  // cv::Canny()
#include <iostream>
#include <vector>
#include <thread>
#include <QtWidgets/QWidget>
#include <qpushbutton.h>
#include <QDialog>
#include <qpixmap.h>
#include <QMessageBox>
#include <qlabel.h>
#include <QGraphicsPixmapItem>
#include <qgraphicsview.h>
#include <fstream>
#include <chrono>
#include <opencv2/stitching.hpp>
using namespace cv::xfeatures2d;
using namespace cv;
using std::cout; using std::cerr; using std::endl; using std::vector;
using namespace std;
class ViewpointSynthesis : public QWidget
{
	Q_OBJECT


public:
	void singleDisplay(VideoCapture capture);
	void stitchedDisplay();
	void gridDisplay();
	void recordCameras();
	void playVideo();
	Mat stitchImages(int &left, int &right);
	void stitchVideos();
	Mat setGrid();
	QPixmap convertImage(Mat finalFrame);
	ViewpointSynthesis(QWidget* parent = Q_NULLPTR);
	

private slots:
	void handleExitButton();
	void handleCam1Button();
	void handleCam2Button();
	void handleCam3Button();
	void handleCam4Button();
	void handleCam5Button();
	void handleCam6Button();
	void handleStitchedView();
	void handleRecordButton();
	void handleStitchRecording();
	void handlePlayRecording();
	void handleGridView();
	void handleStartStream();
	void handleStopStream();
	void handleStopRecording();
	void handleZoomIn();
	void handleZoomOut();

private:
	QPushButton* cam1Button;
	QPushButton* cam2Button;
	QPushButton* cam3Button;
	QPushButton* cam4Button;
	QPushButton* cam5Button;
	QPushButton* cam6Button;
	QPushButton* stitchedViewCam;
	QPushButton* recordButton;
	QPushButton* gridView;
	QPushButton* exitButton;
	QPushButton* startStream;
	QPushButton* stopStream;
	QPushButton* stopRecording;
	QPushButton* stitchRecording;
	QPushButton* playRecording;
	QPushButton* zoomIn;
	QPushButton* zoomOut;
	QLabel* myLabel;
	QLabel* myLabel2;
	QLabel* myLabel3;
	QLabel* myLabel4;
	QGraphicsPixmapItem* pixmap;
};