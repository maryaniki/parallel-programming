import std;

using Matrix = std::vector<std::vector<double>>;

int main()
{
    int sizes[] = { 100, 200, 400, 600, 800, 1000 };

    std::ofstream results("C:\\parallel-programming\\lab1\\results.txt");

    if (!results.is_open())
    {
        std::println("Error open file results.txt");
        return 1;
    }

    results << "n Elements Operations Memory Time\n";

    for (int n : sizes)
    {
        std::cout << "Size matrix: " << n << "x" << n << std::endl;

        Matrix A(n, std::vector<double>(n));
        Matrix B(n, std::vector<double>(n));
        Matrix C(n, std::vector<double>(n, 0.0));

        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                A[i][j] = (double)std::rand() / 1000;
                B[i][j] = (double)std::rand() / 1000;
            }
        }

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                for (int k = 0; k < n; ++k)
                {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
        auto end =std::chrono::high_resolution_clock::now();

        double time = std::chrono::duration<double>(end - start).count();
        std::cout << "Time: " << time << std::endl;

        long long elements = 1LL * n * n;
        std::cout << "Elements: " << elements << std::endl;

        long long operations = (long long)n * n * n;
        std::cout << "Operations: " << operations << std::endl;

        double memoryMB = static_cast<double>(3 * n * n * sizeof(double)) / (1024 * 1024);
        std::cout << "Memory: " << memoryMB << " MB\n" << std::endl;

        results << n << " " << elements << " " << operations << " " << memoryMB << " " << time << "\n";

        if (n == 100)
        {
            std::ofstream fileA("C:\\parallel-programming\\lab1\\A.txt");
            std::ofstream fileB("C:\\parallel-programming\\lab1\\B.txt");
            std::ofstream fileC("C:\\parallel-programming\\lab1\\C_cpp.txt");

            if (!fileA.is_open() || !fileB.is_open() || !fileC.is_open())
            {
                std::println("Error open matrix files");
                return 1;
            }
            for (int i = 0; i < n; ++i)
            {
                for (int j = 0; j < n; ++j)
                {
                    fileA << A[i][j] << " ";
                    fileB << B[i][j] << " ";
                    fileC << C[i][j] << " ";
                }
                fileA << "\n";
                fileB << "\n";
                fileC << "\n";
            }
            fileA.close();
            fileB.close();
            fileC.close();
        }
    }

    results.close();
    std::cout << "Result save";

    return 0;
}