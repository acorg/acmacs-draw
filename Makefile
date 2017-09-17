# -*- Makefile -*-
# Eugene Skepner 2017
# ======================================================================

MAKEFLAGS = -w

# ----------------------------------------------------------------------

SRC_DIR = $(abspath $(ACMACSD_ROOT)/sources)

ACMACS_DRAW_SOURCES = surface.cc surface-cairo.cc continent-path.cc continent-map.cc geographic-path.cc geographic-map.cc
PY_SOURCES = py.cc $(ACMACS_DRAW_SOURCES)
BACKEND = $(DIST)/acmacs_draw_backend$(PYTHON_MODULE_SUFFIX)

TEST_CAIRO_SOURCES = test-cairo.cc
TEST_CAIRO_FONTS_SOURCES = test-cairo-fonts.cc
TEST_DISTINCT_COLORS_SOURCES = test-distinct-colors.cc

# ----------------------------------------------------------------------

include $(ACMACSD_ROOT)/share/makefiles/Makefile.g++
include $(ACMACSD_ROOT)/share/makefiles/Makefile.dist-build.vars

CXXFLAGS = -MMD -g $(OPTIMIZATION) $(PROFILE) -fPIC -std=$(STD) $(WARNINGS) -Icc -I$(AD_INCLUDE) $(PKG_INCLUDES)
LDFLAGS = $(OPTIMIZATION) $(PROFILE)
PKG_INCLUDES = $(shell pkg-config --cflags cairo) $(shell $(PYTHON_CONFIG) --includes)

ACMACSD_LIBS = -L$(AD_LIB) -lacmacsbase
ACMACS_DRAW_LIB = $(DIST)/libacmacsdraw.so
ACMACS_DRAW_LDLIBS = $(ACMACSD_LIBS) $(shell pkg-config --libs cairo)
TEST_CAIRO_LDLIBS = $(ACMACS_DRAW_LDLIBS) -L$(DIST) -lacmacsdraw
BACKEND_LDLIBS = $(ACMACS_DRAW_LDLIBS) $(shell $(PYTHON_CONFIG) --ldflags | sed -E 's/-Wl,-stack_size,[0-9]+//')

PYTHON_VERSION = $(shell python3 -c 'import sys; print("{0.major}.{0.minor}".format(sys.version_info))')
PYTHON_CONFIG = python$(PYTHON_VERSION)-config
PYTHON_MODULE_SUFFIX = $(shell $(PYTHON_CONFIG) --extension-suffix)

# PYTHON_LD_LIB = $(shell $(PYTHON_CONFIG) --ldflags | sed -E 's/-Wl,-stack_size,[0-9]+//')

# ----------------------------------------------------------------------

all: check-python install $(DIST)/test-cairo $(DIST)/test-cairo-fonts $(DIST)/test-distinct-colors $(BACKEND)

install: check-acmacsd-root install-headers $(ACMACS_DRAW_LIB) $(BACKEND)
	$(call install_lib,$(ACMACS_DRAW_LIB))
	if [ -d $(SRC_DIR)/acmacs-draw/py/acmacs_draw ]; then ln -sf $(SRC_DIR)/acmacs-draw/py/acmacs_draw $(AD_PY); fi
	if [ -d $(SRC_DIR)/acmacs-draw/bin ]; then ln -sf $(SRC_DIR)/acmacs-draw/bin/* $(AD_BIN); fi
	ln -sf $(BACKEND) $(AD_PY)

install-headers:
	$(call install_headers,acmacs-draw)

# ----------------------------------------------------------------------

test-cairo: $(DIST)/test-cairo
$(DIST)/test-cairo: $(patsubst %.cc,$(BUILD)/%.o,$(TEST_CAIRO_SOURCES)) $(ACMACS_DRAW_LIB) | $(DIST)
	@echo "LINK       " $@ # '<--' $^
	@$(CXX) $(LDFLAGS) -o $@ $^ $(TEST_CAIRO_LDLIBS)

test-cairo-fonts: $(DIST)/test-cairo-fonts
$(DIST)/test-cairo-fonts: $(patsubst %.cc,$(BUILD)/%.o,$(TEST_CAIRO_FONTS_SOURCES)) $(ACMACS_DRAW_LIB) | $(DIST)
	@echo "LINK       " $@ # '<--' $^
	@$(CXX) $(LDFLAGS) -o $@ $^ $(TEST_CAIRO_LDLIBS)

test-distinct-colors: $(DIST)/test-distinct-colors
$(DIST)/test-distinct-colors: $(patsubst %.cc,$(BUILD)/%.o,$(TEST_DISTINCT_COLORS_SOURCES)) $(ACMACS_DRAW_LIB) | $(DIST)
	@echo "LINK       " $@ # '<--' $^
	@$(CXX) $(LDFLAGS) -o $@ $^ $(TEST_CAIRO_LDLIBS)

# ----------------------------------------------------------------------

-include $(BUILD)/*.d
include $(ACMACSD_ROOT)/share/makefiles/Makefile.dist-build.rules
include $(ACMACSD_ROOT)/share/makefiles/Makefile.rtags

# ----------------------------------------------------------------------

$(ACMACS_DRAW_LIB): $(patsubst %.cc,$(BUILD)/%.o,$(ACMACS_DRAW_SOURCES)) | $(DIST)
	@echo "SHARED     " $@ # '<--' $^
	@$(CXX) -shared $(LDFLAGS) -o $@ $^ $(ACMACS_DRAW_LDLIBS)

$(BACKEND): $(patsubst %.cc,$(BUILD)/%.o,$(PY_SOURCES)) | $(DIST)
	@echo "SHARED     " $@ # '<--' $^
	@$(CXX) -shared $(LDFLAGS) -o $@ $^ $(BACKEND_LDLIBS)

test: install $(DIST)/test-cairo $(DIST)/test-cairo-fonts $(DIST)/test-distinct-colors
	bin/test-acmacs-draw

# ======================================================================
### Local Variables:
### eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
### End:
