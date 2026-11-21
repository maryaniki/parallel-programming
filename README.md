# Лабораторные работы по параллельному программированию
**Студент:** Никитенко Мария

**Группа:** 6214-100503D

## Лабораторные работы
Подробное описание каждой лабораторной работы находится в отдельном README.md внутри соответствующей папки

| № | Название | Технологии |
|---|---|---|
| 1 | [Умножение квадратных матриц с автоматической верификацией](./lab1/) | C++, Python, NumPy |
| 2 | [Параллельное умножение квадратных матриц](./lab2/) | C++, OpenMP |
| 3 | [MPI - параллельное перемножение квадратных матриц](./lab3/) | C++, MPI |

## Структура проекта

```text
parallel-programming/
│
├── lab1/
│   ├── main.cpp
│   ├── verify.py
│   ├── A.txt
│   ├── B.txt
│   ├── C_cpp.txt
│   ├── results.txt
│   └── README.md
│
├── lab2/
│   ├── main.cpp
│   ├── results.txt
│   ├── CMakeLists.txt
│   └── README.md
│
├── lab3/
│   ├── main.cpp
│   ├── results.txt
│   ├── script.pbs
│   └── README.md
│
├── CMakeLists.txt
├── CMakePresets.json
├── .gitignore
└── README.md
```

## Используемые технологии

- C++
- CMake
- Python
- NumPy
- OpenMP
- MPI