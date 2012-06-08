/* -*- mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
// vim:sts=8:sw=8:ts=8:noet:sr:cino=>s,f0,{0,g0,(0,\:0,t0,+0,=s
/* fflas/fflas_faddm.inl
 * Copyright (C) 2010 LinBox
 *
 * Written by Brice Boyer <Brice.Boyer@imag.fr>
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

/*! @internal
 * @file fflas/fflas_faddm.inl
 * @ingroup fflas
 * @brief NO DOC
 */

#ifndef __FFLASFFPACK_fflas_faddm_H
#define __FFLASFFPACK_fflas_faddm_H

#ifdef __FFLASFFPACK_HAVE_SSE2
#include <emmintrin.h>
#endif

namespace FFLAS {

	template<class Field>
	inline void faddm(const Field & F,
			  const FFLAS_TRANSPOSE transA,
			  const size_t M, const size_t N,
			  const typename Field::Element * A, const size_t lda,
			  typename Field::Element * B, const size_t ldb)
	{
		if (!M || !N) return ;
		if (transA ==  FflasNoTrans)
			Protected::faddmNoTrans<typename Field::Element>()(F,M,N,A,lda,B,ldb);
		else
			Protected::faddmTrans<typename Field::Element>()(F,M,N,A,lda,B,ldb);
		return ;
	}


	template<class Field>
	inline void faddm(const Field & F,
			  const FFLAS_TRANSPOSE transA,
			  const FFLAS_TRANSPOSE transB,
			  const size_t M, const size_t N,
			  const typename Field::Element * A, const size_t lda,
			  const typename Field::Element * B, const size_t ldb,
			  typename Field::Element * C, const size_t ldc )
	{
		if (!M || !N) return ;
		if (transA ==  FflasNoTrans)
			if (transB ==  FflasNoTrans)
				Protected::faddmNoTransNoTrans<typename Field::Element>()(F,M,N,A,lda,B,ldb,C,ldc);
			else
				Protected::faddmNoTransTrans<typename Field::Element>()(F,M,N,A,lda,B,ldb,C,ldc);
		else
			if (transB ==  FflasNoTrans)
				Protected::faddmTransNoTrans<typename Field::Element>()(F,M,N,A,lda,B,ldb,C,ldc);
			else
				Protected::faddmTransTrans<typename Field::Element>()(F,M,N,A,lda,B,ldb,C,ldc);

		return ;
	}


	template<class Field>
	inline void fsubm(const Field & F,
			  const FFLAS_TRANSPOSE transA,
			  const size_t M, const size_t N,
			  const typename Field::Element * A, const size_t lda,
			  typename Field::Element * B, const size_t ldb)

	{
		if (!M || !N) return ;
		if (transA ==  FflasNoTrans)
			Protected::fsubmNoTrans<typename Field::Element>()(F,M,N,A,lda,B,ldb);
		else
			Protected::fsubmTrans<typename Field::Element>()(F,M,N,A,lda,B,ldb);
		return ;
	}


	template<class Field>
	inline void fsubm(const Field & F,
			  const FFLAS_TRANSPOSE transA,
			  const FFLAS_TRANSPOSE transB,
			  const size_t M, const size_t N,
			  const typename Field::Element * A, const size_t lda,
			  const typename Field::Element * B, const size_t ldb,
			  typename Field::Element * C, const size_t ldc )
	{
		if (!M || !N) return ;
		if (transA ==  FflasNoTrans)
			if (transB ==  FflasNoTrans)
				Protected::fsubmNoTransNoTrans<typename Field::Element>()(F,M,N,A,lda,B,ldb,C,ldc);
			else
				Protected::fsubmNoTransTrans<typename Field::Element>()(F,M,N,A,lda,B,ldb,C,ldc);
		else
			if (transB ==  FflasNoTrans)
				Protected::fsubmTransNoTrans<typename Field::Element>()(F,M,N,A,lda,B,ldb,C,ldc);
			else
				Protected::fsubmTransTrans<typename Field::Element>()(F,M,N,A,lda,B,ldb,C,ldc);

		return ;
	}

#ifndef DOXYGEN_SHOULD_SKIP_THIS

	namespace Protected {

#undef __FFLAS__FLOAT
#undef __FFLAS__DOUBLE
#undef __FFLAS__GENERIC
#undef __FFLAS__NOTRANSPOSE
#undef __FFLAS__ATRANSPOSE
#undef __FFLAS__ANOTRANSPOSE
#undef __FFLAS__BTRANSPOSE
#undef __FFLAS__BNOTRANSPOSE
#undef __FFLAS__TRANSPOSE
#undef __FFLAS__NOTRANSPOSE

#define __FFLAS__GENERIC
#define __FFLAS__NOTRANSPOSE // no transpose
#include "fflas_faddmin_src.inl"
#undef  __FFLAS__NOTRANSPOSE
#undef  __FFLAS__GENERIC


#define __FFLAS__GENERIC
#define __FFLAS__TRANSPOSE // no transpose
#include "fflas_faddmin_src.inl"
#undef  __FFLAS__TRANSPOSE
#undef  __FFLAS__GENERIC

#define __FFLAS__FLOAT
#define __FFLAS__NOTRANSPOSE // no transpose
#include "fflas_faddmin_src.inl"
#undef  __FFLAS__NOTRANSPOSE
#undef  __FFLAS__FLOAT

#define __FFLAS__FLOAT
#define __FFLAS__TRANSPOSE // no transpose
#include "fflas_faddmin_src.inl"
#undef  __FFLAS__TRANSPOSE
#undef  __FFLAS__FLOAT

#define __FFLAS__DOUBLE
#define __FFLAS__NOTRANSPOSE // no transpose
#include "fflas_faddmin_src.inl"
#undef  __FFLAS__NOTRANSPOSE
#undef  __FFLAS__DOUBLE

#define __FFLAS__DOUBLE
#define __FFLAS__TRANSPOSE // no transpose
#include "fflas_faddmin_src.inl"
#undef  __FFLAS__TRANSPOSE
#undef  __FFLAS__DOUBLE

