# -*- Makefile -*-
# Eugene Skepner 2016
# ======================================================================

MAKEFLAGS = -w

# ----------------------------------------------------------------------

SRC_DIR = $(abspath $(ACMACSD_ROOT)/sources)

ACMACS_DRAW_SOURCES = color.cc surface-cairo.cc

TEST_CAIRO_SOURCES = test-cairo.cc
TEST_CAIRO_FONTS_SOURCES = test-cairo-fonts.cc

# ----------------------------------------------------------------------

CLANG = $(shell if g++ --version 2>&1 | grep -i llvm >/dev/null; then echo Y; else echo N; fi)
ifeq ($(CLANG),Y)
  WEVERYTHING = -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded
  WARNINGS = -Wno-weak-vtables # -Wno-padded
  STD = c++14
else
  WEVERYTHING = -Wall -Wextra
  WARNINGS =
  STD = c++14
endif

# -fvisibility=hidden and -flto make resulting lib smaller (pybind11) but linking is much slower
OPTIMIZATION = -O3 #-fvisibility=hidden -flto
PROFILE = # -pg
CXXFLAGS = -MMD -g $(OPTIMIZATION) $(PROFILE) -fPIC -std=$(STD) $(WEVERYTHING) $(WARNINGS) -Icc -I$(BUILD)/include -I$(ACMACSD_ROOT)/include $(PKG_INCLUDES)
LDFLAGS = $(OPTIMIZATION) $(PROFILE)
PKG_INCLUDES = $$(pkg-config --cflags cairo)

LIB_DIR = $(ACMACSD_ROOT)/lib
ACMACSD_LIBS = -L$(LIB_DIR) -lacmacsbase
ACMACS_DRAW_LIB = $(DIST)/libacmacsdraw.so
ACMACS_DRAW_LDLIBS = $(ACMACSD_LIBS) $$(pkg-config --libs cairo)
TEST_CAIRO_LDLIBS = $(ACMACS_DRAW_LDLIBS) -L$(DIST) -lacmacsdraw

# PYTHON_VERSION = $(shell python3 -c 'import sys; print("{0.major}.{0.minor}".format(sys.version_info))')
# PYTHON_CONFIG = python$(PYTHON_VERSION)-config
# PYTHON_MODULE_SUFFIX = $(shell $(PYTHON_CONFIG) --extension-suffix)
# PYTHON_LD_LIB = $$($(PYTHON_CONFIG) --ldflags | sed -E 's/-Wl,-stack_size,[0-9]+//')

# PKG_INCLUDES = $$($(PYTHON_CONFIG) --includes)

# ----------------------------------------------------------------------

BUILD = build
DIST = $(abspath dist)

# ----------------------------------------------------------------------

all: check-python install $(DIST)/test-cairo $(DIST)/test-cairo-fonts

install: check-acmacsd-root link-includes $(ACMACS_DRAW_LIB)
	ln -sf $(ACMACS_DRAW_LIB) $(ACMACSD_ROOT)/lib
	if [ $$(uname) = "Darwin" ]; then /usr/bin/install_name_tool -id $(ACMACSD_ROOT)/lib/$(notdir $(ACMACS_DRAW_LIB)) $(ACMACSD_ROOT)/lib/$(notdir $(ACMACS_DRAW_LIB)); fi
	if [ -d $(SRC_DIR)/acmacs-draw/py/acmacs_draw ]; then ln -sf $(SRC_DIR)/acmacs-draw/py/acmacs_draw $(ACMACSD_ROOT)/py; fi
	if [ -d $(SRC_DIR)/acmacs-draw/bin ]; then ln -sf $(SRC_DIR)/acmacs-draw/bin/* $(ACMACSD_ROOT)/bin; fi

link-includes:
	if [ ! -d $(ACMACSD_ROOT)/include/acmacs-draw ]; then mkdir $(ACMACSD_ROOT)/include/acmacs-draw; fi
	ln -sf $(abspath cc)/*.hh $(ACMACSD_ROOT)/include/acmacs-draw

# ----------------------------------------------------------------------

$(DIST)/test-cairo: $(patsubst %.cc,$(BUILD)/%.o,$(TEST_CAIRO_SOURCES)) | $(DIST)
	g++ $(LDFLAGS) -o $@ $^ $(TEST_CAIRO_LDLIBS)

$(DIST)/test-cairo-fonts: $(patsubst %.cc,$(BUILD)/%.o,$(TEST_CAIRO_FONTS_SOURCES)) | $(DIST)
	g++ $(LDFLAGS) -o $@ $^ $(TEST_CAIRO_LDLIBS)

# ----------------------------------------------------------------------

-include $(BUILD)/*.d

# ----------------------------------------------------------------------

$(ACMACS_DRAW_LIB): $(patsubst %.cc,$(BUILD)/%.o,$(ACMACS_DRAW_SOURCES)) | $(DIST)
	g++ -shared $(LDFLAGS) -o $@ $^ $(ACMACS_DRAW_LDLIBS)

clean:
	rm -rf $(DIST) $(BUILD)/*.o $(BUILD)/*.d

distclean: clean
	rm -rf $(BUILD)

test: install $(DIST)/test-cairo $(DIST)/test-cairo-fonts

# ----------------------------------------------------------------------

$(BUILD)/%.o: cc/%.cc | $(BUILD)
	@echo $<
	@g++ $(CXXFLAGS) -c -o $@ $<

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
