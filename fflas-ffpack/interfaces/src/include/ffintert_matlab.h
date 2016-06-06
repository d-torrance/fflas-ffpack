/* ----------------------------------------------------------------- -*- C++ -*-
 *
 *  Copyright (c) SILKAN - 2014-2016 - Thomas IZARD
 *
 * -----------------------------------------------------------------------------
 *
 *  This file is part of FFinter.
 * 
 *  FFinter is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published 
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  FFinter is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 * 
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with FFinter.  If not, see <http://www.gnu.org/licenses/>.
 *
 * -----------------------------------------------------------------------------
 *
 *  The FFinter software is part of the HPAC project.
 *  The HPAC project is granted by the french "Agence Nationale de la 
 *  Recherche" (ANR).
 *
 * -------------------------------------------------------------------------- */

#ifndef _FFINTERT_MATLAB_H_
#define _FFINTERT_MATLAB_H_

// -----------------------------------------------------------------------------

#include "ffintert.h"
#include "mat.h"
#include "mex.h"
#include "matrix.h"

// -----------------------------------------------------------------------------

#include <cstring>

// -----------------------------------------------------------------------------

namespace ffintert {

  typedef          char  int8;
  typedef unsigned char  uint8;
  typedef          short int16;
  typedef unsigned short uint16;
  typedef          int   int32;
  typedef unsigned int   uint32;
  typedef long long int  int64;
  typedef unsigned long long int uint64;


  void error(const std::string& txt) 
  {
    mexErrMsgIdAndTxt("fflaswgrt:error", txt.c_str());
  }

  // ---------------------------------------------------------------------------

  bool isBool(const mxArray* array) { return mxIsLogical(array); }

  // ---------------------------------------------------------------------------

  bool isFlag(const mxArray* array) { return mxIsChar(array); }

  // ---------------------------------------------------------------------------

  bool isSize(const mxArray* array) 
  { 
    return (mxIsUint32(array) || mxIsUint64(array) || mxIsUint16(array) || mxIsUint8(array)); 
  }

  // ---------------------------------------------------------------------------

  bool isField(const mxArray* array) { return mxIsDouble(array) || mxIsSingle(array); }

  // ---------------------------------------------------------------------------

  bool isReal(const mxArray* array) { return mxIsDouble(array) || mxIsSingle(array); }

  // ---------------------------------------------------------------------------

  bool isInt(const mxArray* array) { return
      mxIsDouble(array)
      || mxIsInt32(array) || mxIsUint32(array) 
      || mxIsInt64(array) || mxIsUint64(array)
      || mxIsInt16(array) || mxIsUint16(array)
      || mxIsInt8(array)  || mxIsUint8(array); }

  // ---------------------------------------------------------------------------

  bool isElement(const mxArray* array) { return mxIsDouble(array); }

  // ---------------------------------------------------------------------------

  bool isScalar(const mxArray* array) { return mxGetNumberOfElements(array) == 1; }

  // ---------------------------------------------------------------------------

  bool isVector(const mxArray* array) 
  {
    return mxGetNumberOfDimensions(array) == 2 && 
      (mxGetDimensions(array)[0] == 1 || mxGetDimensions(array)[1] == 1);
  }

  // ---------------------------------------------------------------------------

  bool isMatrix(const mxArray* array) 
  {
    return mxGetNumberOfDimensions(array) == 2;
  }

  // ---------------------------------------------------------------------------
  
  bool           getBool(const mxArray* array) 
  {
    return mxGetLogicals(array)[0];
  }

  // ---------------------------------------------------------------------------
  
