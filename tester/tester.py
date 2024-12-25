import subprocess;

path = "./build/solver"
make_command = ["make",  "-C", "./solver"]

permutation_width_max = 9
permutation_certainty_max = 9

test_filepath = "./examples/test5.csv"
solution_filepath = "./examples/test5solutions.csv"
test_number = 100

results = [None] * (permutation_width_max - 1) * (permutation_certainty_max - 1) 

# compile solver
make_process = subprocess.Popen(make_command , stderr=subprocess.STDOUT)
make_process.wait()

for i in range(1, permutation_certainty_max):
    for y in range(1, permutation_width_max):


        test_process = subprocess.Popen(
            [path, str(i), str(y), test_filepath, solution_filepath, str(test_number)],
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
            results[(i * (1 - permutation_width_max) + y)] = test_process.stdout.readline().split(' ')[:-1]

# Close the streams

print("Output:")
print(results)

test_process.stdin.close()
test_process.stdout.close()
test_process.stderr.close()