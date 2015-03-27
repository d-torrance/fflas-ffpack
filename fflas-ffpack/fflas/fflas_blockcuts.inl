/* -*- mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
// vim:sts=8:sw=8:ts=8:noet:sr:cino=>s,f0,{0,g0,(0,\:0,t0,+0,=s
/* fflas/fflas_bounds.inl
 * Copyright (C) 2013 Jean-Guillaume Dumas
 *
 * Written by Jean-Guillaume Dumas <Jean-Guillaume.Dumas@imag.fr>
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

#ifndef __FFLASFFPACK_fflas_blockcuts_INL
#define __FFLASFFPACK_fflas_blockcuts_INL


#define __FFLASFFPACK_MINBLOCKCUTS ((size_t)256)

namespace FFLAS {

    template<CuttingStrategy Method>
    void BlockCuts(size_t& RBLOCKSIZE, size_t& CBLOCKSIZE,
                   const size_t m, const size_t n,
                   const size_t numthreads);

    template<>
    void BlockCuts<SINGLE>(size_t& RBLOCKSIZE,
                              size_t& CBLOCKSIZE,
                              const size_t m, const size_t n,
                              const size_t numthreads) {
        RBLOCKSIZE = m;
        CBLOCKSIZE = n;
    }


    template<>
    void BlockCuts<ROW_FIXED>(size_t& RBLOCKSIZE,
                              size_t& CBLOCKSIZE,
                              const size_t m, const size_t n,
                              const size_t numthreads) {
        RBLOCKSIZE = std::min(m,__FFLASFFPACK_MINBLOCKCUTS);
        CBLOCKSIZE = n;
    }


    template<>
    void BlockCuts<GRAIN_SIZE>(size_t& RBLOCKSIZE,
                              size_t& CBLOCKSIZE,
                              const size_t m, const size_t n,
                              const size_t grainsize) {
        RBLOCKSIZE = std::min(m,grainsize);
        CBLOCKSIZE = std::min(n,grainsize);
    }


    template<>
    void BlockCuts<COLUMN_FIXED>(size_t& RBLOCKSIZE,
                                 size_t& CBLOCKSIZE,
                                 const size_t m, const size_t n,
                                 const size_t numthreads) {
        RBLOCKSIZE = m;
        CBLOCKSIZE = std::min(n,__FFLASFFPACK_MINBLOCKCUTS);
    }


    template<>
    void BlockCuts<BLOCK_FIXED>(size_t& RBLOCKSIZE,
                                size_t& CBLOCKSIZE,
                                const size_t m, const size_t n,
                                const size_t numthreads) {
        RBLOCKSIZE = std::min(m,__FFLASFFPACK_MINBLOCKCUTS);
        CBLOCKSIZE = std::min(n,__FFLASFFPACK_MINBLOCKCUTS);
    }

    template<>
    void BlockCuts<ROW_THREADS>(size_t& RBLOCKSIZE,
                                size_t& CBLOCKSIZE,
                                const size_t m, const size_t n,
                                const size_t numthreads) {
        RBLOCKSIZE = std::max(m/numthreads,(size_t)1);
        CBLOCKSIZE = n;
    }


    template<>
    void BlockCuts<COLUMN_THREADS>(size_t& RBLOCKSIZE,
                                   size_t& CBLOCKSIZE,
                                   const size_t m, const size_t n,
                                   const size_t numthreads) {
        RBLOCKSIZE = m;
        CBLOCKSIZE = std::max(n/numthreads,(size_t)1);
    }

    template<>
    void BlockCuts<BLOCK_THREADS>(size_t& RBLOCKSIZE,
                                  size_t& CBLOCKSIZE,
                                  const size_t m, const size_t n,
                                  const size_t numthreads) {
        if (numthreads<65) {
                //CP: Let's not compute these values all the time
            const short maxtc[64] = {1,2,3,2,5,3,7,4,3,5,11,4,13,7,5,4,17,6,19,5,7,11,23,6,5,13,9,7,29,6,31,8,11,17,7,6,37,19,13,8,41,7,43,11,9,23,47,8,7,10,17,13,53,9,11,8,19,29,59,10,61,31,9,8};
            const short maxtr[64] = {1,1,1,2,1,2,1,2,3,2,1,3,1,2,3,4,1,3,1,4,3,2,1,4,5,2,3,4,1,5,1,4,3,2,5,6,1,2,3,5,1,6,1,4,5,2,1,6,7,5,3,4,1,6,5,7,3,2,1,6,1,2,7,8};

            RBLOCKSIZE=std::max(m/(size_t)maxtr[numthreads-1],(size_t)1);
            CBLOCKSIZE=std::max(n/(size_t)maxtc[numthreads-1],(size_t)1);
        } else {
            const size_t maxt = (size_t)sqrt((double)numthreads);
            size_t maxtr=maxt,maxtc=maxt;
            for(size_t i=maxt; i>=1; --i) {
                size_t j=maxt;
                size_t newpr = i*j;
                for( ; newpr < numthreads; ++j, newpr+=i ) {}
                if (newpr == numthreads) {
                    maxtc = j;
                    maxtr = i;
                    break;
                }
            }
            RBLOCKSIZE=std::max(m/maxtr,(size_t)1);
            CBLOCKSIZE=std::max(n/maxtc,(size_t)1);
        }
    }

    void BlockCuts(size_t& r, size_t& c,
                   size_t m, size_t n,
                   const CuttingStrategy method,
                   const size_t t) {
        switch(method) {
            case BLOCK_THREADS: BlockCuts<BLOCK_THREADS>(r,c,m,n,t); break;
            case ROW_THREADS: BlockCuts<ROW_THREADS>(r,c,m,n,t); break;
            case ROW_FIXED: BlockCuts<ROW_FIXED>(r,c,m,n,t); break;
            case BLOCK_FIXED: BlockCuts<BLOCK_FIXED>(r,c,m,n,t); break;
            case COLUMN_THREADS: BlockCuts<COLUMN_THREADS>(r,c,m,n,t); break;
            case COLUMN_FIXED: BlockCuts<COLUMN_FIXED>(r,c,m,n,t); break;
            default: BlockCuts<BLOCK_THREADS>(r,c,m,n,t);
        };

    }


    void BlockCuts(size_t& rowBlockSize, size_t& colBlockSize,
                   size_t& lastRBS, size_t& lastCBS,
                   size_t& changeRBS, size_t& changeCBS,
                   size_t& numRowBlock, size_t& numColBlock,
                   size_t m, size_t n,
                   const CuttingStrategy method,
                   const size_t numthreads) {
        BlockCuts(rowBlockSize, colBlockSize, m, n, method, numthreads);
        numRowBlock = m/rowBlockSize;
        numColBlock = n/colBlockSize;

        changeRBS = m-rowBlockSize*numRowBlock;
        lastRBS = rowBlockSize;
        if (changeRBS) ++rowBlockSize;

        changeCBS = n-colBlockSize*numColBlock;
        lastCBS = colBlockSize;
        if (changeCBS) ++colBlockSize;


	/*
            // Better preserve numRowBlock and numColBlock
        if (lastRBS) {
            lastRBS = m-rowBlockSize*numRowBlock;
            ++rowBlockSize;
        } else lastRBS = rowBlockSize;
        if (lastCBS) {
            lastCBS = n-colBlockSize*numColBlock;
            ++colBlockSize;
        } else lastCBS = colBlockSize;
*/



