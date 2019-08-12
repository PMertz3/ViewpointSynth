#include "ViewpointSynthesis.h"
//Video captures for up to 6 cameras
VideoCapture capture1(0);
VideoCapture capture2(1);
VideoCapture capture3(2);
VideoCapture capture4(3);
VideoCapture capture5(4);
VideoCapture capture6(5);
//Frames for capturing and size manipulation
Mat capturedFrame;
Mat resizedFrame;
Mat resize2;
//Grid View Mats
Mat blueFrame;
Mat resize1;
Mat frame1;
Mat frame2;
Mat frame3;
Mat frame4;
Mat frame5;
Mat frame6;
Mat top;
Mat bot;
Mat3b gridFrame;
double frameRatio = 0.6;

std::vector<Mat> matArray1;
std::vector<Mat> matArray2;

Mat src;
Mat src2;

//Sizes for the frames
Rect blankROI(32, 24, 576, 432);
Rect myROI(32, 24, 576, 432);
Rect myROIs(32, 24, 650, 450);
Rect myROI2(64, 48, 512, 384);
Rect myROI3(128, 96, 384, 288);

// timer
auto start = std::chrono::system_clock::now();
bool runOnce = true;
int left1 = 0;
int right1 = 0;


//Y Spacer
int ySpace = 480;
//Used for detirmining zoom
double zoom = 1.0;
//Used to detirmine if the stream is stopped or not
bool isStopped = false;
bool isRecording = false;
int numCams = 0;
//Creates the buttons and space for the image display
ViewpointSynthesis::ViewpointSynthesis(QWidget* parent)
	: QWidget(parent)
{
	//Counts the number of active cameras

	for (int i = 0; i < 7; ++i) {
		VideoCapture capture0(i);
		if (capture0.isOpened()) {
			numCams++;
		}
	}
	//Creates a label that will be used to display the captured images
	myLabel = new QLabel("", this);
	myLabel2 = new QLabel("", this);
	myLabel3 = new QLabel("", this);
	myLabel4 = new QLabel("", this);
	//Creates a button for each active camera
	if (numCams >= 1) {
		cam1Button = new QPushButton("Cam1", this);
		cam1Button->setGeometry(QRect(QPoint(50, 50 + ySpace),
			QSize(100, 50)));
		connect(cam1Button, SIGNAL(released()), this, SLOT(handleCam1Button()));
	}
	if (numCams >= 2) {
		cam2Button = new QPushButton("Cam2", this);
		cam2Button->setGeometry(QRect(QPoint(50, 100 + ySpace),
			QSize(100, 50)));
		connect(cam2Button, SIGNAL(released()), this, SLOT(handleCam2Button()));
	}
	if (numCams >= 3) {
		cam3Button = new QPushButton("Cam3", this);
		cam3Button->setGeometry(QRect(QPoint(50, 150 + ySpace),
			QSize(100, 50)));
		connect(cam3Button, SIGNAL(released()), this, SLOT(handleCam3Button()));
	}
	if (numCams >= 4) {
		cam4Button = new QPushButton("Cam4", this);
		cam4Button->setGeometry(QRect(QPoint(150, 50 + ySpace),
			QSize(100, 50)));
		connect(cam4Button, SIGNAL(released()), this, SLOT(handleCam4Button()));
	}
	if (numCams >= 5) {
		cam5Button = new QPushButton("Cam5", this);
		cam5Button->setGeometry(QRect(QPoint(150, 100 + ySpace),
			QSize(100, 50)));
		connect(cam5Button, SIGNAL(released()), this, SLOT(handleCam5Button()));
	}
	if (numCams >= 6) {
		cam6Button = new QPushButton("Cam6", this);
		cam6Button->setGeometry(QRect(QPoint(150, 150 + ySpace),
			QSize(100, 50)));
		connect(cam6Button, SIGNAL(released()), this, SLOT(handleCam6Button()));
	}

	//Creates a button for the stitched view function
	stitchedViewCam = new QPushButton("Stitched View", this);
	stitchedViewCam->setGeometry(QRect(QPoint(270, 75 + ySpace - 15),
		QSize(100, 50)));
	connect(stitchedViewCam, SIGNAL(released()), this, SLOT(handleStitchedView()));

	//Creates a button for the grid view function
	gridView = new QPushButton("Grid View", this);
	gridView->setGeometry(QRect(QPoint(270, 150 + ySpace - 15),
		QSize(100, 50)));
	connect(gridView, SIGNAL(released()), this, SLOT(handleGridView()));

	//Creates a button for the stitched view function
	recordButton = new QPushButton("Record", this);
	recordButton->setGeometry(QRect(QPoint(420, 75 + ySpace - 15),
		QSize(100, 50)));
	connect(recordButton, SIGNAL(released()), this, SLOT(handleRecordButton()));

	//Creates a button for the stitched view function
	stopRecording = new QPushButton("End Recording", this);
	stopRecording->setGeometry(QRect(QPoint(420, 150 + ySpace - 15),
		QSize(100, 50)));
	connect(stopRecording, SIGNAL(released()), this, SLOT(handleStopRecording()));

	//Creates a button for the stitched view function
	stitchRecording = new QPushButton("Stitch Recording", this);
	stitchRecording->setGeometry(QRect(QPoint(560, 75 + ySpace - 15),
		QSize(100, 50)));
	connect(stitchRecording, SIGNAL(released()), this, SLOT(handleStitchRecording()));

	//Creates a button for the stitched view function
	playRecording = new QPushButton("Play Recording", this);
	playRecording->setGeometry(QRect(QPoint(560, 150 + ySpace - 15),
		QSize(100, 50)));
	connect(playRecording, SIGNAL(released()), this, SLOT(handlePlayRecording()));

	//Creates a button for the start stream function
	startStream = new QPushButton("Start Stream", this);
	startStream->setGeometry(QRect(QPoint(700, 75 + ySpace - 15),
		QSize(100, 50)));
	connect(startStream, SIGNAL(released()), this, SLOT(handleStartStream()));

	//Creates a button for the stop stream function
	stopStream = new QPushButton("Stop Stream", this);
	stopStream->setGeometry(QRect(QPoint(700, 150 + ySpace - 15),
		QSize(100, 50)));
	connect(stopStream, SIGNAL(released()), this, SLOT(handleStopStream()));

	//Creates a button for the zoom in function
	zoomIn = new QPushButton("Zoom In", this);
	zoomIn->setGeometry(QRect(QPoint(840, 75 + ySpace - 15),
		QSize(100, 50)));
	connect(zoomIn, SIGNAL(released()), this, SLOT(handleZoomIn()));

	//Creates a button for the zoom in function
	zoomOut = new QPushButton("Zoom Out", this);
	zoomOut->setGeometry(QRect(QPoint(840, 150 + ySpace - 15),
		QSize(100, 50)));
	connect(zoomOut, SIGNAL(released()), this, SLOT(handleZoomOut()));

	//Creates a button for the exit function
	exitButton = new QPushButton("Exit", this);
	exitButton->setGeometry(QRect(QPoint(980, 50 + ySpace),
		QSize(200, 150)));
	connect(exitButton, SIGNAL(released()), this, SLOT(handleExitButton()));

	if (numCams < 2) {
		stitchedViewCam->setEnabled(false);
	}

}


