/*
 * DebugUtil.cpp
 *
 *  Created on: Dec 14, 2018
 *      Author: aiways
 */

#include "DebugUtil.h"
#include <algorithm>
#include <random>

using namespace std;
using namespace cv;

class FeatureMatchUpdater: public KeyHandler{
public:
	FeatureMatchUpdater(Mat img1, const std::vector<KeyPoint>& keypoints1,
			Mat img2, const std::vector<KeyPoint>& keypoints2,
			const std::vector<DMatch>& matches1to2, int n,DebugUtil * dbg_tuil):m_img1(img1),
			m_keypoints1(keypoints1),m_img2(img2),m_keypoints2(keypoints2),
			m_matches1to2(matches1to2),m_n(n),m_dbg_tuil(dbg_tuil){

	}
	DebugUtil * m_dbg_tuil;
	Mat m_img1;
	const std::vector<KeyPoint>& m_keypoints1;
	Mat m_img2;
	const std::vector<KeyPoint>& m_keypoints2;
	const std::vector<DMatch>& m_matches1to2;
	int m_n;
	virtual void update_img(int key){
		if(key == m_dbg_tuil->m_update_key){
			cout<<"update key pressed"<<endl;
			MatchImageAndDisciption res = m_dbg_tuil->generate_match_img(m_img1, m_keypoints1,m_img2, m_keypoints2,m_matches1to2, m_n);
			m_img = res.m_img;
			m_text = res.m_text;
		}
	};
};

void DebugUtil::matimgs(cv::Mat m){
	return matimg(m);
}
DebugUtil::DebugUtil() {
	// TODO Auto-generated constructor stub
	m_matimg_disp_ration = 0.6;
	m_update_key = 1048693;
	m_matched_point_depth = NULL;
}
static void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	KeyHandler  *p_key_handler = (KeyHandler *)userdata;
	if  ( event == EVENT_LBUTTONDOWN )
	{
		//		cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if  ( event == EVENT_RBUTTONDOWN )
	{
		//		cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if  ( event == EVENT_MBUTTONDOWN )
	{
		//		cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if ( event == EVENT_MOUSEMOVE )
	{
		y = y - p_key_handler->m_title_area_height;
		x = x / p_key_handler->m_resize_ratio;
		y = y / p_key_handler->m_resize_ratio;
		if(x>p_key_handler->m_original_single_img_width){
			x = x - p_key_handler->m_original_single_img_width;
		}

		stringstream ss;
		ss<<"mouse pos x="<<x<<",y="<<y<<endl;
		p_key_handler->m_mouse_pos = ss.str();
		//		cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;

	}
}
void DebugUtil::matimg_internal(cv::Mat m, std::string text, KeyHandler *p_key_handler, std::string title){
	if(m.channels() == 1){
		cv::Mat temp;
		cv::cvtColor(m, temp,cv::COLOR_GRAY2BGR);
		m = temp;
	}

	KeyHandler &key_handler = *p_key_handler;
	string win_name =  "AIWAYS SLAM Image Viewer";

	namedWindow( win_name, WINDOW_AUTOSIZE );// Create a window for display.
	key_handler.m_img = m;
	key_handler.m_text = text;

	//set the callback function for any mouse event
	setMouseCallback(win_name, CallBackFunc, (void *)&key_handler);

	while(true){
		Mat res_img;
		key_handler.m_img.copyTo(res_img);

		if(m.cols > 2000){
			cv::resize(key_handler.m_img, res_img, cv::Size(), m_matimg_disp_ration, m_matimg_disp_ration);
			key_handler.m_resize_ratio = m_matimg_disp_ration;
		}else{
			key_handler.m_resize_ratio = 1.0;
		}
		string desciption_text = key_handler.m_text  + key_handler.m_mouse_pos;
		if(desciption_text != ""){
			res_img = appendtext2img(res_img, desciption_text);
		}
		key_handler.m_title_area_height = 0;
		if(title != ""){
			key_handler.m_title_area_height = res_img.rows;
			res_img = prependtext2img(res_img, title);
			key_handler.m_title_area_height = res_img.rows - key_handler.m_title_area_height;
		}
		imshow(win_name, res_img );
		int key = cv::waitKey(10);

		if(key == -1) {
			//proceed with next loop as no key is pressed.
			continue;
		}
		if(key == 1048677){
			//key `e` is pressed, exit the loop
			break;
		}
		key_handler.update_img(key);
	}

	cout<<"done with image showing"<<endl;
	destroyWindow(win_name);
	for(int i=0; i< 6;i++){
		cv::waitKey(1);
	}
	return;
}
void DebugUtil::matimg(cv::Mat m, std::string text, KeyHandler  key_handler){
	key_handler.m_original_single_img_width = m.cols;
	matimg_internal(m, text, &key_handler);
}
std::vector< DMatch > DebugUtil::gen_rnd_matches(const std::vector< DMatch > &matches, int n, std::vector<int> &selected_inds){
	if(n==-1) return matches;
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<int> distribution(0,matches.size()-1);

	std::vector< DMatch > res;
	for(int i=0; i<n; i++){
		int ind = distribution(gen);
		res.push_back(matches[ind]);
		selected_inds.push_back(ind);
	}
	return res;
}
std::vector<std::string> DebugUtil::split_str(const std::string &s, char delim) {
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> elems;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
		// elems.push_back(std::move(item)); // if C++11 (based on comment from @mchiasson)
	}
	return elems;
}
cv::Mat DebugUtil::appendtext2img(cv::Mat m, std::string text){
	return addtext2img(m, text, true);
}
cv::Mat DebugUtil::prependtext2img(cv::Mat m, std::string text){
	return addtext2img(m, text, false);
}
cv::Mat DebugUtil::addtext2img(cv::Mat m, std::string text, bool append){
	Mat outImg;
	m.copyTo(outImg);

	//draw the blank area with text lines
	int height = outImg.rows;
	int width = outImg.cols;
	vector<string> text_lines;
	text_lines = split_str(text, '\n');
	int blank_height = (text_lines.size() + 1) * 30;
	Mat blank_area = Mat::zeros(blank_height, width, outImg.type());

	if(text!=""){
		int y0 = 25;
		int dy = 30;
		int i = 0;

		for(auto &text_line: text_lines){
			int y = y0 + i*dy;
			cv::putText(blank_area, text_line, cv::Point(5,y), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0,255,0), 1);
			i++;
		}
	}
	//do the concatenation
	Mat res_img;
	if(append){
		vconcat(outImg,blank_area,res_img);
	}else{
		vconcat(blank_area, outImg,res_img);
	}
	return res_img;
}

