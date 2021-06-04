#!/bin/bash

id=40947030S

if [ -z ${1+x} ]; then
	echo "homework to zip is unset."
	exit
fi

origin_dir=$1
dest_name=${id}_$1

ln -s $origin_dir $dest_name
zip -r ${dest_name}.zip ${dest_name} -x "*.DS_Store" "*.gitignore" -x $([ -r ${dest_name}/.gitignore ] && cat ${dest_name}/.gitignore | sed -E 's/#.*$//g' | perl -pe 's/^(?!\*)/\*/g' | sed -E "s/(.+)\/\*\*/\1\*\n\1\*/g") -x $([ -r .gitignore ] && cat .gitignore | sed -E 's/#.*$//g' | perl -pe 's/^(?!\*)/\*/g' | sed -E "s/(.+)\/\*\*/\1\*\n\1\*/g")
rm $dest_name