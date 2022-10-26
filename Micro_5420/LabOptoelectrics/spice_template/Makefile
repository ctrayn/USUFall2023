#==================================
# Account and Job Information:
#==================================
SRC=ex1.sp

# Uncomment the following line to build graph images:
GRAPHS=true

#=================================
# No edits below this line 
#=================================

YAML=$(wildcard *.yaml)
OUT=$(SRC:.sp=.out)
PDF=$(YAML:.yaml=.pdf)
PNG=$(PDF:.pdf=.png)

RED=\033[0;31m
GR=\033[0;32m
NC=\033[0m

all: run

.PHONY: run
run: FORCE
ifdef GRAPHS
	@printf "\n===================\nMaking graphs\n===================\n\n"
	make clean; make server; make graphs
else
	@printf "\n===================\nNot making graphs\n===================\n\n"
	cd ~/$(JOBDIR)/$(JOBNAME); make clean; make server || true'
endif

.PHONY: server
server: $(OUT) FORCE

graphs: $(PDF) $(PNG) FORCE

%.out: %.sp FORCE
	ngspice -q -b $< -o $@ || true
	-@grep "ERROR" $@; \
	status=$$?; echo $$status; \
	if [ $$status = 0 ]; \
	then echo -e "\n$(RED)===================================\n ERROR see " $@ "\n==================================$(NC)\n";  \
	else echo -e "\n$(GR)===================================\n COMPLETED " $< "\n==================================$(NC)\n";  \
	fi


%.pdf: %.yaml $(OUT)
	pandoc -f markdown -s --template pandoc_pgfplots_template.latex $< -o $@

%.png:  %.pdf $(OUT)
	convert -density 300 $< -quality 90 -sharpen 0x1 $@

.PHONY: test
test:
	mkdir test || true
	cp * test || true	
	make -C test run

clean:
	@rm -Rf test 
	@rm -f *~ 
	@rm -f *.out 
	@rm -f *.dat 
	@rm -f *.pdf 
	@rm -f *.png 

FORCE:
