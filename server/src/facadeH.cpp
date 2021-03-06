﻿#include "facadeH.h"
#include "Utils.h"

std::pair<int, int> FacadeH::range_NF = std::make_pair(2, 20);
std::pair<int, int> FacadeH::range_NC = std::make_pair(3, 20);

void FacadeH::attachDoors(std::vector<float>& params, const std::vector<int>& selected_win_types) {
	if (selected_win_types[3] < 25) {
		// do nothing
	}
	else {
		// remove the gap between the door and the ground
		params[20] = 0;
	}

	if (selected_win_types[2] < 25) {
		// do nothing
	}
	else {
		// remove the gap between the door and the ground
		params[23] = 0;
	}
}

cv::Mat FacadeH::generateFacade(int width, int height, int thickness, int num_floors, int num_columns, const std::vector<float>& params, const std::vector<int>& selected_win_types, const cv::Scalar& bg_color, const cv::Scalar& fg_color) {
	std::vector<float> decoded_params;
	decodeParams(width, height, num_floors, num_columns, params, selected_win_types, -1, decoded_params);
	
	return generateFacade(width, height, thickness, bg_color, fg_color, decoded_params[0], decoded_params[1], decoded_params[2], decoded_params[3], decoded_params[4], decoded_params[5], decoded_params[6], decoded_params[7], decoded_params[8], decoded_params[9], decoded_params[10], decoded_params[11], decoded_params[12], decoded_params[13], decoded_params[14], decoded_params[15], decoded_params[16], decoded_params[17], decoded_params[18], decoded_params[19], decoded_params[20], decoded_params[21]);
}

void FacadeH::decodeParams(float width, float height, int num_floors, int num_columns, std::vector<float> params, const std::vector<int>& selected_win_types, int mass_grammar_id, std::vector<float>& decoded_params) {
	int NF = std::round(params[0] * (range_NF.second - range_NF.first) + range_NF.first);
	if (NF < range_NF.first) NF = range_NF.first;
	int NC = std::round(params[1] * (range_NC.second - range_NC.first) + range_NC.first);
	if (NC < range_NC.first) NC = range_NC.first;

	////////////////////////////////////////////////////////////////////////////////////////////
	// use the known #floors/#columns if they are provided
	if (num_floors > 0 && num_columns > 0) {
		NF = num_floors;
		NC = num_columns;
	}

	if ((NC - 1) % 2 != 0) NC--;

	////////////////////////////////////////////////////////////////////////////////////////////
	// HACK
	// enforce the minimum value for some parameters
	params[3] = std::max(0.02f, params[3]);	// FH
	params[6] = std::max(0.02f, params[6]);	// TW

	float GH = (float)height / (params[2] + params[3] * (NF - 1) + params[4]) * params[2];
	float FH = (float)height / (params[2] + params[3] * (NF - 1) + params[4]) * params[3];
	float AH = (float)height / (params[2] + params[3] * (NF - 1) + params[4]) * params[4];
	float SW = (float)width / (params[5] * 2 + params[6] * (NC - 1) + params[7]) * params[5];
	float TW = (float)width / (params[5] * 2 + params[6] * (NC - 1) + params[7]) * params[6];
	float TW2 = (float)width / (params[5] * 2 + params[6] * (NC - 1) + params[7]) * params[7];

	float WT = FH / (params[8] + params[9] + params[10]) * params[8];
	float WH = FH / (params[8] + params[9] + params[10]) * params[9];
	float WB = FH / (params[8] + params[9] + params[10]) * params[10];
	float WS = TW / (params[11] * 2 + params[12]) * params[11];
	float WW = TW / (params[11] * 2 + params[12]) * params[12];

	float WT2 = FH / (params[13] + params[14] + params[15]) * params[13];
	float WH2 = FH / (params[13] + params[14] + params[15]) * params[14];
	float WB2 = FH / (params[13] + params[14] + params[15]) * params[15];
	float WS2 = TW2 / (params[16] * 2 + params[17]) * params[16];
	float WW2 = TW2 / (params[16] * 2 + params[17]) * params[17];

	float DT, DH, DB;
	if (selected_win_types[3] < 25) {
		DT = GH / (params[18] + params[19] + params[20]) * params[18];
		DH = GH / (params[18] + params[19] + params[20]) * params[19];
		DB = GH / (params[18] + params[19] + params[20]) * params[20];
	}
	else {
		// remove the gap between the door and the ground
		DT = GH / (params[18] + params[19]) * params[18];
		DH = GH / (params[18] + params[19]) * params[19];
		DB = 0.0f;
	}

	float DT2, DH2, DB2;
	if (selected_win_types[2] < 25) {
		DT2 = GH / (params[21] + params[22] + params[23]) * params[21];
		DH2 = GH / (params[21] + params[22] + params[23]) * params[22];
		DB2 = GH / (params[21] + params[22] + params[23]) * params[23];
	}
	else {
		// remove the gap between the door and the ground
		DT2 = GH / (params[21] + params[22]) * params[21];
		DH2 = GH / (params[21] + params[22]) * params[22];
		DB2 = 0.0f;
	}

	decoded_params.resize(22);
	decoded_params[0] = GH;
	decoded_params[1] = FH;
	decoded_params[2] = AH;
	decoded_params[3] = SW;
	decoded_params[4] = TW;
	decoded_params[5] = TW2;
	decoded_params[6] = WT;
	decoded_params[7] = WH;
	decoded_params[8] = WB;
	decoded_params[9] = WS;
	decoded_params[10] = WW;
	decoded_params[11] = WT2;
	decoded_params[12] = WH2;
	decoded_params[13] = WB2;
	decoded_params[14] = WS2;
	decoded_params[15] = WW2;
	decoded_params[16] = DT;
	decoded_params[17] = DH;
	decoded_params[18] = DB;
	decoded_params[19] = DT2;
	decoded_params[20] = DH2;
	decoded_params[21] = DB2;
}

