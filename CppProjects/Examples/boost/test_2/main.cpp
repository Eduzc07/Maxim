#include <stdio.h>
#include <opencv2/opencv.hpp>
//Boost
//
//  Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#include <string>
#include <boost/asio.hpp>
#include <opencv2/core.hpp>

#include <boost/serialization/split_free.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <cassert>
#include <vector>
#include <iostream>

using namespace cv;
//using namespace std;

BOOST_SERIALIZATION_SPLIT_FREE( cv::Mat )

namespace boost {
namespace serialization {

template <class Archive>
void save( Archive & ar, const cv::Mat & m, const unsigned int version )
{
    size_t elemSize = m.elemSize();
    size_t elemType = m.type();

    ar & m.cols;
    ar & m.rows;
    ar & elemSize;
    ar & elemType;

    const size_t dataSize = m.cols * m.rows * m.elemSize();
    for ( size_t i = 0; i < dataSize; ++i )
        ar & m.data[ i ];
}


template <class Archive>
void load( Archive & ar, cv::Mat& m, const unsigned int version )
{
    int cols, rows;
    size_t elemSize, elemType;

    ar & cols;
    ar & rows;
    ar & elemSize;
    ar & elemType;

    m.create( rows, cols, static_cast< int >( elemType ) );
    const size_t dataSize = m.cols * m.rows * elemSize;
    for (size_t i = 0; i < dataSize; ++i)
        ar & m.data[ i ];
}

} // namespace serialization
} // namespace boost

std::string save( const cv::Mat & mat )
{
    std::ostringstream oss;
    boost::archive::text_oarchive toa( oss );
    toa << mat;

    return oss.str();
}

void load( cv::Mat & mat, const char * data_str )
{
    std::stringstream ss;
    ss << data_str;

    boost::archive::text_iarchive tia( ss );
    tia >> mat;
}

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    Mat image;
    image = imread( argv[1], 1 );

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
   // namedWindow("Display Image", WINDOW_AUTOSIZE );
  // imshow("Display Image", image);

   Mat dst = image.clone();

  for (int i=1;i<5;i=i+2){
   /// Applying Gaussian blur
	GaussianBlur(image, dst, Size(i,i), 0, 0 );
	printf("wait...\n");
        imshow("dst", dst);
	waitKey(200);
   }
   printf("finish...\n");

   imwrite( "New_Image.jpg", dst );

   waitKey(0);
   destroyWindow("dst");
   waitKey(500);

   //bost

    std::string serialized = save( dst );
    std::cout << "serialized   = " << serialized << std::endl;

    cv::Mat deserialized;
    load( deserialized, serialized.c_str() );
    std::cout << "deserialized = \n\n" << deserialized << std::endl;

    imshow("deserialized", deserialized);
    waitKey(0);
    destroyAllWindows();
    return 0;
}
