# ensure the number of argument is not zero
# if it is zero, print the usage and exit
if [ $# -eq 0 ]
then
    echo "Usage: $0 <filename>"
    exit 1
fi

# check if the file exists

if [ ! -f $1 ]
then
    echo "File $1 does not exist"
    exit 2
fi

# check if the file is readable

if [ ! -r $1 ]
then
    echo "File $1 is not readable"
    exit 3
fi

# check if the file ends with .cpp extension

if [[ $1 != *.cpp ]]
then
    echo "File $1 does not end with .cpp extension"
    exit 4
fi

# remove the .cpp extension from the file name
# and store it in a variable

filename=$(echo $1 | cut -d'.' -f1)

# for all the files passed to the script
gpp_args=""
for arg in $@
do
    gpp_args="$gpp_args $arg"
done

# compile the file
g++ $gpp_args -o $filename.out -lGL -lGLU -lglut

# run the file
./$filename.out

# remove the executable
rm $filename.out