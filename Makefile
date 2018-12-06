# -*- Makefile -*-
# ======================================================================

TARGETS = \
  $(ACMACS_DRAW_LIB) \
  $(ACMACS_DRAW_PY_LIB) \
  $(DIST)/test-cairo \
  $(DIST)/test-cairo-fonts \
  $(DIST)/test-distinct-colors

SRC_DIR = $(abspath $(ACMACSD_ROOT)/sources)

ACMACS_DRAW_SOURCES = continent-path.cc continent-map.cc geographic-path.cc geographic-map.cc \
  surface.cc surface-cairo.cc \
  draw-elements.cc draw-grid.cc draw-legend.cc draw-points.cc draw-arrow.cc
# surface-html.cc surface-js-dynamic.cc surface-js-static.cc

ACMACS_DRAW_PY_SOURCES = py.cc $(ACMACS_DRAW_SOURCES)

ACMACS_DRAW_LIB_MAJOR = 1
ACMACS_DRAW_LIB_MINOR = 0
ACMACS_DRAW_LIB_NAME = libacmacsdraw
ACMACS_DRAW_LIB = $(DIST)/$(call shared_lib_name,$(ACMACS_DRAW_LIB_NAME),$(ACMACS_DRAW_LIB_MAJOR),$(ACMACS_DRAW_LIB_MINOR))

ACMACS_DRAW_PY_LIB_MAJOR = 1
ACMACS_DRAW_PY_LIB_MINOR = 0
ACMACS_DRAW_PY_LIB_NAME = acmacs_draw_backend
ACMACS_DRAW_PY_LIB = $(DIST)/$(ACMACS_DRAW_PY_LIB_NAME)$(PYTHON_MODULE_SUFFIX)

# ----------------------------------------------------------------------

all: install

CONFIGURE_PYTHON = 1
CONFIGURE_CAIRO = 1
include $(ACMACSD_ROOT)/share/Makefile.config

LDLIBS = $(AD_LIB)/$(call shared_lib_name,libacmacsbase,1,0) $(CAIRO_LDLIBS) $(CXX_LIBS)

# ----------------------------------------------------------------------

install: install-headers $(TARGETS)
	$(call install_lib,$(ACMACS_DRAW_LIB))
	$(call install_py_lib,$(ACMACS_DRAW_PY_LIB))

install-disabled:
	@#mkdir -p $(AD_SHARE)/js/draw; ln -sf $(shell pwd)/js/* $(AD_SHARE)/js/draw
	@#ln -sf $(shell pwd)/bin/* $(AD_BIN)

test: install $(DIST)/test-cairo $(DIST)/test-cairo-fonts $(DIST)/test-distinct-colors
	bin/test-acmacs-draw

# ----------------------------------------------------------------------

$(ACMACS_DRAW_LIB): $(patsubst %.cc,$(BUILD)/%.o,$(ACMACS_DRAW_SOURCES)) | $(DIST)
	$(call echo_shared_lib,$@)
	$(call make_shared_lib,$(ACMACS_DRAW_LIB_NAME),$(ACMACS_DRAW_LIB_MAJOR),$(ACMACS_DRAW_LIB_MINOR)) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(ACMACS_DRAW_PY_LIB): $(patsubst %.cc,$(BUILD)/%.o,$(ACMACS_DRAW_PY_SOURCES)) | $(DIST)
	$(call echo_shared_lib,$@)
	$(call make_shared_lib,$(ACMACS_DRAW_PY_LIB_NAME),$(ACMACS_DRAW_PY_LIB_MAJOR),$(ACMACS_DRAW_PY_LIB_MINOR)) $(LDFLAGS) -o $@ $^ $(LDLIBS) $(PYTHON_LDLIBS)

$(DIST)/%: $(BUILD)/%.o | $(ACMACS_DRAW_LIB)
	$(call echo_link_exe,$@)
	$(CXX) $(LDFLAGS) -o $@ $^ $(ACMACS_DRAW_LIB) $(LDLIBS) $(AD_RPATH)

# ======================================================================
### Local Variables:
### eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
### End:
