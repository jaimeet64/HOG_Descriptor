#include "opencv2/opencv.hpp"
#include <iostream>
#include<string.h>
#include <opencv2/core/core.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv/cv.h>

using namespace cv;
using namespace std;

int main (int argc, const char * argv[])
{
    string filename = "v1.mp4";     
    VideoCapture cap(filename);   // reading video file
	/*VideoCapture cap(CV_CAP_ANY);*/
	double fps = cap.get(CV_CAP_PROP_FPS);
	cout<<"fps"<<fps<<endl;
    cap.set(CV_CAP_PROP_FRAME_WIDTH,320);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);    
    if (!cap.isOpened())
        return -1;
 
    Mat img;
    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
	cap>>img;
	img.size();
	int row= img.rows;
	int col=img.cols;
	cout<<"rows"<<row<<"col"<<col<<endl;
    namedWindow("video capture", CV_WINDOW_AUTOSIZE);
	int d=0;
	vector<Rect> found;
	cv::Rect myROI(50, 50, 450, 400);
	cv::Mat crop = img(myROI);
    for(;;)
    {
			cap >>img;
			if(d%8==0)
			{
				if (!img.data )
					continue;
				hog.detectMultiScale(crop, found, 0, Size(1.5,2), Size(8,8), 1.05, 2);
			}
			int q=0;
			for (size_t i=0; i<found.size(); i++)
			{	
				Rect r = found[i];
		
				if ((abs(r.x) > 20) && (r.x < 500)&& (abs(r.y)> 100) ) 
				{
					r.x = abs(r.x)+ cvRound(r.width*0.1);
					r.y = abs(r.y)+cvRound(r.height*0.06);
					
					cout<<"X_co-ordinate"<<r.x<<"Y_co-ordinate"<<r.y<<endl;
					r.width = cvRound(r.width*0.8);
					if (r.height <100)
						r.height = cvRound(r.height*0.9);
					else
						r.height=100;
					if( q==0)
					
						rectangle(crop, r.tl(), r.br(), cv::Scalar(0,255,0), 2);  
					else
					{
						if (q==1)
							rectangle(crop, r.tl(), r.br(), cv::Scalar(255,0,0), 2);  
						else
							rectangle(crop, r.tl(), r.br(), cv::Scalar(0,0,255), 2);  
					}
					q=q+1;	
				}
			}
	
		d=d+1;
        imshow("video capture", img);
		cout<<"Frame number is"<<d<<endl;
        if (waitKey(20) >= 0)
            break;
	}
	
    return 0;
}