template<typename T>
std::string DebugUtil::vecstr(std::vector<T> v){
	std::stringstream ss;
	ss<<"[";
	for(size_t i = 0; i < v.size(); ++i)
	{
		if(i != 0)
			ss << ",";
		ss << v[i];
	}
	ss<<"]";
	return ss.str();
}
static cv::Mat drawMatches_1( Mat img1, const std::vector<KeyPoint>& keypoints1,
		Mat img2, const std::vector<KeyPoint>& keypoints2,
		const std::vector<DMatch>& matches1to2){
	Mat res_img;
	RNG& rng = theRNG();
	const int draw_shift_bits = 4;
	const int draw_multiplier = 1 << draw_shift_bits;
	hconcat(img1,img2, res_img);
	for(const auto &m: matches1to2){
		Scalar color = Scalar( rng(256), rng(256), rng(256), 255 );

		Point2f pt1 = keypoints1[m.queryIdx].pt;
		Point2f pt2 = keypoints2[m.trainIdx].pt;
		Point2f dpt2 = Point2f(pt2.x+img1.cols , pt2.y );

		cv::line( res_img,
				Point(cvRound(pt1.x*draw_multiplier), cvRound(pt1.y*draw_multiplier)),
				Point(cvRound(dpt2.x*draw_multiplier), cvRound(dpt2.y*draw_multiplier)),
				color, 3, LINE_AA, draw_shift_bits );
	}

	return res_img;

}
MatchImageAndDisciption DebugUtil::generate_match_img(cv::Mat img1, const std::vector<cv::KeyPoint>& keypoints1,
		cv::Mat img2, const std::vector<cv::KeyPoint>& keypoints2,
		const std::vector<cv::DMatch>& matches1to2, int n){
	Mat img_match;
	std::vector< DMatch > rnd_matches;
	std::vector<int> selected_inds;
	rnd_matches = gen_rnd_matches(matches1to2, n, selected_inds);
	img_match = drawMatches_1 ( img1, keypoints1, img2, keypoints2, rnd_matches);

	//string to describe the rnd matches
	stringstream ss;
	for(const auto match : rnd_matches){
		//query iamge
		ss<<"["<<keypoints1[match.queryIdx].pt.x<<","<<keypoints1[match.queryIdx].pt.y<<"]";
		ss<<",size="<<keypoints1[match.queryIdx].size<<",angle="<<keypoints1[match.queryIdx].angle<<",response="<<keypoints1[match.queryIdx].response;

		//train image
		ss<<"\n["<<keypoints2[match.trainIdx].pt.x<<","<<keypoints2[match.trainIdx].pt.y<<"]";
		ss<<",size="<<keypoints2[match.trainIdx].size<<",angle="<<keypoints2[match.trainIdx].angle<<",response="<<keypoints2[match.trainIdx].response;

		//distance
		ss<<"\nd="<<match.distance;
		//depth if such information exists
		if(m_matched_point_depth){
			float depth = (*m_matched_point_depth)[match.queryIdx];
			ss<<", depth="<<depth;
			ss<<", x="<<(keypoints1[match.queryIdx].pt.x - m_cx) * depth/m_fx;
			ss<<", y="<<(keypoints1[match.queryIdx].pt.y - m_cy) * depth/m_fy;
		}
		ss<<endl;
	}
	ss<<"matched pnts="<<matches1to2.size()<<", current pnts="<<vecstr<int>(selected_inds)<<endl;

	cout<<ss.str()<<endl;
	MatchImageAndDisciption res;
	res.m_img = img_match;
	res.m_text = ss.str();
	return res;

}
void DebugUtil::draw_matches(Mat img1, const std::vector<KeyPoint>& keypoints1,
		Mat img2, const std::vector<KeyPoint>& keypoints2,
		const std::vector<DMatch>& matches1to2, int n, std::string title){
	FeatureMatchUpdater fmu(img1, keypoints1,img2, keypoints2,matches1to2, n,this);
	fmu.m_original_single_img_width = img1.cols;
	fmu.update_img(m_update_key);
	matimg_internal(fmu.m_img, fmu.m_text, &fmu,title);
}
void DebugUtil::draw_matches(cv::Mat img1, const std::vector<cv::KeyPoint>& keypoints1,
		cv::Mat img2, const std::vector<cv::KeyPoint>& keypoints2,
		const std::map<int, cv::DMatch> &matches1to2, int n, std::string title){
	vector<cv::DMatch> matches;
	for(auto const &item : matches1to2){
		matches.push_back(item.second);
	}
	draw_matches(img1, keypoints1,img2, keypoints2,matches, n,title);

}

