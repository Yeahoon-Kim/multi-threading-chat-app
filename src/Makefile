COMMANDTARGET = all clean
DIRECTORY = server client

$(COMMANDTARGET): chatProgram.o $(DIRECTORY)
ifeq ($(MAKECMDGOALS), clean)
	@rm -f *.o
endif

chatProgram.o: chatProgram.c chatProgram.h

$(DIRECTORY):
	$(MAKE) -C $(@) $(MAKECMDGOALS)

.PHONY: $(COMMANDTARGET) $(DIRECTORY)