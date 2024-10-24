#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int x_robot = 0.0f;
int y_robot = 0.0f;

int last_object_x = 0.0f;
int last_object_y = 0.0f;

int main() {
    VideoCapture cap("/home/gilbran/MagangIRIS/OpenCV/MagangIRIS-OpenCV/element/robotcamera.avi");
    if (!cap.isOpened()) {
        return -1;
    }

    Mat frame, hsv_frame, mask;

    int temp = true;
    while (true) {
        cap >> frame;
        if (frame.empty()) {
            break;
        }

        cvtColor(frame, hsv_frame, COLOR_BGR2HSV);
        inRange(hsv_frame, Scalar(5, 150, 150), Scalar(15, 255, 255), mask);

        vector<vector<Point>> contours;
        findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        if (!contours.empty()) {
            size_t largest_contour_index = 0;
            double largest_area = 0.0;

            for (size_t i = 0; i < contours.size(); i++) {
                double area = contourArea(contours[i]);
                if (area > largest_area) {
                    largest_area = area;
                    largest_contour_index = i;
                }
            }

            vector<Point> largest_contour = contours[largest_contour_index];

            Moments M = moments(largest_contour);
            if (M.m00 != 0) {
                int cx = int(M.m10 / M.m00);
                int cy = int(M.m01 / M.m00);

                int object_x = cx * 10.0f;
                int object_y = cy * 10.0f;

                if (temp)
                {
                    last_object_x = object_x;
                    last_object_y = object_y;
                    temp = false;
                }

                x_robot -= object_x - last_object_x;
                y_robot += object_y - last_object_y;

                cout << x_robot << " " << y_robot << endl;
                cout << object_x << " " << last_object_x << endl;
                cout << object_y << " " << last_object_y << endl << endl;

                last_object_x = object_x;
                last_object_y = object_y;

                putText(frame, "Robot Position: (" + to_string(x_robot) + " cm, " + to_string(y_robot) + " cm)", Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 255, 255), 2);

                circle(frame, Point(cx, cy), 5, Scalar(255, 0, 0), -1);
            }
        }
        imshow("Robot Position", frame);

        if (waitKey(30) == 'q') {
            break;
        }
    }

    cap.release();
    destroyAllWindows();
    return 0;
}