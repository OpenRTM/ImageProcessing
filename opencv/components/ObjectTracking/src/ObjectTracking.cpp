// -*- C++ -*-
/*!
 * @file  ObjectTracking.cpp
 * @brief Objecttrack component
 * @date $Date$
 *
 * $Id$
 */

#include "ObjectTracking.h"

const float	ObjectTracking::hRangesArray[2] = { H_RANGE_MIN, H_RANGE_MAX };	/* ヒストグラムのレンジ */

// Module specification
// <rtc-template block="module_spec">
static const char* objecttracking_spec[] =
  {
    "implementation_id", "ObjectTracking",
    "type_name",         "ObjectTracking",
    "description",       "Objecttrack component",
    "version",           "1.2.0",
    "vendor",            "AIST",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    ""
  };
// </rtc-template>


//
//	マウスドラッグによって初期追跡領域を指定する
//
//	引数:
//		event	: マウス左ボタンの状態
//		x		: マウスが現在ポイントしているx座標
//		y		: マウスが現在ポイントしているy座標
//		flags	: 本プログラムでは未使用
//		param	: 本プログラムでは未使用
//
void ObjectTracking::on_mouse(int event, int x, int y, int flags, void* param){
  /* 画像が取得されていなければ、処理を行わない */
  if( resultImage.empty() ){
        return;
  }

  /* 原点の位置に応じてyの値を反転（画像の反転ではない） */
  //if( resultImage->origin == 1 ){
  //      y = resultImage.size().height - y;
  //}
  /* マウスの左ボタンが押されていれば以下の処理を行う */
  if( selectObject == SELECT_ON ){
    selection.x = MIN( x, origin.x );
    selection.y = MIN( y, origin.y );
    selection.width = selection.x + CV_IABS( x - origin.x );
    selection.height = selection.y + CV_IABS( y - origin.y );

    selection.x = MAX( selection.x, 0 );
    selection.y = MAX( selection.y, 0 );
    selection.width = MIN( selection.width, resultImage.size().width );
	selection.height = MIN(selection.height, resultImage.size().height);
    selection.width = selection.width - selection.x;
    selection.height = selection.height - selection.y;
  }
  /* マウスの左ボタンの状態によって処理を分岐 */
  switch( event ){
    case CV_EVENT_LBUTTONDOWN:
      /* マウスの左ボタンが押されたのであれば、原点および選択された領域を設定 */
      origin = cv::Point( x, y );
      selection = cv::Rect( x, y, 0, 0 );
      selectObject = SELECT_ON;
      break;
    case CV_EVENT_LBUTTONUP:
      /* マウスの左ボタンが離されたとき、widthとheightがどちらも正であれば、*/
      /* trackObjectフラグをTRACKING_STARTにする */
      selectObject = SELECT_OFF;
      if( selection.width > 0 && selection.height > 0 ){
        trackObject = TRACKING_START;
      }
      break;
  }
}

//
//	入力された1つの色相値をRGBに変換する
//
//	引数:
//		hue		: HSV表色系における色相値H
//	戻り値：
//		CvScalar: RGBの色情報がBGRの順で格納されたコンテナ
//
cv::Scalar ObjectTracking::hsv2rgb(float hue){
  cv::Mat rgbValue, hsvValue;
  rgbValue.create(cv::Size(1, 1), CV_8UC3);
  hsvValue.create(cv::Size(1, 1), CV_8UC3);

  hsvValue.data[0] = hue;	/* 色相値H */
  hsvValue.data[1] = 255;	/* 彩度値S */
  hsvValue.data[2] = 255;	/* 明度値V */

  /* HSV表色系をRGB表色系に変換する */
  cv::cvtColor( hsvValue, rgbValue, CV_HSV2BGR );


  return cv::Scalar((unsigned char)rgbValue.data[0], (unsigned char)rgbValue.data[1],
					(unsigned char)rgbValue.data[2], 0);


}


