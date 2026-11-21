#include <mpi.h>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <iomanip>

typedef std::vector<double> Matrix;

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int sizes[] = { 200, 400, 800, 1200, 1600, 2000 };

    std::ofstream results;

    if (rank == 0)
    {
        bool write_header = !std::ifstream("results.txt").good();

        results.open("results.txt", std::ios::app);

        if (!results.is_open())
        {
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        if (write_header)
        {
            results << "MatrixSize\tProcesses\tParallel Time (s)\tSequential Time (s)\tSpeedup\n";
        }

        results << std::fixed << std::setprecision(10);
    }

    for (int index = 0; index < 6; index++)
    {
        int n = sizes[index];
        Matrix A, B, C;

        if (rank == 0)
        {
            A.resize(n * n);
            B.resize(n * n);
            C.resize(n * n, 0.0);

            for (int i = 0; i < n * n; ++i)
            {
                A[i] = (double)std::rand() / 1000;
                B[i] = (double)std::rand() / 1000;
            }
        }

        if (rank != 0)
        {
            B.resize(n * n);
        }

        int start_row = n * rank / size;
        int end_row = n * (rank + 1) / size;

        int local_rows = end_row - start_row;
        int local_elements = local_rows * n;

        Matrix local_A(local_elements);
        Matrix local_C(local_elements, 0.0);

        std::vector<int> counts(size);
        std::vector<int> displs(size);

        for (int i = 0; i < size; ++i)
        {
            int start = n * i / size;
            int end = n * (i + 1) / size;

            counts[i] = (end - start) * n;
            displs[i] = start * n;
        }

        MPI_Barrier(MPI_COMM_WORLD);

        double start_time = MPI_Wtime();

        MPI_Bcast(B.data(), n * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        MPI_Scatterv(
            rank == 0 ? A.data() : NULL,
            counts.data(),
            displs.data(),
            MPI_DOUBLE,
            local_A.data(),
            local_elements,
            MPI_DOUBLE,
            0,
            MPI_COMM_WORLD
        );

        for (int i = 0; i < local_rows; ++i)
        {
            for (int k = 0; k < n; ++k)
            {
                double aik = local_A[i * n + k];

                for (int j = 0; j < n; ++j)
                {
                    local_C[i * n + j] += aik * B[k * n + j];
                }
            }
        }

        MPI_Gatherv(
            local_C.data(),
            local_elements,
            MPI_DOUBLE,
            rank == 0 ? C.data() : NULL,
            counts.data(),
            displs.data(),
            MPI_DOUBLE,
            0,
            MPI_COMM_WORLD
        );

        double local_time = MPI_Wtime() - start_time;
        double parallel_time = 0.0;

        MPI_Reduce(
            &local_time,
            &parallel_time,
            1,
            MPI_DOUBLE,
            MPI_MAX,
            0,
            MPI_COMM_WORLD
        );

        if (rank == 0)
        {
            Matrix C_sequential(n * n, 0.0);
            double sequential_start_time = MPI_Wtime();
            for (int i = 0; i < n; ++i)
            {
                for (int k = 0; k < n; ++k)
                {
                    double aik = A[i * n + k];

                    for (int j = 0; j < n; ++j)
                    {
                        C_sequential[i * n + j] += aik * B[k * n + j];
                    }
                }
            }

            double sequential_time = MPI_Wtime() - sequential_start_time;

            double speedup = sequential_time / parallel_time;

            results << n << "x" << n << "\t"
                << size << "\t"
                << parallel_time << "\t"
                << sequential_time << "\t"
                << speedup << "\n";

            results.flush();
        }
    }

    if (rank == 0)
    {
        results.close();
    }

    MPI_Finalize();

    return 0;
}