#!/bin/bash

Nowtime=`date +"%Y-%m-%d_%H:%M:%S"`

eval $(df -m | grep "dev" | tail -n +2 |\
  awk 'BEGIN{sumsum=0; sumused=0;} \
  {printf("pername["NR"]=%s   persum["NR"]=%d perused["NR"]=%d  percent["NR"]=%s ",$1,$2,$4,$5);\
   sumsum=sumsum+$2;sumused=sumused+$4}\
   END {printf("amt=%d sumsum=%d sumused=%d",NR,sumsum,sumused)}')
  
 for (( i = 1; i < amt; i++ ))do
 	echo "$Nowtime 1 ${pername[$i]} ${persum[$i]} ${perused[$i]} ${percent[$i]}" 
done
sumper=$[ (100-sumused*100 /sumsum) ]	
echo "$Nowtime 0 disk $sumsum $sumused ${sumper}%
