#plot kd_tree and kd_spill_tree 
set term aqua font "Helvetica,25"
set title "CIFAR  True NN Percentage" offset 0, -1
set xlabel "# of distance computations" offset 0, 1.2
set ylabel "Fraction Correct NN" offset 2, 0
set xtics offset 0, 0.5
set label font "Helvetica,25"
set xrange [0:10000]
set yrange [0.3:0.85]
plot "kd_tree.dat" using 4:3 title 'KD Tree' w linespoints lw 3,\
	 "kd_spill_tree_edit.dat" every 9::1::54 using 5:($4+0.005):6 with labels notitle, "" every 9::1::54 u 5:4 title 'Spill alpha 0.05' w linespoints lw 3 ,\
	 "kd_spill_tree_edit.dat" every 9::3::56 using 5:($4+0.005):6 with labels notitle, "" every 9::3::56 u 5:4 title 'Spill alpha 0.1' w linespoints lw 3