//Single display is the function that is called when one of the single view buttons is pressed
//It accepts a video capture as an argument
//The video capture takes frames from a given camera and sets them in a variable "capturedFrame"
//The frame is resized to fit the gui and then calls convert image to return a pixmap
//The pixmap is then inserted into the label and displayed on the screen
void ViewpointSynthesis::singleDisplay(VideoCapture capture) {
	zoomIn->setEnabled(true);
	zoomOut->setEnabled(true);
	zoom = 1.0;
	isStopped = false;
	capture >> capturedFrame;
	resizedFrame = capturedFrame(myROI);
	myLabel->setGeometry(QRect(QPoint(250, 0),
		QSize(720, 480)));
	for (;;) {
		if (capture.isOpened()) { //avoid reading from an unopened device
			capture >> capturedFrame;
			myLabel->setPixmap(convertImage(capturedFrame));
		}
		else {
			QMessageBox::information(this, "Warning",
				"No Cam Found");
			break; // If no camera, breaks from method
		}
		int keypress = waitKey(1); //Prevent infinite loop
		while (isStopped) {
			int keypress = waitKey(1); //Prevent infinite loop
		}
	}

}

void ViewpointSynthesis::gridDisplay() {
	zoomIn->setEnabled(false);
	zoomOut->setEnabled(false);
	zoom = 1.0;
	isStopped = false;
	blueFrame = imread("Bb_blue.jpg", IMREAD_COLOR);
	for (;;) {
		if (numCams != 0) { //avoid reading from an unopened device
			myLabel->setGeometry(QRect(QPoint(250, 0),
				QSize(720, 480)));
			resizedFrame = setGrid();
			myLabel->setPixmap(convertImage(resizedFrame));
		}
		else {
			QMessageBox::information(this, "Warning",
				"No Cam Found");
			break; // If no camera, breaks from method
		}
		int keypress = waitKey(1); //Prevent infinite loop
		while (isStopped) {
			int keypress = waitKey(1); //Prevent infinite loop
		}
	}

}


