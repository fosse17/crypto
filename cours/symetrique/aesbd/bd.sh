#!/bin/bash


nbcols="3"
imgsz="75%"

echo "<html><table width='100%'>" > bd.html


col=0
num=1
for f in *.htm
do
	if [ $col -eq $nbcols ]
	then
		col=0
		echo "</tr><tr>" >> bd.html
	fi
	src=`grep -o -E "src=\"[^\"]*\"" $f`
	img=`echo $src | cut -c13-$((${#src}-5))`
	#wget "$img.png"
	echo "<td><table><tr height='99%'><td><img src='http://${img}.png' width='$imgsz' height='$imgsz'/></td></tr><tr><td><center>$num</center></td></tr></table></td>" >> bd.html
	num=$(($num	+1))
	col=$(($col +1))
done

echo "</tr></table></html>" >> bd.html