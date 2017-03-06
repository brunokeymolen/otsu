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


Otsu::Otsu(int w, int h) : w_(w), h_(h), size_(w * h) {

}

Otsu::~Otsu() {
}

void DrawBox(unsigned char *dst, int w, int x1, int y1, int x2, int y2, unsigned char color)
{
    for (int x=x1; x<x2; x++) {
        for (int y=y1; y<y2; y++) {
            int pos = x + (y * w);
            dst[pos] = color; 
        }
    }
}
    
unsigned char* Otsu::Threshold(const unsigned char* src, unsigned char* hist, unsigned char* result)
{
    memset(histogram_, 0, 255 * sizeof(int));

    for (int x = 0; x < w_; x++) {
        for (int y = 0; y < h_; y++) {
            int pos = x + (y * w_);
            histogram_[src[pos]]++;
        }
    }

    int max = 0;
    for (int z=0; z<255; z++) {
        if (histogram_[z] > max) {
            max = histogram_[z];
        }
    }

    double scale = (double)(h_ - 10) / (double)max;
    double bw = (w_ - 10) / 255;

    std::cout << "max: " << max << ", scale: " << scale << std::endl;
    for (int z=0; z<255; z++)
    {
        DrawBox(hist, w_, z*bw, h_-5-(int)(histogram_[z] * scale), (z*bw)+bw, h_-5, 200);
    }

    return hist;
}
}
