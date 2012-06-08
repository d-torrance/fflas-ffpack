/* -*- mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
// vim:sts=8:sw=8:ts=8:noet:sr:cino=>s,f0,{0,g0,(0,\:0,t0,+0,=s
/* fflas/fflas_ftrmm_src.inl
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

#define Mjoin(pre, nam) my_join(pre, nam)
#define my_join(pre, nam) pre ## nam

#ifdef  __FFLAS__BNOTRANSPOSE
#ifdef __FFLAS__ANOTRANSPOSE
#define __FFLAS_incA    1
#define __FFLAS_ldA     lda
#define __FFLAS_A_Trans NoTrans
#define __FFLAS_incB    1
#define __FFLAS_ldB     ldb
#define __FFLAS_B_Trans NoTrans
#endif
#ifdef __FFLAS__ATRANSPOSE
#define __FFLAS_incA    lda
#define __FFLAS_ldA     1
#define __FFLAS_A_Trans Trans
#define __FFLAS_incB    1
#define __FFLAS_ldB     ldb
#define __FFLAS_B_Trans NoTrans
#endif
#else
#ifdef __FFLAS__ANOTRANSPOSE
#define __FFLAS_incA    1
#define __FFLAS_ldA     lda
#define __FFLAS_A_Trans NoTrans
#define __FFLAS_incB    ldb
#define __FFLAS_ldB     1
#define __FFLAS_B_Trans Trans
#endif
#ifdef __FFLAS__ATRANSPOSE
#define __FFLAS_incA    lda
#define __FFLAS_ldA     1
#define __FFLAS_A_Trans Trans
#define __FFLAS_incB    ldb
#define __FFLAS_ldB     1
#define __FFLAS_B_Trans Trans
#endif
#endif


#ifdef __FFLAS__FLOAT
#define __FFLAS_Element float
#endif
#ifdef __FFLAS__DOUBLE
#define __FFLAS_Element double
#endif
#ifdef __FFLAS__GENERIC
#define __FFLAS_Element Element
#endif

#ifndef __FFLAS__GENERIC
template<>
class Mjoin(faddm, Mjoin(__FFLAS_A_Trans, __FFLAS_B_Trans))<__FFLAS_Element> {
public :
	template<class Field>
	void operator() (const Field & F,
			 const size_t M, const size_t N,
			 const typename Field::Element * A, const size_t lda,
			 const typename Field::Element * B, const size_t ldb,
			 typename Field::Element * C, const size_t ldc)
	{
		if (!M || !N ) return; // ne doit jamais arriver, déjà testé !

		// adding (precomputing tB ?)
		for (size_t i = 0 ; i < M ; ++i)
			for (size_t j = 0 ; j < N ; ++j)
				F.add(*(C+i*ldc+j), *(A+i*__FFLAS_ldA+j*__FFLAS_incA), *(B+i*__FFLAS_ldB+j*__FFLAS_incB)) ;


		return ;
	}

};
#else
template<class Element>
class Mjoin(faddm, Mjoin(__FFLAS_A_Trans, __FFLAS_B_Trans)) {
public :
	template<class Field>
	void operator() (const Field & F,
			 const size_t M, const size_t N,
			 const typename Field::Element * A, const size_t lda,
			 const typename Field::Element * B, const size_t ldb,
			 typename Field::Element * C, const size_t ldc)
	{
		if (!M || !N ) return; // ne doit jamais arriver, déjà testé !

		// adding (precomputing tB ?)
		for (size_t i = 0 ; i < M ; ++i)
#ifndef __FFLASFFPACK_HAVE_SSE2
			for (size_t j = 0 ; j < N ; ++j)
				*(C+i*ldc+j) = *(A+i*__FFLAS_ldA+j*__FFLAS_incA) + *(B+i*__FFLAS_ldB+j*__FFLAS_incB) ;
#else
#if defined(__FFLAS__ATRANSPOSE) || defined(__FFLAS__BTRANSPOSE)
		for (size_t j = 0 ; j < N ; ++j)
			*(C+i*ldc+j) = *(A+i*__FFLAS_ldA+j*__FFLAS_incA) + *(B+i*__FFLAS_ldB+j*__FFLAS_incB) ;
#else
#ifdef __FFLAS__DOUBLE
		{
			size_t j = 0 ;
			__m128d *av, *bv, *cv;
			av = (__m128d*)A+i*__FFLAS_ldA; // assume 16-byte aligned
			bv = (__m128d*)B+i*__FFLAS_ldB; // assume 16-byte aligned
			cv = (__m128d*)C+i*ldc;         // assume 16-byte aligned
			for (j = 0; j < N/2; ++j)
				cv[j] = _mm_add_pd(av[j], bv[j]);
			j *= 2 ;
			for (; j < N ; j++)
				*(C+i*ldc+j) = *(A+i*lda+j) + *(B+i*ldb+j) ;
		}
#else
		{
			size_t j = 0 ;
			__m128 *av, *bv, *cv;
			av = (__m128*)A+i*__FFLAS_ldA; // assume 16-byte aligned
			bv = (__m128*)B+i*__FFLAS_ldB; // assume 16-byte aligned
			cv = (__m128*)C+i*ldc;         // assume 16-byte aligned
			for (j = 0; j < N/4; ++j)
				cv[j] = _mm_add_ps(av[j], bv[j]);
			j *= 4 ;
			for (; j < N ; j++)
				*(C+i*ldc+j) = *(A+i*lda+j) + *(B+i*ldb+j) ;
		}
#endif
#endif
#endif
		// reducing :
		if (M == ldc )
			for (size_t i = 0 ; i < M*N ; ++i)
				F.init(*(C+i),*(C+i));
		else
			for (size_t i = 0 ; i < M ; ++i)
				for (size_t j = 0 ; j < N ; ++j)
					F.init(*(C+i*ldc+j), *(C+i*ldc+j));


		return ;
	}

};
#endif

#ifndef __FFLAS__GENERIC
template<>
class Mjoin(fsubm, Mjoin(__FFLAS_A_Trans, __FFLAS_B_Trans))<__FFLAS_Element > {
public :
	template<class Field>
	void operator() (const Field & F,
			 const size_t M, const size_t N,
			 const typename Field::Element * A, const size_t lda,
			 const typename Field::Element * B, const size_t ldb,
			 typename Field::Element * C, const size_t ldc)
	{
		if (!M || !N ) return; // ne doit jamais arriver, déjà testé !

		// adding (precomputing tB ?)
		for (size_t i = 0 ; i < M ; ++i)
			for (size_t j = 0 ; j < N ; ++j)
				*(C+i*ldc+j) = *(A+i*__FFLAS_ldA+j*__FFLAS_incA) - *(B+i*__FFLAS_ldB+j*__FFLAS_incB) ;

		// reducing :
		if (M == ldc )
			for (size_t i = 0 ; i < M*N ; ++i)
				F.init(*(C+i),*(C+i));
		else
			for (size_t i = 0 ; i < M ; ++i)
				for (size_t j = 0 ; j < N ; ++j)
					F.init(*(C+i*ldc+j), *(C+i*ldc+j));

		return ;

	}

};
#else
template<class Element>
class Mjoin(fsubm,Mjoin(__FFLAS_A_Trans,__FFLAS_B_Trans)) {
public :
	template<class Field>
	void operator() (const Field & F,
			 const size_t M, const size_t N,
			 const typename Field::Element * A, const size_t lda,
			 const typename Field::Element * B, const size_t ldb,
			 typename Field::Element * C, const size_t ldc)
	{
		if (!M || !N ) return; // ne doit jamais arriver, déjà testé !

		// adding (precomputing tB ?)
		for (size_t i = 0 ; i < M ; ++i)
			for (size_t j = 0 ; j < N ; ++j)
				F.sub(*(C+i*ldc+j), *(A+i*__FFLAS_ldA+j*__FFLAS_incA), *(B+i*__FFLAS_ldB+j*__FFLAS_incB)) ;
		return ;

	}

};
#endif


#undef Mjoin
#undef my_join
#undef __FFLAS_incB
#undef __FFLAS_incA
#undef __FFLAS_Element
#undef __FFLAS_ldA
#undef __FFLAS_ldB
#undef __FFLAS_Trans
#undef __FFLAS_A_Trans
#undef __FFLAS_B_Trans
#undef FFLAS_A_inc
#undef FFLAS_A_ld


