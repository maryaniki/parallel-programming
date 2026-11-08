import numpy as np

A = np.loadtxt("A.txt")
B = np.loadtxt("B.txt")
C_cpp = np.loadtxt("C_cpp.txt")

C_py = A @ B

if np.allclose(C_cpp, C_py):
    print("Результаты совпадают!")
else:
    print("Результаты не совпадают!")