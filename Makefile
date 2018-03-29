# -*- Makefile -*-
# Eugene Skepner 2017
# ======================================================================

MAKEFLAGS = -w

# ----------------------------------------------------------------------

TARGETS = \
  $(ACMACS_DRAW_LIB) \
  $(ACMACS_DRAW_PY_LIB) \
  $(DIST)/test-cairo \
  $(DIST)/test-cairo-fonts \
  $(DIST)/test-distinct-colors


SRC_DIR = $(abspath $(ACMACSD_ROOT)/sources)

ACMACS_DRAW_SOURCES = continent-path.cc continent-map.cc geographic-path.cc geographic-map.cc \
  surface.cc surface-cairo.cc surface-html.cc \
  draw-elements.cc draw-elements-pdf.cc

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

include $(ACMACSD_ROOT)/share/makefiles/Makefile.g++
include $(ACMACSD_ROOT)/share/makefiles/Makefile.python
include $(ACMACSD_ROOT)/share/makefiles/Makefile.dist-build.vars

CXXFLAGS = -MMD -g $(OPTIMIZATION) $(PROFILE) -fPIC -std=$(STD) $(WARNINGS) -Icc -I$(AD_INCLUDE) $(PKG_INCLUDES)
LDFLAGS = $(OPTIMIZATION) $(PROFILE)
PKG_INCLUDES = $(shell pkg-config --cflags cairo) $(PYTHON_INCLUDES)

LDLIBS = $(AD_LIB)/$(call shared_lib_name,libacmacsbase,1,0) $(shell pkg-config --libs cairo) $(CXX_LIB)

# ----------------------------------------------------------------------

all: $(TARGETS)

install: check-acmacsd-root install-headers $(TARGETS)
	$(call install_lib,$(ACMACS_DRAW_LIB))
	@#$(call install_py_lib,$(ACMACS_DRAW_PY_LIB))
	@#ln -sf $(SRC_DIR)/acmacs-draw/bin/* $(AD_BIN)

test: install $(DIST)/test-cairo $(DIST)/test-cairo-fonts $(DIST)/test-distinct-colors
	bin/test-acmacs-draw

# ----------------------------------------------------------------------

-include $(BUILD)/*.d
include $(ACMACSD_ROOT)/share/makefiles/Makefile.dist-build.rules
include $(ACMACSD_ROOT)/share/makefiles/Makefile.rtags

# ----------------------------------------------------------------------

$(ACMACS_DRAW_LIB): $(patsubst %.cc,$(BUILD)/%.o,$(ACMACS_DRAW_SOURCES)) | $(DIST)
	@printf "%-16s %s\n" "SHARED" $@
	@$(call make_shared,$(ACMACS_DRAW_LIB_NAME),$(ACMACS_DRAW_LIB_MAJOR),$(ACMACS_DRAW_LIB_MINOR)) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(ACMACS_DRAW_PY_LIB): $(patsubst %.cc,$(BUILD)/%.o,$(ACMACS_DRAW_PY_SOURCES)) | $(DIST)
	@printf "%-16s %s\n" "SHARED" $@
	@$(call make_shared,$(ACMACS_DRAW_PY_LIB_NAME),$(ACMACS_DRAW_PY_LIB_MAJOR),$(ACMACS_DRAW_PY_LIB_MINOR)) $(LDFLAGS) -o $@ $^ $(LDLIBS) $(PYTHON_LDLIBS)

$(DIST)/%: $(BUILD)/%.o | $(ACMACS_DRAW_LIB)
	@printf "%-16s %s\n" "LINK" $@
	@$(CXX) $(LDFLAGS) -o $@ $^ $(ACMACS_DRAW_LIB) $(LDLIBS) $(AD_RPATH)

# ======================================================================
### Local Variables:
### eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
### End:
