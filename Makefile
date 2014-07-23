
#
#   make - Names
#

    MAKE_GNOMO=norama-eqr2gnomo
    MAKE_TILEG=norama-tile2gnomo
    MAKE_INVER=norama-gnomo2eqr
    MAKE_ROTAT=norama-rotate

#
#   make - Directories
#

    MAKE_BINARY=bin
    MAKE_DOCUME=doc
	MAKE_LIBRAR=lib
    MAKE_SOURCE=src

#
#   make - All
#

    all:libraries directories $(MAKE_GNOMO) $(MAKE_TILEG) $(MAKE_INVER) $(MAKE_ROTAT)

#
#   make - Suite
#

    $(MAKE_GNOMO):
	$(MAKE) -C $(MAKE_SOURCE)/$(MAKE_GNOMO) clean && $(MAKE) -C $(MAKE_SOURCE)/$(MAKE_GNOMO) all && cp $(MAKE_SOURCE)/$(MAKE_GNOMO)/$(MAKE_BINARY)/$(MAKE_GNOMO) $(MAKE_BINARY)/

    $(MAKE_TILEG):
	$(MAKE) -C $(MAKE_SOURCE)/$(MAKE_TILEG) clean && $(MAKE) -C $(MAKE_SOURCE)/$(MAKE_TILEG) all && cp $(MAKE_SOURCE)/$(MAKE_TILEG)/$(MAKE_BINARY)/$(MAKE_TILEG) $(MAKE_BINARY)/

    $(MAKE_INVER):
	$(MAKE) -C $(MAKE_SOURCE)/$(MAKE_INVER) clean && $(MAKE) -C $(MAKE_SOURCE)/$(MAKE_INVER) all && cp $(MAKE_SOURCE)/$(MAKE_INVER)/$(MAKE_BINARY)/$(MAKE_INVER) $(MAKE_BINARY)/

    $(MAKE_ROTAT):
	$(MAKE) -C $(MAKE_SOURCE)/$(MAKE_ROTAT) clean && $(MAKE) -C $(MAKE_SOURCE)/$(MAKE_ROTAT) all && cp $(MAKE_SOURCE)/$(MAKE_ROTAT)/$(MAKE_BINARY)/$(MAKE_ROTAT) $(MAKE_BINARY)/

#
#   make - Libraries
#

    libraries:
	$(MAKE) -C $(MAKE_LIBRAR)/libgnomonic clean && $(MAKE) -C $(MAKE_LIBRAR)/libgnomonic all

#
#   make - Documentation
#

    documentation:directories
	mkdir -p $(MAKE_DOCUME)/html && rm $(MAKE_DOCUME)/html/* -f
	$(MAKE) -C $(MAKE_SOURCE)/$(MAKE_GNOMO) documentation && cd $(MAKE_DOCUME)/html/ && ln -s ../../$(MAKE_SOURCE)/$(MAKE_GNOMO)/$(MAKE_DOCUME)/html $(MAKE_GNOMO) && cd -
	$(MAKE) -C $(MAKE_SOURCE)/$(MAKE_TILEG) documentation && cd $(MAKE_DOCUME)/html/ && ln -s ../../$(MAKE_SOURCE)/$(MAKE_TILEG)/$(MAKE_DOCUME)/html $(MAKE_TILEG) && cd -
	$(MAKE) -C $(MAKE_SOURCE)/$(MAKE_INVER) documentation && cd $(MAKE_DOCUME)/html/ && ln -s ../../$(MAKE_SOURCE)/$(MAKE_INVER)/$(MAKE_DOCUME)/html $(MAKE_INVER) && cd -
	$(MAKE) -C $(MAKE_SOURCE)/$(MAKE_ROTAT) documentation && cd $(MAKE_DOCUME)/html/ && ln -s ../../$(MAKE_SOURCE)/$(MAKE_ROTAT)/$(MAKE_DOCUME)/html $(MAKE_ROTAT) && cd -

#
#   make - Directories
#

    directories:
	mkdir -p $(MAKE_BINARY)
	mkdir -p $(MAKE_DOCUME)

#
#   make - Cleaning builds
#

    clean:
	rm $(MAKE_BINARY)/* -f

