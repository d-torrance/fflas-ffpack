/* -*- mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
// vim:sts=8:sw=8:ts=8:noet:sr:cino=>s,f0,{0,g0,(0,\:0,t0,+0,=s
/* fflas.h
 * Copyright (C) 2005 Clement Pernet
 *
 * Written by Clement Pernet <Clement.Pernet@imag.fr>
 *
 *
 * ========LICENCE========
 * This file is part of the library FFLAS-FFPACK.
 *
 * FFLAS-FFPACK is free software: you can redistribute it and/or modify
 * it under the terms of the  GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * ========LICENCE========
 *.
 */

/** @file fflas.h
 * @author Clément Pernet.
 * @brief <b>F</b>inite <b>F</b>ield <b>L</b>inear <b>A</b>lgebra <b>S</b>ubroutines
 */

#ifndef __FFLASFFPACK_fflas_H
#define __FFLASFFPACK_fflas_H

#include <cmath>
#include <cstring>

#ifndef MAX
#define MAX(a,b) ((a < b)?b:a)
#endif
#ifndef MIN
#define MIN(a,b) ((a > b)?b:a)
#endif

#include "fflas-ffpack/config-blas.h"
#include "fflas-ffpack/field/unparametric.h"
#include "fflas-ffpack/field/modular-balanced.h"
#include "fflas-ffpack/field/modular-positive.h"

#define WINOTHRESHOLD __FFLASFFPACK_WINOTHRESHOLD

/* Thresholds determining which floating point representation to use, depending
 * on the cardinality of the finite field. This is only used when the element
 * representation is not a floating point type.
 */
#define FLOAT_DOUBLE_THRESHOLD_0 430
#define FLOAT_DOUBLE_THRESHOLD_1 350
#define FLOAT_DOUBLE_THRESHOLD_2 175

#include <float.h>

//#define LB_TRTR
/// @brief FFLAS: <b>F</b>inite <b>F</b>ield <b>L</b>inear <b>A</b>lgebra <b>S</b>ubroutines.
namespace FFLAS {

	// public:
	/// Is matrix transposed ?
	enum FFLAS_TRANSPOSE
	{
		FflasNoTrans=111, /**< Matrix is not transposed */
		FflasTrans  =112  /**< Matrix is transposed */
	};
	/// Is triangular matrix's shape upper ?
	enum FFLAS_UPLO
	{
		FflasUpper=121,  /**< Triangular matrix is Upper triangular (if \f$i>j\f$ then \f$T_{i,j} = 0\f$)*/
		FflasLower=122   /**< Triangular matrix is Lower triangular (if \f$i<j\f$ then \f$T_{i,j} = 0\f$)*/
	};

	/// Is Matrix diagonal implicit ?
	enum FFLAS_DIAG
	{
		FflasNonUnit=131 ,  /**< Triangular matrix has an explicit general diagonal */
		FflasUnit   =132    /**< Triangular matrix has an implicit unit diagonal (\f$T_{i,i} = 1\f$)*//**< */
	};

	/// On what side ?
	enum FFLAS_SIDE
	{
		FflasLeft  = 141, /**< Operator applied on the left */
		FflasRight = 142  /**< Operator applied on the rigth*/
	};

	/** \p FFLAS_BASE  determines the type of the element representation for Matrix Mult kernel.  */
	enum FFLAS_BASE
	{
		FflasDouble  = 151,  /**<  to use the double precision BLAS */
		FflasFloat   = 152,  /**<  to use the single precison BLAS */
		FflasGeneric = 153   /**< for any other domain, that can not be converted to floating point integers */
	};

	/* Representations of Z with floating point elements*/

	typedef FFPACK::UnparametricField<float> FloatDomain;
	typedef FFPACK::UnparametricField<double> DoubleDomain;


	namespace Protected {

		// Prevents the instantiation of the class
		// FFLAS(){}
		template <class X,class Y>
		class AreEqual {
		public:
			static const bool value = false;
		};

		template <class X>
		class AreEqual<X,X> {
		public:
			static const bool value = true;
		};

		//-----------------------------------------------------------------------------
		// Some conversion functions
		//-----------------------------------------------------------------------------

		//---------------------------------------------------------------------
		// Finite Field matrix => double matrix
		//---------------------------------------------------------------------
		template<class Field>
		void MatF2MatD (const Field& F,
				       DoubleDomain::Element* S, const size_t lds,
				       const typename Field::Element* E,
				       const size_t lde,const size_t m, const size_t n)
		{

			const typename Field::Element* Ei = E;
			DoubleDomain::Element *Si=S;
			size_t j;
			for (; Ei < E+lde*m; Ei+=lde, Si += lds)
				for ( j=0; j<n; ++j){
					F.convert(*(Si+j),*(Ei+j));
				}
		}
		//---------------------------------------------------------------------
		// Finite Field matrix => float matrix
		//---------------------------------------------------------------------
		template<class Field>
		void MatF2MatFl (const Field& F,
					FloatDomain::Element* S, const size_t lds,
					const typename Field::Element* E,
					const size_t lde,const size_t m, const size_t n)
		{

			const typename Field::Element* Ei = E;
			FloatDomain::Element *Si=S;
			size_t j;
			for (; Ei < E+lde*m; Ei+=lde, Si += lds)
				for ( j=0; j<n; ++j){
					F.convert(*(Si+j),*(Ei+j));
				}
		}

