{\rtf1\ansi\ansicpg1252\cocoartf1265\cocoasubrtf210
{\fonttbl\f0\fswiss\fcharset0 Helvetica;}
{\colortbl;\red255\green255\blue255;}
\margl1440\margr1440\vieww10380\viewh7200\viewkind0
\pard\tx720\tx1440\tx2160\tx2880\tx3600\tx4320\tx5040\tx5760\tx6480\tx7200\tx7920\tx8640\pardirnatural

\f0\fs24 \cf0 # Gnuplot script for true nn\
set title "CIFAR True NN Percentage"\
set xlabel "Subdomain Size"\
set ylabel "True NN Percentage"\
plot "bsp_spill_tree.dat" every ::1::4 u 5:4 title 'bsp_spill alpha 0.05' w linespoints ,\\\
"bsp_spill_tree.dat" every ::5::8 u 5:4 title 'bsp_spill alpha 0.1' w linespoints ,\\\
"bsp_tree.dat" u 4:3 title 'bsp_tree' w linespoints ,\\\
"kd_spill_tree.dat" every ::1::4 u 5:4 title 'kd_spill alpha 0.05' w linespoints ,\\\
"kd_spill_tree.dat" every ::5::8 u 5:4 title 'kd_spill alpha 0.1' w linespoints ,\\\
"kd_tree.dat" u 4:3 title 'kd_tree' w linespoints ,\\\
"kd_v_spill_tree.dat" every ::1::4 u 5:4 title 'kd_virtual_spill alpha 0.05' w linespoints ,\\\
"kd_v_spill_tree.dat" every ::5::8 u 5:4 title 'kd_virtual_spill alpha 0.1' w linespoints}