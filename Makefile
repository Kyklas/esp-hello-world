#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := esp-hello-world


include help.mk

$(eval $(call help-add,get-modules,Get Git Submodule (recursive) ))
$(eval $(call help-space))
get-modules :
	$(shell git submodule update --init --recursive)

ifdef IDF_PATH_OVERRIDE
export IDF_PATH := $(IDF_PATH_OVERRIDE)
else
# always use local ESP-IDF Submodule
export IDF_PATH := $(realpath ./esp-idf)
endif

COMPONENT_DIRS := $(IDF_PATH)/components 
COMPONENT_DIRS += $(CURDIR)/components $(CURDIR)/main
COMPONENT_DIRS += $(CURDIR)/components-esp-sbfe

include $(IDF_PATH)/make/project.mk

