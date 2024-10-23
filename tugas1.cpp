#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>

using namespace cv;
using namespace std;


float predict_distance(float height) {
    return 55.679389376205265 + -0.21288092526033187 * height + 0.00024869332309331032 * pow(height, 2);
}

int main() {
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        return -1;
    }

    while (true) {
        Mat frame, yuv, mask;

        cap >> frame;
        if (frame.empty()) {
            break;
        }

        cvtColor(frame, yuv, COLOR_BGR2YUV);
        inRange(yuv, Scalar(0, 0, 0), Scalar(255, 122, 122), mask);

        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(mask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        for (int i = 0; i < contours.size(); i++) {
            if (contourArea(contours[i]) > 500) {
                Rect bounding_rect = boundingRect(contours[i]);
                rectangle(frame, bounding_rect, Scalar(0, 255, 255), 2);

                float distance = predict_distance(bounding_rect.height);
                cout << bounding_rect.height << " " << distance << endl;

                putText(frame, "Distance: " + to_string(distance) + " cm", Point(bounding_rect.x, bounding_rect.y - 10), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);
            }
        }

        imshow("Camera", frame);
        imshow("Mask Green", mask);

        if (waitKey(1) == 'q') {
            break;
        }
    }

    cap.release();
    destroyAllWindows();
    return 0;
}