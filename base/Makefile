INCFLAGS = -I..
DEBUG_CCFLAGS = $(INCFLAGS) -c -g
RELEASE_CCFLAGS = $(INCFLAGS) -c -O3 -DNDEBUG
DEBUG_TARGET_DIR = ../debug/lib
RELEASE_TARGET_DIR = ../release/lib
DEBUG_TARGET = $(DEBUG_TARGET_DIR)/libbase.a
RELEASE_TARGET = $(RELEASE_TARGET_DIR)/libbase.a
DEBUG_OBJ_DIR = ../debug/o
RELEASE_OBJ_DIR = ../release/o
DEBUG_OBJECTS = $(DEBUG_OBJ_DIR)/BLsystick.o $(DEBUG_OBJ_DIR)/BLts.o $(DEBUG_OBJ_DIR)/BLdispatcher.o \
	$(DEBUG_OBJ_DIR)/UASDKiobase.o
RELEASE_OBJECTS = $(RELEASE_OBJ_DIR)/BLsystick.o $(RELEASE_OBJ_DIR)/BLts.o $(RELEASE_OBJ_DIR)/BLdispatcher.o \
	$(RELEASE_OBJ_DIR)/UASDKiobase.o

#------------ debug ----------------
.PHONY	:	debug
debug	:	$(DEBUG_TARGET)
$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS)
	mkdir -p $(DEBUG_TARGET_DIR)
	$(AR) r $@ $^
	$(RANLIB) $@

$(DEBUG_OBJ_DIR)/BLsystick.o	:	BLsystick.c BLsystick.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/BLts.o	:	BLts.c BLts.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/BLdispatcher.o	:	BLdispatcher.c BLdispatcher.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/UASDKiobase.o	:	UASDKiobase.c UASDKiobase.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) -o $@ $<

.PHONY	:	clean_debug
clean_debug	:
	rm -f $(DEBUG_TARGET)
	rm -f $(DEBUG_OBJECTS)

#------------ release ----------------
.PHONY	:	release
release	:	$(RELEASE_TARGET)
$(RELEASE_TARGET)	:	$(RELEASE_OBJECTS)
	mkdir -p $(RELEASE_TARGET_DIR)
	$(AR) r $@ $^
	$(RANLIB) $@

$(RELEASE_OBJ_DIR)/BLsystick.o	:	BLsystick.c BLsystick.h
	mkdir -p $(RELEASE_OBJ_DIR)
	$(CC) $(RELEASE_CCFLAGS) -o $@ $<

$(RELEASE_OBJ_DIR)/BLts.o	:	BLts.c BLts.h
	mkdir -p $(RELEASE_OBJ_DIR)
	$(CC) $(RELEASE_CCFLAGS) -o $@ $<

$(RELEASE_OBJ_DIR)/BLdispatcher.o	:	BLdispatcher.c BLdispatcher.h
	mkdir -p $(RELEASE_OBJ_DIR)
	$(CC) $(RELEASE_CCFLAGS) -o $@ $<

$(RELEASE_OBJ_DIR)/UASDKiobase.o	:	UASDKiobase.c UASDKiobase.h
	mkdir -p $(RELEASE_OBJ_DIR)
	$(CC) $(RELEASE_CCFLAGS) -o $@ $<

.PHONY	:	clean_release
clean_release	:
	rm -f $(RELEASE_TARGET)
	rm -f $(RELEASE_OBJECTS)
