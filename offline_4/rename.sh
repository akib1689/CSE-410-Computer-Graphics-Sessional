# find all the cpp and hpp file in the current directory and rename them
# to 1805086_filename.cpp and 1805086_filename.hpp

# get the current directory
dir=$(pwd)

# get the cpp and hpp file
cpp=$(ls *.cpp)
hpp=$(ls *.hpp)

# rename the cpp file
for file in $cpp
do
    mv $file 1805086_$file
done

# rename the hpp file
for file in $hpp
do
    mv $file 1805086_$file
done