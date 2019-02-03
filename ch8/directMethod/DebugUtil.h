/*
 * DebugUtil.h
 *
 *  Created on: Dec 14, 2018
 *      Author: aiways
 */

#ifndef DEBUGUTIL_H_
#define DEBUGUTIL_H_
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include "DebugUtil.h"



class MatchImageAndDisciption{
public:
	cv::Mat m_img;
	std::string m_text;
};

class KeyHandler: public  MatchImageAndDisciption{
public:
	KeyHandler(){
		m_mouse_pos = "";
		m_resize_ratio = 1.0;
		m_original_single_img_width = 0;
		m_title_area_height = 0;
	}
	virtual void update_img(int key){
	};
	virtual ~KeyHandler(){};
	std::string m_mouse_pos;
	float m_resize_ratio;
	int m_original_single_img_width;
	int m_title_area_height;
};

class DebugUtil {
public:
	DebugUtil();
	void matimgs(cv::Mat m);
	void matimg(cv::Mat m, std::string text="", KeyHandler key_handler=KeyHandler());
	void draw_matches(cv::Mat img1, const std::vector<cv::KeyPoint>& keypoints1,
			cv::Mat img2, const std::vector<cv::KeyPoint>& keypoints2,
			const std::vector<cv::DMatch>& matches1to2, int n, std::string title="");
	void draw_matches(cv::Mat img1, const std::vector<cv::KeyPoint>& keypoints1,
				cv::Mat img2, const std::vector<cv::KeyPoint>& keypoints2,
				const std::map<int, cv::DMatch> &matches1to2, int n, std::string title="");
	void draw_matches(std::string img1, const std::vector<cv::KeyPoint>& keypoints1,
					std::string img2, const std::vector<cv::KeyPoint>& keypoints2,
					const std::map<int, cv::DMatch> &matches1to2, int n);

	virtual ~DebugUtil();
	MatchImageAndDisciption generate_match_img(cv::Mat img1, const std::vector<cv::KeyPoint>& keypoints1,
					cv::Mat img2, const std::vector<cv::KeyPoint>& keypoints2,
					const std::vector<cv::DMatch>& matches1to2, int n);
	std::string mattype(cv::Mat m);
	std::string matstr(cv::Mat m);
	std::string matrow(cv::Mat m, int n);
	std::string matcol(cv::Mat m, int n);
	std::string matele(cv::Mat mat, int row, int col);
	template<typename T>
	std::string vecstr(std::vector<T> v);
	int m_update_key;
	//used to calculate world coordinate x and y when drawing matched point
	float m_fx,m_fy,m_cx,m_cy, m_b;
protected:
	//to store the depth of the matched points, used in draw match function.
	std::vector<float> *m_matched_point_depth;

	void draw_matches_internal(std::string img1, const std::vector<cv::KeyPoint>& keypoints1,
						std::string img2, const std::vector<cv::KeyPoint>& keypoints2,
						const std::map<int, cv::DMatch> &matches1to2, int n, std::string title="");
	void draw_matches_internal(std::string img1, const std::vector<cv::KeyPoint>& keypoints1,
							std::string img2, const std::vector<cv::KeyPoint>& keypoints2,
							const std::vector<cv::DMatch> &matches1to2, int n, std::string title="");
	cv::Mat addtext2img(cv::Mat m, std::string text, bool append=true);
	cv::Mat prependtext2img(cv::Mat m, std::string text);
	std::string type2str(int type);
	void matimg_internal(cv::Mat m, std::string text="", KeyHandler *p_key_handler= NULL, std::string title="");
	std::vector<std::string> split_str(const std::string &s, char delim);
	cv::Mat appendtext2img(cv::Mat img, std::string text);
	std::vector< cv::DMatch > gen_rnd_matches(const std::vector< cv::DMatch > &matches, int n, std::vector<int> &selected_inds);

	float m_matimg_disp_ration;
};

extern DebugUtil g_dbg;

#endif /* DEBUGUTIL_H_ */
