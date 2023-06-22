#pragma once
#include "data_frame.h"
#include <cassert>
#include <type_traits>
#include <numeric>
template <typename T>
class portfolio {
private:
	data_frame<T> weights_df;
	double cash_dbl;
	data_frame<T> shares_df;
	double liquid_val_dbl;
public:
	portfolio() {};
	portfolio(data_frame<T> weights, double cash);
	~portfolio() {};
	data_frame<T> get_weights();
	double get_cash();
	void set_cash(double _cash);
	void set_weights(data_frame<T> _weights);
	void run_test(data_frame<T> price_df);
};


template <typename T>
portfolio<T>::portfolio(data_frame<T> weights, double _cash) {
	this->cash_dbl = _cash;
	this->weights_df = weights;
	this->liquid_val_dbl = _cash;
}

template <typename T>
data_frame<T> portfolio<T>::get_weights() {
	return this->weights_df;
}

template <typename T>
double portfolio<T>::get_cash() {
	return this->cash;
}

template <typename T>
void portfolio<T>::set_cash(double _cash) {
	this->cash = _cash;
}

template <typename T>
void portfolio<T>::set_weights(data_frame<T> _weights) {
	this->weights_df = _weights;
}


template <typename T>
void portfolio<T>::run_test(data_frame<T> price_df) {
	static_assert(std::is_same<T,boost::posix_time::ptime>::value, "price_df must be a data_frame of type boost::posix_time::ptime for combine price and portfolio weights");
	std::vector<T> weight_index = this->weights_df.get_index();
	std::vector<T> price_index = price_df.get_index();
	std::vector<double> ror;
	int beg = 0;
	int end = 0;
	T start_date, end_date;
	for (int i = 0; i < weights_df.row(); i++) {
		start_date = weight_index[i];

		if (i < weights_df.row() - 1) {
			end_date = weight_index[i + 1];
		}
		else {
			boost::gregorian::days daysadded(1);

			end_date = price_index[price_index.size()-1] + daysadded;
		}

		while (end < (price_index.size() - 1) and price_index[end] < end_date) {

			if (price_index[end] == start_date) {
				beg = end;
			}

			end++;

		}
		//order execute
		std::vector<int> row_idx(end-beg+2);
		std::iota(row_idx.begin(), row_idx.end(), beg-1);
		data_frame<T> price_df_slice = price_df.get_rows(row_idx);
		data_frame<T> ret_df = price_df_slice.log().row_diff().dropna();
		//get new weight
		Eigen::MatrixXd weight_mx = this->weights_df.get_row(i).get_data();

		//if in the first period ,set the last weights as a zero vector
		if (i == 0) {
			Eigen::MatrixXd last_weight_mx = Eigen::MatrixXd::Zero(1, weight_mx.cols());
		}
		else {
			Eigen::MatrixXd last_weight_mx = this->weights_df.get_data().row(i-1);
		}


		//compute current pfl's total value = cash + asset_exposure,TBD
		
		//update aseet val for each assets by change of shares compared with last period
		Eigen::MatrixXd sub_price_mx = price_df_slice.get_data().row(0);
		Eigen::MatrixXd weighted_val_mx = (weight_mx - last_weight_mx) * (this->liquid_val_dbl); // need to consider commission fee
		weighted_val_mx = weighted_val_mx/sub_price_mx;// divide by available value, may need more sophisticated method to decide the available value than take the close price
		// round the shares to integer
		for (int i=0; i<weighted_val_mx.cols();i++) {
			weighted_val_mx(0, i) = (std::signbit(weighted_val_mx(0, i)) ? -1.0 : 1) * std::floor(std::abs(weighted_val_mx(0, i)));
		}
		Eigen::MatrixXd share_chg_mx = weighted_val_mx;
		//trading price :TBD
		trade_price_mx = sub_price_mx;
		//change position by share change
		double liquid_needed_dbl = 0;
		for (int i = 0; i < share_chg_mx.cols(); i++) {
			if (share_chg_mx(0, i) > 0) {
				//buy
				liquid_needed_dbl += share_chg_mx(0, i)*trade_price_mx(0,i);
			}
			else if (share_chg_mx(0, i) < 0) {
				//sell
				liquid_needed_dbl -= share_chg_mx(0, i) * trade_price_mx(0, i);
			}
		}
		//add new weight
		
		Eigen::MatrixXd stk_ret_mx = ret_df.get_data().transpose();
		Eigen::MatrixXd pfl_ret_mx = weight_mx * stk_ret_mx;
		if (ror.size()!=0) {
			ror.push_back(pfl_ret_mx(0, 0)+ ror.back());
		}
		else {
			ror.push_back(pfl_ret_mx(0, 0));
		}

		for (int j = 1; j < pfl_ret_mx.cols(); j++) {
			ror.push_back(pfl_ret_mx(0,j)+ror.back());
		}

	}
	std::cout << "Rate of Return Series:" << std::endl;
	for (int i = 0; i < ror.size(); i++) {
		std::cout << exp(ror[i])-1 << std::endl;
	}

}