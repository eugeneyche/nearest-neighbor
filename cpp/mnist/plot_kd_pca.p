#plot pca_tree and kd_tree
reset
set term aqua font "Helvetica,25"
set title "MNIST True NN Percentage" offset 0, -1
set xlabel "# of distance computations" offset 0, 1.3
set ylabel "Fraction Correct NN" offset 2, 0
set xtics offset 0, 0.5
set xrange [0:12000]
set output "mnist_kd_pca.pdf"
plot "kd_tree.dat" using 4:3 title 'KD Tree' w linespoints lw 3,\
	 "bsp_tree.dat" using 4:3 title 'PCA Tree' w linespoints lw 3