		//---------------------------------------------------------------------
		// Finite Field matrix => double matrix
		// Special design for upper-triangular matrices
		//---------------------------------------------------------------------
		template<class Field>
		void MatF2MatD_Triangular (const Field& F,
						  typename DoubleDomain::Element* S, const size_t lds,
						  const typename Field::Element* const E,
						  const size_t lde,
						  const size_t m, const size_t n){

			const typename Field::Element* Ei = E;
			typename DoubleDomain::Element* Si = S;
			size_t i=0, j;
			for ( ; i<m;++i, Ei+=lde, Si+=lds)
				for ( j=i; j<n;++j)
					F.convert(*(Si+j),*(Ei+j));
		}

		//---------------------------------------------------------------------
		// Finite Field matrix => float matrix
		// Special design for upper-triangular matrices
		//---------------------------------------------------------------------
		template<class Field>
		void MatF2MatFl_Triangular (const Field& F,
						   typename FloatDomain::Element* S, const size_t lds,
						   const typename Field::Element* const E,
						   const size_t lde,
						   const size_t m, const size_t n){

			const typename Field::Element* Ei = E;
			typename FloatDomain::Element* Si = S;
			size_t i=0, j;
			for ( ; i<m;++i, Ei+=lde, Si+=lds)
				for ( j=i; j<n;++j)
					F.convert(*(Si+j),*(Ei+j));
		}

		//---------------------------------------------------------------------
		// double matrix => Finite Field matrix
		//---------------------------------------------------------------------
		template<class Field>
		void MatD2MatF (const Field& F,
				       typename Field::Element* S, const size_t lds,
				       const typename DoubleDomain::Element* E, const size_t lde,
				       const size_t m, const size_t n)
		{

			typename Field::Element* Si = S;
			const DoubleDomain::Element* Ei =E;
			size_t j;
			for ( ; Si < S+m*lds; Si += lds, Ei+= lde){
				for ( j=0; j<n;++j)
					F.init( *(Si+j), *(Ei+j) );
			}
		}

		//---------------------------------------------------------------------
		// float matrix => Finite Field matrix
		//---------------------------------------------------------------------
		template<class Field>
		void MatFl2MatF (const Field& F,
					typename Field::Element* S, const size_t lds,
					const typename FloatDomain::Element* E, const size_t lde,
					const size_t m, const size_t n){

			typename Field::Element* Si = S;
			const FloatDomain::Element* Ei =E;
			size_t j;
			for ( ; Si < S+m*lds; Si += lds, Ei+= lde){
				for ( j=0; j<n;++j)
					F.init( *(Si+j), *(Ei+j) );
			}
		}

		/**
		 * Computes the threshold parameters for the cascade
		 *        Matmul algorithm.
		 *
		 *
		 * \param F Finite Field/Ring of the computation.
		 * \param k Common dimension of A and B, in the product A x B
		 * \param beta Computing \f$AB + \beta C\f$
		 * \param delayedDim Returns the size of blocks that can be multiplied
		 *                   over Z with no overflow
		 * \param base Returns the type of BLAS representation to use
		 * \param winoRecLevel Returns the number of recursion levels of
		 *                     Strassen-Winograd's algorithm to perform
		 * \param winoLevelProvided tells whether the user forced the number of
		 *                          recursive level of Winograd's algorithm
		 *
		 * @bib
		 * - Dumas, Giorgi, Pernet, arXiv cs/0601133  <a href=http://arxiv.org/abs/cs.SC/0601133>here</a>
		 */
		template <class Field>
		void MatMulParameters (const Field& F,
					      const size_t k,
					      const typename Field::Element& beta,
					      size_t& delayedDim,
					      FFLAS_BASE& base,
					      size_t& winoRecLevel,
					      bool winoLevelProvided=false);


		/**
		 * Computes the maximal size for delaying the modular reduction
		 *         in a dotproduct.
		 *
		 * This is the default version assuming a conversion to a positive modular representation
		 *
		 * \param F Finite Field/Ring of the computation
		 * \param winoRecLevel Number of recusrive Strassen-Winograd levels (if any, 0 otherwise)
		 * \param beta Computing AB + beta C
		 * \param base Type of floating point representation for delayed modular computations
		 *
		 */
		template <class Field>
		size_t DotProdBound (const Field& F,
					    const size_t winoRecLevel,
					    const typename Field::Element& beta,
					    const FFLAS_BASE base);


		/**
		 * Internal function for the bound computation.
		 * Generic implementation for positive representations
		 */
		template <class Field>
		double computeFactorWino (const Field& F, const size_t w);

		template <class Field>
		double computeFactorClassic (const Field& F);


		/**
		 * Determines the type of floating point representation to convert to,
		 *        for BLAS computations.
		 * \param F Finite Field/Ring of the computation
		 * \param w Number of recursive levels in Winograd's algorithm
		 */
		template <class Field>
		FFLAS_BASE BaseCompute (const Field& F, const size_t w);

		/**
		 * Computes the maximal size for delaying the modular reduction
		 *         in a triangular system resolution.
		 *
		 *  Compute the maximal dimension k, such that a unit diagonal triangular
		 *  system of dimension k can be solved over Z without overflow of the
		 *  underlying floating point representation.
		 *
		 *  @bib
		 *  - Dumas, Giorgi, Pernet 06, arXiv:cs/0601133.
		 *
		 * \param F Finite Field/Ring of the computation
		 *
		 */
		template <class Field>
		size_t TRSMBound (const Field& F);

		template <class Field>
		void DynamicPealing( const Field& F,
					    const FFLAS_TRANSPOSE ta,
					    const FFLAS_TRANSPOSE tb,
					    const size_t m, const size_t n, const size_t k,
					    const typename Field::Element alpha,
					    const typename Field::Element* A, const size_t lda,
					    const typename Field::Element* B, const size_t ldb,
					    const typename Field::Element beta,
					    typename Field::Element* C, const size_t ldc,
					    const size_t  ); //kmax

