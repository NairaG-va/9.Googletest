//Работа с тестами
#include <iostream>
#include <math.h>
#include <vector>
#include <gtest/gtest.h>

std::vector<std::vector<double>> function(int n, std::vector<std::vector<double>> matrix) //нахождение обратной матрицы
{
	std::vector<std::vector<double>> inv_matrix;
	
	for (int i = 0; i < n; i++)//делаем обратную матрицу изначально единичной
		{
			std::vector<double> v;
			for (int j = 0; j < n; j++)
				{
					v.push_back((i == j) ? 1 : 0); //т.е равно 1, если i == j, иначе равно 0
				}
			inv_matrix.push_back(v);

		}
		
		
	for (int k = 0; k < n; k++)//приводим матрицу к верхнетреугольной
		{
			for (int i = k + 1; i < n; i++)
				{
					double temp = matrix[i][k] / matrix[k][k];
					for (int j = 0; j < n; j++)
						{
							matrix[i][j] -= temp * matrix[k][j];
							inv_matrix[i][j] -= temp * inv_matrix[k][j];
						}
				}
		}
	
		
	for (int k = n - 1; k > 0; k--)//далее зануляем все, что выше главной диагонали
		{
			for (int i = k - 1; i >= 0; i--)
				{
					double temp = matrix[i][k] / matrix[k][k];
					for (int j = 0; j < n; j++)
						{
							matrix[i][j] -= temp * matrix[k][j];
							inv_matrix[i][j] -= temp * inv_matrix[k][j];
						}
				}
		}
		
		
	for (int i = 0; i < n; i++)//делим на элементы главной диагонали
		{
			for (int j = 0; j < n; j++)
				{
					inv_matrix[i][j] /= matrix[i][i];
				}
		}

//выводим обратную матрицу, т.е inv_matrix
	std::cout << "Inverse matrix:" << "\n";
	for(int i = 0; i < n; i++)
		{
			for(int j = 0; j < n; j++)
				{
					std::cout << inv_matrix[i][j] << "\t\t";
				}
			std::cout << "\n";	
		}
		
	std::cout << "\n";	
		
return inv_matrix;
}

std::vector<std::vector<double>> mult_matrix(int n, std::vector<std::vector<double>>& matrix, std::vector<std::vector<double>>& inv_matrix)
{
	std::vector<std::vector<double>> multiplication;
	
	//заполняем нулями матрицу, отвечающую за произведение 
	for(int i = 0; i < n; i++) 
	{
		std::vector<double> v;
		for (int j = 0; j < n; j++) 
		{
			v.push_back(0.0);
		}
		multiplication.push_back(v);
	}
	
	
	
	for(int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			//multiplication[i][j] = 0;
			for (int k = 0; k < n; k++)
			{
				multiplication[i][j] += matrix[i][k] * inv_matrix[k][j];
			}
		}
	}
	
		
	//выводим произведение матриц, т.е multiplication
	std::cout << "Product of direct and inverse matrix:" << "\n";
	for(int i = 0; i < n; i++)
		{
			for(int j = 0; j < n; j++)
				{
					std::cout << multiplication[i][j] << "\t\t";
				}
			std::cout << "\n";	
		}

	return multiplication;
}


TEST(matrixTests,inversion)
{
	std::vector<std::vector<double>> testVec = {
		{2, 5, 7},
		{6, 3, 4},
		{5, -2, -3} };
		
	std::vector<std::vector<double>> invMatrix = function(testVec.size(), testVec);
	std::vector<std::vector<double>> multMatrix = mult_matrix(testVec.size(), testVec, invMatrix);
	
	for (int i = 0; i < 3; i++)
		{
			ASSERT_LT(abs(multMatrix[i][i] - 1.0), 1e-6);
		}	
	
	for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
				{
					if (j != i) ASSERT_LT(abs(multMatrix[i][j]), 1e-6);
				}
		}
}

TEST(matrixTests,inversion2)
{
	//тестовая матрица
	std::vector<std::vector<double>> testMatrix = {
		{2, 5, 7},
		{6, 3, 4},
		{5, -2, -3} };
	//ее настоящая обратная матрица	
	std::vector<std::vector<double>> inversedMatrix = {
		{1, -1, 1},
		{-38, 41, -34},
		{27, -29, 24} };
	//находим обратную матрицу по нашему методу	
	std::vector<std::vector<double>> invMatrix = function(testMatrix.size(),testMatrix);
	
	for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
				{
					ASSERT_LT(abs(inversedMatrix[i][j] - invMatrix[i][j]), 1e-6);
				}
		}
}


int main(int argc, char* argv[])
{
	int n; //размерность матрицы
	
	try
		{
			n = std::stod(argv[1]);
   
		}
	catch(...)
		{
			std::cout << "Not able to parse arguments" << "\n";
			return 1;
		}
	
	std::vector<std::vector<double>> matrix;
	
	//заполняем исходную матрицу, т.е matrix
	for (int i = 0; i < n; i++)  
		{   
			std::vector<double> v;
			for (int j = 0; j < n; j++) 
			{
				v.push_back(rand()%20);
			}
			matrix.push_back(v);	
		}	
		
	//выводим исходную матрицу, т.е matrix
	std::cout << "Direct matrix:" << "\n";
	for(int i = 0; i < n; i++)
		{
			for(int j = 0; j < n; j++)
				{
					std::cout << matrix[i][j] << "\t\t";
				}
			std::cout << "\n";	
		}
	std::cout << "\n";
	
	std::vector<std::vector<double>> inv_matrix = function(n, matrix);
	std::vector<std::vector<double>> multiplication = mult_matrix(n, matrix, inv_matrix);
	
	::testing::InitGoogleTest(&argc, argv);
	
	return RUN_ALL_TESTS();
	
}

