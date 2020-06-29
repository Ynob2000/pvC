VMATH_VERSION=0.11

all:
	make -C tests

clean:
	make -C tests clean
	
gendoc:
	doxygen
	rm doc/html/*.map doc/html/*.md5
	patch doc/latex/refman.tex < refman.tex.patch
	make -C doc/latex
	#rm -f doc/latex/*.map doc/latex/*.md5
	
cleandoc:
	rm -rf doc/
	mkdir -p foc/

runTests:
	tests/testAll > tests/testAll.log

dist: all tests gendoc clean
	mkdir -p tmp/vmath-$(VMATH_VERSION)/doc
	cp Doxyfile LICENSE Makefile refman.tex.patch tmp/vmath-$(VMATH_VERSION)
	cp -r src/ tmp/vmath-$(VMATH_VERSION)/src
	cp -r tests/ tmp/vmath-$(VMATH_VERSION)/tests
	cp -r doc/html tmp/vmath-$(VMATH_VERSION)/doc/html
	cp doc/latex/refman.pdf tmp/vmath-$(VMATH_VERSION)/doc/vmath-$(VMATH_VERSION)_api_reference_manual.pdf
	cd tmp/ && tar cvfj vmath-$(VMATH_VERSION).tar.bz2 vmath-$(VMATH_VERSION)/
	rm -rf tmp/vmath-$(VMATH_VERSION)
	
