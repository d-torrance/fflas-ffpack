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

/** @internal
 * @file fflas/fflas_faddmin_src.inl
 * @ingroup fflas
 * @brief NO DOC
 */

#define Mjoin(pre, nam) my_join(pre, nam)
#define my_join(pre, nam) pre ## nam

#ifdef  __FFLAS__NOTRANSPOSE
#define __FFLAS_A_inc   1
#define __FFLAS_A_ld    lda
#define __FFLAS_Trans   NoTrans
#else
#define __FFLAS_A_inc   M
#define __FFLAS_A_ld    1
#define __FFLAS_Trans   Trans
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
class Mjoin(faddm, __FFLAS_Trans)<__FFLAS_Element> {
public :
	template<class Field>
	void operator() (const Field & F,
			 const size_t M, const size_t N,
			 const typename Field::Element * A, const size_t lda,
			 typename Field::Element * B, const size_t ldb)
	{
		if (!M || !N ) return; // ne doit jamais arriver, déjà testé !

		// adding (precomputing tB ?)
		for (size_t i = 0 ; i < M ; ++i)
#ifndef __FFLASFFPACK_HAVE_SSE2
			for (size_t j = 0 ; j < N ; ++j)
				*(B+i*ldb+j) += *(A+i*__FFLAS_A_ld+j*__FFLAS_A_inc) ;
#else
#if defined(__FFLAS__TRANSPOSE)
		for (size_t j = 0 ; j < N ; ++j)
			*(B+i*ldb+j) += *(A+i*__FFLAS_A_ld+j*__FFLAS_A_inc) ;
#else
#ifdef __FFLAS__DOUBLE
		{
			size_t j = 0 ;
			__m128d *av, *bv;
			av = (__m128d*)A+i*lda; // assume 16-byte aligned
			bv = (__m128d*)B+i*ldb; // assume 16-byte aligned
			for (j = 0; j < N/2; ++j)
				_mm_add_pd(bv[j], av[j]);
			j *= 2 ;
			for (; j<N ; ++j)
				*(B+i*ldb+j) += *(A+i*lda+j) ;
		}
#else
		{
			size_t j = 0 ;
			__m128 *av, *bv;
			av = (__m128*)A+i*lda; // assume 16-byte aligned
			bv = (__m128*)B+i*ldb; // assume 16-byte aligned
			for (j = 0; j < N/4; ++j)
				_mm_add_ps(bv[j], av[j]);
			j *= 4 ;
			for (; j < N ; j++)
				*(B+i*ldb+j)  += *(A+i*lda+j) ;
		}
#endif
#endif
#endif

		// reducing :
		if (M == ldb )
			for (size_t i = 0 ; i < M*N ; ++i)
				F.init(*(B+i),*(B+i));
		else
			for (size_t i = 0 ; i < M ; ++i)
				for (size_t j = 0 ; j < N ; ++j)
					F.init(*(B+i*ldb+j), *(B+i*ldb+j));

		return ;
	}

};
#else
template<class Element>
class Mjoin(faddm, __FFLAS_Trans) {
public :
	template<class Field>
	void operator() (const Field & F,
			 const size_t M, const size_t N,
			 const typename Field::Element * A, const size_t lda,
			 typename Field::Element * B, const size_t ldb)
	{
		if (!M || !N ) return; // ne doit jamais arriver, déjà testé !

		// adding (precomputing tB ?)
		for (size_t i = 0 ; i < M ; ++i)
			for (size_t j = 0 ; j < N ; ++j)
				F.addin(*(B+i*ldb+j), *(A+i*__FFLAS_A_ld+j*__FFLAS_A_inc)) ;

		return ;
	}

};
#endif

#ifndef __FFLAS__GENERIC
template<>
class Mjoin(fsubm, __FFLAS_Trans)<__FFLAS_Element > {
public :
	template<class Field>
	void operator() (const Field & F,
			 const size_t M, const size_t N,
			 const typename Field::Element * A, const size_t lda,
			 typename Field::Element * B, const size_t ldb)
	{
		if (!M || !N ) return; // ne doit jamais arriver, déjà testé !

		// adding (precomputing tB ?)
		for (size_t i = 0 ; i < M ; ++i)
			for (size_t j = 0 ; j < N ; ++j)
				*(B+i*ldb+j) -= *(A+i*__FFLAS_A_ld+j*__FFLAS_A_inc) ;

		// reducing :
		if (M == ldb )
			for (size_t i = 0 ; i < M*N ; ++i)
				F.init(*(B+i),*(B+i));
		else
			for (size_t i = 0 ; i < M ; ++i)
				for (size_t j = 0 ; j < N ; ++j)
					F.init(*(B+i*ldb+j), *(B+i*ldb+j));
		return ;
	}

};
#else
template<class Element>
class Mjoin(fsubm,__FFLAS_Trans) {
public :
	template<class Field>
	void operator() (const Field & F,
			 const size_t M, const size_t N,
			 const typename Field::Element * A, const size_t lda,
			 typename Field::Element * B, const size_t ldb)
	{
		if (!M || !N ) return; // ne doit jamais arriver, déjà testé !

		// adding (precomputing tB ?)
		for (size_t i = 0 ; i < M ; ++i)
			for (size_t j = 0 ; j < N ; ++j)
				F.subin(*(B+i*ldb+j), *(A+i*__FFLAS_A_ld+j*__FFLAS_A_inc)) ;

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
#undef __FFLAS_A_inc
#undef __FFLAS_A_ld


