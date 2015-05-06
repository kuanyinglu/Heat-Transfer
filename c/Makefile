objects := update.o io.o main.o
c: ../CHeatTransfer

../CHeatTransfer: CHeatTransfer
	cp CHeatTransfer ../CHeatTransfer
CHeatTransfer: $(objects)
	icc -o CHeatTransfer $(objects)
%.o: %.c
	icc -c $<
main.o: update.h io.h
update.o: update.h io.h
io.o: update.h io.h
#gnuplot pgn files here
figures:
	@for i in $$(ls | grep _.....dat); do\
	  t=$${i%.dat};\
	  gnuplot -e "input_file='$$i'; output_file='$${i%.dat}.png'; time=$${t: -4}" hw5.gplot;\
	done

#Phony targets
.PHONY: cleanall clean cleanresults
cleanall: clean cleanresults
clean:
	@rm -f *.o CHeatTransfer
cleanresults:
	@for i in $$(ls | grep _......dat); do\
	  rm $$i;\
	done
info:
	@echo "Call make c or make all in the parent directory
