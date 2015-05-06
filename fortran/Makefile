objects := io.o update.o main.o
fortran: ../FortranHeatTransfer 

../FortranHeatTransfer: FortranHeatTransfer
	cp FortranHeatTransfer ../FortranHeatTransfer
FortranHeatTransfer: $(objects)
	ifort -o FortranHeatTransfer $(objects)
%.o: %.f90
	ifort -c $<
main.o: update_function.mod io_function.mod
update.o: io_function.mod
#Phony targets
.PHONY: clean cleanall cleanresults
cleanall: clean cleanresults
clean:
	@rm -f *.o FortranHeatTransfer *.mod
cleanresults:
	@for i in $$(ls | grep _......dat); do\
	  rm $$i;\
	done
info:
	@echo "Call make fortran or make all in the parent directory
