PWD = $(shell pwd)
SRC = $(PWD)/main
INC = $(PWD)/include

#FILE = $(PWD)/file
MAIN = $(PWD)/main
LINEAR_ALGEBRA = $(PWD)/linear_algebra

glGramSchmidt = $(MAIN)/glGramSchmidt

#SUBDIRS = $(FILE)
SUBDIRS = $(LINEAR_ALGEBRA)

all:	main
	$(shell rm -rf build)
	$(shell mkdir build)
	$(shell cp $(glGramSchmidt) ./build/)

main:	_all
	$(MAKE) -C $(MAIN)
    
_all:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
    done

clean:  mclear
	$(shell rm -rf build)

mclear:	_clean
	$(MAKE) -C $(MAIN) clean

_clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
    done

