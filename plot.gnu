set xlabel "Heuristic value h(g)" font "Calibri, 20"
set ylabel "Path length" font "Calibri, 20"
set title "Path length for h(g)" font "Calibri, 20"
plot "data.txt" using 1:2 lw 2 with linespoints title ""
