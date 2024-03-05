import ast
import matplotlib.pyplot as plt

# Read the content of the file
with open("arduino_data.txt", "r") as file:
    content = file.read()

# Safely evaluate the string as a Python expression
data = ast.literal_eval(content)
x = [None for _ in range(len(data))]

# Convert strings to integers
data_int = [[int(value) for value in sublist] for sublist in data]

reed_f0 = [sublist[0] for sublist in data_int]
reed_f1 = [sublist[1] for sublist in data_int]
reed_b0 = [sublist[2] for sublist in data_int]
reed_b1 = [sublist[3] for sublist in data_int]
reed_b2 = [sublist[4] for sublist in data_int]

print(data_int)

plt.scatter(x, reed_f0)
plt.scatter(x, reed_f1)
plt.scatter(x, reed_b0)
plt.scatter(x, reed_b1)
plt.scatter(x, reed_b2)

