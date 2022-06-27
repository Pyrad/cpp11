#!/bin/bash -

BOOST_LIB_DIR="/d/procs/boost_1_79_0/lib"
#[[ ! -d $BOOST_LIB_DIR ]] && echo "Not a valid path: $BOOST_LIB_DIR" && exit 20
[[ ! -d $BOOST_LIB_DIR ]] && exit 20

flist=(`find $BOOST_LIB_DIR -maxdepth 1 -type f -name "libboost_*"`)
#echo "${#flist[@]}"
ver_str=""
dstr="-d"
for f in ${flist[@]}; do
	fn=`basename $f`
	idx_minus=`expr index "$fn" "-"`
	idx_dot=`expr index "$fn" "."`
	len=`expr $idx_dot - $idx_minus`
	if [[ $len -gt 0 ]] && [[ $idx_minus -gt 0 ]]; then
		idx_minus=`expr $idx_minus - 1`
	else
		ver_str=""
		break
	fi
	#echo $idx_minus $idx_dot
	ver_str=${fn:$idx_minus:$len}
	#echo "ver_str=$ver_str"
	[[ "$ver_str" == *"$dstr"* ]] && continue
	# Find version string without "-d"
	break
done

# Return value is the ver_str
echo "$ver_str"

exit 0

