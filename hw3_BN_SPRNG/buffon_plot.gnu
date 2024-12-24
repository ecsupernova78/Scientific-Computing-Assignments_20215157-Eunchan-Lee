set terminal pngcairo size 1200,600
set output 'buffon_results.png'
set multiplot layout 1,2
set logscale x
set xlabel 'Number of needles (log scale)'
set ylabel 'Estimated pi'
set title 'Estimated pi as n values'
plot 'buffon_data.txt' using 1:2 with linespoints title 'Estimated pi', \
     3.141592653589793 title 'Real pi value (¥ð)' with lines lw 2
set logscale x
set logscale y
set xlabel 'Number of needles (log scale)'
set ylabel 'Error (log scale)'
set title 'Error as n values'
f(x) = a * x + b
fit f(x) 'buffon_data.txt' using (log10($1)):(log10($3)) via a, b
plot 'buffon_data.txt' using 1:3 with linespoints title 'Error', \
     10**(f(log10(x))) title sprintf('Fit: Slope = %0.4f', a) with lines lw 2
unset multiplot
