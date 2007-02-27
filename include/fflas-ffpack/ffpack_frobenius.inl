/* -*- mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */

/* linbox/ffpack/ffpack_charpoly_kgfast.inl
 * Copyright (C) 2006 Clement Pernet
 *
 * Written by Clement Pernet <cpernet@uwaterloo.ca>
 *
 * See COPYING for license information.
 */

#ifndef MIN
#define MIN(a,b) (a<b)?a:b
#endif

//---------------------------------------------------------------------
// Frobenius: Las Vegas algorithm to compute the Frobenius normal form over a large field
//---------------------------------------------------------------------


// 
template <class Field>
void FFPACK::CompressRowsQK (Field& F, const size_t M,
			   typename Field::Element * A, const size_t lda,
			   typename Field::Element * tmp, const size_t ldtmp,
			   const size_t * d, const size_t deg,const size_t nb_blocs){

	size_t currtmp = 0;
	size_t currw = d[0]-1;
	size_t currr = d[0]-1;
	for (int i = 0; i< int(nb_blocs)-1; ++i){
		for (int j = d[i]-1; j<deg-1; ++j, currr++, currtmp++)
			fcopy(F, M, tmp + currtmp*ldtmp, 1,  A + currr*lda, 1);
		//cerr<<"d["<<i<<"] = "<<d[i]<<endl;
		for (int j=0; j < d[i+1] -1; ++j, currr++, currw++){
			fcopy(F, M, A + (currw)*lda, 1, A+(currr)*lda, 1);
		}
		//cerr<<"Ok"<<endl;
	}

	//write_field(F,cerr<<"avant tmp"<<endl,A, currtmp, M, lda);
	for (int i=0; i < currtmp; ++i, currw++){
		fcopy (F, M, A + (currw)*lda, 1, tmp + i*ldtmp, 1);
	}
}

template <class Field>
void FFPACK::CompressRows (Field& F, const size_t M,
			     typename Field::Element * A, const size_t lda,
			     typename Field::Element * tmp, const size_t ldtmp,
			     const size_t * d, const size_t nb_blocs){

	size_t currd = d[0]-1;
	size_t curri = d[0]-1;
	for (int i = 0; i< int(nb_blocs)-1; ++i){
		fcopy(F, M, tmp + i*ldtmp, 1,  A + currd*lda, 1);
		for (int j=0; j < d[i+1] -1; ++j){
			fcopy(F, M, A + (curri++)*lda, 1, A+(currd+j+1)*lda, 1);
		}
		currd += d[i+1];
	}
	for (int i=0; i < int(nb_blocs)-1; ++i){
		fcopy (F, M, A + (curri++)*lda, 1, tmp + i*ldtmp, 1);
	}
}
template <class Field>
void FFPACK::DeCompressRowsQK (Field& F, const size_t M, const size_t N,
			       typename Field::Element * A, const size_t lda,
			       typename Field::Element * tmp, const size_t ldtmp,
			       const size_t * d, const size_t deg,const size_t nb_blocs){
	
	size_t zeroblockdim = 1; // the last block contributes with 1
	size_t currtmp = 0;
	for (int i=0; i<int(nb_blocs)-1; ++i)
		zeroblockdim += deg - d[i];
	//cerr<<"zeroblockdim = "<<zeroblockdim<<endl;
	for (int i=0; i < zeroblockdim - 1; ++i, ++currtmp)
		fcopy(F, M, tmp + currtmp*ldtmp, 1,  A + (N - zeroblockdim +i)*lda, 1);
	//write_field(F,cerr<<"tmp= "<<endl,tmp,zeroblockdim - 1, M,ldtmp);
	currtmp--;
	size_t w_idx = N - 2;
	size_t r_idx = N - zeroblockdim - 1;

	for (int i = int(nb_blocs)-2; i>=0; --i){
		for (size_t j = 0; j < d [i+1] - 1; ++j)
			fcopy (F, M, A + (w_idx--)*lda, 1, A + (r_idx--)*lda, 1);
		for (size_t j = 0; j < deg - d[i]; ++j)
			fcopy (F, M, A + (w_idx--)*lda, 1, tmp + (currtmp--)*ldtmp, 1);
	}
}

