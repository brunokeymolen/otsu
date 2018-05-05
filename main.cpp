// **********************************************************************************
//
// BSD License.
// This file is part of an Otsu implementation.
//
// Copyright (c) 2017, Bruno Keymolen, email: bruno.keymolen@gmail.com
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification,
// are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
// Redistributions in binary form must reproduce the above copyright notice,
// this
// list of conditions and the following disclaimer in the documentation and/or
// other
// materials provided with the distribution.
// Neither the name of "Bruno Keymolen" nor the names of its contributors may be
// used to endorse or promote products derived from this software without
// specific
// prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// **********************************************************************************
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <map>
#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "otsu.hpp"

extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;

std::string img_path;

const char* CW_IMG_ORIGINAL = "Original";
const char* CW_IMG_GRAY = "Grayscale";
const char* CW_IMG_HISTOGRAM = "Histogram";
const char* CW_IMG_RESULT = "Otsu";

void doTransform(std::string);

void usage(char* s) {
    fprintf(stderr, "\n");
    fprintf(stderr, "otsu; clustering-based image thresholding. build: %s-%s \n", __DATE__,
            __TIME__);
    fprintf(stderr,
            "%s -s <source file> [-l <low threshold>] [-h <high threshold>] "
            "[-? help]",
            s);
    fprintf(stderr, "   s: path image file\n");
    fprintf(stderr,
            "\nexample:  ./otsu -s img/harewood.jpg");
    fprintf(stderr, "\n");
}

int main(int argc, char** argv) {
    int c;
    while (((c = getopt(argc, argv, "s:l:h:?"))) != -1) {
        switch (c) {
            case 's':
                img_path = optarg;
                break;
            case 'l':
                //low_threshold = atoi(optarg);
                break;
            case 'h':
                //high_threshold = atoi(optarg);
                break;
            case '?':
            default:
                usage(argv[0]);
                return -1;
        }
    }

    if (img_path.empty()) {
        usage(argv[0]);
        return -1;
    }

    cv::namedWindow(CW_IMG_ORIGINAL, cv::WINDOW_AUTOSIZE);
    cv::namedWindow(CW_IMG_GRAY, cv::WINDOW_AUTOSIZE);
    cv::namedWindow(CW_IMG_HISTOGRAM, cv::WINDOW_AUTOSIZE);
    cv::namedWindow(CW_IMG_RESULT, cv::WINDOW_AUTOSIZE);

    cvMoveWindow(CW_IMG_ORIGINAL, 10, 10);
    cvMoveWindow(CW_IMG_GRAY, 680, 10);
    cvMoveWindow(CW_IMG_HISTOGRAM, 10, 500);
    cvMoveWindow(CW_IMG_RESULT, 680, 500);

    doTransform(img_path);


    return 0;
}

void doTransform(std::string file_path) {
    cv::Mat img_gray;
        
    cv::Mat img_ori = cv::imread(file_path, 1);
    cv::cvtColor(img_ori, img_gray, cv::COLOR_BGR2GRAY);

    int w = img_gray.cols;
    int h = img_ori.rows;

    cv::Mat img_histogram(keymolen::Otsu::HISTOGRAM_H, keymolen::Otsu::HISTOGRAM_W, CV_8UC1, cv::Scalar::all(0));
    cv::Mat img_result(h, w, CV_8UC1, cv::Scalar::all(0));



    while (1) {

        keymolen::Otsu otsu(w, h);
        otsu.Threshold(img_gray.data, img_histogram.data, img_result.data);

        // Visualize all
        cv::imshow(CW_IMG_ORIGINAL, img_ori);
        cv::imshow(CW_IMG_GRAY, img_gray);
        cv::imshow(CW_IMG_HISTOGRAM, img_histogram);
        cv::imshow(CW_IMG_RESULT, img_result);

       
        char c = cv::waitKey(360000);
        
        if (c == 's') {
            cv::imwrite("result.png", img_result);
            cv::imwrite("histogram.png", img_histogram);
        }


        if (c == 27) break;
    }
}
