// cd project7
// cmake .
// make
// ./l071

//#include <opencv2/core.hpp>
//#include <opencv2/imgcodecs.hpp>
#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

void part1(int low, int high, std::string filename) {
    std::string suffix = ".jpg";
    if (filename.size() >= suffix.size() && filename.compare(filename.size() - suffix.size(), suffix.size(), suffix) == 0)
        filename = samples::findFile(filename);
    else
        filename = samples::findFile("coins1.jpg");
    
    if (low == 0 && high == 0) {
        low = 10;
        high = 100;
    }
    
    Mat img = imread(filename, IMREAD_COLOR);
    if(img.empty())
        std::cout << "Could not read the image: " << filename << std::endl;
    //imshow("Display window", img);
    
    Mat grayscale = imread(filename, IMREAD_GRAYSCALE);
    imwrite("imageg.jpg", grayscale);
    
    Mat blurred;
    blur(grayscale, blurred, Size(5,5));
    
    Mat detected_edges;
    cv::Canny(blurred, detected_edges, low, high);
    imwrite("imagef.jpg", detected_edges);
    
    int dimecount = 0;
    int pennycount = 0;
    int nickelcount = 0;
    int quartercount = 0;
    int silverdollarcount = 0;

    vector<Vec3f> houghcircles;
    HoughCircles(detected_edges, houghcircles, HOUGH_GRADIENT, 1, 50, 50, 30, 76, 130);
    for(size_t i = 0; i < houghcircles.size(); i++ ){
        Vec3i c = houghcircles[i];
        Point center = Point(c[0], c[1]);
        // circle center
        circle(img, center, 1, Scalar(0, 0, 0), 3, LINE_AA);
        circle(img, center, 2, Scalar(0, 0, 0), 3, LINE_AA);
        circle(img, center, 3, Scalar(0, 0, 0), 3, LINE_AA);
        circle(img, center, 4, Scalar(0, 0, 0), 3, LINE_AA);

        // circle outline: red for penny, purple for nickel, blue for dime, green for quarter, and yellow for silver dollar
        int radius = c[2];
        if (radius < 80){ // dime 
            circle(img, center, radius, Scalar(255, 0, 0), 3, LINE_AA);
            circle(img, center, radius + 1, Scalar(255, 0, 0), 3, LINE_AA);
            circle(img, center, radius + 2, Scalar(255, 0, 0), 3, LINE_AA);
            circle(img, center, radius + 3, Scalar(255, 0, 0), 3, LINE_AA);
            dimecount ++;
        }
        else if (radius < 95){ // penny 
            circle(img, center, radius, Scalar(0, 0, 255), 3, LINE_AA);
            circle(img, center, radius + 1, Scalar(0, 0, 255), 3, LINE_AA);
            circle(img, center, radius + 2, Scalar(0, 0, 255), 3, LINE_AA);
            circle(img, center, radius + 3, Scalar(0, 0, 255), 3, LINE_AA);
            pennycount ++;
        }
        else if (radius < 100){ // nickel 
            circle(img, center, radius, Scalar(255, 0, 255), 3, LINE_AA);
            circle(img, center, radius + 1, Scalar(255, 0, 255), 3, LINE_AA);
            circle(img, center, radius + 2, Scalar(255, 0, 255), 3, LINE_AA);
            circle(img, center, radius + 3, Scalar(255, 0, 255), 3, LINE_AA);
            nickelcount ++;
        }
        else if (radius < 120){ // quarter 
            circle(img, center, radius, Scalar(0, 255, 0), 3, LINE_AA);
            circle(img, center, radius + 1, Scalar(0, 255, 0), 3, LINE_AA);
            circle(img, center, radius + 2, Scalar(0, 255, 0), 3, LINE_AA);
            circle(img, center, radius + 3, Scalar(0, 255, 0), 3, LINE_AA);
            quartercount ++;
        }
        else{
            circle(img, center, radius, Scalar(0, 255, 255), 3, LINE_AA);
            circle(img, center, radius + 1, Scalar(0, 255, 255), 3, LINE_AA);
            circle(img, center, radius + 2, Scalar(0, 255, 255), 3, LINE_AA);
            circle(img, center, radius + 3, Scalar(0, 255, 255), 3, LINE_AA);
            silverdollarcount ++;
        }
    }
    imwrite("coins.jpg", img);
    
    ofstream output;
    double total = (dimecount * .1) + (pennycount * .01) + (nickelcount * .05) + (quartercount * .25) + (silverdollarcount * .5);
    output.open("results.txt");
    output << dimecount << " dimes, " << pennycount << " pennies, " << nickelcount << " nickels, " << quartercount << 
        " quarters, " << silverdollarcount << " silver dollars. Total sum: $" << total;
    output.close();
}

int main(int argc, char* argv[]) {
    part1(atoi(argv[2]), atoi(argv[4]), std::string(argv[6]));
}