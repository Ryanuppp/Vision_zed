#pragma once
#include<thread>
#include <qobject.h>
#include"utils.hpp"


class CameraProcessor :public QObject {
  Q_OBJECT

signals:
  void sendStartProcess();
  void endProcess();
  void sendFailedProcess();
  void sendNumOfFrames(int num); // 发送总帧数
  void sendPosition(int position); // 发送处理的进度

private:
  sl::Camera zed;
  bool exit_process;
  std::string inputPath;

public slots:
  void startProcess();
  void setInputPath(std::string path);

public:
  CameraProcessor();
  int process();
  void setCamera(Camera& zed);
};