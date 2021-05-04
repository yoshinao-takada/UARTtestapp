INCFLAGS = -I..
DEBUG_CCFLAGS = $(INCFLAGS) -c -g
RELEASE_CCFLAGS = $(INCFLAGS) -c -O3 -DNDEBUG
DEBUG_TARGET_DIR = ../debug/bin
RELEASE_TARGET_DIR = ../release/bin
DEBUG_LIB_DIR = -L../debug/lib
RELEASE_LIB_DIR = -L../release/lib
DEBUG_APP_SYSTICK = $(DEBUG_TARGET_DIR)/systick.exe
RELEASE_APP_SYSTICK = $(RELEASE_TARGET_DIR)/systick.exe
DEBUG_APP_DISPATCHER = $(DEBUG_TARGET_DIR)/dispatcher.exe
RELEASE_APP_DISPATCHER = $(RELEASE_TARGET_DIR)/dispatcher.exe
DEBUG_APP_SENDSIGNAL = $(DEBUG_TARGET_DIR)/sendsignal.exe
RELEASE_APP_SENDSIGNAL = $(RELEASE_TARGET_DIR)/sendsignal.exe
DEBUG_APP_UARTREAD = $(DEBUG_TARGET_DIR)/uartread.exe
RELEASE_APP_UARTREAD = $(RELEASE_TARGET_DIR)/uartread.exe
DEBUG_TARGETS = $(DEBUG_APP_SYSTICK) $(DEBUG_APP_DISPATCHER) $(DEBUG_APP_SENDSIGNAL) $(DEBUG_APP_UARTREAD)
RELEASE_TARGETS = $(RELEASE_APP_SYSTICK) $(RELEASE_APP_DISPATCHER) $(RELEASE_APP_SENDSIGNAL) $(RELEASE_APP_UARTREAD)
DEBUG_OBJ_DIR = ../debug/o
RELEASE_OBJ_DIR = ../release/o

#----- debug -------
.PHONY	:	debug
debug	:	$(DEBUG_TARGETS)

$(DEBUG_APP_SYSTICK)	:	$(DEBUG_OBJ_DIR)/systick.o
	mkdir -p $(DEBUG_TARGET_DIR)
	$(CC) -o $@ $^ $(DEBUG_LIB_DIR) -lbase -lpthread

$(DEBUG_OBJ_DIR)/systick.o	:	systick.c
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) -o $@ $<

$(DEBUG_APP_DISPATCHER)	:	$(DEBUG_OBJ_DIR)/dispatcher.o
	mkdir -p $(DEBUG_TARGET_DIR)
	$(CC) -o $@ $^ $(DEBUG_LIB_DIR) -lbase -lpthread

$(DEBUG_OBJ_DIR)/dispatcher.o	:	dispatcher.c
	$(CC) $(DEBUG_CCFLAGS) -o $@ $<

$(DEBUG_APP_SENDSIGNAL)	:	$(DEBUG_OBJ_DIR)/sendsignal.o
	mkdir -p $(DEBUG_TARGET_DIR)
	$(CC) -o $@ $^ $(DEBUG_LIB_DIR) -lbase -lpthread

$(DEBUG_OBJ_DIR)/sendsignal.o	:	sendsignal.c
	$(CC) $(DEBUG_CCFLAGS) -o $@ $<

$(DEBUG_APP_UARTREAD)	:	$(DEBUG_OBJ_DIR)/uartread.o
	mkdir -p $(DEBUG_TARGET_DIR)
	$(CC) -o $@ $^ $(DEBUG_LIB_DIR) -lbase -lpthread

$(DEBUG_OBJ_DIR)/uartread.o	:	uartread.c
	$(CC) $(DEBUG_CCFLAGS) -o $@ $<

.PHONY	:	clean_debug
clean_debug	:
	rm -f $(DEBUG_TARGETS)
	rm -f $(DEBUG_OBJ_DIR)/systick.o $(DEBUG_OBJ_DIR)/dispatcher.o $(DEBUG_OBJ_DIR)/sendsignal.o
	rm -f $(DEBUG_OBJ_DIR)/uartread.o

#----- release -------
.PHONY	:	release
release	:	$(RELEASE_TARGETS)

$(RELEASE_APP_SYSTICK)	:	$(RELEASE_OBJ_DIR)/systick.o
	mkdir -p $(RELEASE_TARGET_DIR)
	$(CC) -o $@ $^ $(RELEASE_LIB_DIR) -lbase -lpthread

$(RELEASE_OBJ_DIR)/systick.o	:	systick.c
	mkdir -p $(RELEASE_OBJ_DIR)
	$(CC) $(RELEASE_CCFLAGS) -o $@ $<

$(RELEASE_APP_DISPATCHER)	:	$(RELEASE_OBJ_DIR)/dispatcher.o
	mkdir -p $(RELEASE_TARGET_DIR)
	$(CC) -o $@ $^ $(RELEASE_LIB_DIR) -lbase -lpthread

$(RELEASE_OBJ_DIR)/dispatcher.o	:	dispatcher.c
	$(CC) $(RELEASE_CCFLAGS) -o $@ $<

$(RELEASE_APP_SENDSIGNAL)	:	$(RELEASE_OBJ_DIR)/sendsignal.o
	mkdir -p $(RELEASE_TARGET_DIR)
	$(CC) -o $@ $^ $(RELEASE_LIB_DIR) -lbase -lpthread

$(RELEASE_OBJ_DIR)/sendsignal.o	:	sendsignal.c
	$(CC) $(RELEASE_CCFLAGS) -o $@ $<

$(RELEASE_APP_UARTREAD)	:	$(RELEASE_OBJ_DIR)/uartread.o
	mkdir -p $(RELEASE_TARGET_DIR)
	$(CC) -o $@ $^ $(RELEASE_LIB_DIR) -lbase -lpthread

$(RELEASE_OBJ_DIR)/uartread.o	:	uartread.c
	$(CC) $(RELEASE_CCFLAGS) -o $@ $<

.PHONY	:	clean_release
clean_release	:
	rm -f $(RELEASE_TARGETS)
	rm -f $(RELEASE_OBJ_DIR)/systick.o $(RELEASE_OBJ_DIR)/dispatcher.o $(RELEASE_OBJ_DIR)/sendsignal.o
	rm -f $(RELEASE_OBJ_DIR)/uartread.o
