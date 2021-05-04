.PHONY	:	debug
debug	:
	cd base && $(MAKE) $@
	cd app && $(MAKE) $@

.PHONY	:	clean_debug
clean_debug	:
	cd base && $(MAKE) $@
	cd app && $(MAKE) $@

.PHONY	:	release
release	:
	cd base && $(MAKE) $@
	cd app && $(MAKE) $@

.PHONY	:	clean_release
clean_release	:
	cd base && $(MAKE) $@
	cd app && $(MAKE) $@

.PHONY	:	all
all	:
	$(MAKE) clean_debug
	$(MAKE) clean_release
	$(MAKE) debug
	$(MAKE) release