//
//	マウス選択された初期追跡領域におけるHSVのH値でヒストグラムを作成し、ヒストグラムの描画までを行う
//
//	引数:
//		hist		: mainで宣言されたヒストグラム用構造体
//		hsvImage	: 入力画像がHSV表色系に変換された後のIplImage
//		maskImage	: マスク画像用IplImage
//		selection	: マウスで選択された矩形領域
//
void ObjectTracking::CalculateHist(cv::MatND &hist, cv::Mat &hsvImage, cv::Mat &maskImage, cv::Rect &selection){
  int		i;
  int		binW;	/* ヒストグラムの各ビンの、画像上での幅 */
  int		val;	/* ヒストグラムの頻度 */
  double	maxVal;	/* ヒストグラムの最大頻度 */

  /* hsv画像の各画素が値の範囲内に入っているかチェックし、 */
  /* マスク画像maskImageを作成する */
  cv::inRange(hsvImage,
        cv::Scalar( H_RANGE_MIN, S_MIN, MIN(V_MIN,V_MAX), 0 ),
        cv::Scalar( H_RANGE_MAX, S_MAX, MAX(V_MIN,V_MAX), 0 ), 
        maskImage );
  /* hsvImageのうち、とくに必要なHチャンネルをhueImageとして分離する */
  std::vector<cv::Mat> tmp;
  tmp.push_back(hueImage);
  tmp.push_back(cv::Mat());
  tmp.push_back(cv::Mat());
  cv::split(hsvImage, tmp);
  
  /* trackObjectがTRACKING_START状態なら、以下の処理を行う */
  if( trackObject == TRACKING_START ){
    /* 追跡領域のヒストグラム計算とhistImageへの描画 */
    maxVal = 0.0;
	//hueImage.adjustROI(selection.y + selection.size().height, selection.y, selection.x, selection.x + selection.size().width);
	//maskImage.adjustROI(selection.y + selection.size().height, selection.y, selection.x, selection.x + selection.size().width);
	
	cv::Mat hueImage_roi = hueImage(selection);
	cv::Mat maskImage_roi = maskImage(selection);
	

    /* ヒストグラムを計算し、最大値を求める */
	int hbins = 30, sbins = 32;
	//int histSize[] = { hbins, sbins };
	int histSize = 128;
	float hranges[] = { 0, 180 };
	float sranges[] = { 0, 256 };
	const float* ranges[] = { hranges, sranges };
	int image_num = 1;
	int channels[] = { 0 };
	
	
	
	cv::calcHist(&hueImage_roi, image_num, channels, maskImage_roi, hist, 1, &histSize, ranges);
	
	
	cv::minMaxLoc(hist, 0, &maxVal, 0, 0);
	

    /* ヒストグラムの縦軸（頻度）を0-255のダイナミックレンジに正規化 */
    /*if( maxVal == 0.0 ){
		hist.convertTo(hist, CV_8U, 0.0, 0);
      //cvConvertScale( hist->bins, hist->bins, 0.0, 0 );
    } else{
		hist.convertTo(hist, CV_8U, 255.0 / maxVal, 0);
      //cvConvertScale( hist->bins, hist->bins, 255.0 / maxVal, 0 );
    }*/
	normalize(hist, hist, 0, 255, cv::NORM_MINMAX, -1, cv::Mat());
	
    /* hue,mask画像に設定されたROIをリセット */
    trackWindow = selection;
	//trackWindow &= cv::Rect(0, 0, hueImage_roi.cols, hueImage_roi.rows);

    /* trackObjectをTRACKING_NOWにする */
    trackObject = TRACKING_NOW;

    /* ヒストグラム画像をゼロクリア */
	histImage = cv::Mat::zeros(histImage.size(), CV_MAKETYPE(histImage.depth(),histImage.channels()));
    
    /* 各ビンの幅を決める */
	binW = histImage.size().width / histSize;
    /* ヒストグラムを描画する */
	
	
	//cv::normalize(hist, hist, (double)histImage.size().height);
	for (i = 0; i < histSize; i++){
		//val = cvRound(hist.at<float>(i));
      cv::Scalar color = hsv2rgb( i * 180.0 / hdims );
	  /*cv::rectangle(histImage,
            cv::Point( i * binW, histImage.size().height ), 
			cv::Point((i + 1) * binW, histImage.size().height - val),
            color,
            -1, 
            8, 
            0	);*/
	  
	  cv::rectangle(histImage,
		  cv::Point(i * binW, histImage.size().height),
		  cv::Point((i + 1) * binW, histImage.size().height - hist.at<uchar>(i)),
		  cv::Scalar::all(255),               // 矩形の色
		  CV_FILLED                           // 矩形の枠線の太さ。CV_FILLEDの場合塗りつぶし
		  );
	 
    }
  }
  
}

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
ObjectTracking::ObjectTracking(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_orig_imgIn("img_original", m_orig_img),
    m_eventIn("m_event", m_event),
    m_xIn("m_x", m_x),
    m_yIn("m_y", m_y),
    m_out_imgOut("img_output", m_out_img),
    m_hist_imgOut("img_hist", m_hist_img)

    // </rtc-template>
{
	backprojectMode = HIDDEN_BACKPROJECTION;
	selectObject = SELECT_OFF;
	trackObject = TRACKING_STOP;
	showHist = SHOW_HISTOGRAM;
	hdims = H_DIMENSION;		/* ヒストグラムの次元数 */
	
	
	vmin = V_MIN;
	vmax = V_MAX;
}

/*!
 * @brief destructor
 */
ObjectTracking::~ObjectTracking()
{
}



RTC::ReturnCode_t ObjectTracking::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("img_original", m_orig_imgIn);
  addInPort("m_event", m_eventIn);
  addInPort("m_x", m_xIn);
  addInPort("m_y", m_yIn);
  
  // Set OutPort buffer
  addOutPort("img_output", m_out_imgOut);
  addOutPort("img_hist", m_hist_imgOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ObjectTracking::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ObjectTracking::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ObjectTracking::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t ObjectTracking::onActivated(RTC::UniqueId ec_id)
{	
  g_temp_w = 0;
  g_temp_h = 0;



  return RTC::RTC_OK;
}


RTC::ReturnCode_t ObjectTracking::onDeactivated(RTC::UniqueId ec_id)
{


  if (!inputImage.empty())
  {
	  inputImage.release();
  }
  if (!resultImage.empty())
  {
	  resultImage.release();
  }
  if (!hsvImage.empty())
  {
	  hsvImage.release();
  }


  return RTC::RTC_OK;
}


RTC::ReturnCode_t ObjectTracking::onExecute(RTC::UniqueId ec_id)
{
	int		i;
	int		j;

	int x;
	int y;
	int mouse_event;

	/* Snake用のパラメータ */
	float alpha = 1.0;		/* 連続エネルギーの重みパラメータ */
	float beta = 0.5;		/* 曲率の重みパラメータ */
	float gamma = 1.5;		/* 画像エネルギーの重みパラメータ */
	cv::Point pt[SEGMENT];	/* 制御点の座標 */
	cv::Size window;			/* 最小値を探索する近傍サイズ */
	window.width = WINDOW_WIDTH;
	window.height = WINDOW_HEIGHT;
	cv::TermCriteria crit(cv::TermCriteria::MAX_ITER, ITERATION_SNAKE, 1.0);


	if (m_orig_imgIn.isNew()){

		m_orig_imgIn.read();

		/* 各メモリ確保 */
		if (inputImage.empty())
		{
			inputImage.create(cv::Size(m_orig_img.width, m_orig_img.height), CV_8UC3);
		}
		else if (g_temp_w != m_orig_img.width || g_temp_h != m_orig_img.height)
		{
			inputImage.create(cv::Size(m_orig_img.width, m_orig_img.height), CV_8UC3);
		}
		if (resultImage.empty())
		{
			resultImage.create(cv::Size(m_orig_img.width, m_orig_img.height), CV_8UC3);
		}
		else if (g_temp_w != m_orig_img.width || g_temp_h != m_orig_img.height)
		{
			resultImage.create(cv::Size(m_orig_img.width, m_orig_img.height), CV_8UC3);
		}
		if (hsvImage.empty())
		{
			hsvImage.create(cv::Size(m_orig_img.width, m_orig_img.height), CV_8UC3);
		}
		else if (g_temp_w != m_orig_img.width || g_temp_h != m_orig_img.height)
		{
			hsvImage.create(cv::Size(m_orig_img.width, m_orig_img.height), CV_8UC3);
		}
		if (hueImage.empty())
		{
			hueImage.create(cv::Size(m_orig_img.width, m_orig_img.height), CV_8UC1);
		}
		else if (g_temp_w != m_orig_img.width || g_temp_h != m_orig_img.height)
		{
			hueImage.create(cv::Size(m_orig_img.width, m_orig_img.height), CV_8UC1);
		}
		if (maskImage.empty())
		{
			maskImage.create(cv::Size(m_orig_img.width, m_orig_img.height), CV_8UC1);
		}
		else if (g_temp_w != m_orig_img.width || g_temp_h != m_orig_img.height)
		{
			maskImage.create(cv::Size(m_orig_img.width, m_orig_img.height), CV_8UC1);
		}
		if (backprojectImage.empty())
		{
			backprojectImage.create(cv::Size(m_orig_img.width, m_orig_img.height), CV_8UC1);
		}
		else if (g_temp_w != m_orig_img.width || g_temp_h != m_orig_img.height)
		{
			backprojectImage.create(cv::Size(m_orig_img.width, m_orig_img.height), CV_8UC1);
		}
		if (grayImage.empty())
		{
			grayImage.create(cv::Size(m_orig_img.width, m_orig_img.height), CV_8UC1);
		}
		else if (g_temp_w != m_orig_img.width || g_temp_h != m_orig_img.height)
		{
			grayImage.create(cv::Size(m_orig_img.width, m_orig_img.height), CV_8UC1);
		}
		/* ヒストグラム用の画像を確保し、ゼロクリア */
		if (histImage.empty())
		{
			histImage.create(cv::Size(m_orig_img.width, m_orig_img.height), CV_8UC3);
		}
		else if (g_temp_w != m_orig_img.width || g_temp_h != m_orig_img.height)
		{
			histImage.create(cv::Size(m_orig_img.width, m_orig_img.height), CV_8UC3);
		}
		histImage = cv::Mat::zeros(cv::Size(m_orig_img.width, m_orig_img.height), CV_8UC3);


		//resultImage->origin = inputImage->origin;





		/* ヒストグラム構造体の使用を宣言 */
		if (hist.empty()){
			hist = cv::Mat::zeros(1, hdims, CV_8UC1);
		}


		/* InPortの映像の取得 */
		memcpy(inputImage.data, (void *)&(m_orig_img.pixels[0]), m_orig_img.pixels.length());

		/* キャプチャされた画像をresultImageにコピーし、HSV表色系に変換してhsvImageに格納 */
		inputImage.copyTo(resultImage);
		cv::cvtColor(resultImage, hsvImage, CV_BGR2HSV);

		/* WindowのEvent情報の取得に対する処理 */
		if (m_eventIn.isNew() && m_xIn.isNew() && m_yIn.isNew()){
			m_xIn.read();
			m_yIn.read();
			m_eventIn.read();

			x = m_x.data;
			y = m_y.data;
			mouse_event = m_event.data;

			on_mouse(mouse_event, x, y, 0, 0);

			x = 0;
			y = 0;
			mouse_event = 0;
		}

		/* trackObjectフラグがTRACKING_STOP以外なら、以下の処理を行う */
		if (trackObject != TRACKING_STOP){
			
			/* 追跡領域のヒストグラム計算と描画 */
			CalculateHist(hist, hsvImage, maskImage, selection);

			const float *ranges[] = { hRangesArray };
			int image_num = 1;
			int channels[] = { 0 };
			int dnum = 1;
			
			
			/* バックプロジェクションを計算する */
			cv::calcBackProject(&hueImage, image_num, channels, hist, backprojectImage, ranges);
			
			/* backProjectionのうち、マスクが1であるとされた部分のみ残す */
			cv::bitwise_and(backprojectImage, maskImage, backprojectImage);
			
			trackComp = cv::TermCriteria(cv::TermCriteria::EPS | cv::TermCriteria::COUNT, 10, 1);
			
			/* CamShift法による領域追跡を実行する */
			trackRegion = cv::CamShift(backprojectImage,
				trackWindow,
				trackComp);
			cv::ellipse(resultImage, trackRegion, cv::Scalar(0, 0, 255), 3);

			//trackWindow = trackComp.rect;

			/* SnakeImage用のグレースケール画像を作成する */
			/*cv::cvtColor(resultImage, grayImage, cv::COLOR_BGR2GRAY);

			if (backprojectMode == SHOW_BACKPROJECTION){
				cv::cvtColor(backprojectImage, resultImage, CV_GRAY2BGR);
			}*/
			//if (resultImage->origin == 1){
			//	trackRegion.angle = -trackRegion.angle;
			//}

			/* CamShiftでの領域追跡結果をSnakeの初期位置に設定する */
			/*for (i = 0; i < SEGMENT; i++){
				pt[i].x = cvRound(trackRegion.size.width
					* cos(i * 6.28 / SEGMENT + trackRegion.angle)
					/ 2.0 + trackRegion.center.x);
				pt[i].y = cvRound(trackRegion.size.height
					* sin(i * 6.28 / SEGMENT + trackRegion.angle)
					/ 2.0 + trackRegion.center.y);
			}*/
			cv::Mat grayImage_roi = grayImage(trackWindow);
			cv::Mat resultImage_roi = resultImage(trackWindow);
			double qualityLevel = 0.01;
			double minDistance = 10;
			std::vector<cv::Point2f> corners;
			int blockSize = 3;
			bool useHarrisDetector = false;
			double k = 0.04;
			/* Snakeによる輪郭抽出を実行する */
			cv::goodFeaturesToTrack(grayImage_roi, corners, ITERATION_SNAKE, qualityLevel, minDistance, cv::Mat(), blockSize, useHarrisDetector, k);
			for (unsigned i = 1; i < corners.size(); i++){


				/* 各輪郭点の間に線をひいて輪郭線を描画する */
				cv::line(resultImage_roi, corners[i], corners[i-1],
					cv::Scalar(0, 0, 255, 0), 2, 8, 0);
			}
			
		}

		/* マウスで選択中の初期追跡領域の色を反転させる */
		if (selectObject == SELECT_ON && selection.width > 0 && selection.height > 0){

			//resultImage = resultImage.adjustROI(selection.y + selection.size().height, selection.y, selection.x, selection.x + selection.size().width);
			cv::Mat roi = resultImage(selection);
			cv::bitwise_xor(roi, cv::Scalar::all(255), roi);
			//cvResetImageROI( resultImage );
		}
		/* backprojectImageの座標原点が左上の場合、上下を反転させる */
		//if( backprojectImage->origin == 0 ){
		//  cv::flip( backprojectImage, backprojectImage, 0 );
		//}

		/* 画像データのサイズ取得 */
		/*std::vector<cv::Mat> tmp;
		tmp.push_back(backprojectImage);
		tmp.push_back(backprojectImage);
		tmp.push_back(backprojectImage);
		cv::merge(tmp, resultImage);*/
		double len = (resultImage.channels() * resultImage.size().width * resultImage.size().height);
		double leng = (histImage.channels() * histImage.size().width * histImage.size().height);
		m_out_img.pixels.length(len);
		m_hist_img.pixels.length(leng);

		/* 該当のイメージをMemCopyする */
		memcpy((void *)&(m_out_img.pixels[0]), resultImage.data, len);
		memcpy((void *)&(m_hist_img.pixels[0]), histImage.data, leng);

		/* 反転した画像データをOutPortから出力する */
		m_out_img.width = inputImage.size().width;
		m_out_img.height = inputImage.size().height;

		m_hist_img.width = inputImage.size().width;
		m_hist_img.height = inputImage.size().height;

		m_out_imgOut.write();
		m_hist_imgOut.write();


	}

	return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ObjectTracking::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ObjectTracking::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ObjectTracking::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ObjectTracking::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ObjectTracking::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void ObjectTrackingInit(RTC::Manager* manager)
  {
    coil::Properties profile(objecttracking_spec);
    manager->registerFactory(profile,
                             RTC::Create<ObjectTracking>,
                             RTC::Delete<ObjectTracking>);
  }
  
};