//convertImage converts an OpenCV Mat image to a QT pixmap image and returns the pixmap
//This functions takes an OpenCV mat as an argument
QPixmap ViewpointSynthesis::convertImage(Mat capturedFrame) {
	//Resizes the frame according to the amount of zoom that is currently being applied
	cv::resize(resizedFrame, resize2, Size(720, 480));
	//converts the mat to RGB for qt
	cvtColor(resize2, resize2, COLOR_RGB2BGR);
	//Create a new image using the capturedFrame frame data
	QImage img((uchar*)resize2.data, 720, 480, QImage::Format_RGB888);
	//Sets a pixel map to the image
	QPixmap pixmap = QPixmap::fromImage(img);
	return pixmap;
}

//Set grid sets all of the accessable camera feeds in a displayable grid Mat image
Mat ViewpointSynthesis::setGrid() {
	switch (numCams) {
	case 0: {
		QMessageBox::information(this, "Warning",
			"No Cam Found");
		return blueFrame;
		break;
	}
	case 1: {
		capture1 >> frame1;
		cv::resize(frame1, frame1, Size(240, 240));
		cv::resize(blueFrame, blueFrame, Size(240, 240));
		frame2 = blueFrame;
		frame3 = blueFrame;
		frame4 = blueFrame;
		frame5 = blueFrame;
		frame6 = blueFrame;
		matArray1 = { frame1, frame2, frame3 };
		matArray2 = { frame4, frame5, frame6 };
		hconcat(matArray1, top);
		hconcat(matArray2, bot);
		vconcat(top, bot, gridFrame);
		break;
	}
	case 2: {
		capture1 >> frame1;
		capture2 >> frame2;
		cv::resize(frame1, frame1, Size(), frameRatio, frameRatio);
		cv::resize(blueFrame, blueFrame, Size(frame1.cols, frame1.rows), 0, 0);
		cv::resize(frame2, frame2, Size(frame1.cols, frame1.rows), 0, 0);
		frame3 = blueFrame;
		frame4 = blueFrame;
		frame5 = blueFrame;
		frame6 = blueFrame;
		matArray1 = { frame1, frame2, frame3 };
		matArray2 = { frame4, frame5, frame6 };
		hconcat(matArray1, top);
		hconcat(matArray2, bot);
		vconcat(top, bot, gridFrame);
		break;
	}
	case 3: {
		capture1 >> frame1;
		capture2 >> frame2;
		capture3 >> frame3;
		cv::resize(frame1, frame1, Size(), frameRatio, frameRatio);
		cv::resize(blueFrame, blueFrame, Size(frame1.cols, frame1.rows), 0, 0);
		cv::resize(frame2, frame2, Size(frame1.cols, frame1.rows), 0, 0);
		cv::resize(frame3, frame3, Size(frame1.cols, frame1.rows), 0, 0);
		frame4 = blueFrame;
		frame5 = blueFrame;
		frame6 = blueFrame;
		matArray1 = { frame1, frame2, frame3 };
		matArray2 = { frame4, frame5, frame6 };
		hconcat(matArray1, top);
		hconcat(matArray2, bot);
		vconcat(top, bot, gridFrame);
		break;
	}
	case 4: {
		capture1 >> frame1;
		capture2 >> frame2;
		capture3 >> frame3;
		capture4 >> frame4;
		cv::resize(frame1, frame1, Size(), frameRatio, frameRatio);
		cv::resize(blueFrame, blueFrame, Size(frame1.cols, frame1.rows), 0, 0);
		cv::resize(frame2, frame2, Size(frame1.cols, frame1.rows), 0, 0);
		cv::resize(frame3, frame3, Size(frame1.cols, frame1.rows), 0, 0);
		cv::resize(frame4, frame4, Size(frame1.cols, frame1.rows), 0, 0);
		frame5 = blueFrame;
		frame6 = blueFrame;
		matArray1 = { frame1, frame2, frame3 };
		matArray2 = { frame4, frame5, frame6 };
		hconcat(matArray1, top);
		hconcat(matArray2, bot);
		vconcat(top, bot, gridFrame);

		break;
	}
	case 5: {
		capture1 >> frame1;
		capture2 >> frame2;
		capture3 >> frame3;
		capture4 >> frame4;
		capture5 >> frame5;
		cv::resize(frame1, frame1, Size(), frameRatio, frameRatio);
		cv::resize(blueFrame, blueFrame, Size(frame1.cols, frame1.rows), 0, 0);
		cv::resize(frame2, frame2, Size(frame1.cols, frame1.rows), 0, 0);
		cv::resize(frame3, frame3, Size(frame1.cols, frame1.rows), 0, 0);
		cv::resize(frame4, frame4, Size(frame1.cols, frame1.rows), 0, 0);
		cv::resize(frame5, frame5, Size(frame1.cols, frame1.rows), 0, 0);
		frame6 = blueFrame;
		matArray1 = { frame1, frame2, frame3 };
		matArray2 = { frame4, frame5, frame6 };
		hconcat(matArray1, top);
		hconcat(matArray2, bot);
		vconcat(top, bot, gridFrame);

		break;
	}
	case 6: {
		capture1 >> frame1;
		capture2 >> frame2;
		capture3 >> frame3;
		capture4 >> frame4;
		capture5 >> frame5;
		capture6 >> frame6;
		cv::resize(frame1, frame1, Size(), frameRatio, frameRatio);
		cv::resize(frame2, frame2, Size(frame1.cols, frame1.rows), 0, 0);
		cv::resize(frame3, frame3, Size(frame1.cols, frame1.rows), 0, 0);
		cv::resize(frame4, frame4, Size(frame1.cols, frame1.rows), 0, 0);
		cv::resize(frame5, frame5, Size(frame1.cols, frame1.rows), 0, 0);
		cv::resize(frame6, frame6, Size(frame1.cols, frame1.rows), 0, 0);
		matArray1 = { frame1, frame2, frame3 };
		matArray2 = { frame4, frame5, frame6 };
		hconcat(matArray1, top);
		hconcat(matArray2, bot);
		vconcat(top, bot, gridFrame);
		break;
	}
	}
	return gridFrame;
}

