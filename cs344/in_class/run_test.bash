#!/bin/bash

HFLAG=0
EFLAG=0
SFLAG=0
VFLAG=0
SARG=1
EARG=10

function show_help {
	current_date=`date`
	echo Todays date is: ${current_date}
	exit
}

while getopts 'he:s:v' c
do
  case $c in
    h) HFLAG=1;;
    e) EFLAG=1
	EARG=$OPTARG 
	;;
    s) SFLAG=1
	SARG=$OPTARG 
	;;
    v) VFLAG=1 ;;
  esac
done


if [ ${EFLAG} -eq 0 ] && [ ${SFLAG} -eq 0 ]
then
	show_help
fi

if [ ${EARG} -lt ${SARG} ]
then
	echo Error: start value less than end value
	exit 1
fi

if [ ${EARG} -lt 1 ]
then
	${EARG}=10
fi

if [ ${SARG} -lt 1 ]
then
	${SARG}=1
fi

if [ ${HFLAG} -eq 1 ]
then
    	show_help
fi

if [ $VFLAG -eq 1 ]
then
        echo "start= ${SARG}     end= ${EARG}"
	echo "    "
fi

for i in `seq ${SARG} ${EARG}`
do
	./foo ${i}
	RET_VAL=$?
	case ${RET_VAL} in
		0 ) echo ${i} was a  success 
		;;
		1 )if [ $VFLAG -eq 1 ]
		then  
			echo ${i} returned exit status 1
		fi 
		;;
		2 )if [ $VFLAG -eq 1 ]
                then
                        echo ${i} returned exit status 2
                fi
                ;;
		3 )if [ $VFLAG -eq 1 ]
                then
                        echo ${i} returned exit status 3
                fi
                ;;
	esac
done
