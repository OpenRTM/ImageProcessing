/*


*/
#include "MFUtils.h"

/*
 *   Support Function
 */
LPCSTR GetFormatName(const GUID &guid)
{
  IF_EQUAL_RETURN(guid, MFVideoFormat_AI44);
  IF_EQUAL_RETURN(guid, MFVideoFormat_ARGB32);
  IF_EQUAL_RETURN(guid, MFVideoFormat_AYUV);
  IF_EQUAL_RETURN(guid, MFVideoFormat_DV25);
  IF_EQUAL_RETURN(guid, MFVideoFormat_DV50);
  IF_EQUAL_RETURN(guid, MFVideoFormat_DVH1);
  IF_EQUAL_RETURN(guid, MFVideoFormat_DVSD);
  IF_EQUAL_RETURN(guid, MFVideoFormat_DVSL);
  IF_EQUAL_RETURN(guid, MFVideoFormat_H264);
  IF_EQUAL_RETURN(guid, MFVideoFormat_I420);
  IF_EQUAL_RETURN(guid, MFVideoFormat_IYUV);
  IF_EQUAL_RETURN(guid, MFVideoFormat_M4S2);
  IF_EQUAL_RETURN(guid, MFVideoFormat_MJPG);
  IF_EQUAL_RETURN(guid, MFVideoFormat_MP43);
  IF_EQUAL_RETURN(guid, MFVideoFormat_MP4S);
  IF_EQUAL_RETURN(guid, MFVideoFormat_MP4V);
  IF_EQUAL_RETURN(guid, MFVideoFormat_MPG1);
  IF_EQUAL_RETURN(guid, MFVideoFormat_MSS1);
  IF_EQUAL_RETURN(guid, MFVideoFormat_MSS2);
  IF_EQUAL_RETURN(guid, MFVideoFormat_NV11);
  IF_EQUAL_RETURN(guid, MFVideoFormat_NV12);
  IF_EQUAL_RETURN(guid, MFVideoFormat_P010);
  IF_EQUAL_RETURN(guid, MFVideoFormat_P016);
  IF_EQUAL_RETURN(guid, MFVideoFormat_P210);
  IF_EQUAL_RETURN(guid, MFVideoFormat_P216);
  IF_EQUAL_RETURN(guid, MFVideoFormat_RGB24);
  IF_EQUAL_RETURN(guid, MFVideoFormat_RGB32);
  IF_EQUAL_RETURN(guid, MFVideoFormat_RGB555);
  IF_EQUAL_RETURN(guid, MFVideoFormat_RGB565);
  IF_EQUAL_RETURN(guid, MFVideoFormat_RGB8);
  IF_EQUAL_RETURN(guid, MFVideoFormat_UYVY);
  IF_EQUAL_RETURN(guid, MFVideoFormat_v210);
  IF_EQUAL_RETURN(guid, MFVideoFormat_v410);
  IF_EQUAL_RETURN(guid, MFVideoFormat_WMV1);
  IF_EQUAL_RETURN(guid, MFVideoFormat_WMV2);
  IF_EQUAL_RETURN(guid, MFVideoFormat_WMV3);
  IF_EQUAL_RETURN(guid, MFVideoFormat_WVC1);
  IF_EQUAL_RETURN(guid, MFVideoFormat_Y210);
  IF_EQUAL_RETURN(guid, MFVideoFormat_Y216);
  IF_EQUAL_RETURN(guid, MFVideoFormat_Y410);
  IF_EQUAL_RETURN(guid, MFVideoFormat_Y416);
  IF_EQUAL_RETURN(guid, MFVideoFormat_Y41P);
  IF_EQUAL_RETURN(guid, MFVideoFormat_Y41T);
  IF_EQUAL_RETURN(guid, MFVideoFormat_YUY2);
  IF_EQUAL_RETURN(guid, MFVideoFormat_YV12);
  IF_EQUAL_RETURN(guid, MFVideoFormat_YVYU);
  return NULL;
}

/*
 * Startup function to use Media Foundation
 *
 */
HRESULT
startMediaFoundation()
{
  HRESULT hr;
  hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
  if (FAILED(hr)) {
    std::cerr << "Failed initializing COM components" << std::endl;
    return hr;
  }

  hr = MFStartup(MF_VERSION);
  if (FAILED(hr)){
      std::cout << "ERROR: Fail to start Media Foundation." << std::endl;
  }

  return hr;
}

