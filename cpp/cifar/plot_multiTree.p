#plot kd_spill_tree data
set title "CIFAR True NN Percentage (Multiple Tree)"
set xlabel "Search Size"
#font "Helvetica,20"
set ylabel "Fraction Correct NN"
#font "Helvetica,20"
#set xtics font "Helvetica,20"
#set ytics font "Helvetica,20"
#set key font "Helvetica,20"
plot "kd_tree.dat" u 4:3 title 'KD Tree' w linespoints ,\
	 "multi_kd_tree_0.8exclusive.dat" u 4:3 title 'Top 0.8 Max Variance Exclusive Trees' w linespoints ,\
	 "multi_kd_tree_0.8.dat" u 4:3 title 'Top 0.8 Max Variance Inclusive Trees' w linespoints ,\
	 "multi_kd_tree_0.9.dat" u 4:3 title 'Top 0.9 Max Variance  Trees' w linespoints ,\
 	 "multi_kd_tree_first3.dat" u 4:3 title 'First 3 Max Variance Trees' w linespoints ,\
	 "multi_kd_tree_first4.dat" u 4:3 title 'First 4 Max Variance Trees' w linespoints ,\
	 "multi_kd_tree_first5.dat" u 4:3 title 'First 5 Max Variance Trees' w linespoints