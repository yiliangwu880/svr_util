#!/bin/sh

#SVN_VERSION=`svnversion -c |sed 's/^.*://' |sed 's/[A-Z]*$//'`  
#SVN_VERSION=`svn info ${Ŀ¼} | grep Revision | awk -F' ' '{ print $2 }'`

if  [ "$SVN_VERSION" = "Unversioned directory" ]
then  
    SVN_VERSION=0
fi

echo $SVN_VERSION
cd Debug
cmake -DCMAKE_BUILD_TYPE=Debug -DVERSION_REVISION=$SVN_VERSION  ..
make -j9
