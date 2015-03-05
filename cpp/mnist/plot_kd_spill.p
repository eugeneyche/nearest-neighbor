#plot data

set title "CIFAR True NN Percentage (KD Spill Tree)"
set xlabel "Search Size"
#font "Helvetica,20"
set ylabel "Fraction Correct NN"
#font "Helvetica,20"
#set xtics font "Helvetica,20"
#set ytics font "Helvetica,20"
#set key font "Helvetica,20"

plot "kd_spill_tree_copy.dat" every 9::1::54 using 5:4 title 'alpha 0.05' w linespoints ,\
     "" every 9::2::55 u 5:4 title 'alpha 0.07' w linespoints ,\
     "" every 9::3::56 u 5:4 title 'alpha 0.10' w linespoints ,\
     "" every 9::4::57 u 5:4 title 'alpha 0.12' w linespoints ,\
     "" every 9::5::58 u 5:4 title 'alpha 0.15' w linespoints ,\
     "" every 9::6::59 u 5:4 title 'alpha 0.17' w linespoints ,\
     "" every 9::7::60 u 5:4 title 'alpha 0.19' w linespoints ,\
     "" every 9::8::61 u 5:4 title 'alpha 0.21' w linespoints ,\
     "" every 9::9::62 u 5:4 title 'alpha 0.23' w linespoints