		template <class Field>
		void MatVectProd (const Field& F,
					 const FFLAS_TRANSPOSE TransA,
					 const size_t M, const size_t N,
					 const typename Field::Element alpha,
					 const typename Field::Element * A, const size_t lda,
					 const typename Field::Element * X, const size_t incX,
					 const typename Field::Element beta,
					 typename Field::Element * Y, const size_t incY);

		template <class Field>
		void ClassicMatmul(const Field& F,
					  const FFLAS_TRANSPOSE ta,
					  const FFLAS_TRANSPOSE tb,
					  const size_t m, const size_t n, const size_t k,
					  const typename Field::Element alpha,
					  const typename Field::Element * A, const size_t lda,
					  const typename Field::Element * B, const size_t ldb,
					  const typename Field::Element beta,
					  typename Field::Element * C, const size_t ldc,
					  const size_t kmax, const FFLAS_BASE base );

		// Winograd Multiplication  alpha.A(n*k) * B(k*m) + beta . C(n*m)
		// WinoCalc performs the 22 Winograd operations
		template <class Field>
		void WinoCalc (const Field& F,
				      const FFLAS_TRANSPOSE ta,
				      const FFLAS_TRANSPOSE tb,
				      const size_t mr, const size_t nr,const size_t kr,
				      const typename Field::Element alpha,
				      const typename Field::Element* A,const size_t lda,
				      const typename Field::Element* B,const size_t ldb,
				      const typename Field::Element beta,
				      typename Field::Element * C, const size_t ldc,
				      const size_t kmax, const size_t w, const FFLAS_BASE base);

		template<class Field>
		void WinoMain (const Field& F,
				      const FFLAS_TRANSPOSE ta,
				      const FFLAS_TRANSPOSE tb,
				      const size_t m, const size_t n, const size_t k,
				      const typename Field::Element alpha,
				      const typename Field::Element* A,const size_t lda,
				      const typename Field::Element* B,const size_t ldb,
				      const typename Field::Element beta,
				      typename Field::Element * C, const size_t ldc,
				      const size_t kmax, const size_t w, const FFLAS_BASE base);

		// Specialized routines for ftrsm
		template <class Element>
		class ftrsmLeftUpperNoTransNonUnit;
		template <class Element>
		class ftrsmLeftUpperNoTransUnit;
		template <class Element>
		class ftrsmLeftUpperTransNonUnit;
		template <class Element>
		class ftrsmLeftUpperTransUnit;
		template <class Element>
		class ftrsmLeftLowerNoTransNonUnit;
		template <class Element>
		class ftrsmLeftLowerNoTransUnit;
		template <class Element>
		class ftrsmLeftLowerTransNonUnit;
		template <class Element>
		class ftrsmLeftLowerTransUnit;
		template <class Element>
		class ftrsmRightUpperNoTransNonUnit;
		template <class Element>
		class ftrsmRightUpperNoTransUnit;
		template <class Element>
		class ftrsmRightUpperTransNonUnit;
		template <class Element>
		class ftrsmRightUpperTransUnit;
		template <class Element>
		class ftrsmRightLowerNoTransNonUnit;
		template <class Element>
		class ftrsmRightLowerNoTransUnit;
		template <class Element>
		class ftrsmRightLowerTransNonUnit;
		template <class Element>
		class ftrsmRightLowerTransUnit;

		// Specialized routines for ftrmm
		template <class Element>
		class ftrmmLeftUpperNoTransNonUnit;
		template <class Element>
		class ftrmmLeftUpperNoTransUnit;
		template <class Element>
		class ftrmmLeftUpperTransNonUnit;
		template <class Element>
		class ftrmmLeftUpperTransUnit;
		template <class Element>
		class ftrmmLeftLowerNoTransNonUnit;
		template <class Element>
		class ftrmmLeftLowerNoTransUnit;
		template <class Element>
		class ftrmmLeftLowerTransNonUnit;
		template <class Element>
		class ftrmmLeftLowerTransUnit;
		template <class Element>
		class ftrmmRightUpperNoTransNonUnit;
		template <class Element>
		class ftrmmRightUpperNoTransUnit;
		template <class Element>
		class ftrmmRightUpperTransNonUnit;
		template <class Element>
		class ftrmmRightUpperTransUnit;
		template <class Element>
		class ftrmmRightLowerNoTransNonUnit;
		template <class Element>
		class ftrmmRightLowerNoTransUnit;
		template <class Element>
		class ftrmmRightLowerTransNonUnit;
		template <class Element>
		class ftrmmRightLowerTransUnit;