		//
#define __FFLAS__GENERIC
#define __FFLAS__ANOTRANSPOSE // no A transpose
#define __FFLAS__BNOTRANSPOSE // no B transpose
#include "fflas_faddm_src.inl"
#undef  __FFLAS__BNOTRANSPOSE // no B transpose
#undef  __FFLAS__ANOTRANSPOSE // no A transpose
#undef  __FFLAS__GENERIC

#define __FFLAS__GENERIC
#define __FFLAS__ANOTRANSPOSE // no A transpose
#define __FFLAS__BTRANSPOSE // no B transpose
#include "fflas_faddm_src.inl"
#undef  __FFLAS__BTRANSPOSE // no B transpose
#undef  __FFLAS__ANOTRANSPOSE // no A transpose
#undef  __FFLAS__GENERIC


#define __FFLAS__GENERIC
#define __FFLAS__ATRANSPOSE // no A transpose
#define __FFLAS__BNOTRANSPOSE // no B transpose
#include "fflas_faddm_src.inl"
#undef  __FFLAS__BNOTRANSPOSE // no B transpose
#undef  __FFLAS__ATRANSPOSE // no A transpose
#undef  __FFLAS__GENERIC

#define __FFLAS__GENERIC
#define __FFLAS__ATRANSPOSE // no A transpose
#define __FFLAS__BTRANSPOSE // no B transpose
#include "fflas_faddm_src.inl"
#undef  __FFLAS__BTRANSPOSE // no B transpose
#undef  __FFLAS__ATRANSPOSE // no A transpose
#undef  __FFLAS__GENERIC

		//
#define __FFLAS__FLOAT
#define __FFLAS__ANOTRANSPOSE // no A transpose
#define __FFLAS__BNOTRANSPOSE // no B transpose
#include "fflas_faddm_src.inl"
#undef  __FFLAS__BNOTRANSPOSE // no B transpose
#undef  __FFLAS__ANOTRANSPOSE // no A transpose
#undef  __FFLAS__FLOAT

#define __FFLAS__FLOAT
#define __FFLAS__ANOTRANSPOSE // no A transpose
#define __FFLAS__BTRANSPOSE // no B transpose
#include "fflas_faddm_src.inl"
#undef  __FFLAS__BTRANSPOSE // no B transpose
#undef  __FFLAS__ANOTRANSPOSE // no A transpose
#undef  __FFLAS__FLOAT


#define __FFLAS__FLOAT
#define __FFLAS__ATRANSPOSE // no A transpose
#define __FFLAS__BNOTRANSPOSE // no B transpose
#include "fflas_faddm_src.inl"
#undef  __FFLAS__BNOTRANSPOSE // no B transpose
#undef  __FFLAS__ATRANSPOSE // no A transpose
#undef  __FFLAS__FLOAT

#define __FFLAS__FLOAT
#define __FFLAS__ATRANSPOSE // no A transpose
#define __FFLAS__BTRANSPOSE // no B transpose
#include "fflas_faddm_src.inl"
#undef  __FFLAS__BTRANSPOSE // no B transpose
#undef  __FFLAS__ATRANSPOSE // no A transpose
#undef  __FFLAS__FLOAT

		//
#define __FFLAS__DOUBLE
#define __FFLAS__ANOTRANSPOSE // no A transpose
#define __FFLAS__BNOTRANSPOSE // no B transpose
#include "fflas_faddm_src.inl"
#undef  __FFLAS__BNOTRANSPOSE // no B transpose
#undef  __FFLAS__ANOTRANSPOSE // no A transpose
#undef  __FFLAS__DOUBLE

#define __FFLAS__DOUBLE
#define __FFLAS__ANOTRANSPOSE // no A transpose
#define __FFLAS__BTRANSPOSE // no B transpose
#include "fflas_faddm_src.inl"
#undef  __FFLAS__BTRANSPOSE // no B transpose
#undef  __FFLAS__ANOTRANSPOSE // no A transpose
#undef  __FFLAS__DOUBLE


#define __FFLAS__DOUBLE
#define __FFLAS__ATRANSPOSE // no A transpose
#define __FFLAS__BNOTRANSPOSE // no B transpose
#include "fflas_faddm_src.inl"
#undef  __FFLAS__BNOTRANSPOSE // no B transpose
#undef  __FFLAS__ATRANSPOSE // no A transpose
#undef  __FFLAS__DOUBLE

#define __FFLAS__DOUBLE
#define __FFLAS__ATRANSPOSE // no A transpose
#define __FFLAS__BTRANSPOSE // no B transpose
#include "fflas_faddm_src.inl"
#undef  __FFLAS__BTRANSPOSE // no B transpose
#undef  __FFLAS__ATRANSPOSE // no A transpose
#undef  __FFLAS__DOUBLE



	} // Protected

#endif // SKIPPED BY DOXYGEN

} // FFLAS

#endif // __FFLASFFPACK_fflas_faddm_H
