// -*- C++ -*-
/*!
 * @file  RockPaperScissors.cpp
 * @brief RockpaperScissors check compoenet
 * @date $Date$
 *
 * $Id$
 */

#include "RockPaperScissors.h"
#define Label LabelingBS

using namespace std;



// Module specification
// <rtc-template block="module_spec">
static const char* rockpaperscissors_spec[] =
  {
    "implementation_id", "RockPaperScissors",
    "type_name",         "RockPaperScissors",
    "description",       "RockpaperScissors check compoenet",
    "version",           "1.2.0",
    "vendor",            "AIST",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.rock_max", "1.0",
    "conf.default.rock_min", "0.85",
    "conf.default.scissor_max", "0.85",
    "conf.default.scissor_min", "0.7",
    "conf.default.paper_max", "0.7",
    "conf.default.paper_min", "0.5",
    "conf.default.iterations", "4",
    "conf.default.out_mode", "1",
    // Widget
    "conf.__widget__.rock_max", "text",
    "conf.__widget__.rock_min", "text",
    "conf.__widget__.scissor_max", "text",
    "conf.__widget__.scissor_min", "text",
    "conf.__widget__.paper_max", "text",
    "conf.__widget__.paper_min", "text",
    "conf.__widget__.iterations", "text",
    "conf.__widget__.out_mode", "radio",
    // Constraints
    "conf.__constraints__.rock_max", "0<=x<=1.0",
    "conf.__constraints__.rock_min", "0<=x<=1.0",
    "conf.__constraints__.scissor_max", "0<=x<=1.0",
    "conf.__constraints__.scissor_min", "0<=x<=1.0",
    "conf.__constraints__.paper_max", "0<=x<=1.0",
    "conf.__constraints__.paper_min", "0<=x<=1.0",
    "conf.__constraints__.out_mode", "(0,1)",
    ""
  };
// </rtc-template>

Label *createLabeling(){
	return new LabelingBS();
}

int exec(Label *label, cv::Mat target, cv::Mat result,
		 const bool is_sort_region,int region_size_min){
	return label->Exec((unsigned char *)target.data,(short *)result.data,
		target.size().width, target.size().height, is_sort_region, region_size_min);
}

int getNumOfResultRegions(Label *label){
	return label->GetNumOfResultRegions();
}