  // Generate getOrientation, getUplo...
#define GEN_GET_FLAG(__TYPE__, __LETTER__, __S1__, __S2__)	\
  __TYPE__ get##__LETTER__(const mxArray* array)		\
  {								\
    mwSize dim = mxGetNumberOfElements(array);			\
    char str[dim+1];						\
    if (mxGetString(array, str, dim+1))				\
      error("can not read string.");				\
    if (!strcmp(str, #__S1__))					\
      return __S1__;						\
    else if (!strcmp(str, #__S2__))				\
      return __S2__;						\
    error("unknown flag '" + std::string(str) + "'.");		\
    return __S1__;						\
  }

  GEN_GET_FLAG(FlagTrans, Orientation, NoTrans, Trans);
  GEN_GET_FLAG(FlagUplo,  Uplo,  Upper,   Lower);
  GEN_GET_FLAG(FlagSide,  Side,  Left,    Right);
  GEN_GET_FLAG(FlagDiag,  Diag,  NonUnit, Unit);

  // Generate isOrientation, isUplo...
#define GEN_IS_FLAG(__LETTER__, __S1__, __S2__)			\
  bool is##__LETTER__(const mxArray* array)			\
  {								\
    mwSize dim = mxGetNumberOfElements(array);			\
    char str[dim+1];						\
    mxGetString(array, str, dim+1);				\
    return (!strcmp(str, #__S1__) || !strcmp(str, #__S2__));	\
  }

  GEN_IS_FLAG(Orientation, NoTrans, Trans);
  GEN_IS_FLAG(Uplo,  Upper,   Lower);
  GEN_IS_FLAG(Side,  Left,    Right);
  GEN_IS_FLAG(Diag,  NonUnit, Unit);


  // ---------------------------------------------------------------------------

  size_t         getSize(const mxArray* array) 
  {
    size_t ret = 0;
    switch (mxGetClassID(array)) {
    case mxLOGICAL_CLASS: ret = (size_t) mxGetLogicals(array)[0]; break;
    case mxDOUBLE_CLASS:  ret = (size_t) mxGetPr(array)[0]; break;      
    case mxSINGLE_CLASS:  ret = (size_t) ((float*) mxGetData(array))[0]; break;
    case mxINT8_CLASS:    ret = (size_t) ((int8*)  mxGetData(array))[0]; break;
    case mxUINT8_CLASS:   ret = (size_t) ((uint8*) mxGetData(array))[0]; break;
    case mxINT16_CLASS:   ret = (size_t) ((int16*) mxGetData(array))[0]; break;
    case mxUINT16_CLASS:  ret = (size_t) ((uint16*)mxGetData(array))[0]; break;
    case mxINT32_CLASS:   ret = (size_t) ((int32*) mxGetData(array))[0]; break;
    case mxUINT32_CLASS:  ret = (size_t) ((uint32*)mxGetData(array))[0]; break;
    case mxINT64_CLASS:   ret = (size_t) ((int64*) mxGetData(array))[0]; break;
    case mxUINT64_CLASS:  ret = (size_t) ((uint64*)mxGetData(array))[0]; break;
    default:
      error("can not get 'size_t' value from '" + std::string(mxGetClassName(array)) + "'.");
      break;
    }
    return ret;
  }

  // ---------------------------------------------------------------------------

  double         getField(const mxArray* array) 
  {
    return mxGetPr(array)[0];
  }

  // ---------------------------------------------------------------------------

  double         getElement(const mxArray* array) 
  {
    return mxGetPr(array)[0];
  }

  // ---------------------------------------------------------------------------

  double         getReal(const mxArray* array) 
  {
    return mxGetPr(array)[0];
  }

  // ---------------------------------------------------------------------------

  int            getInt(const mxArray* array) 
  {
    if (mxIsDouble(array)) (int) ((double*)mxGetData(array))[0];
    return ((int*)mxGetData(array))[0];
  }

  // ---------------------------------------------------------------------------

  Vector<double> getVector(const mxArray* array) 
  {
    return createVector(mxGetPr(array), mxGetNumberOfElements(array));
  }

  // ---------------------------------------------------------------------------

  Matrix<double> getMatrix(const mxArray* array) 
  {
    return createMatrix(mxGetPr(array), mxGetM(array), mxGetN(array), CMO);
  }

  // ---------------------------------------------------------------------------

  Matrix<double> createMatrix (mxArray*& array, size_t m, size_t n) 
  {
    array = mxCreateDoubleMatrix(m, n, mxREAL);
    return createMatrix(mxGetPr(array), m, n, CMO);
  }

  // ---------------------------------------------------------------------------

  Vector<double> createVector (mxArray*& array, size_t l) 
  {
    array = mxCreateDoubleMatrix(l, 1, mxREAL);
    return createVector(mxGetPr(array), l);
  }

  // ---------------------------------------------------------------------------

  void        createAndSetBool   (mxArray*& array, bool el) 
  {
    array = mxCreateLogicalScalar(el);    
  }

  // ---------------------------------------------------------------------------

  void        createAndSetReal   (mxArray*& array, double el) 
  {
    array = mxCreateDoubleScalar(el);
  }

  // ---------------------------------------------------------------------------

  void         createAndSetElement(mxArray*& array, double el) 
  {
    array = mxCreateDoubleMatrix(1, 1, mxREAL);
    mxGetPr(array)[0] = el;
  }

  // ---------------------------------------------------------------------------

  Vector<double> duplicateVector(const mxArray* src, mxArray*& ret)
  {
    ret = mxDuplicateArray(src);
    return createVector(mxGetPr(ret), mxGetNumberOfElements(ret));
  }
  
  // ---------------------------------------------------------------------------

  Matrix<double> duplicateMatrix(const mxArray* src, mxArray*& ret)
  {
    ret = mxDuplicateArray(src);
    return createMatrix(mxGetPr(ret), mxGetM(ret), mxGetN(ret), CMO);
  }
  
};

// -----------------------------------------------------------------------------

#endif // _FFINTERT_MATLAB_H_
