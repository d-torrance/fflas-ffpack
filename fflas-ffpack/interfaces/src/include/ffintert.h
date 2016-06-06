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

#ifndef _FFINTERT_H_
#define _FFINTERT_H_

// -----------------------------------------------------------------------------

#include <string>
#include <sstream>

// -----------------------------------------------------------------------------

namespace ffintert {

  void error(const std::string&);

  enum FlagTrans { NoTrans = 111, Trans = 112 };
  enum FlagUplo  { Upper   = 121, Lower = 122 };
  enum FlagDiag  { NonUnit = 131, Unit  = 132 };
  enum FlagSide  { Left    = 141, Right = 142 };

  // ---------------------------------------------------------------------------

  // Row-Major Order or Column-Major Order
  enum order_t { RMO, CMO };

  template < typename T > struct Vector 
  {
    size_t _l; // length of the vector
    T*     _p; // pointer to the data

    operator T* () { return _p; }
  };

  template < typename T > 
  Vector<T> createVector(T* ptr, size_t l)
  {
    //    return Vector<T>{l, ptr}; // C++11... but we don't want that, isn't it ?
    Vector<T> v;
    v._l = l;
    v._p = ptr;
    return v;
  }

  template < typename T > size_t length(const Vector<T>& mat) { return mat._l; }
  template < typename T > size_t st    (const Vector<T>& mat) { return 1; }

  // ---------------------------------------------------------------------------

  template < typename T > struct Matrix 
  {
    size_t  _r; // number of rows;
    size_t  _c; // number of columns;
    T*      _p; // pointer to the data;
    order_t _o; // storage order
    FlagTrans _t;
    FlagUplo  _u;
    FlagDiag  _d;
    FlagSide  _s;

    operator T* () { return _p; }
  };
  
  template < typename T >
  Matrix<T> createMatrix(T* ptr, int r, int c, order_t o)
  {
    Matrix<T> m;
    m._r = r;
    m._c = c;
    m._p = ptr;
    m._o = o;
    m._t = NoTrans;
    m._u = Upper;
    m._d = NonUnit;
    m._s = Left;
    return m;
  }

  // Returns leading dimension of the matrix
  template < typename T >
  size_t ld(const Matrix<T>& mat) 
  {
    if (mat._o == CMO) return mat._r;
    else               return mat._c;
  }

  // Returns number of rows of the matrix
  template < typename T >
  size_t rows(const Matrix<T>& mat) { return mat._r; }
  
  // Returns number of columns of the matrix
  template < typename T >
  size_t cols(const Matrix<T>& mat) { return mat._c; }
  
  template < typename T >
  Matrix<T> transpose(const Matrix<T>& mat)
  {
    Matrix<T> ret;
    ret._r = mat._c; // number of rows;
    ret._c = mat._r; // number of columns;
    ret._p = mat._p; // pointer to the data;
    ret._o = mat._o; // storage order
    ret._t = mat._t == NoTrans ? Trans : NoTrans;
    ret._u = mat._u; // ???
    ret._d = mat._d; // ???
    ret._s = mat._s; // ???
    return ret;
  }

  // Returns op(mat): mat if t is NoTrans, transpose(mat) otherwise
  // note that the elements storage is not modified
  template < typename T > 
  Matrix<T> op(const Matrix<T>& mat) 
  {
    if (mat._t == NoTrans) return mat;
    else return transpose(mat);
  }

  template < typename T > FlagTrans trans(const Matrix<T>& mat) { return mat._t; } 
  template < typename T > FlagUplo  uplo (const Matrix<T>& mat) { return mat._u; }
  template < typename T > FlagDiag  diag (const Matrix<T>& mat) { return mat._d; }
  template < typename T > FlagSide  side (const Matrix<T>& mat) { return mat._s; }

  // ---------------------------------------------------------------------------

  inline FlagTrans invert(FlagTrans  m) { return m == NoTrans ? Trans : NoTrans; }
  inline FlagUplo  invert(FlagUplo   m) { return m == Upper   ? Lower : Upper;   }
  inline FlagSide  invert(FlagSide   m) { return m == Left    ? Right : Left;    }
  
  // ---------------------------------------------------------------------------

  inline void check(size_t a, size_t b, const std::string& varName)
  {
    std::ostringstream os;
    os << a << " to " << b;
    if (a != b)
      error("check on variable '" + varName 
     	    + "' failed: trying to change the value of variable from " 
	    + os.str());
	    //	    + std::to_string(a) + " to " + std::to_string(b) + "."); <= requires C++11
  }

};

#endif // _FFINTERT_H_