void DebugUtil::draw_matches(std::string img1, const std::vector<cv::KeyPoint>& keypoints1,
		std::string img2, const std::vector<cv::KeyPoint>& keypoints2,
		const std::map<int, cv::DMatch> &matches1to2, int n){
	std::string title =  "                              " +
			split_str(img1, '/').back() + ",                                 " +
			split_str(img2, '/').back() ;
	draw_matches_internal(img1, keypoints1,img2,keypoints2,matches1to2,n, title );

}
void DebugUtil::draw_matches_internal(std::string img1, const std::vector<cv::KeyPoint>& keypoints1,
		std::string img2, const std::vector<cv::KeyPoint>& keypoints2,
		const std::map<int, cv::DMatch> &matches1to2, int n, std::string title){
	cv::Mat img1_temp = cv::imread(img1, CV_LOAD_IMAGE_COLOR);
	cv::Mat img2_temp = cv::imread(img2, CV_LOAD_IMAGE_COLOR);

	draw_matches(img1_temp, keypoints1,img2_temp, keypoints2,matches1to2, n,title);

}
void DebugUtil::draw_matches_internal(std::string img1, const std::vector<cv::KeyPoint>& keypoints1,
		std::string img2, const std::vector<cv::KeyPoint>& keypoints2,
		const std::vector<cv::DMatch> &matches1to2, int n, std::string title){
	cv::Mat img1_temp = cv::imread(img1, CV_LOAD_IMAGE_COLOR);
	cv::Mat img2_temp = cv::imread(img2, CV_LOAD_IMAGE_COLOR);
	draw_matches(img1_temp, keypoints1,img2_temp, keypoints2,matches1to2, n,title);

}
DebugUtil::~DebugUtil() {
	// TODO Auto-generated destructor stub
}

string DebugUtil::type2str(int type) {
	string r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch ( depth ) {
	case CV_8U:  r = "8U"; break;
	case CV_8S:  r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}

	r += "C";
	r += (chans+'0');

	return r;
}

string DebugUtil::mattype(cv::Mat m){
	return type2str(m.type());
}

std::string DebugUtil::matstr(cv::Mat m){
	stringstream ss;
	ss<<"dims="<<m.dims<<",rows="<<m.rows<<",cols="<<m.cols;
	ss<<","<<mattype(m)<<" : ";
	ss<<m;
	string s = ss.str();
	s.erase(std::remove(s.begin(), s.end(), '\n'),
			s.end());
	return s;
}

std::string DebugUtil::matrow(cv::Mat m, int n){
	return matstr(m.row(n));
}

std::string DebugUtil::matcol(cv::Mat m, int n){
	return matstr(m.col(n));
}

std::string DebugUtil::matele(cv::Mat mat, int row, int col){
	stringstream ss;
	const int type = mat.type();

	if(CV_32F == type){
		ss<<mat.at<float>(row, col);
	}else if(CV_64F == type){
		ss<<mat.at<double>(row, col);
	}else if(CV_8U == type){
		ss<<mat.at<char>(row, col);
	}
	return ss.str();
}

DebugUtil g_dbg;

