#plot kd_tree, kd_spill_tree and kd_virtual_spill_tree 
set term aqua font "Helvetica,25"
set title "CIFAR  True NN Percentage" offset 0, -1
set xlabel "# of distance computations" offset 0, 1.3
set ylabel "Fraction Correct NN" offset 2, 0
set xtics offset 0, 0.5
set yrange [0.35:0.85]
set xrange [0:12000]
plot "kd_spill_tree_edit.dat" every 9::3::56 u 5:4 title 'spill alpha 0.1' w linespoints lw 3,\
"kd_v_spill_tree_edit.dat" every 9::3::56 u 5:4 title 'virtual spill alpha 0.1' w linespoints lw 3