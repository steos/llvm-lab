#!/bin/sh
# This file is part of Kensho.
# Kensho is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# Kensho is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with Kensho.  If not, see <http://www.gnu.org/licenses/>.

 BASEDIR="" 
 
 if test ! -d "Release/bin" ; then
 	if test ! -d "Debug/bin" ; then
 		echo "no kensho build directory found"
 		exit 1
 	else
 		echo "Info: using Debug build"
 		BASEDIR="Debug/bin"
 	fi
 else
 	echo "Info: using Release build"
 	BASEDIR="Release/bin"
 fi
 
 KENSHO="$BASEDIR/kensho"
 
 if test ! -d "test" ; then
 	echo "no test directory found"
 	exit 1
 fi
 
 for s in `ls ./test/*.ks` ; do
 	echo -n "Running $s ... "
  	OUT=`echo $s | sed -e 's/.ks/.out/g'`
  	TMP=`echo $s | sed -e 's/.ks/.tmp/g'`
  	DIFFTMP="$TMP.diff"
 	$KENSHO $s > $TMP 2>&1
 	diff $TMP $OUT > $DIFFTMP 2>&1
 	if test $? -ne 0 ; then
 		echo "ERROR"
 		echo "Expected output didn't match:"
 		cat $DIFFTMP
 	else
 		echo "OK"
 	fi
 	rm $DIFFTMP
 	rm $TMP
 done
 
 
 
 
 