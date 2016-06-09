#!/bin/bash

cat < LICENSE > test.txt

cat LICENSE

cat > hello.txt

ls > filelist.txt

ls >> filelist.txt

sort < filelist.txt

sort < filelist.txt > sortedfilelist.txt

cat README.md | tr a-z A-Z

cat < README.md | tr a-z A-Z

cat README.md | tee newOutputFile

tr a-z A-Z > newOutputFile2
