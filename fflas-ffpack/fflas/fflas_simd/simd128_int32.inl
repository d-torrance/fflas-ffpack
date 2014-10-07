/* -*- mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
// vim:sts=8:sw=8:ts=8:noet:sr:cino=>s,f0,{0,g0,(0,\:0,t0,+0,=s
/*
 * Copyright (C) 2014 the FFLAS-FFPACK group
 *
 * Written by   Bastien Vialla<bastien.vialla@lirmm.fr>
 * BB <bbboyer@ncsu.edu>
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

#ifndef __FFLASFFPACK_fflas_ffpack_utils_simd128_int32_INL
#define __FFLASFFPACK_fflas_ffpack_utils_simd128_int32_INL

// int32_t
template<>
struct Simd128_impl<true, true, true, 4> {

#if defined(__FFLASFFPACK_USE_SIMD)

	/*
	 * alias to 256 bit simd register
	 */
	using vect_t = __m128i;
	using float_t = __m128 ;

	/*
	 * define the scalar type corresponding to the specialization
	 */
	using scalar_t = int32_t;

	/*
	 *  number of scalar_t in a simd register
	 */
	static const constexpr size_t vect_size = 4;

	/*
	 *  alignement required by scalar_t pointer to be loaded in a vect_t
	 */
	static const constexpr size_t alignment = 16;

	/*
	 * Converter from vect_t to a tab.
	 * exple:
	 *      Converter conv;
	 *      conv.v = a;
	 *      scalart_t x = conv.t[1]
	 */
	union Converter{
		vect_t v;
		scalar_t t[vect_size];
	};

	/*
	 *  Return vector of type vect_t with all elements set to zero
	 *  Return [0,0,0,0] int32_t
	 */
	static INLINE CONST vect_t zero()
	{
		return _mm_setzero_si128();
	}

	/*
	 *  Broadcast 32-bit integer a to all all elements of dst. This intrinsic may generate the vpbroadcastw.
	 *  Return [x,x,x,x] int32_t
	 */
	static INLINE CONST vect_t set1(const scalar_t x)
	{
		return _mm_set1_epi32(x);
	}

	/*
	 *  Broadcast 32-bit integer a to all all elements of dst. This intrinsic may generate the vpbroadcastw.
	 *  Return [x0,x1,x2,x3] int32_t
	 */
	static INLINE CONST vect_t set(const scalar_t x0, const scalar_t x1, const scalar_t x2, const scalar_t x3)
	{
		return _mm_set_epi32(x3,x2,x1,x0);
	}

	/*
	 *  Gather 32-bit integer elements with indexes idx[0], ..., idx[3] from the address p in vect_t.
	 *  Return [p[idx[0]], p[idx[1]], p[idx[2]], p[idx[3]]] int32_t
	 */
	template<class T>
	static INLINE PURE vect_t gather(const scalar_t * const p, const T * const idx)
	{
		return set(p[idx[0]], p[idx[1]], p[idx[2]], p[idx[3]]);
	}

	/*
	 * Load 128-bits of integer data from memory into dst.
	 * p must be aligned on a 32-byte boundary or a general-protection exception will be generated.
	 * Return [p[0],p[1],p[2],p[3]] int32_t
	 */
	static INLINE PURE vect_t load(const scalar_t * const p)
	{
		return _mm_load_si128(reinterpret_cast<const vect_t*>(p));
	}

	/*
	 * Load 128-bits of integer data from memory into dst.
	 * p does not need to be aligned on any particular boundary.
	 * Return [p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7]] int32_t
	 */
	static INLINE PURE vect_t loadu(const scalar_t * const p)
	{
		return _mm_loadu_si128(reinterpret_cast<const vect_t*>(p));
	}

	/*
	 * Store 128-bits of integer data from a into memory.
	 * p must be aligned on a 32-byte boundary or a general-protection exception will be generated.
	 */
	static INLINE void store(const scalar_t * p, vect_t v)
	{
		_mm_store_si128(reinterpret_cast<vect_t*>(const_cast<scalar_t*>(p)), v);
	}

	/*
	 * Store 128-bits of integer data from a into memory.
	 * p does not need to be aligned on any particular boundary.
	 */
	static INLINE void storeu(const scalar_t * p, vect_t v)
	{
		_mm_storeu_si128(reinterpret_cast<vect_t*>(const_cast<scalar_t*>(p)), v);
	}

	/*
	 * Add packed 32-bits integer in a and b, and store the results in vect_t.
	 * Args   : [a0, a1, a2, a3] int32_t
	 [b0, b1, b2, b3] int32_t
	 * Return : [a0+b0, a1+b1, a2+b2, a3+b3]   int32_t
	 */
	static INLINE CONST vect_t add(const vect_t a, const vect_t b)
	{
		return _mm_add_epi32(a, b);
	}

	static INLINE vect_t addin(vect_t &a, const vect_t b)
	{
		return a = add(a,b);
	}

	/*
	 * Subtract packed 32-bit integers in b from packed 32-bit integers in a, and store the results in vect_t.
	 * Args   : [a0, a1, a2, a3] int32_t
	 [b0, b1, b2, b3] int32_t
	 * Return : [a0-b0, a1-b1, a2-b2, a3-b3]  int32_t
	 */
	static INLINE CONST vect_t sub(const vect_t a, const vect_t b)
	{
		return _mm_sub_epi32(a, b);
	}

	static INLINE vect_t subin(vect_t &a, const vect_t b)
	{
		return a = sub(a,b);
	}

	/*
	 * Multiply the packed 32-bit integers in a and b, producing intermediate 64-bit integers, and store the low 32 bits of the intermediate integers in vect_t.
	 * Args   : [a0, a1, a2, a3]           int32_t
	 [b0, b1, b2, b3]           int32_t
	 * Return : [a0*b0 mod 2^16-1, a1*b1 mod 2^16-1, a2*b2 mod 2^16-1, a3*b3 mod 2^16-1] int32_t
	 */
	static INLINE CONST vect_t mullo(const vect_t a, const vect_t b)
	{
		return _mm_mullo_epi32(a, b);
	}

	/*
	 * Multiply the packed 32-bit integers in a and b, producing intermediate 64-bit integers, and store the high 32 bits of the intermediate integers in vect_t.
	 * Args   : [a0, a1, a2, a3] int32_t
	 [b0, b1, b2, b3] int32_t
	 * Return :
	 */
	static INLINE CONST vect_t mulhi(const vect_t a, const vect_t b)
	{
		// _mm_mulhi_epi32 emul
#pragma warning "The simd mulhi function is emulate, it may impact the performances."
		vect_t a1, a2, b1, b2;
		a1 = set(0, _mm_extract_epi32(a, 0), 0, _mm_extract_epi32(a, 1));
		a2 = set(0, _mm_extract_epi32(a, 1), 0, _mm_extract_epi32(a, 3));
		b1 = set(0, _mm_extract_epi32(b, 0), 0, _mm_extract_epi32(b, 1));
		b2 = set(0, _mm_extract_epi32(b, 1), 0, _mm_extract_epi32(b, 3));
		a1 = _mm_mul_epi32(a1,b1);
		a2 = _mm_mul_epi32(a2,b2);
		return set(_mm_extract_epi32(a1,0),_mm_extract_epi32(a1,2),_mm_extract_epi32(b1,0),_mm_extract_epi32(b2,0));
	}

	/*
	 * Multiply the low 16-bit integers from each packed 32-bit element in a and b, and store the signed 32-bit results in vect_t.
	 * Args   : [a0, a1, a2, a3]    int32_t
	 [b0, b1, b2, b3]    int32_t
	 * Return : [a0*b0, a1*b1, a2*b2, a3*b3]    int32_t
	 */
	static INLINE CONST vect_t mulx(const vect_t a, const vect_t b)
	{
		return _mm_mul_epi32(a, b);
	}

	/*
	 * Multiply the packed 32-bit integers in a and b, producing intermediate 32-bit integers, and store the low 16 bits of the intermediate integers in vect_t.
	 * Args   : [a0, a1, a2, a3]           int32_t
	 [b0, b1, b2, b3]           int32_t
	 * Return : [a0*b0 mod 2^16-1, a1*b1 mod 2^16-1, a2*b2 mod 2^16-1, a3*b3 mod 2^16-1] int32_t
	 */
	static INLINE CONST vect_t mul(const vect_t a, const vect_t b)
	{
		return mullo(a, b);
	}

	/*
	 *
	 * Args   : [a0, a1, a2, a3]           int32_t
	 [b0, b1, b2, b3]           int32_t
	 [c0, c1, c2, c3]    int32_t
	 * Return : [(a0*b0 mod 2^16-1)+c0, (a1*b1 mod 2^16-1)+c1, (a2*b2 mod 2^16-1)+c2, (a3*b3 mod 2^16-1)+c3] int32_t
	 */
	static INLINE CONST vect_t fmadd(const vect_t c, const vect_t a, const vect_t b)
	{
		return add(c,mul(a,b));
	}

	static INLINE vect_t fmaddin(vect_t & c, const vect_t a, const vect_t b)
	{
		return c = fmadd(c,a,b);
	}

	/*
	 *
	 * Args   : [a0, a1, a2, a3]           int32_t
	 [b0, b1, b2, b3]           int32_t
	 [c0, c1, c2, c3]    int32_t
	 * Return : [-(a0*b0 mod 2^16-1)+c0, -(a1*b1 mod 2^16-1)+c1, -(a2*b2 mod 2^16-1)+c2, -(a3*b3 mod 2^16-1)+c3] int32_t
	 */
	static INLINE CONST vect_t fnmadd(const vect_t c, const vect_t a, const vect_t b)
	{
		return sub(c,mul(a,b));
	}

	static INLINE CONST vect_t fnmaddin(vect_t c, const vect_t a, const vect_t b)
	{
		return c = sub(c,mul(a,b));
	}

	/*
	 *
	 * Args   : [a0, a1, a2, a3]           int32_t
	 [b0, b1, b2, b3]           int32_t
	 [c0, c1, c2, c3]    int32_t
	 * Return : [(a0*b0 mod 2^16-1)-c0, (a1*b1 mod 2^16-1)-c1, (a2*b2 mod 2^16-1)-c2, (a3*b3 mod 2^16-1)-c3] int32_t
	 */
	static INLINE CONST vect_t fmsub(const vect_t c, const vect_t a, const vect_t b)
	{
		return sub(mul(a,b),c);
	}

	/*
	 *
	 * Args   : [a0, a1, a2, a3]           int32_t
	 [b0, b1, b2, b3]           int32_t
	 [c0, c1, c2, c3]    int32_t
	 * Return : [(a0*b0 mod 2^16-1)-c0, (a1*b1 mod 2^16-1)-c1, (a2*b2 mod 2^16-1)-c2, (a3*b3 mod 2^16-1)-c3] int32_t
	 */
	static INLINE CONST vect_t fmsubin(vect_t c, const vect_t a, const vect_t b)
	{
		return c = sub(mul(a,b),c);
	}

	/*
	 * Compare packed 32-bits in a and b for equality, and store the results in vect_t.
	 * Args   : [a0, a1, a2, a3, a4, a5, a6, a7] int32_t
	 [b0, b1, b2, b3, b4, b5, b6, b7] int32_t
	 * Return : [(a0==b0) ? 0xFFFFFFFF : 0, (a1==b1) ? 0xFFFFFFFF : 0,
	 (a2==b2) ? 0xFFFFFFFF : 0, (a3==b3) ? 0xFFFFFFFF : 0,
	 (a4==b4) ? 0xFFFFFFFF : 0, (a5==b5) ? 0xFFFFFFFF : 0,
	 (a6==b6) ? 0xFFFFFFFF : 0, (a7==b7) ? 0xFFFFFFFF : 0]                     int32_t
	 */
	static INLINE CONST vect_t eq(const vect_t a, const vect_t b)
	{
		return _mm_cmpeq_epi32(a, b);
	}

	/*
	 * Compare packed 32-bits in a and b for greater-than, and store the results in vect_t.
	 * Args   : [a0, a1, a2, a3, a4, a5, a6, a7] int32_t
	 [b0, b1, b2, b3, b4, b5, b6, b7] int32_t
	 * Return : [(a0>b0) ? 0xFFFFFFFF : 0, (a1>b1) ? 0xFFFFFFFF : 0,
	 (a2>b2) ? 0xFFFFFFFF : 0, (a3>b3) ? 0xFFFFFFFF : 0,
	 (a4>b4) ? 0xFFFFFFFF : 0, (a5>b5) ? 0xFFFFFFFF : 0,
	 (a6>b6) ? 0xFFFFFFFF : 0, (a7>b7) ? 0xFFFFFFFF : 0]                      int32_t
	 */
	static INLINE CONST vect_t greater(const vect_t a, const vect_t b)
	{
		return _mm_cmpgt_epi32(a, b);
	}

	/*
	 * Compare packed 32-bits in a and b for lesser-than, and store the results in vect_t.
	 * Args   : [a0, a1, a2, a3, a4, a5, a6, a7] int32_t
	 [b0, b1, b2, b3, b4, b5, b6, b7] int32_t
	 * Return : [(a0<b0) ? 0xFFFFFFFF : 0, (a1<b1) ? 0xFFFFFFFF : 0,
	 (a2<b2) ? 0xFFFFFFFF : 0, (a3<b3) ? 0xFFFFFFFF : 0,
	 (a4<b4) ? 0xFFFFFFFF : 0, (a5<b5) ? 0xFFFFFFFF : 0,
	 (a6<b6) ? 0xFFFFFFFF : 0, (a7<b7) ? 0xFFFFFFFF : 0]                      int32_t
	 */
	static INLINE CONST vect_t lesser(const vect_t a, const vect_t b)
	{
		return _mm_cmpgt_epi32(b, a);
	}

	/*
	 * Compare packed 32-bits in a and b for greater or equal than, and store the results in vect_t.
	 * Args   : [a0, a1, a2, a3, a4, a5, a6, a7] int32_t
	 [b0, b1, b2, b3, b4, b5, b6, b7] int32_t
	 * Return : [(a0>=b0) ? 0xFFFFFFFF : 0, (a1>=b1) ? 0xFFFFFFFF : 0,
	 (a2>=b2) ? 0xFFFFFFFF : 0, (a3>=b3) ? 0xFFFFFFFF : 0,
	 (a4>=b4) ? 0xFFFFFFFF : 0, (a5>=b5) ? 0xFFFFFFFF : 0,
	 (a6>=b6) ? 0xFFFFFFFF : 0, (a7>=b7) ? 0xFFFFFFFF : 0]                    int32_t
	 */
	static INLINE CONST vect_t greater_eq(const vect_t a, const vect_t b)
	{
		return vor(greater(a, b), eq(a, b));
	}

	/*
	 * Compare packed 32-bits in a and b for lesser or equal than, and store the results in vect_t.
	 * Args   : [a0, a1, a2, a3, a4, a5, a6, a7] int32_t
	 [b0, b1, b2, b3, b4, b5, b6, b7] int32_t
	 * Return : [(a0<=b0) ? 0xFFFFFFFF : 0, (a1<=b1) ? 0xFFFFFFFF : 0,
	 (a2<=b2) ? 0xFFFFFFFF : 0, (a3<=b3) ? 0xFFFFFFFF : 0,
	 (a4<=b4) ? 0xFFFFFFFF : 0, (a5<=b5) ? 0xFFFFFFFF : 0,
	 (a6<=b6) ? 0xFFFFFFFF : 0, (a7<=b7) ? 0xFFFFFFFF : 0]                     int32_t
	 */
	static INLINE CONST vect_t lesser_eq(const vect_t a, const vect_t b)
	{
		return vor(lesser(a, b), eq(a, b));
	}

	/*
	 * Compute the bitwise AND of packed 32-bits integer in a and b, and store the results in vect_t.
	 * Args   : [a0, a1, a2, a3, a4, a5, a6, a7]
	 [b0, b1, b2, b3, b4, b5, b6, b7]
	 * Return : [a0 AND b0, a1 AND b1, a2 AND b2, a3 AND b3, a4 AND b4, a5 AND b5, a6 AND b6, a7 AND b7]
	 */
	static INLINE CONST vect_t vand(const vect_t a, const vect_t b)
	{
		return _mm_and_si128(b, a);
	}

	/*
	 * Compute the bitwise OR of packed 32-bits integer in a and b, and store the results in vect_t.
	 * Args   : [a0, a1, a2, a3, a4, a5, a6, a7]
	 [b0, b1, b2, b3, b4, b5, b6, b7]
	 * Return : [a0 OR b0, a1 OR b1, a2 OR b2, a3 OR b3, a4 OR b4, a5 OR b5, a6 OR b6, a7 OR b7]
	 */
	static INLINE CONST vect_t vor(const vect_t a, const vect_t b)
	{
		return _mm_or_si128(b, a);
	}

	/*
	 * Compute the bitwise XOR of packed 32-bits integer in a and b, and store the results in vect_t.
	 * Args   : [a0, a1, a2, a3, a4, a5, a6, a7]
	 [b0, b1, b2, b3, b4, b5, b6, b7]
	 * Return : [a0 XOR b0, a1 XOR b1, a2 XOR b2, a3 XOR b3, a4 XOR b4, a5 XOR b5, a6 XOR b6, a7 XOR b7]
	 */
	static INLINE CONST vect_t vxor(const vect_t a, const vect_t b)
	{
		return _mm_xor_si128(b, a);
	}

	/*
	 * Compute the bitwise AND NOT of packed 32-bits integer in a and b, and store the results in vect_t.
	 * Args   : [a0, a1, a2, a3, a4, a5, a6, a7]
	 [b0, b1, b2, b3, b4, b5, b6, b7]
	 * Return : [a0 ANDNOT b0, a1 ANDNOT b1, a2 ANDNOT b2, a3 ANDNOT b3, a4 ANDNOT b4, a5 ANDNOT b5, a6 ANDNOT b6, a7 ANDNOT b7]
	 */
	static INLINE CONST vect_t vandnot(const vect_t a, const vect_t b)
	{
		return _mm_andnot_si128(b, a);
	}

	/*
	 * Horizontally add 32-bits elements of a.
	 * Args   : [a0, a1, a2, a3]
	 * Return : a0+a1+a2+a3
	 */
	static INLINE CONST scalar_t hadd_to_scal(const vect_t a)
	{
		Converter conv;
		conv.v = a;
		return conv.t[0] + conv.t[1] + conv.t[2] + conv.t[3];
	}

	static INLINE CONST vect_t fmaddx(const vect_t c, const vect_t a, const vect_t b)
	{
		return add(mulx(a, b),c);
	}

	static INLINE vect_t fmaddxin(vect_t & c, const vect_t a, const vect_t b)
	{
		return c = fmaddx(c,a,b);
	}

	static INLINE CONST vect_t fnmaddx(const vect_t c, const vect_t a, const vect_t b)
	{
		return sub(mulx(a, b),c);
	}

	static INLINE vect_t fnmaddxin(vect_t & c, const vect_t a, const vect_t b)
	{
		return c = fnmaddx(c,a,b);
	}

	static INLINE CONST vect_t round(const vect_t a)
	{
		return a ;
	}


	static INLINE vect_t mod(vect_t & C, const vect_t & P
				 , const float_t & INVP, const vect_t & NEGP
				 , const vect_t & MIN, const vect_t & MAX
				 , vect_t & Q, vect_t & T
				)
	{
#ifdef __INTEL_COMPILER
		C = _mm_rem_epi32(C,P)
#else
		FFLASFFPACK_abort("pas implementé");
		// C = fnmadd(C,_mm_castps_si128(_mm_floor_ps(_mm_mul_ps(INVP,_mm_castsi128_ps(C)))),P);
#endif
		NORML_MOD(C,P,NEGP,MIN,MAX,Q,T);
		return C;
	}

#else
#error "You need SSE instructions to perform 128 bits operations on int32"
#endif
};

// uint32_t
template<>
struct Simd128_impl<true, true, false, 4> : public Simd128_impl<true, true, true, 4> {
	using scalar_t = uint32_t;

	static INLINE CONST vect_t greater(vect_t a, vect_t b)
	{

		vect_t x;
		x = set1(-(static_cast<scalar_t>(1)<<(sizeof(scalar_t)*8-1)));
		a = sub(x, a);
		b = sub(x, b);
		return _mm_cmpgt_epi32(a, b);
	}

	static INLINE CONST vect_t lesser(vect_t a, vect_t b)
	{
		vect_t x;
		x = set1(-(static_cast<scalar_t>(1)<<(sizeof(scalar_t)*8-1)));
		a = sub(x, a);
		b = sub(x, b);
		return _mm_cmpgt_epi32(a, b);
	}

	static INLINE CONST vect_t greater_eq(const vect_t a, const vect_t b)
	{
		return vor(greater(a, b), eq(a, b));
	}

	static INLINE CONST vect_t lesser_eq(const vect_t a, const vect_t b)
	{
		return vor(lesser(a, b), eq(a, b));
	}
};

#endif // __FFLASFFPACK_fflas_ffpack_utils_simd128_int32_INL