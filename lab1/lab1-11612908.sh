#!/bin/bash

given_root=$1
given_root=`cd $given_root; pwd`"/"
# echo $given_root

cat /dev/null > $2
saveIFS=$IFS
IFS=$'\t\b\n'
tot_d=0
tot_f=0
queue=("$given_root")
head_idx=0
tail_idx=1
while [ ${#queue[@]} -ne 0 ]
do
	curr_root=${queue[$head_idx]}
	unset queue[$head_idx]
	echo "[`basename $curr_root`]" >> $2
	for itm in $(ls $curr_root)
	do
		p=`cd $curr_root; pwd`"/"$itm
		echo "$p" >> $2
		if [ -d "$p" ]
		then
			queue[tail_idx]=$p
			((tot_d++))
			((tail_idx++))
		else
			((tot_f++))
		fi
	done
	((head_idx++))
	echo "" >> $2
done
IFS=$saveIFS

echo "[Directories Count]:$tot_d" >> $2
echo "[Files Count]:$tot_f" >> $2

