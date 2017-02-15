/*
	docker-script: run script files inside Docker containers
	Copyright (c) 2017, Adam Rehn
	
	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:
	
	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/
#include "NvidiaDockerImages.h"

const vector<string>& NvidiaDockerImages::getList()
{
	//The list of docker images that require nvidia-docker to run
	static const vector<string> nvDockerImages = {
		
		//NVIDIA images
		"nvidia/cuda",
		"nvidia/caffe",
		"nvidia/digits",
		
		//Third-party images
		"bvlc/caffe:gpu",
		"dmlc/mxnet:cuda",
		"kaixhin/cuda-theano",
		"microsoft/cntk:latest",
		"tensorflow/tensorflow:latest-gpu"
	};
	
	return nvDockerImages;
}
