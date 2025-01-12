#!/bin/bash

# Input data file
datafile=$1
yamlfile=$2

# Check if the input file is provided
if [ -z "$datafile" ] || [ -z "$yamlfile" ]; then
    echo "Usage: $0 <datafile> <yamlfile>"
    exit 1
fi

# Check if the file exists
if [ ! -f "$datafile" ]; then
	    echo "Data file '$datafile' not found!"
	        exit 1
fi

if [ ! -f "$yamlfile" ]; then
      echo "Yaml file '$yamlfile' not found!"
          exit 1
fi

# Number of rows per block
blocksize=$(grep 'n-fish' "$yamlfile" | awk -F': ' '{print $2}')

# default speed
default_speed=$(grep 'vel-standard' "$yamlfile" | awk -F': ' '{print $2}')

# Length
length=$(grep '^[ \t]*length' "$yamlfile" | awk -F': ' '{print $2}')

# delta t
delta_t=$(grep '^[ \t]*delta-t' "$yamlfile" | awk -F': ' '{print $2}')

# Snapshot interval
snapshot_interval=$(grep '^[ \t]*snapshot-interval' "$yamlfile" | awk -F': ' '{print $2}')

# Number of parallel tasks
N=32

# If frames directory does not exist, create it
if [ ! -d "frames" ]; then
      mkdir frames
fi


# Calculate total lines in the file
total_lines=$(wc -l < "$datafile")

# Calculate total number of blocks
total_blocks=$(( total_lines / blocksize ))

echo "Total lines: $total_lines"
echo "Processing $total_blocks blocks of $blocksize rows each..."

# Generate a Gnuplot script dynamically
gnuplot_script="plot_blocks.gp"
cat <<EOF > $gnuplot_script
blocksize = $blocksize
total_blocks = $total_blocks

# Loop over each block
set terminal png size 800,600
set output sprintf("frames/plot_block_%05d.png", block)
# set title sprintf("Block %d: Rows %d–%d", block, block*blocksize, (block+1)*blocksize-1)
set view 60,30
set xrange [0:$length]
set yrange [0:$length]
set zrange [0:$length]

set ticslevel 0.0

set size ratio -1

default_speed = $default_speed

# Convert to HSL color space, map velocity to range [0,1]
logistic(x) = 1.0 / (1.0 + exp(-x))
hue(vx, vy, vz) = atan2(vy, vx) / (2.0 * pi) + 0.5
saturation(vx, vy, vz) = logistic(sqrt(vx**2 + vy**2 + vz**2) / (sqrt(3 * default_speed**2)))
lightness(vx, vy, vz) = logistic(vz / default_speed)


#plot "output.txt" every ::block*blocksize::(block+1)*blocksize-1 using 1:2 title sprintf("t = %d", block)
splot "output.txt" \
    every ::block*blocksize::(block+1)*blocksize-1 \
    using 1:2:3:(hsv2rgb(hue(\$4, \$5, \$6), saturation(\$4, \$5, \$6), lightness(\$4, \$5, \$6))) \
    title sprintf("t = %.2f", block * $delta_t * $snapshot_interval) \
    with points pointtype 7 \
    lc rgb variable
EOF

# Run Gnuplot
# gnuplot $gnuplot_script

for i in $(seq 0 $((total_blocks-1))); do
  ((j=j%N)); ((j++==0)) && wait
   gnuplot -e "block=$i" plot_blocks.gp &
   echo "Plotting block $i..."
done

# Wait for all the background jobs to finish
wait
echo "All done!"

#output mp4
echo "Creating video..."
ffmpeg -y -framerate 24 -i frames/plot_block_%05d.png -c:v libx264 -pix_fmt yuv420p video.mp4