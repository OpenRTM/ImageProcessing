#!/bin/sh

unused="ncb suo user obj dll exe old lib pyc"

for u in $unused; do
    find ./ -name "*.$u" -exec rm {} \;
done

find ./ -name '*~' -exec rm {} \;

rm -f rtm/idl/*.hh
rm -f rtm/idl/*.cc
rm -f rtm/idl/*Skel.*
rm -f rtm/idl/*Stub.*
rm -rf rtm/idl/Debug
rm -rf rtm/idl/Release


