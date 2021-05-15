#pragma once

#include "ui_Vision_zed.h"
#include <QtWidgets/QMainWindow>
#include "Camera.h"
#include "QTimer"
#include "QTime"
#include "QImage"
#include "QFileDialog"
#include "QProgressBar"
#include <float.h>


class Vision_zed : public QMainWindow
{
    Q_OBJECT

public:
    Vision_zed(QWidget *parent = Q_NULLPTR);
   
    int openCamera();
    void closeCamera();
    void readFrame();
    int recordStart();
    void recordStop();
    void showMessageBox(std::string str);
    void setCameraParameter();
    
       
signals:
  void startProcess();
  void sendInputPath(std::string);
  //void showTips(int type);

public slots:
  void onBtn_startProcess();
  void onBtn_openCamera();
  void onBtn_closeCamera();
  void onBtn_record();
  void onBtn_selectInputPath();
  void onBtn_setCameraParameter();
  void onBtn_setCameraParameter_default();
  void updateRecordTime();
  void readFrameSlot();
  void setEndProcess();
  void setStartProcess();
  void updateProgressBar();
  void setNumOfFrames(int num);
  void setPosition(int position);
  void showFailedProcess();
 


private:
    Ui::Vision_zedClass ui;
    CameraProcessor* mCamPro;
    Camera zed;
    bool is_open, is_record, is_process, is_start; //相机是否打开，是否开始录像，是否开始合成, 是否开始计时
    cv::Mat leftFrame, rightFrame, depthFrame;
    QTimer* play_timer;
    QTimer* record_timer; 
    QTimer* progressBar_timer;
    QTime* timeRecord; //记录时间
    int svo_position;
    int numOfFrames;
    int progressBar_val;
    std::string inputPath;
    std::string outputPath;
    QFileDialog* fileDialog;
    std::string folderPath_input;
    std::string folderPath_output;

    int brightness;
    int contrast;
    int saturation;
    int gain;
};