		// BB : ça peut servir...
#ifdef LB_TRTR
		template <class Element>
		class ftrtrLeftUpperNoTransNonUnitNonUnit;
		template <class Element>
		class ftrtrLeftUpperNoTransUnitNonUnit;
		template <class Element>
		class ftrtrLeftUpperTransNonUnitNonUnit;
		template <class Element>
		class ftrtrLeftUpperTransUnitNonUnit;
		template <class Element>
		class ftrtrLeftLowerNoTransNonUnitNonUnit;
		template <class Element>
		class ftrtrLeftLowerNoTransUnitNonUnit;
		template <class Element>
		class ftrtrLeftLowerTransNonUnitNonUnit;
		template <class Element>
		class ftrtrLeftLowerTransUnitNonUnit;
		template <class Element>
		class ftrtrLeftUpperNoTransNonUnitUnit;
		template <class Element>
		class ftrtrLeftUpperNoTransUnitUnit;
		template <class Element>
		class ftrtrLeftUpperTransNonUnitUnit;
		template <class Element>
		class ftrtrLeftUpperTransUnitUnit;
		template <class Element>
		class ftrtrLeftLowerNoTransNonUnitUnit;
		template <class Element>
		class ftrtrLeftLowerNoTransUnitUnit;
		template <class Element>
		class ftrtrLeftLowerTransNonUnitUnit;
		template <class Element>
		class ftrtrLeftLowerTransUnitUnit;
		template <class Element>
		class ftrtrRightUpperNoTransNonUnitNonUnit;
		template <class Element>
		class ftrtrRightUpperNoTransUnitNonUnit;
		template <class Element>
		class ftrtrRightUpperTransNonUnitNonUnit;
		template <class Element>
		class ftrtrRightUpperTransUnitNonUnit;
		template <class Element>
		class ftrtrRightLowerNoTransNonUnitNonUnit;
		template <class Element>
		class ftrtrRightLowerNoTransUnitNonUnit;
		template <class Element>
		class ftrtrRightLowerTransNonUnitNonUnit;
		template <class Element>
		class ftrtrRightLowerTransUnitNonUnit;
		template <class Element>
		class ftrtrRightUpperNoTransNonUnitUnit;
		template <class Element>
		class ftrtrRightUpperNoTransUnitUnit;
		template <class Element>
		class ftrtrRightUpperTransNonUnitUnit;
		template <class Element>
		class ftrtrRightUpperTransUnitUnit;
		template <class Element>
		class ftrtrRightLowerNoTransNonUnitUnit;
		template <class Element>
		class ftrtrRightLowerNoTransUnitUnit;
		template <class Element>
		class ftrtrRightLowerTransNonUnitUnit;
		template <class Element>
		class ftrtrRightLowerTransUnitUnit;
#endif
		template<class Element>
		class faddmTrans;
		template<class Element>
		class faddmNoTrans;
		template<class Element>
		class fsubmTrans;
		template<class Element>
		class fsubmNoTrans;
		template<class Element>
		class faddmTransTrans;
		template<class Element>
		class faddmNoTransTrans;
		template<class Element>
		class faddmTransNoTrans;
		template<class Element>
		class faddmNoTransNoTrans;
		template<class Element>
		class fsubmTransTrans;
		template<class Element>
		class fsubmNoTransTrans;
		template<class Element>
		class fsubmTransNoTrans;
		template<class Element>
		class fsubmNoTransNoTrans;
	} // protected

	//---------------------------------------------------------------------
	// Level 1 routines
	//---------------------------------------------------------------------

	/** \brief fzero : \f$A \gets 0 \f$.
	 * @param F field
	 * @param n number of elements to zero
	 * \param X vector in \p F
	 * \param incX stride of \p X
	 */
	template<class Field>
	void
	fzero (const Field& F, const size_t n,
	       typename Field::Element *X, const size_t incX)
	{
		if (incX == 1) { // contigous data
			// memset(X,(int)F.zero,n); // might be bogus ?
			for (size_t i = 0 ; i < n ; ++i)
				F.assign(*(X+i), F.zero);

		}
		else { // not contiguous (strided)
			for (size_t i = 0 ; i < n ; ++i)
				F.assign(*(X+i*incX), F.zero);
		}
	}

	/** fscal
	 * \f$x \gets a \cdot x\f$.
	 * @param F field
	 * @param n size of the vectors
	 * @param alpha homotÃ©ti scalar
	 * \param X vector in \p F
	 * \param incX stride of \p X
	 * @bug use cblas_(d)scal when possible
	 * @internal
	 * @todo check if comparison with +/-1,0 is necessary.
	 */
	template<class Field>
	void
	fscal (const Field& F, const size_t n, const typename Field::Element alpha,
	       typename Field::Element * X, const size_t incX)
	{
		typedef typename Field::Element Element ;

		if (F.isOne(alpha))
			return ;

		Element * Xi = X;
		if (F.areEqual(alpha,F.mOne)){
			for (; Xi < X+n*incX; Xi+=incX )
				F.negin( *Xi );
			return;
		}
		if (F.isZero(alpha)){
			fzero(F,n,X,incX);
			return;
		}

		for (; Xi < X+n*incX; Xi+=incX )
			F.mulin( *Xi, alpha );
	}

	/** \brief fcopy : \f$x \gets y \f$.
	 * X is preallocated
	 * @param F field
	 * @param N size of the vectors
	 * \param [out] X vector in \p F
	 * \param incX stride of \p X
	 * \param [in] Y vector in \p F
	 * \param incY stride of \p Y
	 */
	template<class Field>
	void
	fcopy (const Field& F, const size_t N,
	       typename Field::Element * X, const size_t incX,
	       const typename Field::Element * Y, const size_t incY );


	/** \brief faxpy : \f$y \gets \alpha \cdot x + y\f$.
	 * @param F field
	 * @param N size of the vectors
	 * @param alpha scalar
	 * \param X vector in \p F
	 * \param incX stride of \p X
	 * \param Y vector in \p F
	 * \param incY stride of \p Y
	 */
	template<class Field>
	void
	faxpy (const Field& F, const size_t N,
	       const typename Field::Element alpha,
	       const typename Field::Element * X, const size_t incX,
	       typename Field::Element * Y, const size_t incY );

	/** \brief fdot: dot product \f$x^T  y\f$.
	 * @param F field
	 * @param N size of the vectors
	 * \param X vector in \p F
	 * \param incX stride of \p X
	 * \param Y vector in \p F
	 * \param incY stride of \p Y
	 */
	template<class Field>
	typename Field::Element
	fdot (const Field& F, const size_t N,
	      const typename Field::Element * X, const size_t incX,
	      const typename Field::Element * Y, const size_t incY );

