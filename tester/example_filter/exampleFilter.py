# this utility filters the puzzles in examples.csv to exclude puzzles at the indeces appearing in filter.csv
import os
dir_path = "./tester/example_filter/"

with open(dir_path + "examples.csv", "r") as f:
    examples = f.read().split("\n")

with open(dir_path + "filter.csv", "r") as f:
    removeIndeces = f.read().split(",")



for i in range(len(removeIndeces) - 1, 0, -1):
    del examples[int(removeIndeces[i])]

with open(dir_path + "filteredExamples.csv", "w") as f:
    for e in examples:
        f.writelines(e + "\n")

