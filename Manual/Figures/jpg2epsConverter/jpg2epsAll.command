#!/bin/bash
cd `dirname $0`

for x in *.jpg
do
echo "Converting ${x}....."
convert -geometry 600x400 -quality 90 "${x}" eps2:"${x%.jpg}.eps"
done

for x in *.eps
do
mv $x ../eps/
done

