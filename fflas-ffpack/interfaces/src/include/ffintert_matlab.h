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

class Integer {};

template < typename T > struct MXWrapper {};
template < > struct MXWrapper<int32_t> {
  static const mxClassID classID = mxINT32_CLASS;
  static bool is(const mxArray* array) { return mxIsInt32(array); }
};

template < > struct MXWrapper<int64_t>
{
  static const mxClassID classID = mxINT64_CLASS;
  static bool is(const mxArray* array) { return mxIsInt64(array); }
};

template < > struct MXWrapper<float>   {
  static const mxClassID classID = mxSINGLE_CLASS;
  static bool is(const mxArray* array) { return mxIsSingle(array); }
};

template < > struct MXWrapper<double>  {
  static const mxClassID classID = mxDOUBLE_CLASS;
  static bool is(const mxArray* array) { return mxIsDouble(array); }
};

template < > struct MXWrapper<Integer>  {
  static const mxClassID classID = mxDOUBLE_CLASS;
  static bool is(const mxArray*) { return false; }
};

template < > struct MXWrapper<size_t>  {
  static const mxClassID classID = mxUINT64_CLASS;
  static bool is(const mxArray* array) { return mxIsUint64(array); }
};

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

  template < typename T >
  bool isElement(const mxArray* array) { return MXWrapper<T>::is(array); }

  // ---------------------------------------------------------------------------

  bool isScalar(const mxArray* array) { return mxGetNumberOfElements(array) == 1; }

  // ---------------------------------------------------------------------------

  template < typename T >
  bool isVector(const mxArray* array)
  {
    return MXWrapper<T>::is(array) && mxGetNumberOfDimensions(array) == 2 && (mxGetDimensions(array)[0] == 1 || mxGetDimensions(array)[1] == 1);
  }

  // ---------------------------------------------------------------------------

  template < typename T >
  bool isMatrix(const mxArray* array)
  {
    return MXWrapper<T>::is(array) && mxGetNumberOfDimensions(array) == 2;
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
      return Fflas##__S1__;					\
    else if (!strcmp(str, #__S2__))				\
      return Fflas##__S2__;					\
    error("unknown flag '" + std::string(str) + "'.");		\
    return Fflas##__S1__;					\
  }

  GEN_GET_FLAG(FFLAS_C_TRANSPOSE, Orientation, NoTrans, Trans);
  GEN_GET_FLAG(FFLAS_C_UPLO,      Uplo,        Upper,   Lower);
  GEN_GET_FLAG(FFLAS_C_SIDE,      Side,        Left,    Right);
  GEN_GET_FLAG(FFLAS_C_DIAG,      Diag,        NonUnit, Unit);

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
  GEN_IS_FLAG(Uplo,        Upper,   Lower);
  GEN_IS_FLAG(Side,        Left,    Right);
  GEN_IS_FLAG(Diag,        NonUnit, Unit);


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

  template < typename T >
  T     getElement(const mxArray* array)
  {
    return ((T*)mxGetData(array))[0];
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

  template < typename T >
  Vector<T> getVector(const mxArray* array)
  {
    return createVector((T*)mxGetData(array), mxGetNumberOfElements(array));
  }

  template < typename T > const T* constptr(const Vector<T>& v) { return v._p; }
  template < typename T >       T*      ptr(const Vector<T>& v) { return v._p; }

  template < typename T >
  Vector<T> cloneVector(const mxArray* array)
  {
    size_t m = mxGetM(array);
    size_t n = mxGetN(array);
    T* ptr = (T*) malloc(sizeof(T) * m * n);
    std::memcpy(ptr, mxGetData(array), sizeof(T) * m * n);
    return createVector(ptr, m * n);
  }

  template < typename T >
  void clear(Vector<T>& m)
  {
    free(m._p);
  }

  // ---------------------------------------------------------------------------

  template < typename T >
  Matrix<T> getMatrix(const mxArray* array)
  {
    return createMatrix((T*)mxGetData(array), mxGetM(array), mxGetN(array), CMO);
  }

  template < typename T > const T* constptr(const Matrix<T>& m) { return m._p; }
  template < typename T >       T*      ptr(const Matrix<T>& m) { return m._p; }

  template < typename T >
  Matrix<T> cloneMatrix(const mxArray* array)
  {
    size_t m = mxGetM(array);
    size_t n = mxGetN(array);
    T* ptr = (T*) malloc(sizeof(T) * m * n);
    std::memcpy(ptr, mxGetData(array), sizeof(T) * m * n);
    return createMatrix(ptr, m, n, CMO);
  }

  template < typename T >
  void clear(Matrix<T>& m)
  {
    free(m._p);
  }

  // ---------------------------------------------------------------------------

  template < typename T >
  Matrix<T> createMatrix (mxArray*& array, size_t m, size_t n)
  {
    array = mxCreateNumericMatrix(m, n, MXWrapper<T>::classID, mxREAL);
    return createMatrix((T*)mxGetData(array), m, n, CMO);
  }

  // ---------------------------------------------------------------------------

  template < typename T >
  Vector<T> createVector (mxArray*& array, size_t l)
  {
    array = mxCreateNumericMatrix(l, 1, MXWrapper<T>::classID, mxREAL);
    return createVector((T*)mxGetData(array), l);
  }

  // ---------------------------------------------------------------------------

  void        createAndSetBool   (mxArray*& array, bool el)
  {
    array = mxCreateLogicalScalar(el);
  }

  // ---------------------------------------------------------------------------

  void        createAndSetInt   (mxArray*& array, int el)
  {
    array = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
    ((int*)mxGetData(array))[0] = el;
  }

  // ---------------------------------------------------------------------------

  void        createAndSetReal   (mxArray*& array, double el)
  {
    array = mxCreateDoubleScalar(el);
  }

  // ---------------------------------------------------------------------------

  void         createAndSetSize(mxArray*& array, size_t el)
  {
    array = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
    mxGetPr(array)[0] = el;
  }

  // ---------------------------------------------------------------------------

  void         createAndSetElement(mxArray*& array, double el)
  {
    array = mxCreateDoubleMatrix(1, 1, mxREAL);
    mxGetPr(array)[0] = el;
  }

  // ---------------------------------------------------------------------------

  template < typename T >
  void         createAndSetElement(mxArray*& array, T el)
  {
    array = mxCreateNumericMatrix(1, 1, MXWrapper<T>::classID, mxREAL);
    ((T*)mxGetData(array))[0] = el;
  }

  // ---------------------------------------------------------------------------

  template < typename T >
  Vector<T> duplicateVector(const mxArray* src, mxArray*& ret)
  {
    ret = mxDuplicateArray(src);
    return createVector((T*)mxGetData(ret), mxGetNumberOfElements(ret));
  }

  // ---------------------------------------------------------------------------

  template < typename T >
  Matrix<T> duplicateMatrix(const mxArray* src, mxArray*& ret)
  {
    ret = mxDuplicateArray(src);
    return createMatrix((T*)mxGetData(ret), mxGetM(ret), mxGetN(ret), CMO);
  }

};

// -----------------------------------------------------------------------------

#endif // _FFINTERT_MATLAB_H_