/*
 *  Finishing function for Media Foundation
 */
void
endMediaFoundation()
{
  MFShutdown();
  CoUninitialize();
}


/*
 * Support Function: IMFMediaType
 * Get GUID Value
 */
HRESULT
GetGUID_FromMediaType(IMFMediaType *type, REFGUID key, GUID *guid)
{
  HRESULT hr;
  PROPVARIANT var;

  PropVariantInit(&var);

  hr = type->GetItem(key, &var);
  if (SUCCEEDED(hr)){
     if (var.vt == VT_CLSID) {
      *guid = *var.puuid;
    }else{
      return E_FAIL;
    }
  }
  return hr;
}

/*
 * Support Function: IMFMediaType
 * Get UINT32 Value
 *
 */
HRESULT 
GetUINT32_FromMediaType(IMFMediaType *type, REFGUID key, UINT32 *length)
{
  HRESULT hr;
  PROPVARIANT var;
  *length = 0;
  PropVariantInit(&var);
  hr = type->GetItem(key, &var);
  if (FAILED(hr)){
    return hr;
  }
  *length = var.ulVal;
  return hr;
}

/*
 * Support Function: IMFMediaType
 * Get INT32 Value
 *
 */
HRESULT 
GetINT32_FromMediaType(IMFMediaType *type, REFGUID key, INT32 *length)
{
  HRESULT hr;
  PROPVARIANT var;
  *length = 0;
  PropVariantInit(&var);
  hr = type->GetItem(key, &var);
  if (FAILED(hr)){
    return hr;
  }
  *length = (INT32)var.ulVal;
  return hr;
}

/*
 * Support Function: IMFMediaType
 * Get two UINT32 Values
 *
 */
HRESULT
GetTwoUINT32_FromMediaType(IMFMediaType *type, REFGUID key, UINT32 *v1, UINT32 *v2)
{
  HRESULT hr;
  PROPVARIANT var;
  *v1 = 0;
  *v2 = 0;
  PropVariantInit(&var);

  hr = type->GetItem(key, &var);
  if (FAILED(hr)){
    return hr;
  }
  Unpack2UINT32AsUINT64(var.uhVal.QuadPart, v1, v2);
  return hr;
}

/*
 * Compare IMFMediaType by Subtype and FrameSize 
 * 
 */
BOOL
IsSameMediaType(IMFMediaType *type, GUID format, UINT32 width, UINT32 height)
{
  HRESULT hr;
  PROPVARIANT var;
  PropVariantInit(&var);

  hr = type->GetItem(MF_MT_MAJOR_TYPE, &var);
  if (FAILED(hr)){ return FALSE; }
  if (var.vt != VT_CLSID || *var.puuid != MFMediaType_Video) { return FALSE; }

  hr = type->GetItem(MF_MT_SUBTYPE, &var);
  if (FAILED(hr)){ return FALSE; }
  if (var.vt != VT_CLSID || *var.puuid != format) { return FALSE; }

  hr = type->GetItem(MF_MT_FRAME_SIZE, &var);
  if (FAILED(hr)){ return FALSE; }
  if (var.uhVal.QuadPart == Pack2UINT32AsUINT64(width, height) ){ return FALSE; }

  return TRUE;
}

/*
 * Compare IMFMediaType by Subtype
 *
 */
BOOL
CheckMediaFormat(IMFMediaType *type, GUID format)
{
  HRESULT hr;
  PROPVARIANT var;
  PropVariantInit(&var);

  hr = type->GetItem(MF_MT_MAJOR_TYPE, &var);
  if (FAILED(hr)){ return FALSE; }
  if (var.vt != VT_CLSID || *var.puuid != MFMediaType_Video) { return FALSE; }

  hr = type->GetItem(MF_MT_SUBTYPE, &var);
  if (FAILED(hr)){ return FALSE; }
  if (var.vt != VT_CLSID || *var.puuid != format) { return FALSE; }

  return TRUE;
}

/*
 * Compare IMFMediaType by Major type
 */
BOOL
IsVideoType(IMFMediaType *type)
{
  GUID guid;
  HRESULT hr = GetGUID_FromMediaType(type, MF_MT_MAJOR_TYPE, &guid);

  if (SUCCEEDED(hr)){ if (guid == MFMediaType_Video) {  return TRUE; } }
  return FALSE;
}


