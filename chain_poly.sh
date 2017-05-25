#!/bin/bash

### Check number of parameters.

# There are fewer than 2 parameters.
if (( $# < 2 )); then
	echo "$0: Script was executed with too few parameters."
	exit 1

# There are 2 parameters.
elif (( $# == 2 )); then
	program="$1"
	directory="$2"

# There are more than 2 parameters.
else
	echo "$0: Script was executed with too many parameters."
	exit 1
fi

### Check if program is an executable file.

# Program doesn't exist.
if [ ! -f "$program" ]; then
	echo "$0: '$program' does not exist."
	exit 1

# Program is an executable file.
elif [ ! -x "$program" ]; then
	echo "$0: '$program' is not executable."
	exit 1
fi

### Check if directory exists.

if [ ! -d "$directory" ]; then
	# Directory is not really a directory.
	if [ -f "$directory" ]; then
		echo "$0: '$directory' is not a directory"
		exit 1
	# Directory does not exist.
	else
		echo "$0: directory '$directory' does not exist"
		exit 1
	fi
fi

### Put names of all files in the directory into an array.

paths=()

for file in "${directory}"/*; do
	paths+=( "$file" )
done

### Find START file.

i=0
line=""
start_file=""

until [ "$line" == "START" ] || (($i == ${#paths[@]})); do
	line=$(head -1 "${paths[$i]}")
	start_file="${paths[$i]}"
	((i++))
done

if [ "$line" != "START" ]; then
	echo "$0: START file is missing in '$directory'."
	exit 1
fi

### Execute script.

# Create temporary files.
input_file=$(mktemp /tmp/temporary-file.XXXXXXXX)
output_file=$(mktemp /tmp/temporary-file.XXXXXXXX)

current_file="$start_file"
next_file=""

is_time_to_stop=false

# Itertate over files.

until [ $is_time_to_stop == true ]; do

	# Read current file and redirect all lines except first and last to input file.
	while IFS= read -r line; do
		if [ "$line" != "START" ] && [ "$line" != "STOP" ]; then

			# Line starts with "FILE".
			if [[ "$line" == FILE* ]]; then
				next_file="$directory"/"${line#FILE *}"
			else
				echo "$line" >> "$input_file"
			fi
		elif [ "$line" == "STOP" ]; then
				is_time_to_stop=true
		fi
	done < "$current_file"

	# Execute program and redirect output to output file.
	"./$program" < "$input_file" > "$output_file"

	# Swap input and output files.
	temp_file="$input_file"
	input_file="$output_file"
	output_file="$temp_file"

	# Iterate file.
	current_file="$next_file"
done

# Print last output.
cat "$input_file"

exit 0
