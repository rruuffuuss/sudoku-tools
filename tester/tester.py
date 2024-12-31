import subprocess
import numpy as np
import pandas as pd

path = "./build/solver"
make_command = ["make",  "-C", "./solver"]

permutation_width_max = 9
permutation_certainty_max = 9

# minimum needed for testing solver with brute force feature. Width 1 would be solved entirely by brute force.
permutation_width_min = 2
permutation_certainty_min = 5

test_filepath = "./examples/allUnsolveableTest.csv"
solution_filepath = "./examples/allUnsolveableTestSolutions.csv"
test_number = 100

results = [None] * (permutation_width_max - permutation_width_min) * (permutation_certainty_max - permutation_certainty_min) 

# compile solver
make_process = subprocess.Popen(make_command , stderr=subprocess.STDOUT)
make_process.wait()

for i in range(permutation_certainty_max - permutation_certainty_min):
    for y in range(permutation_width_max - permutation_width_min):


        test_process = subprocess.Popen(
            [path, str(y + permutation_width_min), str(i + permutation_certainty_min), test_filepath, solution_filepath, str(test_number)],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE, 
            stderr=subprocess.PIPE, 
            text=True 
        )

        test_process.wait()


        if(test_process.returncode != 0):
            print("test execution failed")
            print(test_process.stdout.readline())
        else: 
            results[(i * (permutation_width_max - permutation_width_min) + y)] = [int(r) for r in test_process.stdout.readline().split(' ')]

# Close the streams

# print("Output:")
# print(results)

test_process.stdin.close()
test_process.stdout.close()
test_process.stderr.close()


df = pd.DataFrame(results, columns= ["width", "certainty", "successNo", "time"])
df["time/puzzle"] = [t / test_number for t in df["time"]]

df = df[df['successNo'] == float(df['successNo'].max())]



df = df.sort_values(by=['time'])
df.sort_index()
print(df)