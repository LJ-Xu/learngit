# version 522 or 530
version ?=530

ifndef SUPPORTCMDS
SUPPORTCMDS:= all objs clean cleanall rebuild
endif

MAKEFILENAME:= Makefile_src

$(SUPPORTCMDS):
	mkdir -p ./src
	sh proto.sh
	for filename in $(MAKEFILENAME); do \
		echo "Making " $$filename $@ ; \
		(make $@ -f $$filename || exit "$$?"); \
	done;
