#include <fast-cpp-csv-parser/csv.h>
#include <iostream>
#include <chrono>
#include <vector>
int main()
{
	// First, load the data.
	// Use data::Load() which transposes the matrix.
	auto start = std::chrono::high_resolution_clock::now();
	io::CSVReader<3> in("E:\\pycharm\\PycharmProjects\\optiver_EDA\\kaggle_data\\train.csv");
	in.read_header(io::ignore_extra_column, "stock_id", "time_id", "target");
	double stock_id; int time_id; double target;
	std::vector<double> stock_id_vec;
	std::vector<int> time_id_vec;
	std::vector<double> target_vec;
	while (in.read_row(stock_id, time_id, target)) {
		stock_id_vec.push_back(stock_id);
		time_id_vec.push_back(time_id);
		target_vec.push_back(target);
	}
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;

}