//             // Better preserve rowBlockSize and colBlockSize
//         lastRBS = m % rowBlockSize;
//         lastCBS = n % colBlockSize;
//          if (lastRBS) ++numRowBlock; else lastRBS = rowBlockSize;
//          if (lastCBS) ++numColBlock; else lastCBS = colBlockSize;
    }


}



namespace FFLAS {
	template <typename blocksize_t=size_t>
    struct ForStrategy1D {
        ForStrategy1D(const blocksize_t n, const ParSeqHelper::Parallel H) {
//             std::cout<<"FS1D n : "<<n<<std::endl;
//             std::cout<<"FS1D method    : "<<method<<std::endl;
//             std::cout<<"FS1D numthreads : "<<numthreads<<std::endl;

            if ( H.method() == BLOCK_THREADS || H.method() == ROW_THREADS || H.method() == COLUMN_THREADS) {
                numBlock = std::max(H.numthreads(),(blocksize_t)1);
            } else if ( H.method() == GRAIN_SIZE ) { 
                numBlock = std::max(n/ H.numthreads(), (blocksize_t)1);
            } else {
                numBlock = std::max(n/__FFLASFFPACK_MINBLOCKCUTS,(blocksize_t)1);
            }
            firstBlockSize = n/numBlock;
            if (firstBlockSize<1) {
                firstBlockSize = (blocksize_t)1;
                numBlock = n;
            }
            changeBS = n - numBlock*firstBlockSize;
            lastBlockSize = firstBlockSize;
            if (changeBS) ++firstBlockSize;

//             std::cout<<"FS1D 1BLOCKSIZE : "<<firstBlockSize<<std::endl;
//             std::cout<<"FS1D 2BLOCKSIZE : "<<lastBlockSize<<std::endl;
//             std::cout<<"FS1D changeBS : "<<changeBS<<std::endl;
//             std::cout<<"FS1D NBlocks : "<<numBlock<<std::endl;
        }

