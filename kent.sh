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
 TYPE=""
 if test ! -d "Release/bin" ; then
 	if test ! -d "Debug/bin" ; then
 		echo "Error: no kensho build directory found. Expected to find"
 		echo "Release/bin or Debug/bin in current directory."
 		exit 1
 	else
 		TYPE="Debug"
 		BASEDIR="Debug/bin"
 	fi
 else
 	TYPE="Release"
 	BASEDIR="Release/bin"
 fi
 
 KENSHO="$BASEDIR/kensho"
 
 # try to invoke the executable
 $KENSHO 2>&1 > /dev/null
 
 # if we couldn't invoke the executable bail out with error
 if test $? -ne 0 ; then 
 	echo "Error: couldn't execute $KENSHO. Expected to find executable"
 	echo "kensho in $BASEDIR."
 	exit 1
 fi
 
 if test ! -d "test" ; then
 	echo "Error: no test directory found. Expected to find ./test"
 	echo "in current directory."
 	exit 1
 fi
 
 errCount=0
 testCount=0
 successCount=0
 skipCount=0
 
 echo " Running tests using $TYPE build ($BASEDIR)"
 echo " =========================================================="
 
 for s in `ls ./test/*.ks` ; do
 	OUT=`echo $s | sed -e 's/.ks/.out/g'`
 	testCount=`expr $testCount + 1`
 	FILENAME=`echo $s | sed -e 's/.\/test\///g'`
 	printf ' %02d: %-47s' $testCount $FILENAME
 	if test ! -f $OUT ; then
 		echo "SKIPPED"
 		printf "%59s\n" 'because of missing *.out file'
 		skipCount=`expr $skipCount + 1`
 	else
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
 
 
 
 
 