cv::Mat FacadeH::generateRandomFacade(int width, int height, int thickness, std::vector<float>& params, float window_displacement, float window_prob) {
	///////////////////////////////////////////////////////////////////////////////////
	// パラメータを設定
	float ratio;

	int NF = utils::genRand(range_NF.first, range_NF.second + 1);
	int NC = utils::genRand(range_NC.first, range_NC.second + 1);
	if ((NC - 1) % 2 != 0) NC--;

	float GH = utils::genRand(0.4, 5);
	float FH = 1;
	float AH = utils::genRand(0, 0.16);
	float SW = utils::genRand(0, 0.35);
	float TW = 1;
	float TW2 = utils::genRand(1.5, 6.5);
	float WT = utils::genRand(0.005, 0.5);
	float WH = 1;
	float WB = utils::genRand(0.005, 1.1);
	float WS = utils::genRand(0.01, 0.5);
	float WW = 1;
	float WT2 = utils::genRand(0.01, 0.42);
	float WH2 = 1;
	float WB2 = utils::genRand(0.01, 0.4);
	float WS2 = utils::genRand(0.01, 1);
	float WW2 = 1;
	float DT = utils::genRand(0.04, 0.45);
	float DH = 1;
	float DB = utils::genRand(0, 1.2);
	float DT2 = utils::genRand(0.04, 0.3);
	float DH2 = 1;
	float DB2 = utils::genRand(0, 0.001);
	

	// 各フロアの各種高さをnormalize
	ratio = FH / (WT + WB + WH);
	WT *= ratio;
	WH *= ratio;
	WB *= ratio;

	// 中央の各種高さをnormalize
	ratio = FH / (WT2 + WB2 + WH2);
	WT2 *= ratio;
	WH2 *= ratio;
	WB2 *= ratio;

	// 各フロアの各種幅をnormalize
	ratio = TW / (WS * 2 + WW);
	WS *= ratio;
	WW *= ratio;
	
	// 中央の各種幅をnormalize
	ratio = TW2 / (WS2 * 2 + WW2);
	WS2 *= ratio;
	WW2 *= ratio;

	// 1Fの各種高さをnormalize
	ratio = GH / (DT + DH + DB);
	DT *= ratio;
	DH *= ratio;
	DB *= ratio;

	// 1Fの中央の各種高さをnormalize
	ratio = GH / (DT2 + DH2 + DB2);
	DT2 *= ratio;
	DH2 *= ratio;
	DB2 *= ratio;
	
	// すべてを画像のサイズにnormalize
	ratio = (float)height / (GH + AH + FH * (NF - 1));
	GH *= ratio;
	FH *= ratio;
	AH *= ratio;
	WT *= ratio;
	WH *= ratio;
	WB *= ratio;
	WT2 *= ratio;
	WH2 *= ratio;
	WB2 *= ratio;
	DT *= ratio;
	DH *= ratio;
	DB *= ratio;
	DT2 *= ratio;
	DH2 *= ratio;
	DB2 *= ratio;
	ratio = (float)width / (SW * 2 + TW * (NC - 1) + TW2);
	SW *= ratio;
	TW *= ratio;
	TW2 *= ratio;
	WS *= ratio;
	WW *= ratio;
	WS2 *= ratio;
	WW2 *= ratio;

	///////////////////////////////////////////////////////////////////////////////////
	// パラメータ値を格納
	params.push_back((float)(NF - range_NF.first) / (range_NF.second - range_NF.first));
	params.push_back((float)(NC - range_NC.first) / (range_NC.second - range_NC.first));
	params.push_back(GH / height);
	params.push_back(FH / height);
	params.push_back(AH / height);
	params.push_back(SW / width);
	params.push_back(TW / width);
	params.push_back(TW2 / width);
	params.push_back(WT / FH);
	params.push_back(WH / FH);
	params.push_back(WB / FH);
	params.push_back(WS / TW);
	params.push_back(WW / TW);
	params.push_back(WT2 / FH);
	params.push_back(WH2 / FH);
	params.push_back(WB2 / FH);
	params.push_back(WS2 / TW2);
	params.push_back(WW2 / TW2);
	params.push_back(DT / GH);
	params.push_back(DH / GH);
	params.push_back(DB / GH);
	params.push_back(DT2 / GH);
	params.push_back(DH2 / GH);
	params.push_back(DB2 / GH);

	return generateFacade(1, width, height, thickness, GH, FH, AH, SW, TW, TW2, WT, WH, WB, WS, WW, WT2, WH2, WB2, WS2, WW2, DT, DH, DB, DT2, DH2, DB2, window_displacement, window_prob);
}

