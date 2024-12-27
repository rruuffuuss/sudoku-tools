# this utility filters the puzzles in examples.csv to exclude puzzles at the indeces appearing in filter.csv
keepFilter = False # true to keep all indeces in filter, false to remove all indeces in filter

import os
dir_path = "./tester/example_filter/"

with open(dir_path + "examples.csv", "r") as f:
    examples = f.read().split("\n")

with open(dir_path + "filter.csv", "r") as f:
    removeIndeces = f.read().split(",")


rm = [ i for i in range(0, 10000)]

if keepFilter:
    for i in range(len(removeIndeces) - 1, 0, -1):
        del examples[int(removeIndeces[i])]
else:
    for i in range(len(removeIndeces) - 1, 0, -1):
        del rm[int(removeIndeces[i])]

    for i in range(len(rm) - 1, 0, -1):
        del examples[int(rm[i])]

with open(dir_path + "filteredExamples.csv", "w") as f:
    for e in examples:
        f.writelines(e + "\n")

