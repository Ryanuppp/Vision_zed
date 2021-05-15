#include "Vision_zed.h"
#include <QThread>
#include <QDateTime>
#include<QMessageBox>
#include <windows.h>
#include <iostream>
#include<direct.h>
#include <sstream>
#include <iomanip>
#include <io.h>
#include <chrono>


Vision_zed::Vision_zed(QWidget *parent)
    : QMainWindow(parent){
    mCamPro = new CameraProcessor();
    QThread *thread = new QThread();
    mCamPro->moveToThread(thread);

    ui.setupUi(this);
    setWindowTitle("Vision_zed");

    connect(thread, &QThread::finished, thread, &QObject::deleteLater);
    thread->start();

    play_timer = new QTimer(this);
    record_timer = new QTimer(this);
    progressBar_timer = new QTimer(this);
    fileDialog = new QFileDialog(this);
    
    is_record = false;
    is_process = false;
    is_open = false;
    is_start = false;
    progressBar_val = 0;
    brightness = 4;
    contrast = 4;
    saturation = 4;
    gain = 90;

    timeRecord = new QTime(0, 0, 0);
    ui.record_timer->setDigitCount(8);
    ui.record_timer->display(timeRecord->toString("hh:mm:ss"));
    
    ui.progressBar->setValue(0);


    connect(progressBar_timer, SIGNAL(timeout()), this, SLOT(updateProgressBar()));
    connect(play_timer, SIGNAL(timeout()), this, SLOT(readFrameSlot()));
    connect(record_timer, SIGNAL(timeout()), this, SLOT(updateRecordTime()));
    
    
    connect(this, SIGNAL(sendInputPath(std::string)), mCamPro, SLOT(setInputPath(std::string)));
    connect(this, SIGNAL(startProcess()), mCamPro, SLOT(startProcess()));
    
    connect(mCamPro, SIGNAL(sendFailedProcess()), this, SLOT(showFailedProcess()));
    connect(mCamPro, SIGNAL(sendNumOfFrames(int)), this, SLOT(setNumOfFrames(int)));
    connect(mCamPro, SIGNAL(sendPosition(int)), this, SLOT(setPosition(int)));
    connect(mCamPro, SIGNAL(endProcess()), this, SLOT(setEndProcess()));
    connect(mCamPro, SIGNAL(sendStartProcess()), this, SLOT(setStartProcess()));
    
    char* buffer;
    if ((buffer = _getcwd(NULL, 0)) == NULL){
      perror("error");
    }
    folderPath_input = std::string(buffer) + "\\sideToSide_video";
    folderPath_output = std::string(buffer) + "\\output";

    if (_access(folderPath_input.c_str(), 0) == -1)	//如果文件夹不存在
      _mkdir(folderPath_input.c_str());

    if (_access(folderPath_output.c_str(), 0) == -1)	//如果文件夹不存在
      _mkdir(folderPath_output.c_str());


}

void Vision_zed::showMessageBox(std::string str) {
  QMessageBox::about(NULL, "Tips", QString::fromLocal8Bit(str.c_str()));
}

int Vision_zed::openCamera() {
  // Set configuration parameters for the ZED
  InitParameters init_parameters;
  init_parameters.camera_resolution = RESOLUTION::HD720;
  init_parameters.depth_mode = DEPTH_MODE::ULTRA;
  init_parameters.camera_fps = 30;
  init_parameters.coordinate_units = UNIT::METER;
  // parseArgs(argc,argv,init_parameters);

   // Open the camera
  auto returned_state = zed.open(init_parameters);
  if (returned_state != ERROR_CODE::SUCCESS) {
    print("Camera Open", returned_state, "Exit program.");
    return EXIT_FAILURE;
  }  
  is_open = true;
  // Set runtime parameters after opening the camera
  RuntimeParameters runtime_parameters;
  runtime_parameters.sensing_mode = SENSING_MODE::STANDARD;
  return EXIT_SUCCESS;
}

void Vision_zed::closeCamera() {
  zed.close();
  is_open = false;
}

