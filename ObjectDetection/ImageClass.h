#include <opencv2/core/types.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/dnn/all_layers.hpp>
#include <fstream>

using namespace cv;
using namespace cv::dnn;

class ImageClass {
	const float INPUT_WIDTH{ 640.0 };
	const float INPUT_HEIGHT{ 640.0 };
	const float SCORE_THRESHOLD{ .5 };
	const float NMS_THRESHOLD{ .45 };
	const float CONFIDENCE_THRESHOLD{ .45 };
	// Text settings
	const float FONT_SCALE{ .7 };
	const int FONT_FACE = FONT_HERSHEY_SIMPLEX;
	const int THICKNESS{ 1 };
	Scalar BLACK{ Scalar(0,0,0) };
	Scalar BLUE{ Scalar(255,178,50) };
	Scalar YELLOW{ Scalar(0,255,255) };
	Scalar RED{ Scalar(0,0,255) };
public:
	void draw_label(Mat& input_image, std::string label, int left, int top);
	std::vector<Mat> pre_process(Mat& input_image, Net& net);
	Mat post_process(Mat& input_image, std::vector<Mat>& outputs, const std::vector<std::string>& class_name);
};