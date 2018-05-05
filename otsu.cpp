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
#include "otsu.hpp"

#include <cmath>
#include <iostream>

#include <stdlib.h>
#include <string.h>
#include <cstdint>
#include <iomanip>

namespace keymolen {

Otsu::Otsu(int w, int h) : w_(w), h_(h), size_(w * h) {}

Otsu::~Otsu() {}

void DrawBox(unsigned char* dst, int w, int x1, int y1, int x2, int y2,
             unsigned char color) {
    for (int x = x1; x < x2; x++) {
        for (int y = y1; y < y2; y++) {
            int pos = x + (y * w);
            dst[pos] = color;
        }
    }
}

unsigned char* Otsu::Threshold(const unsigned char* src, unsigned char* hist,
                               unsigned char* result) {
    memset(histogram_, 0, 255 * sizeof(int));


    //create histogram
    int histmax = 0;
    for (int x = 0; x < w_; x++) {
        for (int y = 0; y < h_; y++) {
            int pos = x + (y * w_);
            histogram_[src[pos]]++;
            //if(histogram_[src[pos]] > histmax)
            //{
            //    histmax = histogram_[src[pos]];
            //}
        }
    }

    for (int z=0; z<255; z++)
    {
        if (histogram_[z] > histmax)
        {
            histmax = histogram_[z];
        }
    }

    double total = w_ * h_;
    double tot = 0.0;
    double wb = 0.0;
    double wf = 0.0;
    double btot = 0.0;
    double max = 0.0;
    int t = 0;
   
    for (int x=0; x<255; x++)
    {
        tot += (double)(histogram_[x] * x);
    }
    
    for (int x=0; x<255; x++)
    {
        wb += histogram_[x];
        if (wb == 0)
        {
            continue;
        }
        wf = total - wb;
        if (wf == 0)
        {
            break;
        }

        btot += (double)(x * histogram_[x]);

        double ub = btot / wb;
        double uf = (tot - btot) / wf;

        double variance = wb * wf * (ub - uf) * (ub - uf);
        if (variance > max)
        {
            max = variance;
            t = x;
        }

     }

    //Draw histogram
    double scale = (double)(HISTOGRAM_H - 10) / (double)histmax;
    double bw = (HISTOGRAM_W - 10) / 255;
    for (int z = 0; z < 255; z++) {
        DrawBox(hist, HISTOGRAM_W, 
                (z * bw) + 10, HISTOGRAM_H - 5 - (int)(histogram_[z] * scale), 
                (z * bw) + bw + 10, HISTOGRAM_H - 5, 
                200);
    }

    DrawBox(hist, HISTOGRAM_W, 
                (t * bw) + 10, 0, 
                (t * bw) + bw + 10, HISTOGRAM_H, 
                250);
 
    
    for (int x = 0; x < w_; x++) {
        for (int y = 0; y < h_; y++) {
            int pos = x + (y * w_);
            if (src[pos] > t) 
            {
                result[pos] = 255;
            }
            else
            {
                result[pos] = 0;
            }
        }
    }


    return hist;
}
}
