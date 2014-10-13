
#
#   make - Configuration - Directories
#

    MAKE_BINARY:=bin
    MAKE_DOCUME:=doc
    MAKE_LIBRAR:=lib
    MAKE_SOURCE:=src

#
#   make - Configuration - Units
#

    MAKE_SOFTS:=$(notdir $(wildcard $(MAKE_SOURCE)/*) )

#
#   make - Configuration - Libraries
#

    MAKE_DEPENDS:=$(MAKE_LIBRAR)/libgnomonic

#
#   make - Build - Default
#

    all:directories libraries units
    build:directories units
    modules:libraries

#
#   make - Build - Stack
#

    units:
	@$(foreach SOFT, $(MAKE_SOFTS), $(MAKE) -C $(MAKE_SOURCE)/$(SOFT) clean && $(MAKE) -C $(MAKE_SOURCE)/$(SOFT) all && cp $(MAKE_SOURCE)/$(SOFT)/$(MAKE_BINARY)/$(SOFT) $(MAKE_BINARY)/ && ) true

#
#   make - Build - Libraries
#

    libraries:
	@$(foreach LIB, $(MAKE_DEPENDS), $(MAKE) -C $(LIB) clean && $(MAKE) -C $(LIB) all && ) true

#
#   make - Build - Documentation
#

    documentation:directories
	mkdir -p $(MAKE_DOCUME)/html && rm $(MAKE_DOCUME)/html/* -f
	@$(foreach DOC, $(MAKE_SOFTS), $(MAKE) -C $(MAKE_SOURCE)/$(DOC) documentation && cd $(MAKE_DOCUME)/html/ && ln -s ../../$(MAKE_SOURCE)/$(DOC)/$(MAKE_DOCUME)/html $(DOC) && cd - && ) true

#
#   make - Management - Directories
#

    directories:
	mkdir -p $(MAKE_BINARY)
	mkdir -p $(MAKE_DOCUME)

#
#   make - Management - Cleaning builds
#

    clean:
	rm $(MAKE_BINARY)/* -f

