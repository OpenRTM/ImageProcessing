/*


*/
#ifndef _MF_UTIL_H_
#define _MF_UTIL_H_

#include <stdio.h>
#include <tchar.h>

#include <iostream>
#include <string>


#ifdef _WIN32_WINNT
#undef _WIN32_WINNT  //0x601
#endif

#include <new>
#include <windows.h>
#include <windowsx.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mferror.h>
#include <mfcaptureengine.h>
#include <mfreadwrite.h>
#include <shlwapi.h>
#include <strsafe.h>
#include <commctrl.h>
#include <d3d11.h>


#pragma comment(lib, "powrprof.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "mfreadwrite.lib")


#ifndef IF_EQUAL_RETURN
#define IF_EQUAL_RETURN(param, val) if(val == param) return #val
#endif

#ifndef FORMAT_MATCH
#define FORMAT_MATCH(param, val, type) if(val == param) return type
#endif


/*
  FUNCTION: SafeRelese (from MSDN)

*/
template <class T> void SafeRelease(T **ppT)
{
  if (*ppT)
  {
    (*ppT)->Release();
    *ppT = NULL;
  }
}

/*
   Functions
*/
LPCSTR GetFormatName(const GUID &format);

/*
 * Start/End Media Foundation
 */
HRESULT startMediaFoundation();
void endMediaFoundation();

/*
 * Get Values from IMFMediaType
 * Attribute Keys of IMFMediaType:
 *    MF_MT_MAJOR_TYPE:  Major type GUID (Audio or Video)
 *    MF_MT_SUBTYPE:     Subtype GUID
 *    MF_MT_FORMAT_TYPE: Format GUID
 *    MF_MT_SAMPLE_SIZE: Size of each sample, in btyes
 *    MF_MT_ALL_SAMPLES_INDEPENDT
 *    MF_MT_FIXED_SIZE_SAMPLES
 *    MF_MT_COMPRESSED
 *    MF_WRAPPED_TYPE
 *    MF_USER_DATA
 *
 *    Video Format Attributes:
 *    MF_MT_AVG_BIT_ERROR_RATE
 *    MF_MT_AVG_BITRATE
 *    MF_MT_CUSTOM_VIDEO_PRIMARIES
 *    MF_MT_DEFAULT_STRIDE
 *    MF_MT_DRM_FLAGS
 *    MF_MT_FRAME_RATE
 *    MF_MT_FRAME_RATE_RANGE_MAX
 *    MF_MT_FRAME_RATE_RANGE_MIN
 *    MF_MT_FRAME_SIZE
 *    MF_MT_GEOMETRIC_APERTURE
 *    MF_MT_INTERRACE_MODE
 *    MF_MT_MAX_KEYFRAME_SPACING
 *    MF_MT_MINIMUM_DISPLAY_APERTURE
 *    MF_MT_MPEG_SEQUENCE_HEADER
 *    MF_MT_MPEG_START_TIME_CODE
 *    MF_MT_MPEG2_FLAGS
 *    MF_MT_MPEG2_LEVEL
 *    MF_MT_MPEG2_PROFILE
 *    MF_MT_ORIGINAL_4CC
 *    MF_MT_PAD_SCAN_APERTURE
 *    MF_MT_PALETTE
 *    MF_MT_PAN_SCAN_APERTURE
 *    MF_MT_PAN_SCAN_ENABLED
 *    MF_MT_PIXEL_ASPECT_RATIO
 *    MF_MT_SOURCE_CONTENT_HINT
 *    MF_MT_TRANSFER_FUNCTION
 *    MF_MT_VIDEO_3D
 *    MF_MT_VIDEO_CHROMA_TITING
 *    MF_MT_VIDEO_LIGHTING
 *    MF_MT_VIDEO_NORMAL_RANGE
 *    MF_MT_VIDEO_PRIMARIES
 *    MF_MT_VIDEO_ROTATION
 *    MF_MT_YUV_MATRIX
 *    MF_XVP_DISABLE_FRC
 */
HRESULT GetGUID_FromMediaType(IMFMediaType *type, REFGUID key, GUID *guid);
HRESULT GetUINT32_FromMediaType(IMFMediaType *type, REFGUID key, UINT32 *length);
HRESULT GetINT32_FromMediaType(IMFMediaType *type, REFGUID key, INT32 *length);
HRESULT GetTwoUINT32_FromMediaType(IMFMediaType *type, REFGUID key, UINT32 *v1, UINT32 *v2);

/*
 *  Check IMFMediaType
 */
HRESULT IsVideoType(IMFMediaType *type, BOOL *b);
BOOL CheckMediaFormat(IMFMediaType *type, GUID format);
BOOL IsSameMediaType(IMFMediaType *type,
	       	GUID format, UINT32 width, UINT32 height);

/*
 * Color Conversion : YUV -> RGB24
 *
 *
 */
int YUY2_to_RGB(char *yuy2_image, char *rgb_image,
	       	int rgb_image_width, int rgb_image_height,
	       	int yuy2_image_size, int coversion_type);
#endif
