#plot kd_spill_tree data
set title "CIFAR True NN Percentage (KD Virtual Spill Tree)"
set xlabel "Search Size"
#font "Helvetica,20"
set ylabel "Fraction Correct NN"
#font "Helvetica,20"
#set xtics font "Helvetica,20"
#set ytics font "Helvetica,20"
#set key font "Helvetica,20"
plot "kd_v_spill_tree_edit.dat" every 9::1::54 using 5:($4+0.005):6 with labels title 'alpha 0.05', "" every 9::1::54 u 5:4 notitle w linespoints ,\
     "" every 9::2::55 using 5:($4+0.005):6 w labels title 'alpha 0.07', "" every 9::2::55 u 5:4 notitle w linespoints ,\
     "" every 9::3::56 using 5:($4+0.005):6 w labels title 'alpha 0.10', "" every 9::3::56 u 5:4 notitle w linespoints ,\
     "" every 9::4::57 using 5:($4+0.005):6 w labels title 'alpha 0.12', "" every 9::4::57 u 5:4 notitle w linespoints ,\
     "" every 9::5::58 using 5:($4+0.005):6 w labels title 'alpha 0.15', "" every 9::5::58 u 5:4 notitle w linespoints ,\
     "" every 9::6::59 using 5:($4+0.005):6 w labels title 'alpha 0.17', "" every 9::6::59 u 5:4 notitle w linespoints ,\
     "" every 9::7::60 using 5:($4+0.005):6 w labels title 'alpha 0.19', "" every 9::7::60 u 5:4 notitle w linespoints ,\
     "" every 9::8::61 using 5:($4+0.005):6 w labels title 'alpha 0.21', "" every 9::8::61 u 5:4 notitle w linespoints ,\
     "" every 9::9::62 using 5:($4+0.005):6 w labels title 'alpha 0.23', "" every 9::9::62 u 5:4 notitle w linespoints
