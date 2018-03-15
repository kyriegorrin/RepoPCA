#!/bin/bash

#Arguments
ORIGINAL=$1
OPTIMIZED=$2
NMIN=$3
NMAX=$4
NSTEP=$5
NEXEC=$6

#Variables de noms executables i sortides
OUTPUT_NAME="executions.dat"
BIN_NAME_ORIGINAL="${ORIGINAL%.c}" 
BIN_NAME_OPTIM="${OPTIMIZED%.c}"

if [ $# -ne 6 ]
    then 
    echo "No Arguments"
    exit 1
fi

#-------------------DEPRECATED---------------------#
#Generating executables
#echo "Compiling executables..."
#gcc $ORIGINAL -O0 -o ${BIN_NAME_ORIGINAL}.O0 
#gcc $ORIGINAL -O1 -o ${BIN_NAME_ORIGINAL}.O1
#gcc $ORIGINAL -O3 -march=native -o ${BIN_NAME_ORIGINAL}.O3
#--------------------------------------------------#

echo "Executing and comparing versions for correctness..."
echo "NOTE: We are not comparing for all the N values for now, just for N=10000."
echo -e "Right now it would take too much time. To be changed in further revisions.\n"
./$ORIGINAL > $ORIGINAL.out
./$OPTIMIZED > $OPTIMIZED.out

#Comprovacions inicials simplificades (temporal, perdem molt de temps si no)
if diff $ORIGINAL.out $OPTIMIZED.out > /dev/null; then 
	echo -e "$ORIGINAL and $OPTIMIZED have the same output.\n"
fi

#Eliminem antic arxiu de sortida si existeix
if [ -f $OUTPUT_NAME ]; then
	rm $OUTPUT_NAME
fi

#Calculs de temps per cada nombre de decimals
#NOTA IMPORTANT: ara per ara només funciona amb execucions no superiors a 1 minut
CONTADOR=$NMIN
AVG_ELAPSED=0
TOTAL_TIME=0

while [ $CONTADOR -le $NMAX ]; do
	for (( i = 0; i < ${NEXEC}; i++ )); do
		echo "Num decimals: $CONTADOR, Execution: $i"
		TIME=$( { /usr/bin/time -f "%E" ./$OPTIMIZED $CONTADOR > /dev/null; } 2>&1 )
		TIME=${TIME#*:}
		TOTAL_TIME=$(bc <<< "scale=4;$TIME+$TOTAL_TIME")
	       	echo "$TIME"	
	done
		
	#Arreglem els valors inferiors a 1 (no printa el 0 a l'esquerra)
	AVG_ELAPSED=$(bc <<< "scale=4;$TOTAL_TIME/$NEXEC" | awk '{printf "%.4f", $0}')
	echo -e "Total time: $TOTAL_TIME  Average time: $AVG_ELAPSED \n"
	
	#Passem valors al fitxer output
	echo "$CONTADOR $AVG_ELAPSED" >> $OUTPUT_NAME

	TOTAL_TIME=0
	AVG_ELAPSED=0

	let CONTADOR+=$NSTEP
done

echo -e "SCRIPT FINISHED. CHECK RESULTS IN $OUTPUT_NAME \n"

#---------TEMPORAL PLOT GENERATION, TO BE IMPROVED------------#
echo -e "Starting plot generation..."
#FIX TEMPORAL. Posa que el nom del plot sigui $OUTPUT_NAME quan puguis/sapiguis fer-ho
gnuplot -p -e 'plot "executions.dat" with linespoints; q'