//Stitches images in real time for 2 cameras
Mat ViewpointSynthesis::stitchImages(int& left, int& right) {
	const double nn_match_ratio = 0.8f; // Nearest-neighbour matching ratio
	capture2 >> src;
	capture1 >> src2;


	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 500;
	Mat desc1, desc2;
	Ptr<ORB> detector = ORB::create();
	detector->setMaxFeatures(minHessian);
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE_HAMMING);
	std::vector<KeyPoint> keypoints1, keypoints2;

	detector->detectAndCompute(src, noArray(), keypoints1, desc1);
	detector->detectAndCompute(src2, noArray(), keypoints2, desc2);

	std::vector< std::vector<DMatch> > knn_matches;
	matcher->knnMatch(desc1, desc2, knn_matches, 2);
	//-- Filter matches using the Lowe's ratio test

	//std::vector< std::vector<DMatch> > good_matches;
	std::vector<KeyPoint> matched1, matched2;
	for (unsigned i = 0; i < knn_matches.size(); i++) {
		if (knn_matches[i][0].distance < nn_match_ratio * knn_matches[i][1].distance) {
			matched1.push_back(keypoints1[knn_matches[i][0].queryIdx]);
			matched2.push_back(keypoints2[knn_matches[i][0].trainIdx]);
		}
	}
	auto endtime = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_secs = endtime - start;
	if (elapsed_secs.count() >= 1 || runOnce == true) {

		int max1 = 1000;
		int max2 = 0;
		std::vector<KeyPoint>::const_iterator it = matched1.begin(), end = matched1.end();
		for (; it != end; ++it) {
			if (it->pt.x < max1) {
				max1 = it->pt.x;
			}
		}
		std::vector<KeyPoint>::const_iterator it2 = matched2.begin(), end2 = matched2.end();
		for (; it2 != end2; ++it2) {
			if (it2->pt.x > max2) {
				max2 = it2->pt.x;
			}
		}

		left = max1 + 32;
		right = max2;
		runOnce = false;
		start = std::chrono::system_clock::now();
	}
	//std::cout << left << " " << right << std::endl;

	int h = 480;
	Mat merge;
	Mat test = cv::Mat::zeros(cv::Size(720, h), CV_8UC3);
	Mat test1 = cv::Mat(src, cv::Rect(left, 0, src.cols - left, src.rows));
	Mat test2 = cv::Mat(src2, cv::Rect(0, 0, right, src2.rows));

	cv::resize(test1, test1, cv::Size(480, h), 0, 0);
	cv::resize(test2, test2, cv::Size(480, h), 0, 0);
	double alpha = 0.1;
	double beta = 1 - alpha;
	//multiply(test2, Scalar(1, 1, 1, 0.5), test2);
	cv::addWeighted(test1, alpha, test2, beta, 0.0, merge);

	Mat small1 = cv::Mat(src, cv::Rect(0, 0, left, src.rows));
	cv::resize(small1, small1, cv::Size(120, h), 0, 0);
	//cv::Rect myROI(max2, 0, src2.cols, src2.rows);

	Mat small2 = Mat(src2, Rect(right, 0, src2.cols - right, src2.rows));
	cv::resize(small2, small2, cv::Size(120, h), 0, 0);
	small1.copyTo(test(Rect(0, 0, small1.cols, h)));
	merge.copyTo(test(Rect(small1.cols, 0, merge.cols, h)));
	small2.copyTo(test(Rect(small1.cols + merge.cols, 0, small2.cols, small2.rows)));
	//drawMatches(src, matched1, src2, matched2, knn_matches, img_matches, Scalar(255,0,0),
	//	Scalar(255, 0, 0));
	//-- Show detected matches
	return test;
}


