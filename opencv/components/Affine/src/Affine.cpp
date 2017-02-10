// -*- C++ -*-
/*!
 * @file  Affine.cpp
 * @brief Affine image component
 * @date $Date$
 *
 * $Id$
 */

#include "Affine.h"
#include <iostream>

using namespace std;


// Module specification
// <rtc-template block="module_spec">
static const char* affine_spec[] =
  {
    "implementation_id", "Affine",
    "type_name",         "Affine",
    "description",       "Affine image component",
    "version",           "1.2.0",
    "vendor",            "AIST",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.affine_matrix", "0.825,-0.167,40;-0.1,0.83,30",
    // Widget
    "conf.__widget__.affine_matrix", "text",
    // Constraints
    ""
  };
// </rtc-template>

std::istream& operator>>(std::istream& is, std::vector<std::vector<double> >& v)
{
    size_t nRow, nCol;
    std::string strOrig;
    std::vector<std::string> strRow;
    std::vector<std::string> strCol;

    is >> strOrig;

    strRow = coil::split(strOrig ,";");
    nRow = strRow.size();
    v.resize(nRow);
    
    for(size_t i = 0; i < nRow; ++i)
    {

        strCol = coil::split(strRow[i] ,",");
        nCol = strCol.size();
        v[i].resize(nCol);

        for(size_t j = 0; j < nCol; ++j)
        {
            double tv;

            if (coil::stringTo(tv, strCol[j].c_str()))
            {
                v[i][j] = tv;
            }
        }
    }
  
    return is;
}

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
Affine::Affine(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_image_origIn("original_image", m_image_orig),
    m_image_affineOut("affined_image", m_image_affine)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
Affine::~Affine()
{
}



RTC::ReturnCode_t Affine::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("original_image", m_image_origIn);
  
  // Set OutPort buffer
  addOutPort("affined_image", m_image_affineOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("affine_matrix", m_ve2dbMatrix, "0.825,-0.167,40;-0.1,0.83,30");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Affine::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Affine::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Affine::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t Affine::onActivated(RTC::UniqueId ec_id)
{




  return RTC::RTC_OK;
}


RTC::ReturnCode_t Affine::onDeactivated(RTC::UniqueId ec_id)
{


  


  return RTC::RTC_OK;
}


RTC::ReturnCode_t Affine::onExecute(RTC::UniqueId ec_id)
{    
  /* Common CV actions */
  /* 新しいデータのチェック */
  if (m_image_origIn.isNew()) 
  {
    /* InPortデータの読み込み */
    m_image_origIn.read();
	


	cv::Mat m_image_buff(cv::Size(m_image_orig.width, m_image_orig.height), CV_8UC3, (void *)&(m_image_orig.pixels[0]));
    /* InPortの画像データをIplImageのimageDataにコピー */
	//memcpy(m_image_buff.data, (void *)&(m_image_orig.pixels[0]), m_image_orig.pixels.length());
	
	cv::Mat_<double> m_affineMatrix(2, 3);
   	/* 変換後の座標を設定する */
	
    // Check configuration validations
    if(isConfigurationValidated())
    {
		m_affineMatrix(0, 0) = m_ve2dbMatrix[0][0];
		m_affineMatrix(0, 1) = m_ve2dbMatrix[0][1];
		m_affineMatrix(0, 2) = m_ve2dbMatrix[0][2];
		m_affineMatrix(1, 0) = m_ve2dbMatrix[1][0];
		m_affineMatrix(1, 1) = m_ve2dbMatrix[1][1];
		m_affineMatrix(1, 2) = m_ve2dbMatrix[1][2];

           
    }else
    {
      cout<<"Incorrect configuration information."<<endl;

      return RTC::RTC_ERROR;
    }

	
	
	cv::Mat m_image_dest;
    /* 変換行列を反映させる */
	warpAffine(m_image_buff, m_image_dest, m_affineMatrix, m_image_dest.size());

    /* 画像データのサイズ取得 */
	int len = m_image_dest.channels() * m_image_dest.size().width * m_image_dest.size().height;

    /* 画面のサイズ情報を入れる */

	m_image_affine.pixels.length(len);
	m_image_affine.width = m_image_dest.size().width;
	m_image_affine.height = m_image_dest.size().height;

    /* 反転した画像データをOutPortにコピー */
    memcpy((void *)&(m_image_affine.pixels[0]), m_image_dest.data,len);

    /* 反転した画像データをOutPortから出力する */
    m_image_affineOut.write();
  }

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Affine::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Affine::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Affine::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Affine::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Affine::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/* Martixのサイズだけチェックする */
bool Affine::isConfigurationValidated()
{
  /* AffineのMartixのサイズは2*3 */
  if(m_ve2dbMatrix.size() < 2)
    return false;

  if(m_ve2dbMatrix[0].size() < 3)
    return false;
  if(m_ve2dbMatrix[1].size() < 3)
    return false;

  return true;
}



extern "C"
{
 
  void AffineInit(RTC::Manager* manager)
  {
    coil::Properties profile(affine_spec);
    manager->registerFactory(profile,
                             RTC::Create<Affine>,
                             RTC::Delete<Affine>);
  }
  
};