/*
 * Color Image Conversion. YUV -> RGB
 *  http://msdn.microsoft.com/en-us/library/aa904813%28VS.80%29.aspx#yuvformats_2
 */
int clip255(int x)
{
  if(x > 255) return 255;
  if(x < 0)   return 0;
  return x;
}

int clipd255(double x)
{
  if(x > 255) return 255;
  if(x < 0)   return 0;
  return (int)x;
}

int 
YUY2_to_RGB(char *yuy2_image, char *rgb_image, int rgb_img_width, int rgb_img_height, int yuy2_img_size, int conv_type)
{
  int i, j;
  int Y0, V0, U0, Y1;
  int p, stride;
  unsigned int nCount = 0;
  int count = yuy2_img_size;

  rgb_img_height /= 2;

  for (i= 0; i< rgb_img_height; i++)
  {
    stride = i*rgb_img_width;
    for (j= 0; j< rgb_img_width; j++)
    {
      count -= 4;
      if (count < 0)
      {
         return nCount;
      }

      p = (stride+j) * 4;

      Y0=(yuy2_image[ p   ] & 0xff) ;
      V0=(yuy2_image[ p+1 ] & 0xff) - 128;
      Y1=(yuy2_image[ p+2 ] & 0xff) ;
      U0=(yuy2_image[ p+3 ] & 0xff) - 128;

      if(conv_type == 1){ /// BT709
        rgb_image[nCount++]=clipd255((Y0            +1.574800*V0));
        rgb_image[nCount++]=clipd255((Y0-0.187324*U0-0.468124*V0));
        rgb_image[nCount++]=clipd255((Y0+1.855600*U0));

        rgb_image[nCount++]=clipd255((Y1            +1.574800*V0));
        rgb_image[nCount++]=clipd255((Y1-0.187324*U0-0.468124*V0));
        rgb_image[nCount++]=clipd255((Y1+1.855600*U0));

      }else if (conv_type == 2){ /// BT601
        rgb_image[nCount++]=clipd255((Y0            +1.402000*V0));
        rgb_image[nCount++]=clipd255((Y0-0.344136*U0-0.714136*V0));
        rgb_image[nCount++]=clipd255((Y0+1.77200*U0));

        rgb_image[nCount++]=clipd255((Y1            +1.402000*V0));
        rgb_image[nCount++]=clipd255((Y1-0.344136*U0-0.714136*V0));
        rgb_image[nCount++]=clipd255((Y1+1.77200*U0));

      }else if (conv_type == 3){ /// SMPTE240M
        rgb_image[nCount++]=clipd255((Y0            +1.576000*V0));
        rgb_image[nCount++]=clipd255((Y0-0.227000*U0-0.477000*V0));
        rgb_image[nCount++]=clipd255((Y0+1.826000*U0));

        rgb_image[nCount++]=clipd255((Y1            +1.576000*V0));
        rgb_image[nCount++]=clipd255((Y1-0.227000*U0-0.477000*V0));
        rgb_image[nCount++]=clipd255((Y1+1.826000*U0));

      }else{
        Y0 -= 16;
        Y1 -= 16;

#if 0
        rgb_image[nCount++]=clipd255((1.164383*Y0            +1.596027*V0));
        rgb_image[nCount++]=clipd255((1.164383*Y0-0.391762*U0-0.812968*V0));
        rgb_image[nCount++]=clipd255((1.164383*Y0+2.017232*U0));

        rgb_image[nCount++]=clipd255((1.164383*Y1            +1.596027*V0));
        rgb_image[nCount++]=clipd255((1.164383*Y1-0.391762*U0-0.812968*V0));
        rgb_image[nCount++]=clipd255((1.164383*Y1+2.017232*U0));
#else
      rgb_image[nCount++]=clip255((298*Y0      + 409*V0+128) >> 8);
      rgb_image[nCount++]=clip255((298*Y0-100*U0-208*V0+128) >> 8);
      rgb_image[nCount++]=clip255((298*Y0+516*U0       +128) >> 8);

      rgb_image[nCount++]=clip255((298*Y1      + 409*V0+128) >> 8);
      rgb_image[nCount++]=clip255((298*Y1-100*U0-208*V0+128) >> 8);
      rgb_image[nCount++]=clip255((298*Y1+516*U0       +128) >> 8);
#endif
      }
    }
  }
  return nCount;
}