//Records mp4 videos from each of the connected cameras
void ViewpointSynthesis::recordCameras() {
	isRecording = true;
	// framerate of the created video stream
	double fps = 25.0;
	bool isColor = (src.type() == CV_8UC3);
	if (numCams == 1) {
		myLabel->setGeometry(QRect(QPoint(250, 0),
			QSize(720, 480)));
		Mat src;
		// use default camera as video source
		capture1 >> src;
		//--- INITIALIZE VIDEOWRITER
		VideoWriter writer;
		string filename1 = "./result.mp4";
		writer.open(filename1, 0x00000020, fps, src.size(), isColor);
		//--- GRAB AND WRITE LOOP
		while (isRecording)
		{
			// check if we succeeded
			if (!capture1.read(src)) {
				break;
			}
			// encode the frame into the videofile stream
			resizedFrame = src(myROI);
			myLabel->setPixmap(convertImage(src));
			writer.write(src);
			// show live and wait for a key with timeout long enough to show images
			if (waitKey(5) >= 0)
				break;
		}
	}
	if (numCams == 2) {
		myLabel->setGeometry(QRect(QPoint(100, 0),
			QSize(500, 400)));
		myLabel2->setGeometry(QRect(QPoint(600, 0),
			QSize(500, 400)));
		Mat src;
		Mat src1;
		// use default camera as video source

		capture1 >> src;
		capture2 >> src1;
		//--- INITIALIZE VIDEOWRITER
		VideoWriter writer;
		VideoWriter writer1;
		string filename1 = "./cam1.mp4";
		string filename2 = "./cam2.mp4";// name of the output video file
		writer.open(filename1, 0x00000020, fps, src.size(), isColor);
		writer1.open(filename2, 0x00000020, fps, src1.size(), isColor);
		//--- GRAB AND WRITE LOOP
		while (isRecording)
		{
			// check if we succeeded
			if (!capture1.read(src)) {
				break;
			}
			if (!capture2.read(src1)) {
				break;
			}
			// encode the frame into the videofile stream
			resizedFrame = src(myROI);
			myLabel->setPixmap(convertImage(src));
			resizedFrame = src1(myROI);
			myLabel2->setPixmap(convertImage(src1));
			writer.write(src);
			writer1.write(src1);
			// show live and wait for a key with timeout long enough to show images
			if (waitKey(5) >= 0)
				break;
		}
	}
	if (numCams == 3) {
		myLabel->setGeometry(QRect(QPoint(0, 0),
			QSize(500, 250)));
		myLabel2->setGeometry(QRect(QPoint(500, 0),
			QSize(500, 250)));
		myLabel3->setGeometry(QRect(QPoint(250, 250),
			QSize(500, 250)));

		Mat src;
		Mat src1;
		Mat src2;
		// use default camera as video source

		capture1 >> src;
		capture2 >> src1;
		capture3 >> src2;
		//--- INITIALIZE VIDEOWRITER
		VideoWriter writer;
		VideoWriter writer1;
		VideoWriter writer2;
		string filename1 = "./cam1.mp4";
		string filename2 = "./cam2.mp4";// name of the output video file
		string filename3 = "./cam3.mp4";// name of the output video file
		writer.open(filename1, 0x00000020, fps, src.size(), isColor);
		writer1.open(filename2, 0x00000020, fps, src1.size(), isColor);
		writer2.open(filename3, 0x00000020, fps, src2.size(), isColor);
		//--- GRAB AND WRITE LOOP
		while (isRecording)
		{
			// check if we succeeded
			if (!capture1.read(src)) {
				break;
			}
			if (!capture2.read(src1)) {
				break;
			}
			if (!capture3.read(src2)) {
				break;
			}
			resizedFrame = src(myROI);
			myLabel->setPixmap(convertImage(src));
			resizedFrame = src1(myROI);
			myLabel2->setPixmap(convertImage(src1));
			resizedFrame = src2(myROI);
			myLabel3->setPixmap(convertImage(src2));

			writer.write(src);
			writer1.write(src1);
			writer2.write(src2);
			// show live and wait for a key with timeout long enough to show images
			if (waitKey(5) >= 0)
				break;
		}
	}
	if (numCams == 4) {
		myLabel->setGeometry(QRect(QPoint(0, 0),
			QSize(500, 250)));
		myLabel2->setGeometry(QRect(QPoint(500, 0),
			QSize(500, 250)));
		myLabel->setGeometry(QRect(QPoint(0, 250),
			QSize(500, 250)));
		myLabel2->setGeometry(QRect(QPoint(500, 250),
			QSize(500, 250)));
		Mat src;
		Mat src1;
		Mat src2;
		Mat src3;
		// use default camera as video source

		capture1 >> src;
		capture2 >> src1;
		capture3 >> src2;
		capture4 >> src3;
		//--- INITIALIZE VIDEOWRITER
		VideoWriter writer;
		VideoWriter writer1;
		VideoWriter writer2;
		VideoWriter writer3;

		string filename1 = "./cam1.mp4";
		string filename2 = "./cam2.mp4";// name of the output video file
		string filename3 = "./cam3.mp4";// name of the output video file
		string filename4 = "./cam4.mp4";// name of the output video file
		writer.open(filename1, 0x00000020, fps, src.size(), isColor);
		writer1.open(filename2, 0x00000020, fps, src1.size(), isColor);
		writer2.open(filename3, 0x00000020, fps, src2.size(), isColor);
		writer3.open(filename4, 0x00000020, fps, src3.size(), isColor);
		//--- GRAB AND WRITE LOOP
		while (isRecording)
		{
			// check if we succeeded
			if (!capture1.read(src)) {
				break;
			}
			if (!capture2.read(src1)) {
				break;
			}
			if (!capture3.read(src2)) {
				break;
			}
			if (!capture3.read(src2)) {
				break;
			}
			// encode the frame into the videofile stream
			resizedFrame = src(myROI);
			myLabel->setPixmap(convertImage(src));
			resizedFrame = src1(myROI);
			myLabel2->setPixmap(convertImage(src1));
			resizedFrame = src2(myROI);
			myLabel3->setPixmap(convertImage(src2));
			resizedFrame = src3(myROI);
			myLabel4->setPixmap(convertImage(src3));

			writer.write(src);
			writer1.write(src1);
			writer2.write(src2);
			writer3.write(src3);
			// show live and wait for a key with timeout long enough to show images
			if (waitKey(5) >= 0)
				break;
		}
	}
	myLabel->setGeometry(QRect(QPoint(250, 0),
		QSize(720, 480)));
	myLabel2->setGeometry(QRect(QPoint(0, 0),
		QSize(0, 0)));
	myLabel3->setGeometry(QRect(QPoint(0, 0),
		QSize(0, 0)));
	myLabel4->setGeometry(QRect(QPoint(0, 0),
		QSize(0, 0)));
}

void ViewpointSynthesis::stitchVideos() {
	myLabel->setGeometry(QRect(QPoint(250, 0),
		QSize(720, 480)));
	bool isColor = (src.type() == CV_8UC3);
	double fps = 25.0;
	if (numCams == 2) {

		Mat src, src2;

		Ptr<Stitcher> stitcher = Stitcher::create();

		VideoCapture cap1("./cam1.mp4");
		VideoCapture cap2("./cam2.mp4");
		VideoWriter writer;
		Mat test;
		Mat test2;
		std::vector<Mat> imgs(2);

		string filename = "result.mp4";             // name of the output video file
		writer.open(filename, 0x00000020, fps, Size(720, 480), isColor);
		// check if we succeeded
		if (!writer.isOpened()) {
			cerr << "Could not open the output video file for write\n";
			QMessageBox::information(this, "Warning",
				"File error");
		}
		for (;;) {
			if (cap1.read(src)) {}
			else
				break;
			if (cap2.read(src2)) {}
			else
				break;
			imgs = { src, src2 };
			Stitcher::Status status = stitcher->stitch(imgs, test);
			if (!test.empty()) {
				cv::resize(test, test2, Size(720, 480), 0, 0);
			}
			else {
				break;
			}
			resizedFrame = test2(myROI);
			myLabel->setPixmap(convertImage(test2));
			writer.write(test2);
			char c = (char)waitKey(25);

			if (c == 27)
				break;
		}
	}
	if (numCams == 3) {
		Mat src, src2, src3;

		Ptr<Stitcher> stitcher = Stitcher::create();

		VideoCapture cap1("./cam1.mp4");
		VideoCapture cap2("./cam2.mp4");
		VideoCapture cap3("./cam3.mp4");
		VideoWriter writer;
		Mat test;
		Mat test2;
		std::vector<Mat> imgs(3);
		string filename = "result.mp4";             // name of the output video file
		writer.open(filename, 0x00000020, fps, Size(720, 480), isColor);
		// check if we succeeded
		if (!writer.isOpened()) {

			QMessageBox::information(this, "Warning",
				"File error");
		}
		for (;;) {
			if (cap1.read(src)) {}
			else
				break;
			if (cap2.read(src2)) {}
			else
				break;
			if (cap3.read(src3)) {}
			else
				break;
			imgs = { src, src2, src3 };
			Stitcher::Status status = stitcher->stitch(imgs, test);
			if (!test.empty()) {
				cv::resize(test, test2, Size(720, 480), 0, 0);
			}
			else {
				break;
			}
			resizedFrame = test2(myROIs);
			myLabel->setPixmap(convertImage(test2));
			writer.write(test2);
			char c = (char)waitKey(25);

			if (c == 27)
				break;
		}
	}
	if (numCams == 4) {
		Mat src, src2, src3, src4;

		Ptr<Stitcher> stitcher = Stitcher::create();

		VideoCapture cap1("./cam1.mp4");
		VideoCapture cap2("./cam2.mp4");
		VideoCapture cap3("./cam3.mp4");
		VideoCapture cap4("./cam4.mp4");
		VideoWriter writer;
		Mat test;
		Mat test2;
		std::vector<Mat> imgs(4);
		string filename = "result.mp4";             // name of the output video file
		writer.open(filename, 0x00000020, fps, Size(720, 480), isColor);
		// check if we succeeded
		if (!writer.isOpened()) {

			QMessageBox::information(this, "Warning",
				"File error");
		}
		for (;;) {
			if (cap1.read(src)) {}
			else
				break;
			if (cap2.read(src2)) {}
			else
				break;
			if (cap3.read(src3)) {}
			else
				break;
			if (cap4.read(src4)) {}
			else
				break;
			imgs = { src, src2, src3, src4 };
			Stitcher::Status status = stitcher->stitch(imgs, test);
			if (!test.empty()) {
				cv::resize(test, test2, Size(720, 480), 0, 0);
			}
			else {
				break;
			}
			resizedFrame = test2(myROI);
			myLabel->setPixmap(convertImage(test2));
			writer.write(test2);
			char c = (char)waitKey(25);

			if (c == 27)
				break;
		}
	}

}


