/* -*- mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
// vim:sts=8:sw=8:ts=8:noet:sr:cino=>s,f0,{0,g0,(0,\:0,t0,+0,=s

/*
 * Copyright (C) FFLAS-FFPACK
 * Written by Clément Pernet
 * This file is Free Software and part of FFLAS-FFPACK.
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


//--------------------------------------------------------------------------
//          Test for the echelon factorisation
//--------------------------------------------------------------------------
// usage: test-echelon p A n, for n lsp factorization
// of A over Z/pZ
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//#define DEBUG 1
// Debug option  0: no debug
//               1: check A = LQUP
//-------------------------------------------------------------------------
using namespace std;



//#define __LUDIVINE_CUTOFF 1
#include <iostream>
#include <iomanip>
#include "Matio.h"
#include "fflas-ffpack/utils/timer.h"
#include "fflas-ffpack/field/modular-balanced.h"
#include "fflas-ffpack/ffpack/ffpack.h"

using namespace FFPACK;
typedef Givaro::Modular<double> Field;

int main(int argc, char** argv){
	cerr<<setprecision(20);
	int i,j,nbf,m,n;
	int R=0;

	if (argc!=4){
		cerr<<"usage : test-lqup <p> <A> <i>"<<endl
		    <<"        to do i Echelon factorisation of A with "
		    <<endl;
		exit(-1);
	}
	Field F((uint64_t)atoi(argv[1]));
	Field::Element * A;

	A = read_field(F,argv[2],&m,&n);

	size_t *P = FFLAS::fflas_new<size_t>(n);
	size_t *Q = FFLAS::fflas_new<size_t>(m);

	//	size_t cutoff = atoi(argv[3]);
	nbf = atoi(argv[3]);

 FFLAS::Timer tim,timc;
	timc.clear();


	for ( i=0;i<nbf;i++){
		if (i) {
			FFLAS::fflas_delete( A);
			A = read_field(F,argv[2],&m,&n);
		}
		for (j=0;j<n;j++)
			P[j]=0;
		for (j=0;j<m;j++)
			Q[j]=0;
		tim.clear();
		tim.start();
		R = (int)FFPACK::ColumnEchelonForm (F, m, n, A, n, P, Q);
		tim.stop();
		timc+=tim;
	}
	//write_field (F,cerr<<"Result = "<<endl, A, m,n,n);

// 	cerr<<"P = [";
// 	for (size_t i=0; i<n; ++i)
// 		cerr<<P[i]<<" ";
// 	cerr<<"]"<<endl;
// 	cerr<<"Q = [";
// 	for (size_t i=0; i<m; ++i)
// 		cerr<<Q[i]<<" ";
// 	cerr<<"]"<<endl;
#if DEBUG
	Field::Element * L = FFLAS::fflas_new<Field::Element>(m*n);
	Field::Element * U = FFLAS::fflas_new<Field::Element>(n*n);
	Field::Element * X = FFLAS::fflas_new<Field::Element>(m*n);

	Field::Element zero,one;
	F.init(zero,0.0);
	F.init(one,1.0);
	for (int i=0; i<R; ++i){
		for (int j=0; j<=i; ++j)
			F.assign ( *(U + i*n + j), zero);
		F.init (*(U+i*(n+1)),one);
		for (int j=i+1; j<n; ++j)
			F.assign (*(U + i*n + j), *(A+ i*n+j));
	}
	for (int i=R;i<n; ++i){
		for (int j=0; j<n; ++j)
			F.assign(*(U+i*n+j), zero);
		F.init(*(U+i*(n+1)),one);
	}
	FFPACK::applyP( F, FFLAS::FflasLeft, FFLAS::FflasTrans, n, 0, R, U, n, P);

	for ( int i=0; i<m; ++i ){
		int j=0;
		for (; j <= ((i<R)?i:R) ; ++j )
			F.assign( *(L + i*m+j), *(A+i*n+j));
		for (; j<m; ++j )
			F.assign( *(L+i*m+j), zero);
	}
// 	cerr<<"P = ";
// 	for (size_t i=0; i<n;++i)
// 		cerr<<" "<<P[i];
// 	cerr<<endl;
// 	cerr<<"Q = ";
// 	for (size_t i=0; i<m;++i)
// 		cerr<<" "<<Q[i];
// 	cerr<<endl;

// 	write_field(F,cerr<<"A = "<<endl,A,m,n,n);
//  	write_field(F,cerr<<"L = "<<endl,L,m,n,n);
//  	write_field(F,cerr<<"U = "<<endl,U,m,n,n);

	Field::Element * B =  read_field(F,argv[2],&m,&n);

	FFLAS::fgemm (F, FFLAS::FflasNoTrans, FFLAS::FflasNoTrans, m,n,n, 1.0,
		      B, n, U, n, 0.0, X,n);
	//FFLAS::fflas_delete( A);

	bool fail = false;
	for (int i=0; i<m; ++i)
		for (int j=0; j<n; ++j)
			if (!F.areEqual (*(L+i*n+j), *(X+i*n+j)))
				fail=true;

 	// write_field(F,cerr<<"X = "<<endl,X,m,n,n);
//  	write_field(F,cerr<<"L = "<<endl,L,m,n,n);

	FFLAS::fflas_delete( B);
	if (fail)
		cerr<<"FAIL"<<endl;


	else
		cerr<<"PASS"<<endl;

// 	cout<<m<<" "<<n<<" M"<<endl;
// 	for (size_t i=0; i<m; ++i)
// 		for (size_t j=0; j<n; ++j)
// 			if (!F.isZero(*(A+i*n+j)))
// 				cout<<i+1<<" "<<j+1<<" "<<(*(A+i*n+j))<<endl;
// 	cout<<"0 0 0"<<endl;

	FFLAS::fflas_delete( U);
	FFLAS::fflas_delete( L);
	FFLAS::fflas_delete( X);
#endif
	FFLAS::fflas_delete( A);
	FFLAS::fflas_delete( P);
	FFLAS::fflas_delete( Q);

	double t = timc.usertime();
	double numops = m*m/1000.0*(n-m/3.0);

	cerr<<m<<"x"<< n
	    << " : rank = " << R << "  ["
	    << ((double)nbf/1000.0*(double)numops / t)
	    << " MFops "
	    << " in "
	    << t/nbf<<"s"
	    <<"]"<< endl;
// 	cout<<m
// 	    <<" "<<((double)nbf/1000.0*(double)numops / t)
// 	    <<" "<<t/nbf
// 	    <<endl;

	return 0;
}