	/** \brief fswap: \f$ X \leftrightarrow Y\f$.
	 * @param F field
	 * @param N size of the vectors
	 * \param X vector in \p F
	 * \param incX stride of \p X
	 * \param Y vector in \p F
	 * \param incY stride of \p Y
	 */
	template<class Field>
	void
	fswap (const Field& F, const size_t N, typename Field::Element * X, const size_t incX,
	       typename Field::Element * Y, const size_t incY )
	{

		typename Field::Element tmp;
		typename Field::Element * Xi = X;
		typename Field::Element * Yi=Y;
		for (; Xi < X+N*incX; Xi+=incX, Yi+=incY ){
			F.assign( tmp, *Xi );
			F.assign( *Xi, *Yi );
			F.assign( *Yi, tmp );
		}
	}

	//---------------------------------------------------------------------
	// Level 2 routines
	//---------------------------------------------------------------------

	/** \brief fcopy : \f$A \gets B \f$.
	 * @param F field
	 * @param m number of rows to copy
	 * @param n number of cols to copy
	 * \param A matrix in \p F
	 * \param lda stride of \p A
	 * \param B vector in \p F
	 * \param ldb stride of \p B
	 */
	template<class Field>
	void
	fcopy (const Field& F, const size_t m, const size_t n,
	       typename Field::Element * A, const size_t lda,
	       const typename Field::Element * B, const size_t ldb ) ;

	/** \brief fzero : \f$A \gets 0 \f$.
	 * @param F field
	 * @param m number of rows to zero
	 * @param n number of cols to zero
	 * \param A matrix in \p F
	 * \param lda stride of \p A
	 * @warning may be buggy if Element is larger than int
	 */
	template<class Field>
	void
	fzero (const Field& F, const size_t m, const size_t n,
	       typename Field::Element * A, const size_t lda)
	{
		/*  use memset only with Elements that are ok */
		if (n == lda) { // contigous data
			// memset(A,(int) F.zero,m*n); // might be bogus ?
			fzero(F,m*n,A,1);
		}
		else { // not contiguous (strided)
			for (size_t i = 0 ; i < m ; ++i)
				// memset(A+i*lda,(int) F.zero,n) ; // might be bogus ?
				fzero(F,n,A+i*lda,1);
		}
	}

	/** fscal
	 * \f$A \gets a \cdot A\f$.
	 * @param F field
	 * @param m number of rows
	 * @param n number of cols
	 * @param alpha homotecie scalar
	 * \param A matrix in \p F
	 * \param lda stride of \p A
	 * @internal
	 */
	template<class Field>
	void
	fscal (const Field& F, const size_t m , const size_t n,
	       const typename Field::Element alpha,
	       typename Field::Element * A, const size_t lda)
	{
		typedef typename Field::Element Element ;

		if (F.isOne(alpha)) {
			return ;
		}
		else {
			if (lda == n) {
				fscal(F,n*m,alpha,A,1);
			}
			else {
				for (size_t i = 0 ; i < m ; ++i)
					fscal(F,n,alpha,A+i*lda,1);
			}

			return;
		}
	}

	/** \brief fmove : \f$A \gets B \f$ and \f$ B \gets 0\f$.
	 * @param F field
	 * @param m number of rows to copy
	 * @param n number of cols to copy
	 * \param A matrix in \p F
	 * \param lda stride of \p A
	 * \param B vector in \p F
	 * \param ldb stride of \p B
	 */
	template<class Field>
	void
	fmove (const Field& F, const size_t m, const size_t n,
	       typename Field::Element * A, const size_t lda,
	       typename Field::Element * B, const size_t ldb )
	{
		fcopy(F,m,n,A,lda,B,ldb);
		fzero(F,m,n,B,ldb);
	}

	/** fadd : matrix addition.
	 * Computes \p C = \p A + \p B.
	 * @param F field
	 * @param M rows
	 * @param N cols
	 * @param A dense matrix of size \c MxN
	 * @param lda leading dimension of \p A
	 * @param B dense matrix of size \c MxN
	 * @param ldb leading dimension of \p B
	 * @param C dense matrix of size \c MxN
	 * @param ldc leading dimension of \p C
	 */
	template <class Field>
	void
	fadd (const Field& F, const size_t M, const size_t N,
	      const typename Field::Element* A, const size_t lda,
	      const typename Field::Element* B, const size_t ldb,
	      typename Field::Element* C, const size_t ldc)
	{
		const typename Field::Element *Ai = A, *Bi = B;
		typename Field::Element *Ci = C;
		for (; Ai < A+M*lda; Ai+=lda, Bi+=ldb, Ci+=ldc)
			for (size_t i=0; i<N; i++)
				F.add (Ci[i], Ai[i], Bi[i]);
	}

	/** fsub : matrix subtraction.
	 * Computes \p C = \p A - \p B.
	 * @param F field
	 * @param M rows
	 * @param N cols
	 * @param A dense matrix of size \c MxN
	 * @param lda leading dimension of \p A
	 * @param B dense matrix of size \c MxN
	 * @param ldb leading dimension of \p B
	 * @param C dense matrix of size \c MxN
	 * @param ldc leading dimension of \p C
	 */
	template <class Field>
	void
	fsub (const Field& F, const size_t M, const size_t N,
	      const typename Field::Element* A, const size_t lda,
	      const typename Field::Element* B, const size_t ldb,
	      typename Field::Element* C, const size_t ldc)
	{
		const typename Field::Element * Ai = A, *Bi = B;
		typename Field::Element *Ci = C;
		for (; Ai < A+M*lda; Ai+=lda, Bi+=ldb, Ci+=ldc)
			for (size_t i=0; i<N; i++)
				F.sub (Ci[i], Ai[i], Bi[i]);
	}