template <class Field>
void FFPACK::DeCompressRows (Field& F, const size_t M, const size_t N,
			     typename Field::Element * A, const size_t lda,
			     typename Field::Element * tmp, const size_t ldtmp,
			     const size_t * d, const size_t nb_blocs){
	
	for (int i=0; i<int(nb_blocs)-1; ++i)
		fcopy(F, M, tmp + i*ldtmp, 1, A + (N-nb_blocs+i)*lda, 1);
	
	size_t w_idx = N - 2;
	size_t r_idx = N - nb_blocs - 1;
	for (int i = int(nb_blocs)-2; i>=0; --i){
		for (size_t j = 0; j<d[i+1]-1; ++j)
			fcopy (F, M, A + (w_idx--)*lda, 1, A + (r_idx--)*lda, 1);
		fcopy (F, M, A + (w_idx--)*lda, 1, tmp + i*ldtmp, 1);
	}
}

template <class Field>
void FFPACK::CompressRowsQA (Field& F, const size_t M,
			     typename Field::Element * A, const size_t lda,
			     typename Field::Element * tmp, const size_t ldtmp,
			     const size_t * d, const size_t nb_blocs){

	size_t currd = 0;
	size_t curri = 0;
	for (size_t i = 0; i< nb_blocs; ++i){
		fcopy(F, M, tmp + i*ldtmp, 1,  A + currd*lda, 1);
		for (size_t j=0; j < d[i] -1; ++j)
			fcopy(F, M, A + (curri++)*lda, 1, A+(currd+j+1)*lda, 1);
		currd += d[i];
	}
	for (size_t i=0; i < nb_blocs; ++i)
		fcopy (F, M, A + (curri++)*lda, 1, tmp + i*ldtmp, 1);
}

template <class Field>
void FFPACK::DeCompressRowsQA (Field& F, const size_t M, const size_t N,
			       typename Field::Element * A, const size_t lda,
			       typename Field::Element * tmp, const size_t ldtmp,
			       const size_t * d, const size_t nb_blocs){
	
	for (size_t i=0; i<nb_blocs; ++i)
		fcopy(F, M, tmp + i*ldtmp, 1, A + (N-nb_blocs+i)*lda, 1);

	size_t w_idx = N - 1;
	size_t r_idx = N - nb_blocs - 1;
	for (int i = int(nb_blocs)-1; i>=0; --i){
		for (size_t j = 0; j<d[i]-1; ++j)
			fcopy (F, M, A + (w_idx--)*lda, 1, A + (r_idx--)*lda, 1);
		fcopy (F, M, A + (w_idx--)*lda, 1, tmp + i*ldtmp, 1);
	}
}


