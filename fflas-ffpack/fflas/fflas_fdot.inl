/* -*- mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
// vim:sts=8:sw=8:ts=8:noet:sr:cino=>s,f0,{0,g0,(0,\:0,t0,+0,=s

/* fflas_fdot.inl
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

#ifndef __FFLASFFPACK_fdot_INL
#define __FFLASFFPACK_fdot_INL


// Default implementation
// Specializations should be written
// to increase efficiency


namespace FFLAS {

	template<class Field>
	inline typename Field::Element
	fdot( const Field& F, const size_t N,
	      typename Field::ConstElement_ptr x, const size_t incx,
	      typename Field::ConstElement_ptr y, const size_t incy )
	{

		typename Field::Element d;
		typename Field::ConstElement_ptr xi = x;
		typename Field::ConstElement_ptr yi = y;
		F.init( d );
		for ( ; xi < x+N*incx; xi+=incx, yi+=incy )
			F.axpyin( d, *xi, *yi );
		return d;
	}

	template<>
	inline Givaro::DoubleDomain::Element
	fdot( const Givaro::DoubleDomain& , const size_t N,
	      Givaro::DoubleDomain::ConstElement_ptr x, const size_t incx,
	      Givaro::DoubleDomain::ConstElement_ptr y, const size_t incy )
	{

		return cblas_ddot( (int)N, x, (int)incx, y, (int)incy );
	}

	template<>
	inline Givaro::FloatDomain::Element
	fdot( const Givaro::FloatDomain& , const size_t N,
	      Givaro::FloatDomain::ConstElement_ptr x, const size_t incx,
	      Givaro::FloatDomain::ConstElement_ptr y, const size_t incy )
	{

		return cblas_sdot( (int)N, x, (int)incx, y, (int)incy );
	}

} // FFLAS

#endif // __FFLASFFPACK_fdot_INL