	//! fsubin
	template <class Field>
	void
	fsubin (const Field& F, const size_t M, const size_t N,
		const typename Field::Element* B, const size_t ldb,
		typename Field::Element* C, const size_t ldc)
	{
		const typename Field::Element * Bi = B;
		typename Field::Element *Ci = C;
		for (; Ci < C+M*ldc; Bi+=ldb, Ci+=ldc)
			for (size_t i=0; i<N; i++)
				F.subin (Ci[i], Bi[i]);
	}

	//! faddin
	template <class Field>
	void
	faddin (const Field& F, const size_t M, const size_t N,
		const typename Field::Element* B, const size_t ldb,
		typename Field::Element* C, const size_t ldc)
	{
		const typename Field::Element * Bi = B;
		typename Field::Element *Ci = C;
		for (; Ci < C+M*ldc; Bi+=ldb, Ci+=ldc)
			for (size_t i=0; i<N; i++)
				F.addin (Ci[i], Bi[i]);
	}


	/**  @brief finite prime Field GEneral Matrix Vector multiplication.
	 *
	 *  Computes  \f$Y \gets \alpha \mathrm{op}(A) X + \beta Y \f$.
	 * @param F field
	 * \param TransA if \c TransA==FflasTrans then \f$\mathrm{op}(A)=A^t\f$.
	 * @param M rows
	 * @param N cols
	 * @param alpha scalar
	 * @param A dense matrix of size \c MxN
	 * @param lda leading dimension of \p A
	 * @param X dense vector of size \c N
	 * @param incX stride of \p X
	 * @param beta scalar
	 * @param[out] Y dense vector of size \c M
	 * @param incY stride of \p Y
	 */
	template<class Field>
	void
	fgemv (const Field& F, const FFLAS_TRANSPOSE TransA,
	       const size_t M, const size_t N,
	       const typename Field::Element alpha,
	       const typename Field::Element * A, const size_t lda,
	       const typename Field::Element * X, const size_t incX,
	       const  typename Field::Element beta,
	       typename Field::Element * Y, const size_t incY);

	/**  @brief fger: GEneral ?
	 *
	 *  Computes  \f$A \gets \alpha x . y^T + A\f$
	 * @param F field
	 * @param M rows
	 * @param N cols
	 * @param alpha scalar
	 * @param[in,out] A dense matrix of size \c MxN and leading dimension \p lda
	 * @param lda leading dimension of \p A
	 * @param x dense vector of size \c M
	 * @param incx stride of \p X
	 * @param y dense vector of size \c N
	 * @param incy stride of \p Y
	 */
	template<class Field>
	void
	fger (const Field& F, const size_t M, const size_t N,
	      const typename Field::Element alpha,
	      const typename Field::Element * x, const size_t incx,
	      const typename Field::Element * y, const size_t incy,
	      typename Field::Element * A, const size_t lda);

	/** @brief ftrsv: TRiangular System solve with Vector
	 *  Computes  \f$ X \gets \mathrm{op}(A^{-1}) X\f$
	 *  @param F field
	 * @param X vector of size \p N on a field \p F
	 * @param incX stride of \p  X
	 * @param A a matrix of leading dimension \p lda and size \p N
	 * @param lda leading dimension of \p A
	 * @param N number of rows or columns of \p A according to \p TransA
	 * \param TransA if \c TransA==FflasTrans then \f$\mathrm{op}(A)=A^t\f$.
	 * \param Diag if \c Diag==FflasUnit then \p A is unit.
	 * \param Uplo if \c Uplo==FflasUpper then \p A is upper triangular
	 */
	template<class Field>
	void
	ftrsv (const Field& F, const FFLAS_UPLO Uplo,
	       const FFLAS_TRANSPOSE TransA, const FFLAS_DIAG Diag,
	       const size_t N,const typename Field::Element * A, const size_t lda,
	       typename Field::Element * X, int incX);

	//---------------------------------------------------------------------
	// Level 3 routines
	//---------------------------------------------------------------------

	/** @brief ftrsm: <b>TR</b>iangular <b>S</b>ystem solve with <b>M</b>atrix.
	 * Computes  \f$ B \gets \alpha \mathrm{op}(A^{-1}) B\f$ or  \f$B \gets \alpha B \mathrm{op}(A^{-1})\f$.
	 * \param F field
	 * \param Side if \c Side==FflasLeft then  \f$ B \gets \alpha \mathrm{op}(A^{-1}) B\f$ is computed.
	 * \param Uplo if \c Uplo==FflasUpper then \p A is upper triangular
	 * \param TransA if \c TransA==FflasTrans then \f$\mathrm{op}(A)=A^t\f$.
	 * \param Diag if \c Diag==FflasUnit then \p A is unit.
	 * \param M rows of \p B
	 * \param N cols of \p B
	 * @param alpha scalar
	 * \param A triangular invertible matrix. If \c Side==FflasLeft then \p A is \f$N\times N\f$, otherwise \p A is \f$M\times M\f$
	 * @param lda leading dim of \p A
	 * @param B matrix of size \p MxN
	 * @param ldb leading dim of \p B
	 * @bug unsafe with \c Trans==FflasTrans (debugging in progress)
	 * @bug \f$\alpha\f$ must be non zero.
	 */
	template<class Field>
	void
	ftrsm (const Field& F, const FFLAS_SIDE Side,
	       const FFLAS_UPLO Uplo,
	       const FFLAS_TRANSPOSE TransA,
	       const FFLAS_DIAG Diag,
	       const size_t M, const size_t N,
	       const typename Field::Element alpha,
	       typename Field::Element * A, const size_t lda,
	       typename Field::Element * B, const size_t ldb);

