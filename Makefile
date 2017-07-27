# -*- Makefile -*-
# Eugene Skepner 2016
# ======================================================================

MAKEFLAGS = -w

# ----------------------------------------------------------------------

SRC_DIR = $(abspath $(ACMACSD_ROOT)/sources)

ACMACS_DRAW_SOURCES = surface-cairo.cc continent-path.cc continent-map.cc geographic-path.cc geographic-map.cc
PY_SOURCES = py.cc $(ACMACS_DRAW_SOURCES)
BACKEND = $(DIST)/acmacs_draw_backend$(PYTHON_MODULE_SUFFIX)

TEST_CAIRO_SOURCES = test-cairo.cc
TEST_CAIRO_FONTS_SOURCES = test-cairo-fonts.cc
TEST_DISTINCT_COLORS_SOURCES = test-distinct-colors.cc

# ----------------------------------------------------------------------

include $(ACMACSD_ROOT)/share/Makefile.g++

# -fvisibility=hidden and -flto make resulting lib smaller (pybind11) but linking is much slower
OPTIMIZATION = -O3 #-fvisibility=hidden -flto
PROFILE = # -pg
CXXFLAGS = -MMD -g $(OPTIMIZATION) $(PROFILE) -fPIC -std=$(STD) $(WEVERYTHING) $(WARNINGS) -Icc -I$(BUILD)/include -I$(ACMACSD_ROOT)/include $(PKG_INCLUDES)
LDFLAGS = $(OPTIMIZATION) $(PROFILE)
PKG_INCLUDES = $$(pkg-config --cflags cairo) $$($(PYTHON_CONFIG) --includes)

LIB_DIR = $(ACMACSD_ROOT)/lib
ACMACSD_LIBS = -L$(LIB_DIR) -lacmacsbase
ACMACS_DRAW_LIB = $(DIST)/libacmacsdraw.so
ACMACS_DRAW_LDLIBS = $(ACMACSD_LIBS) $$(pkg-config --libs cairo)
TEST_CAIRO_LDLIBS = $(ACMACS_DRAW_LDLIBS) -L$(DIST) -lacmacsdraw
BACKEND_LDLIBS = $(ACMACS_DRAW_LDLIBS) $$($(PYTHON_CONFIG) --ldflags | sed -E 's/-Wl,-stack_size,[0-9]+//')

PYTHON_VERSION = $(shell python3 -c 'import sys; print("{0.major}.{0.minor}".format(sys.version_info))')
PYTHON_CONFIG = python$(PYTHON_VERSION)-config
PYTHON_MODULE_SUFFIX = $(shell $(PYTHON_CONFIG) --extension-suffix)

# PYTHON_LD_LIB = $$($(PYTHON_CONFIG) --ldflags | sed -E 's/-Wl,-stack_size,[0-9]+//')

# ----------------------------------------------------------------------

BUILD = build
DIST = $(abspath dist)

# ----------------------------------------------------------------------

all: check-python install $(DIST)/test-cairo $(DIST)/test-cairo-fonts $(DIST)/test-distinct-colors $(BACKEND)

install: check-acmacsd-root install-headers $(ACMACS_DRAW_LIB) $(BACKEND)
	ln -sf $(ACMACS_DRAW_LIB) $(ACMACSD_ROOT)/lib
	if [ $$(uname) = "Darwin" ]; then /usr/bin/install_name_tool -id $(ACMACSD_ROOT)/lib/$(notdir $(ACMACS_DRAW_LIB)) $(ACMACSD_ROOT)/lib/$(notdir $(ACMACS_DRAW_LIB)); fi
	if [ -d $(SRC_DIR)/acmacs-draw/py/acmacs_draw ]; then ln -sf $(SRC_DIR)/acmacs-draw/py/acmacs_draw $(ACMACSD_ROOT)/py; fi
	if [ -d $(SRC_DIR)/acmacs-draw/bin ]; then ln -sf $(SRC_DIR)/acmacs-draw/bin/* $(ACMACSD_ROOT)/bin; fi
	ln -sf $(BACKEND) $(ACMACSD_ROOT)/py

install-headers:
	if [ ! -d $(ACMACSD_ROOT)/include/acmacs-draw ]; then mkdir $(ACMACSD_ROOT)/include/acmacs-draw; fi
	ln -sf $(abspath cc)/*.hh $(ACMACSD_ROOT)/include/acmacs-draw

# ----------------------------------------------------------------------

test-cairo: $(DIST)/test-cairo
$(DIST)/test-cairo: $(patsubst %.cc,$(BUILD)/%.o,$(TEST_CAIRO_SOURCES)) $(ACMACS_DRAW_LIB) | $(DIST)
	$(GXX) $(LDFLAGS) -o $@ $^ $(TEST_CAIRO_LDLIBS)

test-cairo-fonts: $(DIST)/test-cairo-fonts
$(DIST)/test-cairo-fonts: $(patsubst %.cc,$(BUILD)/%.o,$(TEST_CAIRO_FONTS_SOURCES)) $(ACMACS_DRAW_LIB) | $(DIST)
	$(GXX) $(LDFLAGS) -o $@ $^ $(TEST_CAIRO_LDLIBS)

test-distinct-colors: $(DIST)/test-distinct-colors
$(DIST)/test-distinct-colors: $(patsubst %.cc,$(BUILD)/%.o,$(TEST_DISTINCT_COLORS_SOURCES)) $(ACMACS_DRAW_LIB) | $(DIST)
	$(GXX) $(LDFLAGS) -o $@ $^ $(TEST_CAIRO_LDLIBS)

# ----------------------------------------------------------------------

-include $(BUILD)/*.d

# ----------------------------------------------------------------------

$(ACMACS_DRAW_LIB): $(patsubst %.cc,$(BUILD)/%.o,$(ACMACS_DRAW_SOURCES)) | $(DIST)
	$(GXX) -shared $(LDFLAGS) -o $@ $^ $(ACMACS_DRAW_LDLIBS)

$(BACKEND): $(patsubst %.cc,$(BUILD)/%.o,$(PY_SOURCES)) | $(DIST)
	$(GXX) -shared $(LDFLAGS) -o $@ $^ $(BACKEND_LDLIBS)

clean:
	rm -rf $(DIST) $(BUILD)/*.o $(BUILD)/*.d

distclean: clean
	rm -rf $(BUILD)

test: install $(DIST)/test-cairo $(DIST)/test-cairo-fonts $(DIST)/test-distinct-colors
	bin/test-acmacs-draw

# ----------------------------------------------------------------------

$(BUILD)/%.o: cc/%.cc | $(BUILD) install-headers
	@echo $<
	@$(GXX) $(CXXFLAGS) -c -o $@ $<

# ----------------------------------------------------------------------

check-acmacsd-root:
ifndef ACMACSD_ROOT
	$(error ACMACSD_ROOT is not set)
endif

check-python:
	@printf 'import sys\nif sys.version_info < (3, 5):\n print("Python 3.5 is required")\n exit(1)' | python3

$(DIST):
	mkdir -p $(DIST)

$(BUILD):
	mkdir -p $(BUILD)

.PHONY: check-acmacsd-root check-python

# ======================================================================
### Local Variables:
### eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
### End:
