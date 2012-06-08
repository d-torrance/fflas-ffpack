/* -*- mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
// vim:sts=8:sw=8:ts=8:noet:sr:cino=>s,f0,{0,g0,(0,\:0,t0,+0,=s

/* fflas-ffpack/modular-positive.h
 * Copyright (C) 2003 Pascal Giorgi
 *               2008 Clement Pernet
 * Written by Clement Pernet <clement.pernet@gmail.com>
 *            Pascal Giorgi  <pascal.giorgi@ens-lyon.fr>
 *
 * ------------------------------------
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

#ifndef __FFLASFFPACK_modular_double_H
#define __FFLASFFPACK_modular_double_H

#include <math.h>
#include "fflas-ffpack/field/modular-randiter.h"
#include "fflas-ffpack/field/nonzero-randiter.h"
#include "fflas-ffpack/utils/debug.h"
#include <float.h>

namespace FFPACK {

	template <>
	class Modular<double> {
	public:
		typedef double Element;

	protected:

		Element         modulus;
		unsigned long   lmodulus;

		//double inv_modulus;

	public:
		typedef unsigned long FieldInt;

		const Element one  ;
		const Element zero ;
		const Element mOne ;


		static const bool balanced = false ;

		typedef ModularRandIter<double> RandIter;
		typedef NonzeroRandIter<Modular<double>, ModularRandIter<double> > NonZeroRandIter;


		Modular () :
			modulus(0),lmodulus(0),
			one(0),zero(0),mOne(0)
		{}


		Modular (int32_t p, int exp = 1) :
			modulus((double)p), lmodulus((unsigned long)p)//, inv_modulus(1./(double)p)
			,one(1),zero(0),mOne(modulus -1)
		{
#ifdef DEBUG
			if(modulus <= 1)
				throw Failure(__func__,__FILE__,__LINE__,"modulus must be > 1");
			if( exp != 1 ) throw Failure(__func__,__FILE__,__LINE__,"exponent must be 1");
			if(modulus > getMaxModulus())
				throw Failure(__func__,__FILE__,__LINE__,"modulus is too big");
#endif
		}

		Modular (Element p) :
			modulus(p), lmodulus((unsigned long)p)
			,one(1),zero(0),mOne(modulus -1)
		{
#ifdef DEBUG
			if( modulus <= 1 )
				throw Failure(__func__,__FILE__,__LINE__,"modulus must be > 1");
			if( modulus > getMaxModulus())
				throw Failure(__func__,__FILE__,__LINE__,"modulus is too big");
#endif
		}

		Modular (unsigned long int p) :
			modulus((Element)p), lmodulus(p)
			,one(1),zero(0),mOne(modulus -1)
		{
#ifdef DEBUG
			if( (Element) modulus <= 1 )
				throw Failure(__func__,__FILE__,__LINE__,"modulus must be > 1");
			if( (Element) modulus > getMaxModulus())
				throw Failure(__func__,__FILE__,__LINE__,"modulus is too big");
#endif
		}



		Modular(const Modular<Element>& mf) :
			modulus(mf.modulus),
			lmodulus(mf.lmodulus)
			,one(mf.one),zero(mf.zero),mOne(mf.mOne)
		{}

		Modular<Element> & assign(const Modular<Element> &F)
		{
			modulus = F.modulus;
			lmodulus= F.lmodulus;
			//inv_modulus = F.inv_modulus;
			F.assign(const_cast<Element&>(one),F.one);
			F.assign(const_cast<Element&>(zero),F.zero);
			F.assign(const_cast<Element&>(mOne),F.mOne);
			return *this;
		}

#if 1
		const Modular &operator=(const Modular<double> &F)
		{
			modulus = F.modulus;
			lmodulus= F.lmodulus;
			//inv_modulus = F.inv_modulus;
			F.assign(const_cast<Element&>(one),F.one);
			F.assign(const_cast<Element&>(zero),F.zero);
			F.assign(const_cast<Element&>(mOne),F.mOne);
			return *this;
		}

#endif


		unsigned long &cardinality (unsigned long &c) const
		{
			return c = lmodulus ;
		}

		unsigned long cardinality () const
		{
			return lmodulus ;
		}

		unsigned long & characteristic (unsigned long &c) const
		{
			return c = lmodulus ;
		}

		unsigned long characteristic () const
		{
			return lmodulus;
		}

		unsigned long &convert (unsigned long &x, const Element &y) const
		{
			return x = (unsigned long)(y);
		}

		Element &convert (Element &x, const Element& y) const
		{
			return x=y;
		}

		float &convert (float &x, const Element& y) const
		{
			return x=(float)y;
		}

		std::ostream &write (std::ostream &os) const
		{
			return os << "double mod " << (int)modulus;
		}

		std::istream &read (std::istream &is)
		{
			is >> modulus;
#ifdef DEBUG
			if(modulus <= 1)
				throw Failure(__func__,__FILE__,__LINE__,"modulus must be > 1");
			if(modulus > 94906265)
				throw Failure(__func__,__FILE__,__LINE__,"modulus is too big");
#endif

			return is;
		}

		std::ostream &write (std::ostream &os, const Element &x) const
		{
			return os << (int)x;
		}

		std::istream &read (std::istream &is, Element &x) const
		{
			double tmp;
			is >> tmp;
			init(x,tmp);
			return is;
		}

		Element &init (Element &x, const unsigned long &y) const
		{
			x = Element(y % lmodulus);
			if (x < 0.) x += modulus;
			return x;
		}

		Element &init (Element &x, const long &y) const
		{
			// no problem here because double<long
			x = Element(y % (long)lmodulus);
			if (x < 0.) x += modulus;
			return x;
		}

		Element &init (Element &x, const unsigned int &y) const
		{
			x = Element(y % lmodulus);
			if (x < 0.) x += modulus;
			return x;
		}

		Element &init (Element &x, const int &y) const
		{
			// no problem here because int<=long
			x = Element(y % (long)lmodulus);
			if (x < 0.) x += modulus;
			return x;
		}

		Element& init(Element& x, Element y) const
		{

			x = fmod (y, modulus);
			if (x < 0.) x += modulus;
			return x;
		}

		Element& init(Element& x, float y) const
		{

			x = fmod ((Element)y, modulus);
			if (x < 0.) x += modulus;
			return x;
		}

		Element& init(Element& x) const
		{
			return x=0.;
		}

		 Element& assign(Element& x, const Element& y) const
		{
			return x = y;
		}

		 bool areEqual (const Element &x, const Element &y) const
		{
			return x == y;
		}

		  bool isZero (const Element &x) const
		{
			return x == 0.;
		}

		 bool isOne (const Element &x) const
		{
			return x == 1.;
		}

		 Element &add (Element &x, const Element &y, const Element &z) const
		{
			x = y + z;
			if ( x >= modulus ) x -= modulus;
			return x;
		}

		 Element &sub (Element &x, const Element &y, const Element &z) const
		{
			x = y - z;
			if (x < 0) x += modulus;
			return x;
		}

		 Element &mul (Element &x, const Element &y, const Element &z) const
		{
			x = y*z;
			return init(x,x);
		}

		 Element &div (Element &x, const Element &y, const Element &z) const
		{
			Element temp;
			inv (temp, z);
			return mul (x, y, temp);
		}

		 Element &neg (Element &x, const Element &y) const
		{
			if(y == 0) return x = 0;
			else return x = modulus - y;
		}

		 Element &inv (Element &x, const Element &y) const
		{
			// The extended Euclidean algoritm
			int x_int, y_int, q, tx, ty, temp;
			x_int = int (modulus);
			y_int = int (y);
			tx = 0;
			ty = 1;

			while (y_int != 0) {
				// always: gcd (modulus,residue) = gcd (x_int,y_int)
				//         sx*modulus + tx*residue = x_int
				//         sy*modulus + ty*residue = y_int
				q = x_int / y_int; // integer quotient
				temp = y_int; y_int = x_int - q * y_int;
				x_int = temp;
				temp = ty; ty = tx - q * ty;
				tx = temp;
			}

			if (tx < 0) tx += (int)modulus;

			// now x_int = gcd (modulus,residue)
			return x = (Element)tx;


		}

		 Element &axpy (Element &r,
				      const Element &a,
				      const Element &x,
				      const Element &y) const
		{
			r = a * x + y;
			return init(r,r);

		}

		 Element &addin (Element &x, const Element &y) const
		{
			x += y;
			if (  x >= modulus ) x -= modulus;
			return x;
		}

		 Element &subin (Element &x, const Element &y) const
		{
			x -= y;
			if (x < 0.) x += modulus;
			return x;
		}

		 Element &mulin (Element &x, const Element &y) const
		{
			return mul(x,x,y);
		}

		 Element &divin (Element &x, const Element &y) const
		{
			return div(x,x,y);
		}

		 Element &negin (Element &x) const
		{
			if (x == 0.) return x;
			else return x = modulus - x;
		}

		 Element &invin (Element &x) const
		{
			return inv (x, x);
		}

		 Element &axpyin (Element &r, const Element &a, const Element &x) const
		{
			r = r + a * x;
			return r = fmod(r, modulus);

		}

		static  Element getMaxModulus()
		{
			return 67108864.0;  // 2^26
			// return  1 << (DBL_MANT_DIG >> 1);  // 2^(DBL_MANT_DIG/2)
			// return 94906265 ;
		}

	};

} // FFPACK

// const double FFPACK::Modular<double>::one  =  1UL;
// const double FFPACK::Modular<double>::zero =  0UL;




#include "field-general.h"

#endif
