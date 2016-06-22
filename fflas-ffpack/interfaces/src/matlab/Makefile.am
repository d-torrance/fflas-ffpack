# Copyright (c) 2015-2016 SILKAN
# written by Thomas Izard <izard@silkan.com>
# granted by the ANR HPAC
#
# ========LICENCE========
# This file is part of the library FFLAS-FFPACK.
#
# FFLAS-FFPACK is free software: you can redistribute it and/or modify
# it under the terms of the  GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
# ========LICENCE========
#

SRC= $(wildcard *.cpp)
LB = $(SRC:.cpp=.mexa64)

make_int_dir:
	@mkdir -p $(INTF_BIN_CXX_PATH)
	@mkdir -p $(INTF_BIN_MATLAB_PATH)


all: $(LB)

install-exec-local: make_int_dir $(LB)
	cp *.mexa64 $(INTF_BIN_MATLAB_PATH)
	-cp *.m     $(INTF_BIN_MATLAB_PATH)

%.mexa64: %.cpp $(INTF_BIN_CXX_PATH)/ff_%.a
	$(MEX) "CXXFLAGS=$(CXXFLAGS) $(AVXFLAGS) -fPIC $(OMPFLAGS)" $(INTF_BIN_CXX_PATH)/ff_$*.a $*.cpp -I$(INTF_INC_CXX_PATH) -I$(INTF_INC_MATLAB_PATH) $(CBLAS_LIBS) $(GIVARO_LIBS) "LDFLAGS=\$$LDFLAGS $(OMPFLAGS)"

$(INTF_BIN_CXX_PATH)/%.a:$(INTF_SRC_CXX_PATH)/%.o
	$(AR) $(ARFLAGS) $@ $<

$(INTF_SRC_CXX_PATH)/%.o: $(INTF_SRC_CXX_PATH)/%.cpp
	$(CXX) $(CXXFLAGS) $(AVXFLAGS) -c -fPIC $< $(GIVARO_CFLAGS) -I$(INTF_INC_CXX_PATH) -I$(top_srcdir) -o $@

clean-local:
	rm -rf *.mexa64
	rm -rf $(INTF_BIN_MATLAB_PATH)
