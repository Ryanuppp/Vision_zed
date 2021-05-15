#include "Camera.h"
#include <opencv2/opencv.hpp>
#include <QDateTime>

using namespace sl;


CameraProcessor::CameraProcessor() {
  inputPath = "";
  exit_process = false;
}

void CameraProcessor::startProcess() {
  int status = process();
  if (status == EXIT_FAILURE) {
    emit sendFailedProcess();
  }
}

int CameraProcessor::process(){
    if (inputPath._Equal("")) {
      return  EXIT_FAILURE;
    }

    // Specify SVO path parameter
    InitParameters init_parameters;
    init_parameters.input.setFromSVOFile(inputPath.c_str());
    init_parameters.coordinate_units = UNIT::MILLIMETER;
    //init_parameters.enable_right_side_measure = true;

    // Open the camera
    ERROR_CODE zed_open_state = zed.open(init_parameters);
    if (zed_open_state != ERROR_CODE::SUCCESS) {
      print("Camera Open", zed_open_state, "Exit program.");
      return EXIT_FAILURE;
    }

    // Get image size
    Resolution image_size = zed.getCameraInformation().camera_configuration.resolution;

    Mat left_image(image_size, MAT_TYPE::U8_C4);
    cv::Mat left_image_ocv = slMat2cvMat(left_image);

    Mat right_image(image_size, MAT_TYPE::U8_C4);
    cv::Mat right_image_ocv = slMat2cvMat(right_image);


    Mat depth_image(image_size, MAT_TYPE::U8_C4);
    cv::Mat depth_image_ocv = slMat2cvMat(depth_image);

    Mat disp_image(image_size, MAT_TYPE::F32_C1);
    cv::Mat depth16_image_ocv = slMat2cvMat(disp_image);

    cv::Mat image_L = cv::Mat(image_size.height, image_size.width, CV_8UC3);
    cv::Mat image_R = cv::Mat(image_size.height, image_size.width, CV_8UC3);
    cv::Mat image_depth = cv::Mat(image_size.height, image_size.width, CV_8UC3);
    cv::Mat image_process = cv::Mat(image_size.height, image_size.width, CV_8UC3);

    // Create video writer
    cv::VideoWriter video_writer_left, video_writer_right, video_writer_3D;
    

    int fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy_MM_dd_hh_mm_ss");
    int frame_rate = fmax(zed.getInitParameters().camera_fps, 25); // Minimum write rate in OpenCV is 25
    video_writer_left.open("./output/"+ current_date.toStdString() +"_left.avi", fourcc, frame_rate, cv::Size(image_size.width, image_size.height));
    if (!video_writer_left.isOpened()) {
      print("Error: OpenCV video writer_left cannot be opened. Please check the .avi file path and write permissions.");
      zed.close();
      return EXIT_FAILURE;
    }
    video_writer_right.open("./output/" + current_date.toStdString() + "_right.avi", fourcc, frame_rate, cv::Size(image_size.width, image_size.height));
    if (!video_writer_right.isOpened()) {
      print("Error: OpenCV video writer_right cannot be opened. Please check the .avi file path and write permissions.");
      zed.close();
      return EXIT_FAILURE;
    }
    video_writer_3D.open("./output/" + current_date.toStdString() + "_3d.avi", fourcc, frame_rate, cv::Size(image_size.width, image_size.height));
    if (!video_writer_3D.isOpened()) {
      print("Error: OpenCV video writer_3D cannot be opened. Please check the .avi file path and write permissions.");
      zed.close();
      return EXIT_FAILURE;
    }
    
    RuntimeParameters rt_param;
    rt_param.sensing_mode = SENSING_MODE::FILL;

    int nb_frames = zed.getSVONumberOfFrames();
    emit sendNumOfFrames(nb_frames);
    int svo_position = 0;
    zed.setSVOPosition(svo_position);

    while (!exit_process) {
      sl::ERROR_CODE err = zed.grab(rt_param);
      if (err == ERROR_CODE::SUCCESS) {
        svo_position = zed.getSVOPosition();
        emit sendPosition(svo_position);
        // Retrieve SVO images
        zed.retrieveImage(left_image, VIEW::LEFT);
        zed.retrieveImage(right_image, VIEW::RIGHT);
        zed.retrieveImage(depth_image, VIEW::DEPTH);
        zed.retrieveMeasure(disp_image, MEASURE::DISPARITY);

        //disp_image.write("123");

        int numOfDisparity = 0;
        float dispf = 0;
       
          // Convert SVO image from RGBA to RGB
          //cv::cvtColor(left_image_ocv, image_sideByside(cv::Rect(0, 0, image_size.width, image_size.height)), cv::COLOR_BGRA2BGR);
          //cv::cvtColor(right_image_ocv, image_sideByside(cv::Rect(image_size.width, 0, image_size.width, image_size.height)), cv::COLOR_BGRA2BGR);
          // Write the RGB image in the video
          cv::cvtColor(left_image_ocv, image_L, cv::COLOR_BGRA2BGR);
          cv::cvtColor(right_image_ocv, image_R, cv::COLOR_BGRA2BGR);
          cv::cvtColor(depth_image_ocv, image_depth, cv::COLOR_BGRA2BGR);
          image_process = image_L.clone();

          // 红蓝通道转换
          for (int j = 0; j < image_size.height; j++) {
            uchar* p = image_process.ptr<uchar>(j);
            uchar* q = image_R.ptr<uchar>(j);
            for (int k = numOfDisparity; k < image_size.width; k++) {
              disp_image.getValue(j, k, &dispf);
              if (_finite(dispf)) {
                //左图BG通道转换为右图的bg通道
                dispf /= 16.f;
                p[k * 3] = q[(k + (int)dispf) * 3];
                p[k * 3 + 1] = q[(k + (int)dispf) * 3 + 1];
                //	p[k * 3 + 2] =q[(k-dispf) * 3 + 2];
              }
            }
          }
          video_writer_left.write(image_L);
          video_writer_right.write(image_R);
          video_writer_3D.write(image_process);
          ProgressBar((float)(svo_position / (float)nb_frames), 30);
        
       }        // Display progress
       
    else if (err == sl::ERROR_CODE::END_OF_SVOFILE_REACHED) {
        print("SVO end has been reached. Exiting now.");
        exit_process= true;
      }
      else {
        print("Grab Error: ", err);
        exit_process = true;
      }
    }
    
    // Close the video writer
    video_writer_left.release();
    video_writer_right.release();
    zed.close();
    emit endProcess();
    exit_process = false;
    return EXIT_SUCCESS;
}


void CameraProcessor::setCamera(Camera& zed) {
  this->zed = zed;
}

void CameraProcessor::setInputPath(std::string inputPath) {
  this->inputPath = inputPath;
}