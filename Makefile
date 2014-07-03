
#
#   make - Names
#

    MAKE_GNOMO=norama-eqr2gnomo
    MAKE_INVER=norama-gnomo2eqr

#
#   make - Directories
#

    MAKE_BINARY=bin
    MAKE_SOURCE=src

#
#   make - All
#

    all:$(MAKE_GNOMO) $(MAKE_INVER)

#
#   make - Suite
#

    $(MAKE_GNOMO):directories
	$(MAKE) -C $(MAKE_SOURCE)/$(MAKE_GNOMO) clean && $(MAKE) -C $(MAKE_SOURCE)/$(MAKE_GNOMO) all && cp $(MAKE_SOURCE)/$(MAKE_GNOMO)/$(MAKE_BINARY)/$(MAKE_GNOMO) $(MAKE_BINARY)/

    $(MAKE_INVER):directories
	$(MAKE) -C $(MAKE_SOURCE)/$(MAKE_INVER) clean && $(MAKE) -C $(MAKE_SOURCE)/$(MAKE_INVER) all && cp $(MAKE_SOURCE)/$(MAKE_INVER)/$(MAKE_BINARY)/$(MAKE_INVER) $(MAKE_BINARY)/

#
#   make - Directories
#

    directories:
	mkdir -p $(MAKE_BINARY)

#
#   make - Cleaning builds
#

    clean:
	rm $(MAKE_BINARY)/* -f

