###############################################################################
################### MOOSE Application Standard Makefile #######################
###############################################################################
#
# Optional Environment variables
# MOOSE_DIR        - Root directory of the MOOSE project 
#
###############################################################################
# Use the MOOSE submodule if it exists and MOOSE_DIR is not set
MOOSE_SUBMODULE    := $(CURDIR)/../moose
ifneq ($(wildcard $(MOOSE_SUBMODULE)/framework/Makefile),)
  MOOSE_DIR        ?= $(MOOSE_SUBMODULE)
else
  MOOSE_DIR        ?= $(shell dirname `pwd`)/../moose
endif

# framework
FRAMEWORK_DIR      := $(MOOSE_DIR)/framework
include $(FRAMEWORK_DIR)/build.mk
include $(FRAMEWORK_DIR)/moose.mk
###############################################################################

################################## MODULES ####################################
PHASE_FIELD       := yes
SOLID_MECHANICS   := yes
TENSOR_MECHANICS  := yes
HEAT_CONDUCTION   := yes
MISC              := yes
include           $(MOOSE_DIR)/modules/modules.mk
###############################################################################

# Extra stuff for CPPUNIT
CPPUNIT_DIR 		?= $(PACKAGES_DIR)/cppunit
ADDITIONAL_INCLUDES 	:= -I$(CPPUNIT_DIR)/include
ADDITIONAL_LIBS 	:= -L$(CPPUNIT_DIR)/lib -lcppunit

# dep apps
APPLICATION_DIR    := $(CURDIR)/..
APPLICATION_NAME   := marmot
include            $(FRAMEWORK_DIR)/app.mk

APPLICATION_DIR    := $(CURDIR)
APPLICATION_NAME   := marmot-unit
BUILD_EXEC         := yes
DEP_APPS           ?= $(shell $(FRAMEWORK_DIR)/scripts/find_dep_apps.py $(APPLICATION_NAME))
include $(FRAMEWORK_DIR)/app.mk

# Find all the MARMOT unit test source files and include their dependencies.
marmot-unit_srcfiles := $(shell find $(CURDIR) -name "*.C")
marmot-unit_deps := $(patsubst %.C, %.$(obj-suffix).d, $(marmot-unit_srcfiles))
-include $(marmot-unit_deps)

###############################################################################
# Additional special case targets should be added here