void ViewpointSynthesis::playVideo() {
	Mat src;
	//Reuses src
	VideoCapture cap1("./result.mp4");
	cap1 >> src;
	resizedFrame = src(myROIs);
	myLabel->setGeometry(QRect(QPoint(250, 0),
		QSize(720, 480)));
	myLabel->setPixmap(convertImage(src));
	for (;;) {

		// Capture frame-by-frame
		cap1 >> src;

		// If the frame is empty, break immediately
		if (src.empty())
			break;

		// Display the resulting frame
		myLabel->setPixmap(convertImage(src));

		// Press  ESC on keyboard to exit
		char c = (char)waitKey(25);

		if (c == 27)
			break;
	}
}

void ViewpointSynthesis::stitchedDisplay() {
	zoomIn->setEnabled(true);
	zoomOut->setEnabled(true);
	zoom = 1.0;
	isStopped = false;
	capturedFrame = stitchImages(left1, right1);
	blankROI = myROIs;
	resizedFrame = capturedFrame(myROIs);
	if (zoom == 2) {
		blankROI = myROI2;
	}
	if (numCams < 2) {
		QMessageBox::information(this, "Warning",
			"Not enough cameras");
	}
	else {
		for (;;) {
			capturedFrame = stitchImages(left1, right1);
			resizedFrame = capturedFrame(blankROI);
			myLabel->setGeometry(QRect(QPoint(250, 0),
				QSize(720, 480)));
			myLabel->setPixmap(convertImage(resizedFrame));
			int keypress = waitKey(1); //Prevent infinite loop
			while (isStopped) {
				int keypress = waitKey(1); //Prevent infinite loop
			}
		}
	}
}


