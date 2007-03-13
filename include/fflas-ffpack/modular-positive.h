/* -*- mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */

/* linbox/field/modular-double.h
 * Copyright (C) 2003 Pascal Giorgi
 *
 * Written by Pascal Giorgi <pascal.giorgi@ens-lyon.fr>
 *
 * ------------------------------------
 *
 * See COPYING for license information.
 */




#ifndef __LINBOX_MODULAR_DOUBLE_H
#define __LINBOX_MODULAR_DOUBLE_H


#include <math.h>
# include <sys/time.h>




// Namespace in which all LinBox code resides
	
	template< class Element >
	class Modular;

	template <class Element>
	class ModularRandIter
	{
	public:
		ModularRandIter (const Modular<Element> &F):_F(F){
			struct timeval tp;
			gettimeofday(&tp, 0) ;
			long _seed = (long)(tp.tv_usec);
			srand48(_seed);
		}
		ModularRandIter (const ModularRandIter<Element> &R) 
			: _F (R._F) {}
		Element &random (Element &a) const
		{ return _F.init(a,(double)lrand48()); }
	private:
		Modular<Element> _F;
	
	};

	template <>
	class Modular<double> {

	protected:

		double  modulus;
		long   lmodulus;

		//double inv_modulus;
		
	public:	       
		typedef double Element;
		typedef unsigned long FieldInt;
		typedef ModularRandIter<double> RandIter;

		const bool balanced;

		Modular (): balanced (false) {}

// 		Modular (int32 p, int exp = 1)  : modulus((double)p), lmodulus(p)//, inv_modulus(1./(double)p) 
// 		{
			// if(modulus <= 1)
// 				throw PreconditionFailed(__FUNCTION__,__LINE__,"modulus must be > 1");
// 			if( exp != 1 ) throw PreconditionFailed(__FUNCTION__,__LINE__,"exponent must be 1");
// 			unsigned long max;
// 			if(modulus > (double) FieldTraits<Modular<double> >::maxModulus(max))
// 				throw PreconditionFailed(__FUNCTION__,__LINE__,"modulus is too big");
			
// 		}

		Modular (double p) : modulus(p),  lmodulus((unsigned long)p), balanced(false) {
			// if( modulus <= 1 )
// 				throw PreconditionFailed(__FUNCTION__,__LINE__,"modulus must be > 1");
// 			unsigned long max;
// 			if( modulus > (double) FieldTraits<Modular<double> >::maxModulus(max))
// 				throw PreconditionFailed(__FUNCTION__,__LINE__,"modulus is too big");
		}

		Modular (long int p) :modulus((double)p), lmodulus(p),  balanced(false) {
// 			if( (double) modulus <= 1 )
// 				throw PreconditionFailed(__FUNCTION__,__LINE__,"modulus must be > 1");
// 			unsigned long max;
// 			if( (double) modulus > (double) FieldTraits<Modular<double> >::maxModulus(max))
// 				throw PreconditionFailed(__FUNCTION__,__LINE__,"modulus is too big");
		}

		Modular (const unsigned long& p) : modulus((double) p),lmodulus(p), balanced(false) //, inv_modulus(1./(double)p)
		{
// 			if(modulus <= 1)
// 				throw PreconditionFailed(__FUNCTION__,__LINE__,"modulus must be > 1");
// 	             	if(modulus > 94906265)
// 				throw PreconditionFailed(__FUNCTION__,__LINE__,"modulus is too big");
				
		}

		Modular(const Modular<double>& mf) : modulus(mf.modulus), lmodulus(mf.lmodulus), balanced(false)//,inv_modulus(mf.inv_modulus)
		{}

		const Modular &operator=(const Modular<double> &F) {
			modulus = F.modulus;
			lmodulus= F.lmodulus;
			//inv_modulus = F.inv_modulus;
			return *this;
		}

	
		unsigned long &cardinality (unsigned long &c) const{ 
			return c = (unsigned long)(modulus);
		}

		unsigned long &characteristic (FieldInt& c) const {
			return c = (FieldInt)(modulus); 
		}

		unsigned long &convert (unsigned long &x, const Element &y) const { 
			return x = (unsigned long)(y);
		}

		double &convert (double &x, const Element& y) const {
			return x=y;
		}
		
		std::ostream &write (std::ostream &os) const {
			return os << "double mod " << (int)modulus;
		}
		
		std::istream &read (std::istream &is) {
			is >> modulus; 
// 			if(modulus <= 1) 
// 				throw PreconditionFailed(__FUNCTION__,__LINE__,"modulus must be > 1");
// 		 	if(modulus > 94906265) 
// 				throw PreconditionFailed(__FUNCTION__,__LINE__,"modulus is too big");

			return is;
		}
		
		std::ostream &write (std::ostream &os, const Element &x) const {
			return os << x;
		}

		std::istream &read (std::istream &is, Element &x) const {
			unsigned long tmp;
			is >> tmp;
			init(x,tmp); 
			return is;
		}
		

		Element &init (Element &x, const unsigned long &y) const  {
			//	x = mpz_fdiv_ui(y.get_mpz(),lmodulus );
			x = y % long (modulus);
			if (x < 0) x += modulus;
			return x;
		}

		inline Element& init(Element& x, double y =0) const {		  

			//double tmp = y;

			/*
			int sign=0;
			if (tmp < 0.0) {
				tmp=-tmp;
				sign=1;
			}	
			*/	

			//			tmp = floor (y + 0.5);

			//Some odds donot support it. It is in C99.
			//tmp = round (y); 
			
			x = fmod (y, modulus);

			/*
			if (tmp > modulus) 
				tmp -= (modulus * floor( tmp*inv_modulus));

			if ( (!tmp) || (tmp == modulus) ){
				return x = 0.0;
				
			}
			else
				if (sign)
					return x = modulus-tmp;
				else
					return x = tmp;
			*/

			if (x < 0) x += modulus;

			return x;
		}

		
		
		inline Element& assign(Element& x, const Element& y) const {
			return x = y;
		}
									
		
		inline bool areEqual (const Element &x, const Element &y) const {
			return x == y;
		}

		inline  bool isZero (const Element &x) const {
			return x == 0.; 
		}
		
		inline bool isOne (const Element &x) const {
			return x == 1.; 
		}

		inline Element &add (Element &x, const Element &y, const Element &z) const {
			x = y + z;
			if ( x >= modulus ) x -= modulus;
			return x;
		}
 
		inline Element &sub (Element &x, const Element &y, const Element &z) const {
			x = y - z;
			if (x < 0) x += modulus;
			return x;
		}
		
		inline Element &mul (Element &x, const Element &y, const Element &z) const {		
			double tmp= y*z;
			x= fmod(tmp, modulus);
			//x= tmp - floor(tmp*inv_modulus)*modulus;
		  
			return x;
		}
 
		inline Element &div (Element &x, const Element &y, const Element &z) const {
			Element temp;
			inv (temp, z);
			return mul (x, y, temp);
		}
 
		inline Element &neg (Element &x, const Element &y) const {
			if(y == 0) return x = 0;
			else return x = modulus - y;
		}
 
		inline Element &inv (Element &x, const Element &y) const {
			// The extended Euclidean algoritm 
			int x_int, y_int, q, tx, ty, temp;
			x_int = (int) modulus;
			y_int = (int) y;
			tx = 0; 
			ty = 1;
		  
			while (y_int != 0) {
				// always: gcd (modulus,residue) = gcd (x_int,y_int)
				//         sx*modulus + tx*residue = x_int
				//         sy*modulus + ty*residue = y_int
				q = x_int / y_int; // unsigned long quotient
				temp = y_int; y_int = x_int - q * y_int;
				x_int = temp;
				temp = ty; ty = tx - q * ty;
				tx = temp;
			}
		  
			if (tx < 0) tx += (int)modulus;
		  
			// now x_int = gcd (modulus,residue)
			return x = (double)tx;
		  
		  
		}

		inline Element &axpy (Element &r, 
				      const Element &a, 
				      const Element &x, 
				      const Element &y) const {
			double tmp = a * x + y;
			return r= fmod(tmp, modulus); 
			//return r= tmp- floor(tmp*inv_modulus)*modulus; 

		}

		inline Element &addin (Element &x, const Element &y) const {
			x += y;
			if (  x >= modulus ) x -= modulus;
			return x;
		}
 
		inline Element &subin (Element &x, const Element &y) const {
			x -= y;
			if (x < 0.) x += modulus;
			return x;
		}
 
		inline Element &mulin (Element &x, const Element &y) const {
			return mul(x,x,y);
		}
 
		inline Element &divin (Element &x, const Element &y) const {
			return div(x,x,y);
		}
 
		inline Element &negin (Element &x) const {
			if (x == 0.) return x; 
			else return x = modulus - x; 
		}
		
		inline Element &invin (Element &x) const {
			return inv (x, x);
		}
		
		inline Element &axpyin (Element &r, const Element &a, const Element &x) const {
			double tmp = r + a * x;
			return r = fmod(tmp, modulus); 

			//return r= tmp- floor(tmp*inv_modulus)*modulus; 
		}

		static inline double getMaxModulus()
			{ return 94906265.0; } // floor( 2^26.5 )
		
	};

#endif