int Vision_zed::recordStart() {
  QDateTime current_date_time = QDateTime::currentDateTime();
  QString current_date = current_date_time.toString("yyyy_MM_dd_hh_mm_ss");
  String path_output((folderPath_input+"\\"+current_date.toStdString()+".svo").c_str());
  auto returned_state = zed.enableRecording(RecordingParameters(path_output, SVO_COMPRESSION_MODE::H264));
  if (returned_state != ERROR_CODE::SUCCESS) {
    print("Recording ZED : ", returned_state);
    zed.close();
    return EXIT_FAILURE;
  }
  is_record = true;
  sl::RecordingStatus rec_status;
  int frames_recorded = 0;
  for (int i = 0; i < 10;i++) {
    // Each new frame is added to the SVO file
    rec_status = zed.getRecordingStatus();
    if (rec_status.status)
      frames_recorded++;
    print("Frame count: " + std::to_string(frames_recorded));
  }
  return EXIT_SUCCESS;
}
void Vision_zed::recordStop() {
  zed.disableRecording();
  is_record = false;
}

void Vision_zed::setCameraParameter() {
  zed.setCameraSettings(VIDEO_SETTINGS::BRIGHTNESS, brightness);
  zed.setCameraSettings(VIDEO_SETTINGS::CONTRAST, contrast);
  zed.setCameraSettings(VIDEO_SETTINGS::SATURATION, saturation);
  zed.setCameraSettings(VIDEO_SETTINGS::GAIN, gain);
}

void Vision_zed::readFrame() {
  // Set runtime parameters after opening the camera
  RuntimeParameters runtime_parameters;
  runtime_parameters.sensing_mode = SENSING_MODE::STANDARD;

  // Prepare new image size to retrieve half-resolution images
  Resolution image_size = zed.getCameraInformation().camera_configuration.resolution;
  int new_width = image_size.width / 2;
  int new_height = image_size.height / 2;

  Resolution new_image_size(new_width, new_height);

  Mat image_zed_L(new_width, new_height, MAT_TYPE::U8_C4);
  Mat image_zed_R(new_width, new_height, MAT_TYPE::U8_C4);
  leftFrame = slMat2cvMat(image_zed_L);
  rightFrame = slMat2cvMat(image_zed_R);

#ifndef HAVE_CUDA // If no cuda, use CPU memory
  Mat depth_image_zed(new_width, new_height, MAT_TYPE::U8_C4);
  depthFrame = slMat2cvMat(depth_image_zed);
#else
  Mat depth_image_zed_gpu(new_width, new_height, MAT_TYPE::U8_C4, sl::MEM::GPU); // alloc sl::Mat to store GPU depth image
  cv::cuda::GpuMat depth_image_ocv_gpu = slMat2cvMatGPU(depth_image_zed_gpu); // create an opencv GPU reference of the sl::Mat
  cv::Mat depth_image_ocv; // cpu opencv mat for display purposes
#endif

  if (zed.grab(runtime_parameters) == ERROR_CODE::SUCCESS) {
    // Retrieve the left image, depth image in half-resolution
    zed.retrieveImage(image_zed_L, VIEW::LEFT, MEM::CPU, new_image_size);
    zed.retrieveImage(image_zed_R, VIEW::RIGHT, MEM::CPU, new_image_size);

#ifndef HAVE_CUDA 
    // retrieve CPU -> the ocv reference is therefore updated
    zed.retrieveImage(depth_image_zed, VIEW::DEPTH, MEM::CPU, new_image_size);
#else
    // retrieve GPU -> the ocv reference is therefore updated
    zed.retrieveImage(depth_image_zed_gpu, VIEW::DEPTH, MEM::GPU, new_image_size);
#endif    
    // Display image and depth using cv:Mat which share sl:Mat data


#ifdef HAVE_CUDA
    // download the Ocv GPU data from Device to Host to be displayed
    depth_image_ocv_gpu.download(depth_image_ocv);
#endif
    //cv::imshow("left", leftFrame);
    //cv::Mat a = cv::imread("photo/1.jpg");
    cv::cvtColor(leftFrame, leftFrame, cv::COLOR_BGR2RGB);
    cv::cvtColor(rightFrame, rightFrame, cv::COLOR_BGR2RGB);
    cv::cvtColor(depthFrame, depthFrame, cv::COLOR_BGR2RGB);
    QImage image_L = QImage((const uchar*)leftFrame.data, leftFrame.cols, leftFrame.rows, QImage::Format_RGB888);
    ui.left->setPixmap(QPixmap::fromImage(image_L));

    QImage image_R = QImage((const uchar*)rightFrame.data, rightFrame.cols, rightFrame.rows, QImage::Format_RGB888);
    ui.right->setPixmap(QPixmap::fromImage(image_R));

    QImage image_D = QImage((const uchar*)depthFrame.data, depthFrame.cols, depthFrame.rows, QImage::Format_RGB888);
    ui.depth->setPixmap(QPixmap::fromImage(image_D));
    //cv::waitKey(10);
  
}
#ifdef HAVE_CUDA
  // sl::Mat GPU memory needs to be free before the zed
  depth_image_zed_gpu.free();
#endif
}