cv::Mat FacadeH::generateFacade(int width, int height, int thickness, const cv::Scalar& bg_color, const cv::Scalar& fg_color, float GH, float FH, float AH, float SW, float TW, float TW2, float WT, float WH, float WB, float WS, float WW, float WT2, float WH2, float WB2, float WS2, float WW2, float DT, float DH, float DB, float DT2, float DH2, float DB2, float window_displacement, float window_prob) {
	cv::Mat result(height, width, CV_8UC3, bg_color);

	int NF = std::round((float)(height - AH - GH) / FH) + 1;
	int NC = std::round((float)(width - SW * 2 - TW2) / TW) + 1;

	window_prob = 1 - utils::genRand(0, 1 - window_prob);

	// 1Fの窓を描画
	{
		// 左半分
		for (int j = 0; j < (NC - 1) / 2; ++j) {
			float x1 = SW + TW * j + WS;
			float y1 = height - DB - DH;
			float x2 = SW + TW * j + WS + WW;
			float y2 = height - DB;

			if (window_displacement > 0) {
				x1 += utils::genRand(-TW * window_displacement, TW * window_displacement);
				y1 += utils::genRand(-GH * window_displacement, GH * window_displacement);
				x2 += utils::genRand(-TW * window_displacement, TW * window_displacement);
				y2 += utils::genRand(-GH * window_displacement, GH * window_displacement);
			}

			if (utils::genRand() < window_prob) {
				cv::rectangle(result, cv::Point(std::round(x1), std::round(y1)), cv::Point(std::round(x2), std::round(y2)), fg_color, thickness);
			}
		}

		// 中央
		{
			float x1 = SW + TW * (NC - 1) / 2 + WS2;
			float y1 = height - DB2 - DH2;
			float x2 = SW + TW * (NC - 1) / 2 + WS2 + WW2;
			float y2 = height - DB2;

			if (window_displacement > 0) {
				x1 += utils::genRand(-TW2 * window_displacement, TW2 * window_displacement);
				y1 += utils::genRand(-GH * window_displacement, GH * window_displacement);
				x2 += utils::genRand(-TW2 * window_displacement, TW2 * window_displacement);
				y2 += utils::genRand(-GH * window_displacement, GH * window_displacement);
			}

			if (utils::genRand() < window_prob) {
				cv::rectangle(result, cv::Point(std::round(x1), std::round(y1)), cv::Point(std::round(x2), std::round(y2)), fg_color, thickness);
			}
		}

		// 右半分
		for (int j = 0; j < (NC - 1) / 2; ++j) {
			float x1 = SW + TW * (NC - 1) / 2 + TW2 + TW * j + WS;
			float y1 = height - DB - DH;
			float x2 = SW + TW * (NC - 1) / 2 + TW2 + TW * j + WS + WW;
			float y2 = height - DB;

			if (window_displacement > 0) {
				x1 += utils::genRand(-TW * window_displacement, TW * window_displacement);
				y1 += utils::genRand(-GH * window_displacement, GH * window_displacement);
				x2 += utils::genRand(-TW * window_displacement, TW * window_displacement);
				y2 += utils::genRand(-GH * window_displacement, GH * window_displacement);
			}

			if (utils::genRand() < window_prob) {
				cv::rectangle(result, cv::Point(std::round(x1), std::round(y1)), cv::Point(std::round(x2), std::round(y2)), fg_color, thickness);
			}
		}
	}

	// 2F以上の窓を描画
	for (int i = 0; i < NF - 1; ++i) {
		// 左半分
		for (int j = 0; j < (NC - 1) / 2; ++j) {
			float x1 = SW + TW * j + WS;
			float y1 = height - GH - FH * i - WB - WH;
			float x2 = SW + TW * j + WS + WW;
			float y2 = height - GH - FH * i - WB;

			if (window_displacement > 0) {
				x1 += utils::genRand(-TW * window_displacement, TW * window_displacement);
				y1 += utils::genRand(-FH * window_displacement, FH * window_displacement);
				x2 += utils::genRand(-TW * window_displacement, TW * window_displacement);
				y2 += utils::genRand(-FH * window_displacement, FH * window_displacement);
			}

			if (utils::genRand() < window_prob) {
				cv::rectangle(result, cv::Point(std::round(x1), std::round(y1)), cv::Point(std::round(x2), std::round(y2)), fg_color, thickness);
			}
		}

		// 中央
		{
			float x1 = SW + TW * (NC - 1) / 2 + WS2;
			float y1 = height - GH - FH * i - WB2 - WH2;
			float x2 = SW + TW * (NC - 1) / 2 + WS2 + WW2;
			float y2 = height - GH - FH * i - WB2;

			if (window_displacement > 0) {
				x1 += utils::genRand(-TW2 * window_displacement, TW2 * window_displacement);
				y1 += utils::genRand(-FH * window_displacement, FH * window_displacement);
				x2 += utils::genRand(-TW2 * window_displacement, TW2 * window_displacement);
				y2 += utils::genRand(-FH * window_displacement, FH * window_displacement);
			}

			if (utils::genRand() < window_prob) {
				cv::rectangle(result, cv::Point(std::round(x1), std::round(y1)), cv::Point(std::round(x2), std::round(y2)), fg_color, thickness);
			}
		}

		// 右半分
		for (int j = 0; j < (NC - 1) / 2; ++j) {
			float x1 = SW + TW * (NC - 1) / 2 + TW2 + TW * j + WS;
			float y1 = height - GH - FH * i - WB - WH;
			float x2 = SW + TW * (NC - 1) / 2 + TW2 + TW * j + WS + WW;
			float y2 = height - GH - FH * i - WB;

			if (window_displacement > 0) {
				x1 += utils::genRand(-TW * window_displacement, TW * window_displacement);
				y1 += utils::genRand(-FH * window_displacement, FH * window_displacement);
				x2 += utils::genRand(-TW * window_displacement, TW * window_displacement);
				y2 += utils::genRand(-FH * window_displacement, FH * window_displacement);
			}

			if (utils::genRand() < window_prob) {
				cv::rectangle(result, cv::Point(std::round(x1), std::round(y1)), cv::Point(std::round(x2), std::round(y2)), fg_color, thickness);
			}
		}
	}

	return result;
}

