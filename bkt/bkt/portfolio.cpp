

//
//template <typename T>
//portfolio<T>::portfolio(data_frame<T> weights, double cash) {
//	this->cash = cash;
//	this->weights_df = weights;
//}
//
//template <typename T>
//data_frame<T> portfolio<T>::get_weights() {
//	return this->weights_df;
//}
//
//template <typename T>
//double portfolio<T>::get_cash() {
//	return this->cash;
//}
//
//template <typename T>
//void portfolio<T>::set_cash(double _cash) {
//	this->cash = _cash;
//}
//
//template <typename T>
//void portfolio<T>::set_weights(data_frame<T> _weights) {
//	this->weights_df = _weights;
//}
//
//
//template <typename T>
//void portfolio<T>::run_test(data_frame<T> price_df) {
//	assert(T == boost::posix_time::ptime, "price_df must be a data_frame of type boost::posix_time::ptime for combine price and portfolio weights");
//	std::vector<T> weight_index = this->weights_df.get_index();
//	std::vector<T> price_index = price_df.get_index();
//	std::vector<double> ror;
//	int beg = 0;
//	int end = 0;
//	T start_date(), end_date;
//	for (int i = 0; i < weights_df.row(); i++) {
//		start_date = weight_index[i];
//
//		if (i < weights_df.row() - 1) {
//			end_date = weight_index[i + 1];
//		}
//		else {
//			boost::gregorian::days daysadded(3);
//			end_date = price_df.get_index[-1] + daysadded;
//		}
//
//		while (price_index[end] >= start_date and price_index[end] < end_date) {
//
//			if (price_index[end] == start_date) {
//				beg = end;
//			}
//
//			end++;
//
//		}
//
//		data_frame<T> price_df_slice = price_df.slice(beg - 1, end);
//		data_frame<T> ret_df = price_df_slice.log().row_diff().dropna();
//		Eigen::MatrixXd weight_mx = this->weight_df.get_data().row(i);
//		Eigen::MatrixXd stk_ret_mx =  ret_df.get_data();
//		Eigen::MatrixXd pfl_ret_mx =  weight_mx*stk_ret_mx;
//		ror.push_back((1.0 + pfl_ret_mx(0, 0)) * ror.back() - 1);
//
//		for (int j = 1; j < pfl_ret_mx.cols(); j++) {
//			pfl_ret_mx(0, j) = (1.0 + pfl_ret_mx(0, j)) * pfl_ret_mx(0, j - 1) - 1.0;
//			ror.push_back(pfl_ret_mx(0, j));
//		}
//
//	}
//
//	for (int i = 0; i < ror.size(); i++) {
//		std::cout << ror[i] << std::endl;
//	}
//
//}