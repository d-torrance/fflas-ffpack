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

SRC= $(sort $(wildcard *_test_*.m))
LB = $(SRC:.m=)

GREEN  = \33[32;01m
RED    = \33[31;01m
BLUE   = \33[34m
BBLUE  = \33[34;01m
YELLOW = \33[33;01m
RESET  = \33[0m
LANG=C # used for printf...

check: footer

header:
	rm -rf tests.log
	@printf "$(BBLUE)-------------------------------------------------------$(RESET)\n"
	@printf "$(BBLUE)   TEST MATLAB INTERFACES$(RESET)\n"
	@printf "$(BBLUE)-------------------------------------------------------$(RESET)\n"

footer: $(LB)
	@all=`grep Test tests.log | wc -l`; \
	pass=`grep PASS tests.log | wc -l`; \
	fail=`grep FAIL tests.log | wc -l`; \
	pp=`echo "scale=2; ($$pass/$$all)*100" | bc -l`; \
	pf=`echo "scale=2; ($$fail/$$all)*100" | bc -l`; \
	printf "$(BBLUE)-------------------------------------------------------$(RESET)\n"; \
	printf "$(BBLUE)  TOTAL: %5d\n" "$$all"; \
	printf "$(BBLUE)   PASS: %5d (%6.2f%%)\n" "$$pass" "$$pp"; \
	printf "$(BBLUE)   FAIL: %5d (%6.2f%%)\n" "$$fail" "$$pf"; \
	printf "$(BBLUE)-------------------------------------------------------$(RESET)\n";

%:%.m header
	@printf "$(BLUE)-------------------------------------------------------$(RESET)\n"
	@printf "$(BLUE)Testing $*: $(RESET) "
	@LD_PRELOAD=$(LIB_STD_CXX) $(MATLAB) -nojvm -nodisplay -r "exit($*('$(INTF_BIN_MATLAB_PATH)'));" > /dev/null; \
	if [ $$? -eq 0 ] ; then \
		printf "$(GREEN)PASS$(RESET)\n"; \
		echo "Test: $* : PASS" >> tests.log; \
	else \
		printf "$(RED)FAIL$(RESET)\n"; \
		echo "Test: $* : FAIL" >> tests.log; \
	fi;

