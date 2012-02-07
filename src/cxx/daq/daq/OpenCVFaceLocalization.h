/**
 * @file cxx/daq/daq/OpenCVFaceLocalization.h
 * @date Thu Feb 2 11:22:57 2012 +0100
 * @author Francois Moulin <Francois.Moulin@idiap.ch>
 *
 * Copyright (C) 2011-2012 Idiap Reasearch Institute, Martigny, Switzerland
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef OPENCVFACELOCALIZATION_H
#define OPENCVFACELOCALIZATION_H

#include <daq/FaceLocalization.h>

#include <cv.h>

namespace bob { namespace daq {

/**
 * Provide face localization using OpenCV
 */
class OpenCVFaceLocalization:public FaceLocalization
{
public:
  OpenCVFaceLocalization(const char* model_path);
  virtual ~OpenCVFaceLocalization();
    
  virtual void imageReceived (blitz::Array<unsigned char, 2>& image, CaptureStatus& status);
  virtual bool start();
  virtual void stop();
  
  void localize();
  
private:
  pthread_t thread;
  
  cv::Mat img;
  pthread_mutex_t img_mutex;

  CvHaarClassifierCascade* cascade;

  bool mustStop;
};

}}

#endif // OPENCVFACELOCALIZATION_H