int FacadeH::clusterWindowTypes(std::vector<std::vector<fs::WindowPos>>& win_rects) {
	for (int i = 0; i < win_rects.size() - 1; ++i) {
		for (int j = 0; j < win_rects[i].size(); ++j) {
			if (j == (win_rects[i].size() - win_rects[i].size() % 2) / 2) {
				win_rects[i][j].type = 0;
			}
			else {
				win_rects[i][j].type = 1;
			}
		}
	}

	for (int j = 0; j < win_rects.back().size(); ++j) {
		if (j == (win_rects.back().size() - win_rects.back().size() % 2) / 2) {
			win_rects.back()[j].type = 2;
		}
		else {
			win_rects.back()[j].type = 3;
		}
	}

	return 4;
}

std::vector<cv::Scalar> FacadeH::getFacadeColors(const std::vector<float>& params, const cv::Mat& facade_img, float width, float height, int cluster_count) {
	std::vector<cv::Scalar> ans;

	int GH = params[0] * 100 / height * facade_img.rows;
	int TW2 = params[5] * 100 / width * facade_img.cols;
	int LEFT = (facade_img.cols - TW2) / 2;
	cv::Mat copy_facade_img = facade_img.clone();
	cv::rectangle(copy_facade_img, cv::Rect(LEFT, 0, TW2, copy_facade_img.rows), cv::Scalar(0, 0, 0), -1);
	ans.push_back(fs::getDominantColor(cv::Mat(facade_img, cv::Rect(LEFT, 0, TW2, facade_img.rows - GH)), cluster_count));
	ans.push_back(fs::getDominantColor(cv::Mat(copy_facade_img, cv::Rect(0, 0, copy_facade_img.cols, copy_facade_img.rows - GH)), cluster_count));
	ans.push_back(fs::getDominantColor(cv::Mat(facade_img, cv::Rect(LEFT, facade_img.rows - GH, TW2, GH)), cluster_count));
	ans.push_back(fs::getDominantColor(cv::Mat(copy_facade_img, cv::Rect(0, copy_facade_img.rows - GH, copy_facade_img.cols, GH)), cluster_count));

	return ans;
}
