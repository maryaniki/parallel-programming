import std;
#include <omp.h>

using Matrix = std::vector<std::vector<double>>;

int main()
{
    int sizes[] = { 100, 200, 400, 600, 800, 1000 };
    int threads[] = { 1, 2, 4, 8 };

    std::ofstream results("C:\\parallel-programming\\lab2\\results.txt");

    if (!results.is_open())
    {
        std::println("Error open file results.txt");
        return 1;
    }

    results << "n Threads Time Speedup\n";

    omp_set_dynamic(0);

    for (int n : sizes)
    {
        std::cout << "\nSize matrix: " << n << "x" << n << "\n";

        Matrix A(n, std::vector<double>(n));
        Matrix B(n, std::vector<double>(n));

        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                A[i][j] = (double)std::rand() / 1000;
                B[i][j] = (double)std::rand() / 1000;
            }
        }

        double time_1 = 0.0;

        for (int threadCount : threads)
        {
            Matrix C(n, std::vector<double>(n, 0.0));
            omp_set_num_threads(threadCount);
            auto start = std::chrono::high_resolution_clock::now();

#pragma omp parallel for
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

            auto end = std::chrono::high_resolution_clock::now();
            double time = std::chrono::duration<double>(end - start).count();

            if (threadCount == 1)
            {
                time_1 = time;
            }

            double speedup = time_1 / time;

            std::cout << "Threads: " << threadCount << "\n";
            std::cout << "Time: " << time << " seconds\n";
            std::cout << "Speedup: " << speedup << "\n";

            results << n << " "
                << threadCount << " "
                << time << " "
                << speedup << "\n";
        }
    }

    results.close();
    std::cout << "Result save";

    return 0;
}