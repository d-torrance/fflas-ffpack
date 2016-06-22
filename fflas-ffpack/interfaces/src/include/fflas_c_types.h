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

#ifndef _FFLAS_C_TYPES_H_
#define _FFLAS_C_TYPES_H_

// -----------------------------------------------------------------------------

enum FP_C_RP { FP_C_RP_Modular, FP_C_RP_ModularBalanced };

// public:
/// Is matrix transposed ?
enum FFLAS_C_TRANSPOSE {
  FflasNoTrans = 111, /**< Matrix is not transposed */
  FflasTrans   = 112  /**< Matrix is transposed */
};
/// Is triangular matrix's shape upper ?
enum FFLAS_C_UPLO {
  FflasUpper = 121, /**< Triangular matrix is Upper triangular (if \f$i>j\f$ then \f$T_{i,j} = 0\f$)*/
  FflasLower = 122  /**< Triangular matrix is Lower triangular (if \f$i<j\f$ then \f$T_{i,j} = 0\f$)*/
};

/// Is the triangular matrix implicitly unit diagonal ?
enum FFLAS_C_DIAG {
  FflasNonUnit = 131, /**< Triangular matrix has an explicit arbitrary diagonal */
  FflasUnit    = 132 /**< Triangular matrix has an implicit unit diagonal (\f$T_{i,i} = 1\f$)*/ /**< */
};

/// On what side ?
enum FFLAS_C_SIDE {
  FflasLeft  = 141,/**< Operator applied on the left */
  FflasRight = 142 /**< Operator applied on the rigth*/
};

// -----------------------------------------------------------------------------

#endif // _FFLAS_C_TYPES_H_
