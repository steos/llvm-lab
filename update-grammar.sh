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

 # verify that we have enough args
 if test $# -ne 4 ; then
	echo "missing arguments"
	exit 1
 fi
 
 GRAMMAR_SRC="$1"
 GRAMMAR_OUT="$2"
 GRAMMAR_DST_C="$3"
 GRAMMAR_DST_H="$4"
 CWD=`pwd`
 
 SRCDIR=`dirname $GRAMMAR_SRC`;
 
 if test ! -d $SRCDIR ; then
 	echo "Error: $SRCDIR is no directory"
 	exit 1
 fi
 if test ! -d $GRAMMAR_DST_C ; then
 	echo "Error: $GRAMMAR_DST_C is no directory"
 	exit 1
 fi
 if test ! -d $GRAMMAR_DST_H ; then
 	echo "Error: $GRAMMAR_DST_H is no directory"
 	exit 1
 fi
 
 # remove temporary directory if it exists
 if test -d $GRAMMAR_OUT ; then
 	echo "removing output directory $GRAMMAR_OUT"
 	rm -rf $GRAMMAR_OUT
 fi
 
 # create temporary directory and antlr version info
 mkdir $GRAMMAR_OUT
 touch "$GRAMMAR_OUT/antlr.version"
 
 # check if we can execute antlr
 antlr -version > "$GRAMMAR_OUT/antlr.version" 2>&1
 if test $? -ne 0 ; then
 	echo "Error: ANTLR was not found on the path"
 	rm -rf "$GRAMMAR_OUT"
 	exit 1
 else
 	# verify that we have the correct antlr version
 	head -n 1 "$GRAMMAR_OUT/antlr.version" | egrep '3\.1\.[0-9]' > /dev/null
 	if test $? -ne 0 ; then
 		echo "ANTLR was not detected as being version 3.1.x"
 		echo "ANTLR 3.1.x is required to regenerate the parser"
 		rm -rf "$GRAMMAR_OUT"
 		exit 1
 	else
 		# (re)generate parser
 		echo -n "regenerating grammar with ANTLR 3.1.x ... "
 		antlr -o $GRAMMAR_OUT $GRAMMAR_SRC > "$GRAMMAR_OUT/antlr.log" 2>&1
 		if test $? -ne 0; then
 			echo "ERROR"
 			cat "$GRAMMAR_OUT/antlr.log"
 			rm -rf "$GRAMMAR_OUT"
 			exit 1
 		fi
 		
 		# rename *.c to *.cc 
 		for s in `ls $GRAMMAR_OUT/*.c` ; do
 			mv "$s" "${s}c"
		done
		
		# move generated files to their destination
		mv $GRAMMAR_OUT/*.cc "$GRAMMAR_DST_C"
		mv $GRAMMAR_OUT/*.h "$GRAMMAR_DST_H"
		
		# cleanup
		rm -rf "$GRAMMAR_OUT"
		
		echo "OK"
		exit 0
 	fi
 fi