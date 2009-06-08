#!/bin/bash
#
# Script to summarize the output files of the itunes store, and outputs a chart
# url

cut -f 10,12 S_W* | grep -v Units | perl -ane '@F = split /\s+/; $tot{$F[1]} += int($F[0]); END { foreach $s (sort keys %tot) { $total += $tot{$s}; print "$s: $tot{$s}\n"; } print "Total: $total\n"; }'


# Dump a google chart API URL
cut -f 10,12 S_W* | grep -v Units | perl -ane '@F = split /\s+/; $tot{$F[1]} += int($F[0]); END { $max = 0; foreach $s (sort keys %tot) { if ($tot{$s} > $max) { $max = $tot{$s}; } } print "http://chart.apis.google.com/chart?chs=177x100&cht=lc&chd=t:0"; foreach $s (sort keys %tot) { $x = 100 * $tot{$s} / $max; printf(",%0.0d", $x); } print "\n"; }'
