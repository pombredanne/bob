#!/usr/bin/env python
#
# vim: set fileencoding=utf-8 :
# Laurent El Shafey <Laurent.El-Shafey@idiap.ch>
# 27 Sept 2011

import os, sys
import unittest
import torch
import numpy

#############################################################################
# Tests blitz-based extrapolation implementation with values returned 
#############################################################################

########################## Values used for the computation ##################
eps = 1e-3
a5 = numpy.array([1,2,3,4,5], 'float64')
a14_zeros = numpy.array([0,0,0,0,1,2,3,4,5,0,0,0,0,0], 'float64')
a14_twos = numpy.array([2,2,2,2,1,2,3,4,5,2,2,2,2,2], 'float64')
a14_nearest = numpy.array([1,1,1,1,1,2,3,4,5,5,5,5,5,5], 'float64')
a14_circular = numpy.array([2,3,4,5,1,2,3,4,5,1,2,3,4,5], 'float64')
a14_mirror = numpy.array([4,3,2,1,1,2,3,4,5,5,4,3,2,1], 'float64')


A22 = numpy.array([1,2,3,4], 'float64').reshape(2,2)
A44_zeros = numpy.array([0,0,0,0,0,1,2,0,0,3,4,0,0,0,0,0], 'float64').reshape(4,4)
A44_twos = numpy.array([2,2,2,2,2,1,2,2,2,3,4,2,2,2,2,2], 'float64').reshape(4,4)
A44_nearest = numpy.array([1,1,2,2,1,1,2,2,3,3,4,4,3,3,4,4], 'float64').reshape(4,4)
A44_circular = numpy.array([4,3,4,3,2,1,2,1,4,3,4,3,2,1,2,1], 'float64').reshape(4,4)
A44_mirror = numpy.array([1,1,2,2,1,1,2,2,3,3,4,4,3,3,4,4], 'float64').reshape(4,4)
#############################################################################


def compare(v1, v2, width):
  return abs(v1-v2) <= width

def test_extrapolate_1D(res, reference, obj):
  # Tests the extrapolation
  obj.assertEqual(res.shape, reference.shape)
  for i in range(res.shape[0]):
    obj.assertTrue(compare(res[i], reference[i], eps))

def test_extrapolate_2D(res, reference, obj):
  # Tests the extrapolation
  obj.assertEqual(res.shape, reference.shape)
  for i in range(res.shape[0]):
    for j in range(res.shape[1]):
      obj.assertTrue(compare(res[i,j], reference[i,j], eps))


##################### Unit Tests ##################  
class ExtrapolationTest(unittest.TestCase):
  """Performs extrapolation product"""

##################### Convolution Tests ##################  
  def test_convolution_1D_zeros(self):
    b = numpy.zeros((14,), 'float64')
    torch.sp.extrapolateZero(a5,b)
    test_extrapolate_1D(b,a14_zeros,self)

  def test_convolution_1D_twos(self):
    b = numpy.zeros((14,), 'float64')
    torch.sp.extrapolateConstant(a5,b,2.)
    test_extrapolate_1D(b,a14_twos,self)

  def test_convolution_1D_nearest(self):
    b = numpy.zeros((14,), 'float64')
    torch.sp.extrapolateNearest(a5,b)
    test_extrapolate_1D(b,a14_nearest,self)

  def test_convolution_1D_circular(self):
    b = numpy.zeros((14,), 'float64')
    torch.sp.extrapolateCircular(a5,b)
    test_extrapolate_1D(b,a14_circular,self)

  def test_convolution_1D_mirror(self):
    b = numpy.zeros((14,), 'float64')
    torch.sp.extrapolateMirror(a5,b)
    test_extrapolate_1D(b,a14_mirror,self)

  def test_convolution_2D_zeros(self):
    B = numpy.zeros((4,4), 'float64')
    torch.sp.extrapolateZero(A22,B)
    test_extrapolate_2D(B,A44_zeros,self)

  def test_convolution_2D_twos(self):
    B = numpy.zeros((4,4), 'float64')
    torch.sp.extrapolateConstant(A22,B,2.)
    test_extrapolate_2D(B,A44_twos,self)

  def test_convolution_2D_nearest(self):
    B = numpy.zeros((4,4), 'float64')
    torch.sp.extrapolateNearest(A22,B)
    test_extrapolate_2D(B,A44_nearest,self)
  
  def test_convolution_2D_circular(self):
    B = numpy.zeros((4,4), 'float64')
    torch.sp.extrapolateCircular(A22,B)
    test_extrapolate_2D(B,A44_circular,self)
 

  def test_convolution_2D_mirror(self):
    B = numpy.zeros((4,4), 'float64')
    torch.sp.extrapolateMirror(A22,B)
    test_extrapolate_2D(B,A44_mirror,self)


##################### Main ##################  
if __name__ == '__main__':
  sys.argv.append('-v')
  if os.environ.has_key('TORCH_PROFILE') and \
      os.environ['TORCH_PROFILE'] and \
      hasattr(torch.core, 'ProfilerStart'):
    torch.core.ProfilerStart(os.environ['TORCH_PROFILE'])
  os.chdir(os.path.realpath(os.path.dirname(sys.argv[0])))
  unittest.main()
  if os.environ.has_key('TORCH_PROFILE') and \
      os.environ['TORCH_PROFILE'] and \
      hasattr(torch.core, 'ProfilerStop'):
    torch.core.ProfilerStop()
