/*

*/

#ifndef __MF_CAPTURE_H__
#define __MF_CAPTURE_H__

#include "MFUtils.h"

#include <opencv2/opencv.hpp>

enum FMT_Values {
  FMT_RGB24, FMT_YUY2, FMT_UNKNOWN=256
};

/*
 * Helper Class for Video Capture using Media Foudation
 *
 */
class MFCapture
{
  /*
   *  Hepler Class for  Supported Video Format
   */
   class VideoFormat
   {
    /*
     *  Properties
     */
     FMT_Values format;
     UINT32 width, height;
     UINT32 index;
     std::string name;

   public:
     /*
      * Construcors
      */
     VideoFormat(): format(FMT_UNKNOWN), width(0),height(0), index(-1), next(NULL), prev(NULL) { toString(); }
     
     VideoFormat(FMT_Values fmt, UINT32 w, UINT32 h, UINT32 idx): format(fmt), width(w),height(h),
           index(idx),next(NULL), prev(NULL) { toString(); }

     /*
      * Deconstructor
      */
     ~VideoFormat()
     {
       VideoFormat *tmp=this->next;
       if(tmp){ delete tmp; }
       this->next=NULL;
     }

     /*
      * Compare Video Format
      */
     BOOL equalto(FMT_Values fmt, UINT32 w, UINT32 h)
     {
       if(this->format == fmt && this->width == w && this->height == h) return TRUE;
       return FALSE;
     }

     BOOL equalto(std::string var)
     {
       return (name == var);
     }

     /*
      *  Check Format List
      */
     BOOL isMember(FMT_Values fmt, UINT32 w, UINT32 h)
     {
       VideoFormat *tmp = this;
       do {
         if(tmp->equalto(fmt,w,h)){ return TRUE; }
         else tmp = tmp->next;
       } while(tmp);

       return FALSE;
     }

     /*
      * Append format
      */
     void append(FMT_Values fmt, UINT32 w, UINT32 h, UINT32 idx)
     {
       if(isMember(fmt, w, h) == FALSE)
       {
         VideoFormat *lst = last();
         lst->next = new VideoFormat(fmt, w, h, idx);
         lst->next->prev = lst;
       }
     }

     /*
      *  Convert to string
      */
     std::string toString()
     {
       if(format == FMT_RGB24)
       {
         name = std::string("RGB24_");
       }else if(format == FMT_YUY2){
         name = std::string("YUY2_");
       }else{
         name = std::string("UNKNOWN_");
       }
       name = name + std::to_string(width);
       name = name + std::string("x") + std::to_string(height);
       return name;
     }

     /*
      *  Print format list to stdout
      */
     void printAll()
     {
       VideoFormat *tmp=this;
       std::cout << "====== printAll ==========" << std::endl;
       do {
         std::cout << tmp->index << ": " << tmp->name << std::endl;
         tmp = tmp->next;
       } while(tmp);
     }

     /*
      * Return length of format list
      */
     UINT32 length()
     {
       UINT32 len=0;
       VideoFormat *tmp=this->top();
       do {
         len += 1;
         if(tmp->next == NULL) { break;}
         tmp = tmp->next;
       } while(tmp);

       return len;
     }

     /*
      * get VideoFormat by nth value from the format list
      */
     VideoFormat *get(UINT32 idx)
     {
       UINT32 pos=0;
       VideoFormat *tmp=this->top();
       do {
         pos += 1;
         if(pos > idx) { break;}
         tmp = tmp->next;
       } while(tmp);

       return tmp;
     }

     /*
      * get the index value of the format list from an index of IMFMedeiaType List
      */
     INT32 getIndex(DWORD idx)
     {
       INT32 res = -1;

       VideoFormat *tmp=this->top();
       do {
         res += 1;
         if(tmp->index == idx) { return res;}
         if(tmp->next == NULL) { break;}
         tmp = tmp->next;
       } while(tmp);

       return -1;
     }

     /*
      * get the next diffrent index value of IMFMedeiaType List
      *  Input: 'idx' is the index value of IMFMediaType List 
      */
     INT32 getNextId(DWORD idx)
     {
       INT32 n = getIndex(idx)+1;
       INT32 len = length();
       INT32 res = 0;
       if(len == n)
       {
         res = get(1)->index;
       }else{
         res = get(n)->index;
       }
       return res;
     }

     /*
      *  get the index value
      */
     INT32 getIndexByName(const char* str)
     {
       UINT32 pos=0;
       VideoFormat *tmp=this->top();
       do {
         pos += 1;
         if(tmp->equalto(std::string(str))) { return tmp->index; }
         if(tmp->next == NULL) { break;}
         tmp = tmp->next;
       } while(tmp);

       return -1;
     }

