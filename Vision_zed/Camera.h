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
  void sendNumOfFrames(int num); // ������֡��
  void sendPosition(int position); // ���ʹ���Ľ���

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