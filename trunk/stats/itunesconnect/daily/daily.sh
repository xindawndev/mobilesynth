#!/bin/bash
#
# Script to summarize the output files of the itunes store, and outputs a chart
# url.

cut -f 10,12 S_D* | grep -v Units | awk '{ tot[$2] += $1 } END {  for (s in tot) { total += tot[s]; print s ": " tot[s] } print "Total: " total }'


# Dump a google chart API URL
cut -f 10,12 S_D* | grep -v Units | awk '{ tot[$2] += $1; total += $1 } END {  for (s in tot) { if (tot[s] > max) { max = tot[s]; } } printf "http://chart.apis.google.com/chart?chs=177x100&cht=lc&chd=t:0"; for (s in tot) { x = 100 * tot[s] / max; printf  "," x } printf "\n" }'
