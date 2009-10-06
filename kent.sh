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
 		echo " no kensho build directory found"
 		exit 1
 	else
 		echo " Info: using Debug build"
 		BASEDIR="Debug/bin"
 	fi
 else
 	echo " Info: using Release build"
 	BASEDIR="Release/bin"
 fi
 
 KENSHO="$BASEDIR/kensho"
 
 if test ! -d "test" ; then
 	echo " no test directory found"
 	exit 1
 fi
 
 errCount=0
 testCount=0
 successCount=0
 skipCount=0
 
 echo " =========================================================="
 
 for s in `ls ./test/*.ks` ; do
 	OUT=`echo $s | sed -e 's/.ks/.out/g'`
 	if test ! -f $OUT ; then
 		echo " Skipping $s because of missing *.out file"
 		skipCount=`expr $skipCount + 1`
 	else
 		testCount=`expr $testCount + 1`
 		printf ' Running %-43s' $s
	  	TMP=`echo $s | sed -e 's/.ks/.tmp/g'`
	  	DIFFTMP="$TMP.diff"
	 	$KENSHO $s > $TMP 2>&1
	 	# we're using --strip-trailing-cr to make the diffs
	 	# work with mingw/msys because the kensho programs
	 	# only ever output LF line endings but msys/mingw
	 	# converts them to CRLF.
	 	diff -T --strip-trailing-cr $TMP $OUT > $DIFFTMP 2>&1
		EXIT_STATUS=$?
	 	if test $EXIT_STATUS -ne "0" ; then
	 		errCount=`expr $errCount + 1`
	 		echo "FAILURE"
	 		echo " diff returned $EXIT_STATUS, expected output didn't match:"
	 		echo " -----------------------------"
	 		cat $DIFFTMP
	 		echo " -----------------------------"
	 	else
	 		successCount=`expr $successCount + 1`
	 		echo "SUCCESS"
	 	fi
	 	rm $DIFFTMP
	 	rm $TMP
	 fi
 done
 
 echo " =========================================================="
 echo ""
 echo " Skipped: $skipCount"
 echo " Success: $successCount"
 echo " Failure: $errCount"
 
 if [ "$errCount" = "0" ] ; then
 	echo ""
 	echo " Congratulations, all tests passed!"
 fi
 
 
 
 
 