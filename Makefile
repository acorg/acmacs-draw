# -*- Makefile -*-
# ======================================================================

TARGETS = \
  $(ACMACS_DRAW_LIB) \
  $(DIST)/drawi \
  $(DIST)/test-cairo \
  $(DIST)/test-cairo-fonts \
  $(DIST)/test-distinct-colors

# $(ACMACS_DRAW_PY_LIB)

SRC_DIR = $(abspath $(ACMACSD_ROOT)/sources)

ACMACS_DRAW_SOURCES = \
  draw-elements.cc    \
  surface-cairo.cc    \
  draw-points.cc      \
  point-style-data.cc \
  draw-arrow.cc       \
  draw-legend.cc      \
  surface.cc          \
  continent-map.cc    \
  geographic-map.cc   \
  draw-grid.cc        \
  continent-path.cc   \
  geographic-path.cc  \
  drawi-settings.cc   \
  drawi-generator.cc  \
  reference-panel-plot.cc

# surface-html.cc surface-js-dynamic.cc surface-js-static.cc

ACMACS_DRAW_LIB_MAJOR = 1
ACMACS_DRAW_LIB_MINOR = 0
ACMACS_DRAW_LIB_NAME = libacmacsdraw
ACMACS_DRAW_LIB = $(DIST)/$(call shared_lib_name,$(ACMACS_DRAW_LIB_NAME),$(ACMACS_DRAW_LIB_MAJOR),$(ACMACS_DRAW_LIB_MINOR))

# ----------------------------------------------------------------------

all: install

CONFIGURE_CAIRO = 1
include $(ACMACSD_ROOT)/share/Makefile.config

LDLIBS = \
  $(AD_LIB)/$(call shared_lib_name,libacmacsbase,1,0) \
  $(AD_LIB)/$(call shared_lib_name,libacmacschart,2,0) \
  $(CAIRO_LIBS) \
  $(CXX_LIBS)

# ----------------------------------------------------------------------

install: install-headers make-installation-dirs $(TARGETS)
	$(call install_lib,$(ACMACS_DRAW_LIB))
	$(call install_all,$(AD_PACKAGE_NAME))

install-disabled:
	$(call make_dir,$(AD_SHARE)/js/draw)
    #ln -sf $(shell pwd)/js/* $(AD_SHARE)/js/draw
	#ln -sf $(shell pwd)/bin/* $(AD_BIN)

test: install $(DIST)/test-cairo $(DIST)/test-cairo-fonts $(DIST)/test-distinct-colors
	echo ">>> acmacs-draw tests used py interface and now disabled"

# ifneq ($(DEBUG),1)
# 	bin/test-acmacs-draw
# else
# 	echo ">> WARNING: acmacs-draw tests do not work with address sanitizer (via python)"
# endif
.PHONY: test

# ----------------------------------------------------------------------

$(ACMACS_DRAW_LIB): $(patsubst %.cc,$(BUILD)/%.o,$(ACMACS_DRAW_SOURCES)) | $(DIST) install-headers
	$(call echo_shared_lib,$@)
	$(call make_shared_lib,$(ACMACS_DRAW_LIB_NAME),$(ACMACS_DRAW_LIB_MAJOR),$(ACMACS_DRAW_LIB_MINOR)) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(DIST)/%: $(BUILD)/%.o | $(ACMACS_DRAW_LIB) install-headers
	$(call echo_link_exe,$@)
	$(CXX) $(LDFLAGS) -o $@ $^ $(ACMACS_DRAW_LIB) $(LDLIBS) $(AD_RPATH)

# ======================================================================
### Local Variables:
### eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
### End:
