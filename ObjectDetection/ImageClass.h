#pragma once
#include <opencv2/opencv.hpp>
#include <fstream>

using namespace cv;
using namespace cv::dnn;

class ImageClass {
	// Image settings
	const float INPUT_WIDTH{ 640.0 };
	const float INPUT_HEIGHT{ 640.0 };
	const float SCORE_THRESHOLD{ .5 };
	const float NMS_THRESHOLD{ .45 };
	const float CONFIDENCE_THRESHOLD{ .45 };
	// Text settings
	const float FONT_SCALE = .7;
	const int FONT_FACE = FONT_HERSHEY_SIMPLEX;
	const int THICKNESS{ 1 };
	//
	Scalar BLACK{ Scalar(0,0,0) };
	Scalar BLUE{ Scalar(255,178,50) };
	Scalar YELLOW{ Scalar(0,255,255) };
	Scalar RED{ Scalar(0,0,255) };
	std::vector<std::string> class_list;
	// Load image.
	Mat frame;
	Net net;
public:
	ImageClass();
	~ImageClass();
	void draw_label(Mat& input_image, std::string label, int left, int top);
	std::vector<Mat> pre_process(Mat&& input_image, Net&& net);
	Mat post_process(Mat&& input_image, std::vector<Mat>& outputs, const std::vector<std::string>& class_name);
	std::vector<std::string> get_classes();
	void set_image(std::string image_uri);
	Net get_model();
	Mat get_image();
	float get_input_width();
	float get_input_height();
	float get_score_threshold();
	float get_nms_threshold();
	float get_confidence_threshold();
	float get_font_scale();
	int get_font_face();
	int get_thickness();
	Scalar get_red();
};