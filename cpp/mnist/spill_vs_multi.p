#plot multi_tree and spill_tree data
set title "CIFAR True NN Percentage"# (Multiple Tree VS. Spill Tree)
set xlabel "Search Size"
#font "Helvetica,20"
set ylabel "Fraction Correct NN"
#font "Helvetica,20"
#set xtics font "Helvetica,20"
#set ytics font "Helvetica,20"
#set key font "Helvetica,20"
plot "kd_spill_tree_copy.dat" every 9::3::56 using 5:4 title 'Spill Tree alpha 0.10' w linespoints ,\
	 "kd_v_spill_tree_copy.dat" every 9::1::54 using 5:4 title 'Virtual Spill Tree alpha 0.10' w linespoints ,\
     "multi_kd_tree_first4.dat" u 4:3 title 'Multiple Trees' w linespoints
