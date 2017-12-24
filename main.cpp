#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat src; Mat src_gray;

bool checkEllipseShape(Mat src,vector<Point> contour,RotatedRect ellipse,double ratio=0.01)
{
	//get all the point on the ellipse point
	vector<Point> ellipse_point;

	//get the parameter of the ellipse
	Point2f center = ellipse.center;
	double a_2 = pow(ellipse.size.width*0.5,2);
	double b_2 = pow(ellipse.size.height*0.5,2);
	double ellipse_angle = (ellipse.angle*3.1415926)/180;
	

	//the uppart
	for(int i=0;i<ellipse.size.width;i++)
	{
		double x = -ellipse.size.width*0.5+i;
		double y_left = sqrt( (1 - (x*x/a_2))*b_2 );

		//rotate
        //[ cos(seta) sin(seta)]
        //[-sin(seta) cos(seta)]
        cv::Point2f rotate_point_left;
        rotate_point_left.x =  cos(ellipse_angle)*x - sin(ellipse_angle)*y_left;
        rotate_point_left.y = +sin(ellipse_angle)*x + cos(ellipse_angle)*y_left;

		//trans
		rotate_point_left += center;

		//store
		ellipse_point.push_back(Point(rotate_point_left));
	}
	//the downpart
	for(int i=0;i<ellipse.size.width;i++)
	{
		double x = ellipse.size.width*0.5-i;
		double y_right = -sqrt( (1 - (x*x/a_2))*b_2 );

		//rotate
        //[ cos(seta) sin(seta)]
        //[-sin(seta) cos(seta)]
        cv::Point2f rotate_point_right;
		rotate_point_right.x =  cos(ellipse_angle)*x - sin(ellipse_angle)*y_right;
        rotate_point_right.y = +sin(ellipse_angle)*x + cos(ellipse_angle)*y_right;

		//trans
		rotate_point_right += center;

		//store
		ellipse_point.push_back(Point(rotate_point_right));

	}


	vector<vector<Point> > contours1;
	contours1.push_back(ellipse_point);
	//drawContours(src,contours1,-1,Scalar(255,0,0),2);

	//match shape
	double a0 = matchShapes(ellipse_point,contour,CV_CONTOURS_MATCH_I1,0);  
	if (a0>0.01)
	{
		return true;      
	}

	return false;
}

/** @function main */
int main( int argc, char** argv )
{
	//load images
	src = imread( argv[1], 1 );

	// convert into gray
	cvtColor( src, src_gray, CV_BGR2GRAY );

	Mat threshold_output;
	vector<vector<Point> > contours;

	// find contours
	int threshold_value = threshold( src_gray, threshold_output, 0, 255, CV_THRESH_BINARY|CV_THRESH_OTSU);
	findContours( threshold_output, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);


	//fit ellipse
	vector<RotatedRect> minEllipse(contours.size());
  	for( int i = 0; i < contours.size(); i++ )
	{ 
		//point size check
		if(contours[i].size()<10)
		{
			continue;
		}

		//point area
		if(contourArea(contours[i])<10)
		{
			continue;
		}

		minEllipse[i] = fitEllipse(Mat(contours[i]));


		//check shape
		if(checkEllipseShape(src,contours[i],minEllipse[i]))
		{
			continue;
		}

        Scalar color = Scalar( 0, 0, 255);
		// ellipse
		
		ellipse( src, minEllipse[i], color, 2);

	}

	/// 结果在窗体中显示
	imwrite("ellipse.jpg",src);


  	return(0);
}
