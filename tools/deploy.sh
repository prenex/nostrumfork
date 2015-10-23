#!/bin/sh
DIR=`pwd`
cd `dirname $0`/..
make clean
tar cfz nosgi.tar.gz --exclude tools --exclude nosgi.tar.gz --exclude-vcs *
cd $DIR