void releaseLabeling(Label *label){
	delete label;
}

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
RockPaperScissors::RockPaperScissors(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_img_inputIn("image_input", m_img_input),
    m_img_outputOut("image_output", m_img_output),
    m_resultOut("result", m_result)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
RockPaperScissors::~RockPaperScissors()
{
}



RTC::ReturnCode_t RockPaperScissors::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("image_input", m_img_inputIn);
  
  // Set OutPort buffer
  addOutPort("image_output", m_img_outputOut);
  addOutPort("result", m_resultOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("rock_max", m_rock_max, "1.0");
  bindParameter("rock_min", m_rock_min, "0.85");
  bindParameter("scissor_max", m_scissor_max, "0.85");
  bindParameter("scissor_min", m_scissor_min, "0.7");
  bindParameter("paper_max", m_paper_max, "0.7");
  bindParameter("paper_min", m_paper_min, "0.5");
  bindParameter("iterations", m_iterations, "4");
  bindParameter("out_mode", m_out_mode, "1");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RockPaperScissors::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RockPaperScissors::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RockPaperScissors::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t RockPaperScissors::onActivated(RTC::UniqueId ec_id)
{


  m_prev_judge = "";

  return RTC::RTC_OK;
}


RTC::ReturnCode_t RockPaperScissors::onDeactivated(RTC::UniqueId ec_id)
{
  

  if (!m_image_buff.empty())
  {
	  m_image_buff.release();
  }
  if (!m_hsv_buff.empty())
  {
	  m_hsv_buff.release();
  }
  if (!m_convexHull_buff.empty())
  {
	  m_convexHull_buff.release();
  }
  if (!m_skin_buff.empty())
  {
	  m_skin_buff.release();
  }
  if (!m_temp_buff.empty())
  {
	  m_temp_buff.release();
  }
  if (!m_label_buff.empty())
  {
	  m_label_buff.release();
  }
  if (!m_output_buff.empty())
  {
	  m_output_buff.release();
  }

  return RTC::RTC_OK;
}


RTC::ReturnCode_t RockPaperScissors::onExecute(RTC::UniqueId ec_id)
{
	
  /* 新データのチェック */
  if(m_img_inputIn.isNew()){
    /* データの読み込み */
    m_img_inputIn.read();

	m_image_buff.create(cv::Size(m_img_input.width, m_img_input.height), CV_8UC3);
	m_hsv_buff.create(cv::Size(m_img_input.width, m_img_input.height), CV_8UC3);
	m_convexHull_buff.create(cv::Size(m_img_input.width, m_img_input.height), CV_8UC3);
	m_skin_buff.create(cv::Size(m_img_input.width, m_img_input.height), CV_8UC1);
	m_temp_buff.create(cv::Size(m_img_input.width, m_img_input.height), CV_8UC1);
	m_label_buff.create(cv::Size(m_img_input.width, m_img_input.height), CV_16SC1);
	m_output_buff.create(cv::Size(m_img_input.width, m_img_input.height), CV_8UC3);

	

	
    /* InPortの映像の取得 */
    memcpy(m_image_buff.data,(void *)&(m_img_input.pixels[0]),m_img_input.pixels.length());
    /* 肌色を抽出する */
    extractSkinColor();
    /* 欠損領域を補間する */
    interpolate();
	
    /* ラベリングを行う */
    Label *labeling = createLabeling();
	cv::Mat pointMatrix;  /* 手領域用行列 */
	cv::Mat hullMatrix;   /* ConvexHull用行列 */

    exec( labeling, m_skin_buff, m_label_buff, true, IGNORE_SIZE );
	
    if(getNumOfResultRegions( labeling ) > 0 )
    {
      /* IGNORE_SIZEよりも大きな領域があった場合 */
      int handarea;     /* 手領域の面積 */
      int hullarea;     /* ConvexHull内の面積 */
      int hullcount;    /* ConvexHullの頂点の数 */
      //std::vector<cv::Point> handpoint; /* 手領域内の点の座標配列 */
      //std::vector<int> hull;        /* ConvexHullの頂点のhandpointにおけるindex番号 */
      

      /* 最大領域(手領域)の抽出を行う */
      handarea = pickupMaxArea();

      /* ConvexHullを生成する */
      createConvexHull( handarea, pointMatrix, hullMatrix );
	  
      //hullcount = hullMatrix.cols;
	  hullcount = hullMatrix.rows;

      /* ConvexHullを描画する */
	  drawConvexHull(pointMatrix, hullMatrix, hullcount);
	 
      /* ConvexHull内の面積を求める */
	  hullarea = calcConvexHullArea(pointMatrix, hullMatrix, hullcount);
	  
      /* ジャンケンの判定を行う */
      decide( handarea, hullarea );

      /* メモリを解放する */
      
	  

    } else {

      /* 画像を初期化する */
		m_convexHull_buff = cv::Mat::zeros(cv::Size(m_img_input.width, m_img_input.height), CV_8UC3);
    }
	
    releaseLabeling( labeling );
	


    /* 左上が原点の場合 */
    //cv::flip( m_skin_buff, m_skin_buff, 0 );
	

    /* 左上が原点の場合 */
    //cv::flip( m_convexHull_buff, m_convexHull_buff, 0 );
	
	std::vector<cv::Mat> tmp;
	tmp.push_back(m_skin_buff);
	tmp.push_back(m_skin_buff);
	tmp.push_back(m_skin_buff);
	cv::Mat test_mat = cv::Mat(cv::Size(m_img_input.width, m_img_input.height), CV_8UC3);
	cv::merge(tmp, test_mat);
	
    /* 画像データのサイズ取得 */
	int len = (m_convexHull_buff.channels() * m_convexHull_buff.size().width * m_convexHull_buff.size().height);
	
    m_img_output.pixels.length(len);

    /* 該当のイメージをMemCopyする */
	memcpy((void *)&(m_img_output.pixels[0]), m_convexHull_buff.data, len);
	

    /* 反転した画像データをOutPortから出力する */
	m_img_output.width = m_image_buff.size().width;
	m_img_output.height = m_image_buff.size().height;
	
    m_img_outputOut.write();

	

    }

    return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RockPaperScissors::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RockPaperScissors::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RockPaperScissors::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RockPaperScissors::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RockPaperScissors::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

//
//	肌色を抽出する
//
void RockPaperScissors::extractSkinColor( void )
{
	cv::Vec3b color;   /* HSV表色系で表した色 */
  unsigned char h;  /* H成分 */
  unsigned char s;  /* S成分 */
  unsigned char v;  /* V成分 */

  /* BGRからHSVに変換する */
  cv::cvtColor(m_image_buff, m_hsv_buff, cv::COLOR_BGR2HSV);

  /* 肌色抽出 */
  for (int x = 0; x < m_hsv_buff.size().width; x++)
  {
	  for (int y = 0; y < m_hsv_buff.size().height; y++)
    {
		color = m_hsv_buff.at<cv::Vec3b>(cv::Point(x, y));
      h = color.val[0];
      s = color.val[1];
      v = color.val[2];

      if( h <= HMAX && h >= HMIN &&
          s <= SMAX && s >= SMIN &&
          v <= VMAX && v >= VMIN )
      {
        /* 肌色の場合 */
		  m_skin_buff.at<uchar>(cv::Point(x, y)) = 255;
        //cvSetReal2D( m_skin_buff, y, x, 255 );
      } else {
		  m_skin_buff.at<uchar>(cv::Point(x, y)) = 0;
        //cvSetReal2D( m_skin_buff, y, x, 0 );
      }
    }
  }
}

//
//	欠損領域を補間する
//
void RockPaperScissors::interpolate( void )
{
  /* 膨張をITERATIONS回行う */
  cv::dilate( m_skin_buff, m_temp_buff, m_iterations );

  /* 収縮をITERATIONS回行う */
  cv::erode( m_temp_buff, m_skin_buff, m_iterations );
}

//
//	最大領域(手領域)の抽出を行う
//
//	戻り値:
//		手領域の面積
//
int RockPaperScissors::pickupMaxArea( void )
{
  int handarea = 0;   /* 手領域の面積 */

  for (int x = 0; x < m_label_buff.size().width; x++)
  {
	  for (int y = 0; y < m_label_buff.size().height; y++)
    {
		if (m_label_buff.at<short>(cv::Point(x, y)) == 1)
      {
        /* 最大領域だった場合 */
        handarea++;
		m_convexHull_buff.at<cv::Vec3b>(cv::Point(x, y)) = cv::Vec3b(255, 255, 255);
        
      } else {
		  m_skin_buff.at<uchar>(cv::Point(x, y)) = 0;
		  m_convexHull_buff.at< cv::Vec3b>(cv::Point(x, y)) = cv::Vec3b(0, 0, 0);
      }
    }
  }
  return handarea;
}

//
//	ConvexHullを生成する
//
//	引数:
//		handarea    : 手領域の面積(点の数)
//		handpoint   : 手領域内の点の座標配列へのポインタ
//		hull        : ConvexHullの頂点のhandpointにおけるindex番号へのポインタ
//		pointMatrix : 手領域用行列へのポインタ
//		hullMatrix  : ConvexHull用行列へのポインタ
//
void RockPaperScissors::createConvexHull(int handarea, cv::Mat &pointMatrix, cv::Mat &hullMatrix )
{
  int i=0;

  /* ConvexHullを計算するために必要な行列を生成する */
  
  //*handpoint=( cv::Point * )malloc( sizeof( CvPoint ) * handarea );
	//*hull = ( int * )malloc( sizeof( int ) * handarea );
	
  pointMatrix.create(cv::Point(handarea, 1), CV_32SC2);
  hullMatrix.create(cv::Point(handarea, 1), CV_32SC1);
  

  
  for( int x = 0; x < m_skin_buff.size().width; x++ )
  {
	  for (int y = 0; y < m_skin_buff.size().height; y++)
    {
		if (m_skin_buff.at<uchar>(cv::Point(x,y)) == 255)
      {
		  pointMatrix.at<cv::Vec2i>(i) = cv::Vec2i(x, y);
		  
		  //hull[i] = 0;
        i++;
      }
    }
  }

  
  //pointMatrix.copyTo(handpoint);
  //hullMatrix.copyTo(hull);
  

  /* ConvexHullを生成する */
#if CV_MAJOR_VERSION < 3
  cv::convexHull( pointMatrix, hullMatrix, CV_CLOCKWISE, 0 );
#else
  cv::convexHull( pointMatrix, hullMatrix, true, 0 );
#endif
}

//
//	ConvexHullを描画する
//
//	引数:
//		handpoint       : 手領域内の点の座標配列
//		hull            : ConvexHullの頂点のhandpointにおけるindex番号
//		hullcount       : ConvexHullの頂点の数
//
void RockPaperScissors::drawConvexHull(cv::Mat &handpoint, cv::Mat &hull, int hullcount)
{
	
	cv::Point pt0 = handpoint.at<cv::Vec2i>(hull.at<long>(hullcount - 1));
	
  for( int i = 0; i < hullcount; i++ )
  {
	  
	  cv::Point pt = handpoint.at<cv::Vec2i>(hull.at<long>(i));
	  
    cv::line( m_convexHull_buff, pt0, pt, CV_RGB( 0, 255, 0 ) );
    pt0 = pt;
  }
}

//
//	ConvexHull内の面積を求める
//
//	引数:
//		handpoint       : 手領域内の点の座標配列
//		hull            : ConvexHullの頂点のhandpointにおけるindex番号
//		hullcount       : ConvexHullの頂点の数　　
//
//	戻り値:
//		ConvexHull内の面積
//
int RockPaperScissors::calcConvexHullArea(cv::Mat &handpoint, cv::Mat &hull, int hullcount)
{
  /* ConvexHullの頂点からなる行列を生成 */
  //cv::Point *hullpoint = ( cv::Point * )malloc( sizeof( CvPoint ) * hullcount );
	//std::vector<cv::Point> hullpoint;

  
	cv::Mat hMatrix;
	hMatrix.create(cv::Point(1, hullcount), CV_32SC2);

  for( int i = 0; i < hullcount; i++ )
  {
	  hMatrix.at<cv::Vec2i>(i) = handpoint.at<cv::Vec2i>(hull.at<long>(i));
  }
 
  //hMatrix.copyTo(hullpoint);
  

  /* ConvexHull内の点の数を数える */
  int hullarea = 0;
  for( int x = 0; x < m_convexHull_buff.size().width; x++ )
  {
	  for (int y = 0; y < m_convexHull_buff.size().height; y++)
    {
      if( cv::pointPolygonTest( hMatrix, cv::Point2f( x, y ), 0 ) > 0)
      {
        hullarea++;
      }
    }
  }


  return hullarea;
}


//
//	ジャンケンの判定を行う
//
//	引数:
//		handarea : 手領域の面積
//		hullarea : ConvexHull内の面積
//
void RockPaperScissors::decide( int handarea, int hullarea )
{
  double ratio; /* ConvexHull内の面積に対する手領域の面積の割合 */
  string err_judge = "Threshold out of range";
  string judge;

  ratio=handarea / ( double )hullarea;	
  
  if( ratio >= m_rock_min && ratio <= m_rock_max ) {
    judge = "Rock";
  }
  else if( ratio >= m_scissor_min && ratio <= m_scissor_max )
  {
    judge = "Scissors";
  }
  else if( ratio >= m_paper_min && ratio <= m_paper_max )
  {
    judge = "Paper";
  }

  if( m_out_mode == 1 )
  {
    /* 前回とジャンケンの種類が異なる場合のみ出力する
     * ConvexHull領域は一定以上の大きさでないと、ジャンケンの判定は意味がない
     * ここでは暫定的に10000以上の領域に対してジャンケンを判定するものとする
     */
    if( judge != err_judge && m_prev_judge != judge && hullarea >= 10000 )
    {
      //printf( "Ratio = %lf  : 判定領域の大きさ = %d\n", ratio,  hullarea );
	  printf("Ratio = %lf  : Size of hand region = %d\n", ratio, hullarea);
      printf( "%s\n", judge.c_str() );
      m_prev_judge = judge;

      m_result.data = judge.c_str();
      m_resultOut.write();
    }
  }
  else
  {
    /* すべての結果を出力する */
    //printf( "Ratio = %lf  : 判定領域の大きさ = %d\n", ratio,  hullarea );
	 printf("Ratio = %lf  : Size of hand region = %d\n", ratio, hullarea);
    printf( "%s\n", judge.c_str() );
  }
}


extern "C"
{
 
  void RockPaperScissorsInit(RTC::Manager* manager)
  {
    coil::Properties profile(rockpaperscissors_spec);
    manager->registerFactory(profile,
                             RTC::Create<RockPaperScissors>,
                             RTC::Delete<RockPaperScissors>);
  }
  
};


