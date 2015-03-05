#plot pca_tree and pca_spill_tree 
set term aqua font "Helvetica,25"
set title "CIFAR  True NN Percentage" offset 0, -1
set xlabel "# of distance computations" offset 0, 1.2
set ylabel "Fraction Correct NN" offset 2, 0
set xtics offset 0, 0.5
set label font "Helvetica,25"
set xrange [0:10000]
set yrange [0.3:0.85]
plot "pca_tree.dat" using 4:3 title 'PCA Tree' w linespoints lw 3,\
	 "pca_spill_tree_edit.dat" every ::1::10 using 5:4 title 'Spill alpha 0.05' w linespoints lw 3 ,\
	 "pca_spill_tree_edit.dat" every ::11::20 using 5:4 title 'Spill alpha 0.1' w linespoints lw 3