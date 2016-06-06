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

// -----------------------------------------------------------------------------
// This file contains the runtime functions specific to the CXX wrappers.
// In particular, in this file must be defined function concerning types:
// to<Name of type> : taking a RT type and returning a FFLAS type (explicit cast
//                    function)
// -----------------------------------------------------------------------------

#ifndef _FFINTERT_CXX_H_
#define _FFINTERT_CXX_H_

// -----------------------------------------------------------------------------

#include <string>
#include "ffintert.h"
#include "fflas-ffpack/fflas/fflas.h"
#include "givaro/modular-balanced.h"

// -----------------------------------------------------------------------------

namespace ffintert {

  Givaro::ModularBalanced<double>           toField(double F)           { return  Givaro::ModularBalanced<double>(F); }
  Givaro::ModularBalanced<double>::Element* toMatrix(Matrix<double>& A) { return (Givaro::ModularBalanced<double>::Element*)A._p; }
  Givaro::ModularBalanced<double>::Element* toVector(Vector<double>& V) { return (Givaro::ModularBalanced<double>::Element*)V._p; }
  Givaro::ModularBalanced<double>::Element  toElement(double e)         { return  Givaro::ModularBalanced<double>::Element(e); }

  FFLAS::FFLAS_TRANSPOSE toOrientation(FlagTrans f) { return (f == NoTrans ? FFLAS::FflasNoTrans : FFLAS::FflasTrans); }
  FFLAS::FFLAS_UPLO      toUplo (FlagUplo  f) { return (f == Upper   ? FFLAS::FflasUpper   : FFLAS::FflasLower); }
  FFLAS::FFLAS_DIAG      toDiag (FlagDiag  f) { return (f == NonUnit ? FFLAS::FflasNonUnit : FFLAS::FflasUnit ); }
  FFLAS::FFLAS_SIDE      toSide (FlagSide  f) { return (f == Left    ? FFLAS::FflasLeft    : FFLAS::FflasRight); }

};

#endif // _FFINTERT_CXX_H_