	/** @brief ftrmm: <b>TR</b>iangular <b>M</b>atrix <b>M</b>ultiply.
	 * Computes  \f$ B \gets \alpha \mathrm{op}(A) B\f$ or  \f$B \gets \alpha B \mathrm{op}(A)\f$.
	 * @param F field
	 * \param Side if \c Side==FflasLeft then  \f$ B \gets \alpha \mathrm{op}(A) B\f$ is computed.
	 * \param Uplo if \c Uplo==FflasUpper then \p A is upper triangular
	 * \param TransA if \c TransA==FflasTrans then \f$\mathrm{op}(A)=A^t\f$.
	 * \param Diag if \c Diag==FflasUnit then \p A is implicitly unit.
	 * \param M rows of \p B
	 * \param N cols of \p B
	 * @param alpha scalar
	 * \param A triangular matrix. If \c Side==FflasLeft then \p A is \f$N\times N\f$, otherwise \p A is \f$M\times M\f$
	 * @param lda leading dim of \p A
	 * @param B matrix of size \p MxN
	 * @param ldb leading dim of \p B
	 * @bug unsafe with \c Trans==FflasTrans (debugging in progress)
	 */
	template<class Field>
	void
	ftrmm (const Field& F, const FFLAS_SIDE Side,
	       const FFLAS_UPLO Uplo,
	       const FFLAS_TRANSPOSE TransA,
	       const FFLAS_DIAG Diag,
	       const size_t M, const size_t N,
	       const typename Field::Element alpha,
	       typename Field::Element * A, const size_t lda,
	       typename Field::Element * B, const size_t ldb);

	/** @brief  fgemm: <b>F</b>ield <b>GE</b>neral <b>M</b>atrix <b>M</b>ultiply.
	 *
	 * Computes \f$C = \alpha \mathrm{op}(A) \times \mathrm{op}(B) + \beta C\f$
	 * \param F field.
	 * \param ta if \c ta==FflasTrans then \f$\mathrm{op}(A)=A^t\f$, else \f$\mathrm{op}(A)=A\f$,
	 * \param tb same for \p B
	 * \param m see \p A
	 * \param k see \p A
	 * \param n see \p B
	 * \param alpha scalar
	 * \param beta scalar
	 * \param A \f$\mathrm{op}(A)\f$ is \f$m \times k\f$
	 * \param B \f$\mathrm{op}(B)\f$ is \f$k \times n\f$
	 * \param C \f$C\f$ is \f$m \times n\f$
	 * \param lda leading dimension of \p A
	 * \param ldb leading dimension of \p B
	 * \param ldc leading dimension of \p C
	 * \param w recursive levels of Winograd's algorithm are used
	 * @warning \f$\alpha\f$ \e must be invertible
	 */
	template<class Field>
	typename Field::Element*
	fgemm( const Field& F,
	       const FFLAS_TRANSPOSE ta,
	       const FFLAS_TRANSPOSE tb,
	       const size_t m,
	       const size_t n,
	       const size_t k,
	       const typename Field::Element alpha,
	       const typename Field::Element* A, const size_t lda,
	       const typename Field::Element* B, const size_t ldb,
	       const typename Field::Element beta,
	       typename Field::Element* C, const size_t ldc,
	       const size_t w)
	{

		if (!(m && n && k)) return C;

		if (F.isZero (alpha)){
			fscal(F, m, n, beta, C, ldc);
			return C;
		}



		size_t kmax = 0;
		size_t winolevel = w;
		FFLAS_BASE base;
		Protected::MatMulParameters (F, MIN(MIN(m,n),k), beta, kmax, base,
					     winolevel, true);
		Protected::WinoMain (F, ta, tb, m, n, k, alpha, A, lda, B, ldb, beta,
				     C, ldc, kmax, winolevel, base);
		return C;
	}

	/** @brief  fgemm: <b>F</b>ield <b>GE</b>neral <b>M</b>atrix <b>M</b>ultiply.
	 *
	 * Computes \f$C = \alpha \mathrm{op}(A) \mathrm{op}(B) + \beta C\f$.
	 * Automatically set Winograd recursion level
	 * \param F field.
	 * \param ta if \c ta==FflasTrans then \f$\mathrm{op}(A)=A^t\f$, else \f$\mathrm{op}(A)=A\f$,
	 * \param tb same for matrix \p B
	 * \param m see \p A
	 * \param k see \p A
	 * \param n see \p B
	 * \param alpha scalar
	 * \param beta scalar
	 * \param A \f$\mathrm{op}(A)\f$ is \f$m \times k\f$
	 * \param B \f$\mathrm{op}(B)\f$ is \f$k \times n\f$
	 * \param C \f$C\f$ is \f$m \times n\f$
	 * \param lda leading dimension of \p A
	 * \param ldb leading dimension of \p B
	 * \param ldc leading dimension of \p C
	 * @warning \f$\alpha\f$ \e must be invertible
	 */
	template<class Field>
	typename Field::Element*
	fgemm (const Field& F,
	       const FFLAS_TRANSPOSE ta,
	       const FFLAS_TRANSPOSE tb,
	       const size_t m,
	       const size_t n,
	       const size_t k,
	       const typename Field::Element alpha,
	       const typename Field::Element* A, const size_t lda,
	       const typename Field::Element* B, const size_t ldb,
	       const typename Field::Element beta,
	       typename Field::Element* C, const size_t ldc)
	{

		if (!(m && n && k)) return C;
		if (F.isZero (alpha)){
			for (size_t i = 0; i<m; ++i)
				fscal(F, n, beta, C + i*ldc, 1);
			return C;
		}

#ifdef _LB_DEBUG
		/*  check if alpha is invertible. XXX do it in F.isInvertible(Element&) ? */
		typename Field::Element e ;
		F.init(e,1UL);
		F.divin(e,alpha);
		F.mulin(e,alpha);
		FFLASFFPACK_check(F.isOne(e));
#endif
		size_t w, kmax;
		FFLAS_BASE base;

		Protected::MatMulParameters (F, MIN(MIN(m,n),k), beta, kmax, base, w);

		Protected::WinoMain (F, ta, tb, m, n, k, alpha, A, lda, B, ldb, beta,
				     C, ldc, kmax, w, base);
		return C;
	}