template <class Field, class Polynomial>
std::list<Polynomial>&
FFPACK::Frobenius (const Field& F, std::list<Polynomial>& frobeniusForm, 
		   const size_t N, typename Field::Element * A, const size_t lda, const size_t c){
	
	static typename Field::Element one, zero, mone;
	F.init(one, 1UL);
	F.neg(mone, one);
	F.init(zero, 0UL);

	size_t * rp = new size_t[2*N];

	size_t noc = static_cast<size_t>(ceil(N/double(c)));

	// Building the workplace matrix Aw
	typename Field::Element *K = new typename Field::Element[N*(noc*c)];
	typename Field::Element *K2 = new typename Field::Element[N*(noc*c)];
	size_t ldk = N;
	// Permutations such that
	// A = QA [ I Ac ] PA
	//        [ 0 Ac ]
	//
	// K = [ I Kc ] PK
	//     [ 0 Kc ]
	
	size_t *dA = new size_t[N]; //PA
	size_t *dK = new size_t[N];


	size_t Nrest = N-(c-1)*noc;
	typename Field::RandIter g (F);
	for (size_t i = 0; i < noc; ++i)
 		for (size_t j = 0; j < N; ++j)
 			g.random( *(K + i*ldk +j) );

	for (size_t i = 1; i<c; ++i)
		fgemm( F, FflasNoTrans, FflasTrans,  noc, N, N, one,
		       K+(i-1)*noc*ldk, ldk, A, lda, zero, K+i*noc*ldk, ldk);
//  	fgemm( F, FflasNoTrans, FflasTrans, Nrest, N, N, one,
//  	       K+(c-2)*noc*ldk, ldk, A, lda, zero, K+(c-1)*noc*ldk, ldk);

// 	write_field(F,cerr<<"K = "<<endl,K, c*noc,N,ldk);
// 	write_field(F,cerr<<"A = "<<endl,A, N,N,ldk);
	size_t * Pk = new size_t[N];
	size_t * Qk = new size_t[c*noc];

	// K2 <- K (re-ordering)
	size_t w_idx = 0;
	for (size_t i=0; i<noc; ++i)
		for (size_t j=0; j<c; ++j)
			fcopy(F, N, (K2+(w_idx++)*ldk), 1, (K+(i+j*noc)*ldk), 1);

	for (size_t i=0; i<noc*c; ++i)
		fcopy (F, N, (K+i*ldk), 1, K2+i*ldk, 1);
	
	size_t R = LUdivine(F, FflasNonUnit, noc*c, N, K, ldk, Pk, Qk, FfpackLQUP);
	if (R<N){
		std::cerr<<"Preconditionning failed; not yet implemented"<<std::endl;
		exit(-1);
	}
	
	size_t row_idx = 0;
	size_t * degini = new size_t[noc];
	for (size_t i=0; i<noc; ++i)
		degini[i]=0;
	// size_t * degK = new size_t[c];
// 	for (size_t i=0; i<c; ++i)
// 		degK[i]=0;
	
	size_t i=0;
	row_idx=0;
	for (size_t k = 0; k<noc; ++k){
		size_t d = 0;
		while ( (d<c) && (row_idx<R) && (Qk[row_idx] == i)) {i++; row_idx++; d++;}
		degini[k] = d;
		i = Qk[row_idx];
	}
	
// 	for (size_t i = 0; i<N; ++i)
// 		if (Qk[row_idx] == i){
// 			degini[i % noc] ++;
// 	//		degK[i/noc] ++;
// 			row_idx ++;
// 		}

	// Selection of the last iterate of each of the block
	size_t bk_idx = 0;
	for (size_t i = 0; i < noc; ++i){
#if DEBUG
		std::cerr<<"degini ["<<i<<"] = "<<degini[i]<<std::endl;
#endif
		fcopy (F, N, (K2+i*ldk), 1, (K2 + (bk_idx + degini[i]-1)*ldk), 1);
		bk_idx+= degini[i];
	}
// 	for (size_t i = 0; i < c; ++i)
// 		cerr<<"degK["<<i<<"] = "<<degK[i]<<endl;

	// K <- K A^T 
	fgemm( F, FflasNoTrans, FflasTrans, noc, N, N, one,  K2, ldk, A, lda, zero, K2+noc*ldk, ldk);

	// K <- K P^T
	applyP (F, FflasRight, FflasTrans, noc, 0, R, K2+noc*ldk, ldk, Pk);

	// K <- K U^-1
	ftrsm (F, FflasRight, FflasUpper, FflasNoTrans, FflasNonUnit, noc, R, one, K, ldk, K2+noc*ldk, ldk);
	
	// K <- K Q^T
	applyP (F, FflasRight, FflasTrans, noc, 0, R, K2+noc*ldk, ldk, Qk);

	// K <- K L^-1
	//ftrsm(F, FflasRight, FflasLower, FflasNoTrans, FflasUnit, N, R
	solveLB (F, FflasRight, noc, N, R, K, ldk, Qk, K2+noc*ldk, ldk);
	delete[] Pk;
	delete[] Qk;
	size_t deg = c+1;
	size_t Ma = noc;
	size_t Mk;
	size_t Ncurr = N;
	for (size_t i=0; i<noc; ++i)
 		dA[i] = degini[i];
	
// 	size_t deg = 2;
// 	size_t Ma = N;
// 	size_t Mk;
// 	size_t Ncurr = N;
 	size_t block_idx, it_idx, rp_val, nb_full_blocks;
	bk_idx = 0;
	
	//	write_field(F,cerr<<"Apres preconditionnement, K = "<<endl, K2+noc*ldk, noc,N,ldk);
	typename Field::Element *Arp = new typename Field::Element[N*Ma];
	typename Field::Element *Ac = new typename Field::Element[N*Ma];
	size_t ldac = Ma;
	size_t ldarp = N;
	
	for (size_t i=0; i < N; ++i)
 		for (size_t j=0; j<Ma; ++j)
			*(Ac + i*Ma +j) = *(K2 + i + (j+noc)*ldk);
	//  	for (size_t i=0; i < noc; ++i){
// 		for (size_t k=0; k < degini[i]; ++k){
// 			for (size_t j=0; j < noc; ++j)
// 				*(Ac + (w_idx)*N + j) = *(K2 + (noc+j)*ldk + i + bk_idx);
// 			bk_idx += degK[k];
// 			w_idx++;
// 			}
// 		bk_idx = 0;
// 	}

	//delete[] degK;
	delete[] degini;
	//	write_field(F,cerr<<"Apres preconditionnement, Ac = "<<endl, Ac, N, noc, N);
	
	do{
		delete[] K;
		delete[] K2;
		K = new typename Field::Element[Ncurr*Ma];
		K2 = new typename Field::Element[Ncurr*Ma];
		ldk = Ma;
 
		//cerr<<deg<<endl;
		//write_field(F, cerr<<"Ac = "<<endl, Ac, Ncurr, Ma, N);
		// Computation of the rank profile
		for (size_t i=0; i < Ncurr; ++i)
			for (size_t j=0; j < Ma; ++j)
				*(Arp + j*ldarp + Ncurr-i-1) = *(Ac + i*ldac + j);
		for (size_t i=0; i<2*Ncurr; ++i)
			rp[i] = 0;
		size_t R = SpecRankProfile (F, Ma, Ncurr, Arp, ldarp, deg-1, rp);
		if (R < Ncurr){
			std::cerr<<"FAIL R<Ncurr"<<std::endl;
			exit(-1);
		}
		// Computation of the degree vector dK
		//cerr<<"// Computation of the degree vector dK"<<endl;
		it_idx = 0;
		rp_val = 0;
		size_t g = 0;
		size_t dtot=0;
		block_idx = 0;
		nb_full_blocks = 0;
#if DEBUG
		std::cerr<<"dK = ";
#endif
		while (dtot<Ncurr){
			do {g++; rp_val++; it_idx++;}
			while ( /*(g<Ncurr ) &&*/ (rp[g] == rp_val) && (it_idx < deg ));
			if ((block_idx)&&(it_idx > dK[block_idx-1])){
				std::cerr<<"FAIL d non decroissant"<<std::endl;
				exit(-1);
			}
			dK[block_idx++] = it_idx;
			dtot += it_idx;
#if DEBUG
			std::cerr<<dK[block_idx-1]<<" ";
#endif
			if (it_idx == deg)
				nb_full_blocks ++;
			it_idx=0;
			rp_val = rp[g];
		}

#if DEBUG
		std::cerr<<std::endl;
#endif
		Mk = block_idx;
		//cerr<<"Mk, nb_full_blocks = "<<Mk<<" "<<nb_full_blocks<<endl;


				
		// Selection of dense colums of K 
		//cerr<<"// Selection of dense colums of K "<<endl;
		for (size_t i=0; i < nb_full_blocks; ++i){
			fcopy (F, Ncurr, K+i, ldk, Ac+i, ldac);
		}
		//write_field(F, cerr<<"K = "<<endl, K, Ncurr, Mk, ldk);
		
		// K <- QK K
		// cerr<<"// K <- QK K "<<endl;
// 		DeCompressRows (F, Mk - nb_full_blocks, Ncurr-nb_full_blocks*deg, K + nb_full_blocks*(deg-1)*ldk, ldk, Arp, N, dK+nb_full_blocks, Mk-nb_full_blocks);
// 		write_field(F, cerr<<"K = "<<endl, K, Ncurr, Mk, ldk);

		size_t pos = nb_full_blocks*(deg-1);
		for (size_t i = nb_full_blocks; i < Mk; ++i){
			//	cerr<<"*(K + "<<i<<") = "<<(*(K+i))<<"ldk = "<<ldk<<endl;
			for (size_t j=0; j<Ncurr; ++j)
				F.assign (*(K + i + j*ldk), zero);
			F.assign (*(K + i + (pos + dK[i]-1)*ldk), one);
			//cerr<<"dA["<<i<<"] = "<<dA[i]<<endl;
			pos += dA[i];
		}
		//write_field(F, cerr<<"K = "<<endl, K, Ncurr, Mk, ldk);

		// Copying K2 <- K
		//cerr<<"// Copying K2 <- K"<<endl;
		for (size_t i=0; i<Mk; ++i)
			fcopy (F, Ncurr, K2+i, ldk, K+i, ldk);
		CompressRowsQK (F, Mk, K2 + nb_full_blocks*(deg-1)*ldk, ldk, Arp, ldarp, dK+nb_full_blocks, deg, Mk-nb_full_blocks);

		
		// K <- PA K
		//cerr<<"// K <- PA K"<<endl;
		CompressRows (F, nb_full_blocks, K, ldk, Arp, ldarp, dA, Ma);
		//write_field(F, cerr<<"K = "<<endl, K, Ncurr, Mk, ldk);
		
		// A <- newQA^T K (compress)
		//cerr<<"// K <- newQA^T K (compress)"<<endl;
		CompressRowsQA (F, Ma, Ac, ldac, Arp, ldarp, dA, Ma);
		//write_field(F, cerr<<"K = "<<endl, K, Ncurr, Mk, ldk);

		// K <- A K
		// cerr<<"// K <- A K"<<endl;
// 		cerr<<"C <- AB + C"<<endl;
		// write_field(F,cerr<<"A = "<<endl, Ac, Ncurr-Ma, Ma, N);
// 		write_field(F,cerr<<"B = "<<endl, K+(Ncurr-Ma)*ldk, Ma,nb_full_blocks, ldk);
// 		write_field(F,cerr<<"C = "<<endl, K, Ncurr-Ma, nb_full_blocks, ldk);
		
		fgemm (F, FflasNoTrans, FflasNoTrans, Ncurr-Ma, nb_full_blocks, Ma, one, Ac, ldac, K+(Ncurr-Ma)*ldk, ldk, one, K, ldk);
		fgemm (F, FflasNoTrans, FflasNoTrans, Ma, nb_full_blocks, Ma, one, Ac+(Ncurr-Ma)*ldac, ldac, K+(Ncurr-Ma)*ldk, ldk, zero, Arp, ldarp);
		for (size_t i=0; i< Ma; ++i)
			fcopy(F, nb_full_blocks, K+(Ncurr-Ma+i)*ldk, 1, Arp+i*ldarp, 1);
		//write_field(F, cerr<<"K = "<<endl, K, Ncurr, Mk, ldk);
		
		// Copying the last rows of A times K
		//cerr<<"// Copying the last rows of k "<<nb_full_blocks<<" "<<Mk<<endl;
		size_t offset = (deg-2)*nb_full_blocks;
		for (size_t i = nb_full_blocks; i < Mk; ++i) {
			//cerr<<"dK["<<i<<"] = "<<dK[i]<<" deg = "<<deg<<endl;
			for (size_t j=0; j<Ncurr; ++j)
				F.assign(*(K+i+j*ldk), zero);
			if (dK[i] == dA[i]) // copy the column of A
				fcopy (F, Ncurr, K+i, ldk, Ac+i, ldac);
			else{
				F.assign (*(K + i + (offset+dK[i]-1)*ldk), one);
				//cerr<<"*(K + "<<i<<" + ("<<offset <<"+dK["<<i<<"]-1)*ldk) := 1 "<<endl;
			}
			offset += dA[i]-1;
		}
		//write_field(F, cerr<<"K = "<<endl, K, Ncurr, Mk, ldk);
				
		// K <- QA K
		//cerr<<"// K <- QA K"<<endl;
		DeCompressRowsQA (F, Mk, Ncurr, K, ldk, Arp, ldarp, dA, Ma);
		//write_field(F, cerr<<"K = "<<endl, K, Ncurr, Mk, ldk);

		// K <- QK^T K
		//cerr<<"// K <- QK^T K"<<endl;
		CompressRowsQK (F, Mk, K + nb_full_blocks*(deg-1)*ldk, ldk, Arp, ldarp, dK+nb_full_blocks, deg, Mk-nb_full_blocks);
		//write_field(F, cerr<<"K = "<<endl, K, Ncurr, Mk, ldk);
		
		// K <- K^-1 K
		//cerr<<"// K <- K^-1 K"<<endl;
		size_t *P=new size_t[Mk];
		size_t *Q=new size_t[Mk];
		if (LUdivine (F, FflasNonUnit, Mk, Mk , K2 + (Ncurr-Mk)*ldk, ldk, P, Q, FfpackLQUP) < Mk){
			std::cerr<<"FAIL R2 < MK"<<std::endl;
			//			exit(-1);
		}
			
		//cerr<<"Mk, ldk ="<<Mk<<" "<<ldk<<endl;
		ftrsm (F, FflasLeft, FflasLower, FflasNoTrans, FflasUnit, Mk, Mk, one, K2 + (Ncurr-Mk)*ldk, ldk, K+(Ncurr-Mk)*ldk, ldk);
		//cerr<<"2"<<endl;
		ftrsm (F, FflasLeft, FflasUpper, FflasNoTrans, FflasNonUnit, Mk, Mk, one, K2+(Ncurr-Mk)*ldk, ldk, K+(Ncurr-Mk)*ldk, ldk);
		//cerr<<"3"<<endl;
		applyP (F, FflasLeft, FflasTrans, Mk, 0, Mk, K+(Ncurr-Mk)*ldk,ldk, P);
		//cerr<<"4"<<endl;
		fgemm (F, FflasNoTrans, FflasNoTrans, Ncurr-Mk, Mk, Mk, mone, K2, ldk, K+(Ncurr-Mk)*ldk,ldk, one, K, ldk);
		delete[] P;
		delete[] Q;
		//write_field(F, cerr<<"K = "<<endl, K, Ncurr, Mk, ldk);
		
		// K <- PK^T K
		//cerr<<"// K <- PK^T K"<<endl;
		DeCompressRows (F, Mk, Ncurr, K, ldk, Arp, ldarp, dK, Mk);
		//write_field(F, cerr<<"K = "<<endl, K, Ncurr, Mk, ldk);
		
		// K <- K PK (dA <- dK)
		//cerr<<"// K <- K PK (dA <- dK)"<<endl;
		if (nb_full_blocks*deg < Ncurr)
			Ma = nb_full_blocks+1;
		else
			Ma = nb_full_blocks;
		
		for (size_t i=0; i< Ma; ++i)
			dA[i] = dK[i];
		//write_field(F, cerr<<"K = "<<endl, K, Ncurr, Mk, ldk);
		
		// Recovery of the completed invariant factors
		//cerr<<"// Recovery of the completed invariant factors"<<endl;

		offset = Ncurr-1;
		size_t oldNcurr = Ncurr;
		for (size_t i=Mk-1; i>=nb_full_blocks+1;  --i)
			if (dK[i] >= 1){ 
				Polynomial * P = new Polynomial (dK [i]);
				for (size_t j=0; j < dK[i]; ++j)
					F.neg( P->operator[](dK[i]-j-1), *(K + i + (offset-j)*ldk));
				frobeniusForm.push_front(*P);
				offset -= dK[i];
				Ncurr -= dK[i];
				//block_idx--;
				//Ma--;
			}
		//write_field(F,cerr,K,oldNcurr, Mk, ldk);
		// cerr<<"nb_full_blocks, Mk, Ncurr, oldNcurr, offset  = "
// 		    <<nb_full_blocks<<" "<<Mk<<" "<<Ncurr<<" "<<oldNcurr<<" "<<offset<<endl;
		for (size_t i= offset+1; i<oldNcurr; ++i)
			for (size_t j=0; j<nb_full_blocks+1; ++j){
				//		cerr<<"K + "<<i<<"*ldk + "<<j<<" = "<<(*(K+i*ldk+j))<<endl;
				if (!F.isZero( *(K+i*ldk+j) )){
					std::cerr<<"FAIL C != 0"<<std::endl;
					exit(-1);
				}
			}
		// offset = deg * nb_full_blocks + dK[nb_full_blocks];
// 		for (size_t i=nb_full_blocks+1; i<Mk; ++i)
// 			if (dK[i] >= 1){ 
// 				Polynomial * P = new Polynomial (dK [i]);
// 				for (size_t j=0; j < dK[i]; ++j)
// 					F.neg( P->operator[](j), *(K + i + (offset+j)*ldk));
// 				frobeniusForm.push_front(*P);
// 				offset += dK[i];
// 				Ncurr -= dK[i];
// 				//block_idx--;
// 				//Ma--;
// 			}

		
		// A <- K
		//cerr<<"// A <- K"<<endl;
		delete[] Ac;
		delete[] Arp;
		Ac = new typename Field::Element[Ncurr*Mk];
		ldac = Mk;
		Arp = new typename Field::Element[Ncurr*Mk];
		ldarp=Ncurr;
		for (size_t i=0; i < Ncurr; ++i )
			fcopy (F, Mk, Ac + i*ldac, 1, K + i*ldk, 1);

		deg++;
			
	} while ((nb_full_blocks >= 1) && (Mk > 1));

	// Recovery of the first invariant factor
	//cerr<<"// Recovery of the first invariant factor"<<endl;
	Polynomial * P = new Polynomial (dK [0]);
	for (size_t j=0; j < dK[0]; ++j)
		F.neg( P->operator[](j), *(K  + j*ldk));
	frobeniusForm.push_front(*P);
	delete[] rp;
	delete[] Arp;
	delete[] Ac;
	delete[] K;
	delete[] K2;
	delete[] dA;
	delete[] dK;
	return frobeniusForm;
}

