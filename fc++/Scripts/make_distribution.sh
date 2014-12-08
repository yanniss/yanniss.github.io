#!/usr/local/bin/tcsh -f

if ($#argv != 0 && $#argv != 1) then
    echo $0 \[version number\]
    exit
endif

#You need to create a directory called FC++.[version] and another
#called FC++-clients.[version] underneath the current directory, before
#you run this script.

if ($#argv == 1) then
    set version = .$1
else
    set version = ""
endif
set libdir=FC++$version
set clientdir=FC++-clients$version
if (! -d $libdir || ! -d $clientdir) then
    echo You need to have directories called \"$libdir\" and \"$clientdir\".
    exit
endif

zip -r $libdir.zip $libdir
zip -r $clientdir.zip $clientdir/
tar.lcl -cvf $libdir.tar $libdir
tar.lcl -cvf $clientdir.tar $clientdir
gzip -c $libdir.tar > $libdir.tar.gz
gzip -c $clientdir.tar > $clientdir.tar.gz

set target_dir="~yannis/WWW-personal/fc++/"
cp -rf $libdir $target_dir/
cp -rf $clientdir $target_dir/
mv -f $libdir.zip $target_dir/
mv -f $libdir.tar $target_dir/
mv -f $libdir.tar.gz $target_dir/
mv -f $clientdir.zip $target_dir/
mv -f $clientdir.tar $target_dir/
mv -f $clientdir.tar.gz $target_dir/

# Now create download page
create_table.sh $libdir > $libdir-download.html
create_table.sh $clientdir > $clientdir-download.html

mv -f $libdir-download.html $target_dir/
mv -f $clientdir-download.html $target_dir/

# Set the right permissions for Brian
chgrp -R landes $target_dir/*
chmod -R g+rw $target_dir/*
