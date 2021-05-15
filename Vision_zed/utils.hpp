#pragma once
#ifndef _UTILS_HPP
#define _UTILS_HPP

#include<iostream>
#include <sl/Camera.hpp>
#include <opencv2/opencv.hpp>

using namespace sl;

#if defined (__OPENCV_ALL_HPP__) || defined(OPENCV_ALL_HPP)
// Conversion function between sl::Mat and cv::Mat
static cv::Mat slMat2cvMat(sl::Mat& input) {
  int cv_type = -1;
  switch (input.getDataType()) {
  case sl::MAT_TYPE::F32_C1: cv_type = CV_32FC1; break;
  case sl::MAT_TYPE::F32_C2: cv_type = CV_32FC2; break;
  case sl::MAT_TYPE::F32_C3: cv_type = CV_32FC3; break;
  case sl::MAT_TYPE::F32_C4: cv_type = CV_32FC4; break;
  case sl::MAT_TYPE::U8_C1: cv_type = CV_8UC1; break;
  case sl::MAT_TYPE::U8_C2: cv_type = CV_8UC2; break;
  case sl::MAT_TYPE::U8_C3: cv_type = CV_8UC3; break;
  case sl::MAT_TYPE::U8_C4: cv_type = CV_8UC4; break;
  default: break;
  }
  // Since cv::Mat data requires a uchar* pointer, we get the uchar1 pointer from sl::Mat (getPtr<T>())
  // cv::Mat and sl::Mat will share a single memory structure
  return cv::Mat(input.getHeight(), input.getWidth(), cv_type, input.getPtr<sl::uchar1>(sl::MEM::CPU));
}
#endif

// Display progress bar
static void ProgressBar(float ratio, unsigned int w) {
  unsigned int c = ratio * w;
  for (unsigned int x = 0; x < c; x++) std::cout << "=";
  for (unsigned int x = c; x < w; x++) std::cout << " ";
  std::cout << (int)(ratio * 100) << "% ";
  std::cout << "\r" << std::flush;
}

static bool directoryExists(std::string diectory) {
  struct stat info;
  if (stat(diectory.c_str(), &info) != 0)
    return false;
  else if (info.st_mode & S_IFDIR)  // S_ISDIR() doesn't exist on my windows 
    return true;
  else
    return false;
}

static void print(std::string msg_prefix, ERROR_CODE err_code = ERROR_CODE::SUCCESS, std::string msg_suffix = "");

static void print(std::string msg_prefix, ERROR_CODE err_code, std::string msg_suffix) {
  if (err_code != ERROR_CODE::SUCCESS)
    std:: cout << "[Error] ";
  std::cout << msg_prefix << " ";
  if (err_code != ERROR_CODE::SUCCESS) {
    std::cout << " | " << toString(err_code) << " : ";
    std::cout << toVerbose(err_code);
  }
  if (!msg_suffix.empty())
    std::cout << " " << msg_suffix;
  std::cout << std::endl;
}

#endif