//Handlers for buttons cam1-cam6
void ViewpointSynthesis::handleGridView()
{
	gridDisplay();
}
//Handlers for buttons cam1-cam6
void ViewpointSynthesis::handleCam1Button()
{
	singleDisplay(capture1);
}
void ViewpointSynthesis::handleCam2Button()
{
	singleDisplay(capture2);
}
void ViewpointSynthesis::handleCam3Button()
{
	singleDisplay(capture3);
}
void ViewpointSynthesis::handleCam4Button()
{
	singleDisplay(capture4);
}
void ViewpointSynthesis::handleCam5Button()
{
	singleDisplay(capture5);
}
void ViewpointSynthesis::handleCam6Button()
{
	singleDisplay(capture6);
}

//Stops the stream by setting isStopped to true
//If already stopped displays an error message
void ViewpointSynthesis::handleStopStream()
{
	if (isStopped == true) {
		QMessageBox::information(this, "Warning",
			"Stream already stopped");
	}
	isStopped = true;
}
//Starts the stream by setting isStopped to false
//If currently running displays an error message
void ViewpointSynthesis::handleStartStream()
{
	if (isStopped == false) {
		QMessageBox::information(this, "Warning",
			"Stream already running");
	}
	isStopped = false;
}
void ViewpointSynthesis::handleStopRecording()
{
	isRecording = false;
}
void ViewpointSynthesis::handleStitchedView()
{
	stitchedDisplay();
}

void ViewpointSynthesis::handleRecordButton()
{
	recordCameras();
}

void ViewpointSynthesis::handleStitchRecording()
{
	stitchVideos();
}

void ViewpointSynthesis::handlePlayRecording()
{
	playVideo();
}


//Handles the zoom out button and sets the proper zoom value
//If zoomed out too much, an error message will appear
void ViewpointSynthesis::handleZoomOut()
{
	if (zoom == 1) {
		QMessageBox::information(this, "Warning",
			"Cannot zoom out any further");
	}
	else {
		if (zoom == 3) {
			resizedFrame = capturedFrame(myROI2);
			myLabel->setPixmap(convertImage(capturedFrame));
			zoom = 2;
		}
		else {
			resizedFrame = capturedFrame(myROI);
			myLabel->setPixmap(convertImage(capturedFrame));
			zoom = 1;
		}

	}
}

//Handles the zoom in button and sets the proper zoom value
//If zoomed out too much, an error message will appear
void ViewpointSynthesis::handleZoomIn()
{
	if (zoom == 3) {
		QMessageBox::information(this, "Warning",
			"Cannot zoom in any further");
	}
	else {
		if (zoom == 1.0) {
			resizedFrame = capturedFrame(myROI2);
			myLabel->setPixmap(convertImage(capturedFrame));
			zoom = 2;
		}
		else {
			resizedFrame = capturedFrame(myROI3);
			myLabel->setPixmap(convertImage(capturedFrame));
			zoom = 3;
		}

	}
}

//This handles the exit button and releases the camera data and exits the program
void ViewpointSynthesis::handleExitButton()
{
	capture1.release();
	capture2.release();
	capture3.release();
	capture4.release();
	capture5.release();
	capture6.release();
	exit(0);
}