//SLOT
void Vision_zed::onBtn_openCamera() {
  if (!is_open) {
    if(openCamera()==EXIT_SUCCESS)
      play_timer->start(25);
    else
      showMessageBox("相机打开失败！");
  }
  else 
    showMessageBox("相机已打开！");
  
}

void Vision_zed::onBtn_closeCamera() {
  if (is_open) {
    play_timer->stop();
    closeCamera();
  }
  else
    showMessageBox("请先打开相机!");
}

void Vision_zed::onBtn_startProcess() {
  std::string path = ui.inputPath->text().toStdString();
  if (!is_process) {
    is_process = !is_process;
    emit sendInputPath(path);
    emit startProcess();
    numOfFrames = 0;
    svo_position = 0;
    progressBar_val = 0;
    progressBar_timer->start(1000);
  }
  else
    showMessageBox("正在合成中...");
}

void Vision_zed::onBtn_record() {
  if (is_open) {
    if (!is_record) {
      //mCamPro->setCamera(zed);
      //emit startRecord(zed);
      if(recordStart()==EXIT_SUCCESS)
      //记录时间 Timer更新
        record_timer->start(1000);
      else
        showMessageBox("开启录像模式失败!");
    }
    else {
      recordStop();
      record_timer->stop();
      timeRecord->setHMS(0, 0, 0);
      ui.record_timer->display(timeRecord->toString("hh:mm:ss"));
    }
    
  }
  else
    showMessageBox("请先打开相机!");
}
void Vision_zed::onBtn_selectInputPath() {
  QString fileName= QFileDialog::getOpenFileName(this, tr("选择输入视频文件"), tr("."), tr("视频文件(*.svo);;"));
  ui.inputPath->setText(fileName);
}

void Vision_zed::onBtn_setCameraParameter() {
  if (is_open) {
    brightness = ui.brightness->value();
    contrast = ui.contrast->value();
    gain = ui.gain->value();
    saturation = ui.saturation->value();
    setCameraParameter();
  }
  else
    showMessageBox("请先打开相机!");
}
void Vision_zed::onBtn_setCameraParameter_default() {
  if (is_open) {
    brightness = 4;
    contrast = 4;
    gain = 50;
    saturation = 4;
    ui.brightness->setValue(brightness);
    ui.contrast->setValue(contrast);
    ui.gain->setValue(gain);
    ui.saturation->setValue(saturation);
    setCameraParameter();
  }
  else
    showMessageBox("请先打开相机!");
}

void Vision_zed::readFrameSlot() {
  readFrame();
}

void Vision_zed::setEndProcess() {
  is_process = !is_process;
  progressBar_timer->stop();
  showMessageBox("合成完成!");
  ui.progressBar->setValue(0);
}

void Vision_zed::setStartProcess() {
  is_process = !is_process;
}

void Vision_zed::updateRecordTime() {
  *timeRecord = timeRecord->addSecs(1);
  ui.record_timer->display(timeRecord->toString("hh:mm::ss"));
}

void Vision_zed::setNumOfFrames(int num) {
  numOfFrames = num;
}

void Vision_zed::setPosition(int position) {
  svo_position = position;
}

void Vision_zed::updateProgressBar() {
  if (numOfFrames != 0) {
    int pbValue = (float)svo_position / numOfFrames *100;
    if (pbValue != progressBar_val) {
      progressBar_val = pbValue;
      ui.progressBar->setValue(progressBar_val);
    }
  }
}
void Vision_zed::showFailedProcess() {
  showMessageBox("打开文件失败！请检查输入文件目录!");
}

