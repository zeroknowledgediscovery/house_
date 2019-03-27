#!/bin/bash

cut -c 2-10 tpid.csv > tpid_.csv
cut -c 2-10 tnid.csv > tnid_.csv
cut -c 2-10 fnid.csv > fnid_.csv
cut -c 2-10 fpid.csv > fpid_.csv

./bin/extract_id -I tpid_.csv -D DX_age5.dat -o tpdb.dat
./bin/extract_id -I fnid_.csv -D DX_age5.dat -o fndb.dat
./bin/extract_id -I fpid_.csv -D DX_age5.dat -o fpdb.dat
./bin/extract_id -I tnid_.csv -D DX_age5.dat -o tndb.dat

./bin/inspect_db  -D tpdb.dat -o aut_tp.txt -a agg_tp.txt -d icd9data2.dat
./bin/inspect_db  -D tndb.dat -o aut_tn.txt -a agg_tn.txt -d icd9data2.dat
./bin/inspect_db  -D fndb.dat -o aut_fn.txt -a agg_fn.txt -d icd9data2.dat
./bin/inspect_db  -D fpdb.dat -o aut_fp.txt -a agg_fp.txt -d icd9data2.dat


