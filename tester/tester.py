import subprocess;

path = "./solver/solver"

process = subprocess.Popen(
    path,
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE, 
    stderr=subprocess.PIPE, 
    text=True 
)

process.wait()

# Close the streams
output = process.stdout.readlines()  # Read one line of output
print("Output:")
for o in output:
    print(o)

process.stdin.close()
process.stdout.close()
process.stderr.close()