        blocksize_t initialize() {
            ibeg = 0; iend = firstBlockSize;
//             std::cout << "FS1D 0   : " << 0 << std::endl;
//             std::cout << "FS1D ibeg: " << ibeg << std::endl;
//             std::cout << "FS1D iend: " << iend << std::endl;

            return current = 0;
        }
        bool isTerminated() const { return current == numBlock; }

        blocksize_t begin() const { return ibeg; }
        blocksize_t end() const { return iend; }
        
        blocksize_t blocksize() const { return firstBlockSize; }
        blocksize_t numblocks() const { return numBlock; }
                

        blocksize_t operator++() {
            ibeg = iend;
            iend += (++current<changeBS?firstBlockSize:lastBlockSize);

//             std::cout << "FS1D i   : " << current << std::endl;
//             std::cout << "FS1D ibeg: " << ibeg << std::endl;
//             std::cout << "FS1D iend: " << iend << std::endl;


            return current;
        }

    protected:
        blocksize_t ibeg, iend;

        blocksize_t current;
        blocksize_t firstBlockSize,lastBlockSize;
        blocksize_t changeBS;
        blocksize_t numBlock;

    };

	template <typename blocksize_t=size_t>
    struct ForStrategy2D {
        ForStrategy2D(const blocksize_t m, const blocksize_t n, const ParSeqHelper::Parallel H) {
            BlockCuts(rowBlockSize, colBlockSize,
                      lastRBS, lastCBS,
                      changeRBS, changeCBS,
                      numRowBlock, numColBlock,
                      m, n, H.method(), H.numthreads());

            BLOCKS = numRowBlock * numColBlock;
        }


        blocksize_t initialize() {
            _ibeg = 0; _iend = rowBlockSize;
            _jbeg = 0; _jend = colBlockSize;
            return current = 0;
        }
        bool isTerminated() const { return current == BLOCKS; }

        blocksize_t ibegin() const { return _ibeg; }
        blocksize_t jbegin() const { return _jbeg; }
        blocksize_t iend() const { return _iend; }
        blocksize_t jend() const { return _jend; }
        

        blocksize_t operator++() {
            ++current;
            blocksize_t icurr = current/numColBlock;
            blocksize_t jcurr = current%numColBlock;
            if (jcurr) {
                _jbeg = _jend;
                _jend += (jcurr<changeCBS?colBlockSize:lastCBS);
            } else {
                _ibeg = _iend;
                _iend += (icurr<changeRBS?rowBlockSize:lastRBS);
                _jbeg = 0;
                _jend = colBlockSize;
            }
            return current;
        }

        friend std::ostream& operator<<(std::ostream& out, const ForStrategy2D& FS2D) {
            out<<"RBLOCKSIZE: "<<FS2D.rowBlockSize<<std::endl;
            out<<"CBLOCKSIZE: "<<FS2D.colBlockSize<<std::endl;
            out<<"changeRBS : "<<FS2D.changeRBS<<std::endl;
            out<<"changeCBS : "<<FS2D.changeCBS<<std::endl;
            out<<"lastRBS   : "<<FS2D.lastRBS<<std::endl;
            out<<"lastCBS   : "<<FS2D.lastCBS<<std::endl;
            out<<"NrowBlocks: "<<FS2D.numRowBlock<<std::endl;
            out<<"NcolBlocks: "<<FS2D.numColBlock<<std::endl;
            out<<"curr: " << FS2D.current << '/' << FS2D.BLOCKS << std::endl;
            out<<"_ibeg: " << FS2D._ibeg << std::endl;
            out<<"_iend: " << FS2D._iend << std::endl;
            out<<"_jbeg: " << FS2D._jbeg << std::endl;
            out<<"_jend: " << FS2D._jend << std::endl;
            return out;
        }
                
        blocksize_t rowblocksize() const { return rowBlockSize; }
        blocksize_t rownumblocks() const { return numRowBlock; }
        blocksize_t colblocksize() const { return colBlockSize; }
        blocksize_t colnumblocks() const { return numColBlock; }


    protected:
        blocksize_t _ibeg, _iend, _jbeg, _jend;
        blocksize_t rowBlockSize, colBlockSize;

        blocksize_t current;
        blocksize_t lastRBS; blocksize_t lastCBS;
        blocksize_t changeRBS; blocksize_t changeCBS;
        blocksize_t numRowBlock; blocksize_t numColBlock;
        blocksize_t BLOCKS;

   };

}



#endif

