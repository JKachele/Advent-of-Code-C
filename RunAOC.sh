#!/usr/bin/env bash

# Prompt the user for the year to run
while true; do
    read -p "Enter a Year to Run (eg. 2024 or 24): " year

    # Validate the input
    if [[ $year =~ ^[0-9]+$ ]]; then
        if ((year < 100)); then
            year="20$year"  # Convert two-digit year to four-digit
        elif ((year >= 1000)); then
            year="${year:0:4}"  # Ensure it's a four-digit year
        else
            echo "Please enter a valid 4 or 2 digit year."
            continue
        fi

        # Check if the year is a folder in the src directory
        if [[ -d "src/$year" ]]; then
            break
        else
            echo "The year $year does not exist as a folder in the src directory."
        fi
    else
        echo "Invalid input. Please enter a valid year."
    fi
done

# Prompt the user for a day to run
while true; do
    read -p "Enter a Day to Run: " day

    # Validate the input
    if [[ $day =~ ^[0-9]+$ ]]; then
        # Format the day as two digits (e.g., 01, 02, ..., 25)
        day=$(printf "%02d" $day)

        # Check if the day is a file in the src/$year folder
        if [[ -f "src/$year/Day$day.c" ]]; then
            break
        else
            echo "The file for Day $day does not exist in the src/$year folder."
        fi
    else
        echo "Invalid input. Please enter a valid day."
    fi
done

printf '\e[A\e[K'
printf '\e[A\e[K'
printf "Running Advent of Code %d, Day %d...\n\n" $year $day
make -f Makefile FILE=src/$year/Day$day.c -s $1
