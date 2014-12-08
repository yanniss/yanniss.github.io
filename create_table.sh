#!/usr/local/bin/tcsh -f

#Given a directory, create an HTML table with pointers to each file within
#the directory. Used for making FC++ download pages.

if ($#argv != 1 && $#argv != 2) then
    echo $0 \<directory pathname\> \[number of columns\]
    exit
endif

set dir = $1
if ($#argv == 2) then
    set columns = $2
else
    set columns = 5
endif

cd $dir
set files = `ls`
set column = 0

echo \<center\>\<Table Border\>
foreach file ($files)
    if ($column == 0) then
        echo \<TR\>
    endif
    echo \<TD\>\<A Href=\"$dir/$file\"\>$file\<\/A\>\<\/TD\>
    @ column++
    if ($column == $columns) then
        set column = 0
        echo \<\/TR\>
    endif
end
if ($column != 0) then
   echo \<\/TR\>
endif

echo \<\/Table\>\<\/center\>
