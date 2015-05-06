all: c fortran
#C program compiling
.PHONY: c
c:
	$(MAKE) -C c/
#Fortran program compiling
.PHONY: fortran
fortran: 
	$(MAKE) -C fortran/
#gnuplot pgn files here
figures:
	@for i in $$(ls | grep _......dat); do\
	  t=$${i%.dat};\
	  gnuplot -e "input_file='$$i'; output_file='$${i%.dat}.png'; time=$${t: -4}" gplot;\
	done

#Phony targets
.PHONY: cleanall clean cleanresults info cleanfigures
cleanall: clean cleanresults cleanfigures
clean:
	@rm -f CHeatTransfer FortranHeatTransfer
	$(MAKE) -C c/ clean
	$(MAKE) -C fortran/ clean
cleanfigures:
	@rm -f *.png
cleanresults:
	@for i in $$(ls | grep _......dat); do\
	  rm $$i;\
	done
info:
	@echo "make all: bulids both C and Fortran programs"
	@echo "make c: builds only C program"
	@echo "make fortran: builds only Fortran program"
	@echo "make clean: removes all executable files, object files, and module files"
	@echo "make cleanresults: removes result files"
	@echo "make cleanfigures: removes all .png files"
	@echo "make figres: runs .gplot file using name of results file as input and create .png files"
