#!/bin/bash

id=40947030S

if [ -z ${1+x} ]; then
	echo "homework to zip is unset."
	exit
fi
ln -s hw0$1 ${id}_hw0$1
zip -r ${id}_hw0$1.zip ${id}_hw0$1 -x "*.DS_Store"
rm ${id}_hw0$1