     /*
      *  get the last VideoFormat from the format list
      */
     VideoFormat *last()
     {
       VideoFormat *tmp=this;
       do {
         if(tmp->next == NULL) { return tmp;}
         tmp = tmp->next;
       } while(tmp);

       return NULL;
     }

     /*
      *  get the first VideoFormat from the format list
      */
     VideoFormat *top()
     {
       VideoFormat *tmp=this;
       do {
         if(tmp->prev == NULL) { return tmp;}
         tmp = tmp->prev;
       } while(tmp);

       return NULL;
     }

     /*
      * pointers for the format list
      */
     VideoFormat *next;
     VideoFormat *prev;
   };  /// end of VideoFormat Class.

public:    
  /*
   *  Methods of MFCapture
   */
  MFCapture();
  ~MFCapture();
  /*
   *  Activate Capture Device
   */
  HRESULT ActivateDevice(INT32 id=0);
  /*
   *  Shutdown Capture Device
   */
  HRESULT ShutdownDevice();

  /*
   * get IMFMediaTypeHandler
   */
  HRESULT GetMediaTypeHandler();
  /*
   * get the current IMFMediaType from the capture device
   */
  HRESULT GetCurrentType();
  /*
   * get the attributes of IMFMediaType.
   * If the arg equal to NULL, the current IMFMediaType used
   */
  HRESULT GetMediaTypeInfo(IMFMediaType *type=NULL);
  /*
   * Create Image Buffer for OpenCV operations.
   * This function allocate image buffer.
   */
  void CreateImageBuffer(UINT depth=IPL_DEPTH_8U, UINT32 channels=3);
  /*
   *  Create SourceReader from media source.
   */
  HRESULT CreateSourceReader();
  /*
   *  get IMFSample  
   */
  HRESULT GetSample();
  /*
   *  get the image data
   *   The image data will store in image_buf(IplImage).
   *   If the video format is 'YUY2', the date will convert to RGB24 format.
   */
  cv::Mat MFCapture::GetBufferData();

  /*
   * Enumarate available capture device.
   * This function shoule called in 'AvtivateDevice'.
   */
  HRESULT EnumerateCaptureFormats();

  /*
   *  Set Video Format by Index of IMFMEdiaType list
   */
  HRESULT SetDeviceFormat(DWORD idx);

  /*
   *  Set Video Format by  the name of VideoFormat
   */
  INT32 SetDeviceFormatByName(const char *name);

  /*
   * Display summary of IMFMediaType's attribute
   */
  HRESULT ShowMediaTypeInfo(IMFMediaType *pType=NULL, int i=0);

  /*
   * Display list of VideoFormat 
   */
  void PrintFormatList(){ this->video_format->printAll(); }

  /*
   *  get avaiable the next index of IMFMediaType list
   */
  INT32 GetNextFormatID(INT32 id){ return this->video_format->getNextId(id); }

  /*
   *  get avaiable IMFMediaType Index by Name
   */
  INT32 GetFormatIndexByName(char *buf){ return this->video_format->getIndexByName(buf); }

  /*
   * Old method initialize capture device
   */
  HRESULT SelectDevice(UINT32 idx);
  /*
   * Test implementation for MFT
   */
  HRESULT EnumerateMFT(GUID inType, GUID outType);
  IMFSample *ConvertToRGB();
protected:
  /*
   *  Free all elements of outMediaTypes.
   */
  HRESULT FreeOutMediaTypes();

private:
  /*
   *  Handler
   */
  HRESULT hr;

public:
  /*
   * Attribute
   */
  IMFAttributes *pAttributes;
  /*
   *  Number of video source devices.
   */ 
  UINT32 nDevs;
  /*
   *   Device ID = The index value of video source device list.
   */ 
  INT32 devId;
  /*
   *  IMFActivate for the video source device list
   */
  IMFActivate **ppDevices;

  /*
   *  Media Source
   */
  IMFMediaSource *pSource;
  /*
   * Handler for MediaType
   */
  IMFMediaTypeHandler *pHandler;
  /*
   * List of availabel MediaTypes
   */
  IMFMediaType **outMediaTypes;
  DWORD numOutMediaTypes;
  /*
   *  Media  Foundation Reader.
   */
  IMFSourceReader *pReader;
  /*
   *  Sampler
   */
  IMFSample *pSample;

  /*
   *  ID of YUV Convertion Matrix
   */
  UINT32 YUV_Matrix;

  /*
   * Current MediaType
   */
  IMFMediaType *pType;
  GUID   image_format;

  INT32  image_stride;
  UINT32 image_length;
  UINT32 image_width, image_height;
  cv::Mat image_buf;

  /*
   *  List of VideoFormat
   */
  class VideoFormat *video_format;

  ////  TEST for MediaFoundation Transform
  IMFTransform *pMFT;
};


#endif