	/** @brief fsquare: Squares a matrix.
	 * compute \f$ C \gets \alpha \mathrm{op}(A) \mathrm{op}(A) + \beta C\f$ over a Field \p F
	 * Avoid the conversion of B
	 * @param ta  if \c ta==FflasTrans, \f$\mathrm{op}(A)=A^T\f$.
	 * @param F field
	 * @param n size of \p A
	 * @param alpha scalar
	 * @param beta scalar
	 * @param A dense matrix of size \c nxn
	 * @param lda leading dimension of \p A
	 * @param C dense matrix of size \c nxn
	 * @param ldc leading dimension of \p C
	 */
	template<class Field>
	typename Field::Element* fsquare (const Field& F,
						 const FFLAS_TRANSPOSE ta,
						 const size_t n,
						 const typename Field::Element alpha,
						 const typename Field::Element* A,
						 const size_t lda,
						 const typename Field::Element beta,
						 typename Field::Element* C,
						 const size_t ldc);
#ifdef LB_TRTR
	// BB
	/** @brief ftrtr: Triangular-Triangular matrix multiplication.
	 * \f$B \gets \alpha \mathrm{op}(A) \times B\f$ (for FFLAS_SIDE::FflasLeft)
	 * A and B are triangular, with B UpLo
	 * and op(A) = A, A^T according to TransA
	 * A and B can be (non)unit
	 *
	 */
	template<class Field>
	typename Field::Element* ftrtr (const Field& F, const FFLAS_SIDE Side,
					       const FFLAS_UPLO Uplo,
					       const FFLAS_TRANSPOSE TransA,
					       const FFLAS_DIAG ADiag,
					       const FFLAS_DIAG BDiag,
					       const size_t M,
					       const typename Field::Element alpha,
					       typename Field::Element * A, const size_t lda,
					       typename Field::Element * B, const size_t ldb);
#endif

	/** faddm.
	 * A <- A+op(B)
	 * with op(B) = B or B^T
	 */
	template<class Field>
	void faddm(const Field & F,
			  const FFLAS_TRANSPOSE transA,
			  const size_t M, const size_t N,
			  const typename Field::Element * A, const size_t lda,
			  typename Field::Element * B, const size_t ldb);

	/** faddm.
	 * C <- op(A)+op(B)
	 * with op(B) = B or B^T
	 */
	template<class Field>
	void faddm(const Field & F,
			  const FFLAS_TRANSPOSE transA,
			  const FFLAS_TRANSPOSE transB,
			  const size_t M, const size_t N,
			  const typename Field::Element * A, const size_t lda,
			  const typename Field::Element * B, const size_t ldb,
			  typename Field::Element * C, const size_t ldc );

	/** fsubm.
	 * A <- A-op(B)
	 * with op(B) = B or B^T
	 */
	template<class Field>
	void fsubm(const Field & F,
			  const FFLAS_TRANSPOSE transA,
			  const size_t M, const size_t N,
			  const typename Field::Element * A, const size_t lda,
			  typename Field::Element * B, const size_t ldb) ;

	/** fsubm.
	 * C <- op(A)-op(B)
	 * with op(B) = B or B^T
	 */
	template<class Field>
	void fsubm(const Field & F,
			  const FFLAS_TRANSPOSE transA,
			  const FFLAS_TRANSPOSE transB,
			  const size_t M, const size_t N,
			  const typename Field::Element * A, const size_t lda,
			  const typename Field::Element * B, const size_t ldb,
			  typename Field::Element * C, const size_t ldc );

	/** MatCopy makes a copy of the matrix M into a new allocated space.
	 * @param F field
	 * @param M rows of \p A
	 * @param N cols of \p A
	 * @param A matrix to be copied
	 * @param lda leading dimension of \p A
	 * @return a copy \p C of \p A with stride \p N
	 * @warning \p A and \p C belong to the same field.
	 */
	template<class Field>
	typename Field::Element* MatCopy (const Field& F,
						 const size_t M, const size_t N,
						 const typename Field::Element * A,
						 const size_t lda)
	{

		typename Field::Element * C = new typename Field::Element[M*N];
		for (size_t i = 0; i < N; ++i)
			for (size_t j = 0; j < N; ++j)
				F.assign(*(C + i*N + j),*(A + i*lda + j));
		return C;
	}

	/** \brief Computes the number of recursive levels to perform.
	 *
	 * \param m the common dimension in the product AxB
	 */
	size_t WinoSteps (const size_t m);



} // class FFLAS

#include "fflas_bounds.inl"
#include "fflas_fgemm.inl"
#include "fflas_fgemv.inl"
#include "fflas_fger.inl"
#include "fflas_ftrsm.inl"
#include "fflas_ftrmm.inl"
#include "fflas_ftrsv.inl"
#include "fflas_faxpy.inl"
#include "fflas_fdot.inl"
#include "fflas_fcopy.inl"

//BB
#ifdef LB_TRTR
#include "fflas_ftrtr.inl"
#endif

#include "fflas_faddm.inl"

#undef LB_TRTR

#endif // __FFLASFFPACK_fflas_H


