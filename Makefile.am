SUBDIRS = src doc

dist_pkgdata_DATA = \
	VERSION

SPHINXBUILD     = @SPHINXBUILD@
TOPSOURCEDIR    = $(top_srcdir)
TOPBUILDDIR     = $(top_builddir)

clean-local:
	-rm -rf doc/html

.PHONY: doc
doc: html

html-local:
if ENABLE_SPHINX
	$(TOPSOURCEDIR)/doc/makeDocs.sh "$(SPHINXBUILD)" "$(TOPSOURCEDIR)" "$(TOPBUILDDIR)"
else
	@echo "======================================================"
	@echo "Skipping documentation building (see configure output)"
	@echo "======================================================"
endif

.PHONY: install-doc
install-doc: install-html

install-html-local: html
if ENABLE_SPHINX
	mkdir -p "@htmldir@"
	cp -a $(TOPBUILDDIR)/doc/build/html/* "@htmldir@/"
else
	@echo "=========================================================="
	@echo "Skipping documentation installation (see configure output)"
	@echo "=========================================================="
endif

uninstall-local:
	rm -rf @htmldir@
