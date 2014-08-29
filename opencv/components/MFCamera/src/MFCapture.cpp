/*
 *  Image Capture Device Class for Media Foundation
 *  This Class support only sync mode.
 *
 *  
 *   Copyright(C) Isao Hara, 2014, AIST Japan
 */

#include "MfCapture.h"

/*
 *  Constructor
 */
MFCapture::MFCapture() : nDevs(0), devId(-1), image_width(0), image_height(0),
      image_buf(NULL), pSource(NULL), pReader(NULL), pAttributes(NULL),
      ppDevices(NULL), pType(NULL),  image_format(MFVideoFormat_RGB24), YUV_Matrix(0),
      outMediaTypes(NULL),numOutMediaTypes(0),image_stride(0),
      image_length(0),pMFT(NULL),pHandler(NULL)
{
  // Create an attribute store to specify the enumeration parameters.
  hr = MFCreateAttributes(&pAttributes, 1);
  if (FAILED(hr))
  {
    return;
  }

  // Source type: video capture devices
  hr = pAttributes->SetGUID(
    MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
    MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID
    );
  if (FAILED(hr))
  {
    SafeRelease(&pAttributes);
    return;
  }

  // Enumerate devices.
  hr = MFEnumDeviceSources(pAttributes, &ppDevices, &nDevs);
  if (FAILED(hr))
  {
    SafeRelease(&pAttributes);
    return;
  }
  /*
   * The video format list.
   * The first element is a dummy element....
   */
  video_format = new VideoFormat();
}

/*
 * Deconstructor
 */
MFCapture::~MFCapture()
{
  SafeRelease(&pReader);
  SafeRelease(&pAttributes);  
  SafeRelease(&pType);
  
  SafeRelease(&pHandler);
  FreeOutMediaTypes();

  delete  video_format;
  
  for (DWORD i=0; i<nDevs; i++)
  {
    ppDevices[i]->ShutdownObject();
    SafeRelease(&ppDevices[i]);
  }
  CoTaskMemFree(ppDevices);
  SafeRelease(&pSource);
  if (image_buf != NULL) { cvReleaseImage(&image_buf); }
}

/*
 * Protect Function: FreeOutMediaTypes
 */
HRESULT
MFCapture::FreeOutMediaTypes()
{
  if(outMediaTypes){
    for (UINT32 i=0; i < numOutMediaTypes; i++)
    {
      SafeRelease(&(outMediaTypes[i]));
    }
    free(outMediaTypes);

    numOutMediaTypes=0;
    outMediaTypes = NULL; 
  }

  return S_OK;
}

/*
 * ActivateDevice
 *
 */
HRESULT
MFCapture::ActivateDevice(INT32 id)
{
  devId=id;

  if(devId >= 0){
    // Activate the capture device
    hr = ppDevices[devId]->ActivateObject(IID_PPV_ARGS(&pSource));
    if (FAILED(hr))
    {
      std::cout << "ERROR: Fail to ActivateObject. " << std::endl;
      return hr;
    }
    // enumerate available video format to capture image
    hr = EnumerateCaptureFormats();
    if (FAILED(hr))
    {
      std::cout << "ERROR: Fail to EnumerateCaptureForamt. " << std::endl;
      return hr;
    }
    // coount up the reference
     pSource->AddRef();
     return hr;
  }
  return E_INVALIDARG;
}

/*
 * Shutdown Deveice
 */
HRESULT
MFCapture::ShutdownDevice()
{
  if(devId >= 0){
    ppDevices[devId]->ShutdownObject();
    SafeRelease(&pSource);
  }
  return S_OK;
}


/*
 * GetMediaTypeHandler()
 *
 */
HRESULT
MFCapture::GetMediaTypeHandler()
{
  BOOL selected;
  IMFPresentationDescriptor *presDesc = NULL;
  IMFStreamDescriptor *strmDesc = NULL;

  hr = pSource->CreatePresentationDescriptor(&presDesc);
  if (FAILED(hr)){
    std::cerr <<  "Fail to CreatePresentationDescriptor" << std::endl;
    return hr;
  }

  hr = presDesc->GetStreamDescriptorByIndex(0, &selected, &strmDesc);
  if (FAILED(hr)){
    std::cerr <<  "Fail to GetStreamDescriptorByIndex" << std::endl;
    SafeRelease(&presDesc);
    return hr;
  }

  hr = strmDesc->GetMediaTypeHandler(&pHandler);
  if (FAILED(hr)){
    std::cerr <<  "Fail to GetMediaTypeHandler" << std::endl;
  }
  SafeRelease(&presDesc);
  SafeRelease(&strmDesc);
  return hr;
}

/*
 *  getCurrentMediaType()
 *
 */
HRESULT
MFCapture::GetCurrentType()
{
  if(pHandler == NULL)
  {
    hr = GetMediaTypeHandler();
    if (FAILED(hr))
    {
      return hr;
    }
  }

  hr = pHandler->GetCurrentMediaType(&pType);
  if (FAILED(hr))
  {
    std::cerr << "ERROR: Fail to GetCurrentMediaType " << std::endl;
    return hr;
  }

  hr = GetMediaTypeInfo(pType);
  if (FAILED(hr))
  {
    std::cerr << "ERROR: Fail to GetMediaTypeInfo " << std::endl;
  }

  return hr;
}

/*
 * get MediaType's Attrivutes.
 * 
 */
HRESULT 
MFCapture::GetMediaTypeInfo(IMFMediaType *type)
{
  if (type == NULL){
    type = pType;
  }
  /*
   * Video format 
   */
  hr = GetGUID_FromMediaType(type, MF_MT_SUBTYPE, &image_format);
  if (FAILED(hr)){
    return hr;
  }
  /*
   *  Frame size (image width and height)
   */
  hr = GetTwoUINT32_FromMediaType(type, MF_MT_FRAME_SIZE, &image_width, &image_height);
  if (FAILED(hr)){
    return hr;
  }
  /*
   *  Sampling size (= image size of a frame)
   */
  hr = GetUINT32_FromMediaType(type, MF_MT_SAMPLE_SIZE, &image_length);
  if (FAILED(hr)){
    return hr;
  }
  /*
   *  YUV conversion matrix, this value used by 'YUY2_to_RGB' function
   */
  hr = GetUINT32_FromMediaType(type, MF_MT_YUV_MATRIX, &YUV_Matrix);
  if (FAILED(hr)){
    return hr;
  }

  /*
   * Stride 
   */
  hr = GetINT32_FromMediaType(type, MF_MT_DEFAULT_STRIDE, &image_stride);
  if (FAILED(hr)){
    return hr;
  }
  return hr;
}

/*
 * Create Image Buffer
 *    depth: IPL_DEPTH_8U, IPL_DEPTH_8S,IPL_DEPTH_16U,
 *           IPL_DEPTH_16S, IPL_DEPTH_32S, IPL_DEPTH_32F, IPL_DEPTH74F
 *     channels: 1: GreyScale, 3: RGB , 4: Other
 */
void
MFCapture::CreateImageBuffer(UINT32 depth, UINT32 channels)
{
  if (image_buf != NULL) { cvReleaseImage(&image_buf); }
  image_buf = cvCreateImage(cvSize(image_width, image_height), depth, channels);
  image_length = image_width * image_height * channels;

  return;
}

/*
 *  Create Sourec Reader
 *
 */
HRESULT
MFCapture::CreateSourceReader()
{
  IMFAttributes *pAttr=NULL;
  
  hr = MFCreateAttributes(&pAttr, 1);
  if (FAILED(hr))
  { 
    std::cerr << "Fail to create attribute." << std::endl;
    return hr;
  }
  pAttr->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, 1);

  SafeRelease(&pReader);
  hr = MFCreateSourceReaderFromMediaSource(pSource, pAttr, &pReader);

  if (FAILED(hr)) {
    std::cerr << "Fail to create IMFSourceReader object." << std::endl;
  }

  SafeRelease(&pAttr);
  return hr;
}

/*
 *  Get IMFSample
 */
HRESULT
MFCapture::GetSample()
{
  DWORD streamIndex, flags;
  LONGLONG timeStamp;

  if(pReader)
  {
     SafeRelease(&pSample);
     hr = pReader->ReadSample( (DWORD)MF_SOURCE_READER_ANY_STREAM,
                              0, &streamIndex, &flags, &timeStamp, &pSample);
  }else{
    hr = E_INVALIDARG;
  }
  return hr;
}

/*
 *  Get Captured Image
 */
IplImage *
MFCapture::GetBufferData()
{
  IMFMediaBuffer *buff=NULL;
  DWORD maxLen, curLen;
  BYTE *memory;

  IMFSample *sample=NULL;

  DWORD streamIndex, flags;
  LONGLONG timeStamp;

  if(pReader){
    hr = pReader->ReadSample( (DWORD)MF_SOURCE_READER_ANY_STREAM,
                              0, &streamIndex, &flags, &timeStamp, &sample);
  }

  if (SUCCEEDED(hr) && sample)
  {
    hr = sample->GetBufferByIndex(0, &buff);
    if (FAILED(hr))
    {
      return NULL;
    }

    hr = buff->Lock(&memory, &maxLen, &curLen);
    if (FAILED(hr))
    {
      return NULL;
    }

    UINT32 size = image_length;
    if (size > curLen){ size = curLen; }
#if 1
    if(image_format == MFVideoFormat_YUY2){
      /// If the captured image format is YUY2, convert to RGB24
      YUY2_to_RGB((char *)memory, (char *)image_buf->imageData,
                     image_buf->width, image_buf->height, size, YUV_Matrix);

    }else{ // Should be MFVideoFormat_RGB24
      memcpy(image_buf->imageData, (void *)memory, size);
    }
#else
      memcpy(image_buf->imageData, (void *)memory, size);
#endif

    hr = buff->Unlock();
    SafeRelease(&buff);
    SafeRelease(&sample);

    return image_buf;
  }
  return NULL;
}

/*
 * Enumerate supported Mediatypes 
 */
HRESULT 
MFCapture::EnumerateCaptureFormats()
{
  UINT32 i;

  if(pHandler == NULL)
  {
    hr = GetMediaTypeHandler();
    if (FAILED(hr))
    {
      return hr;
    }
  }

  FreeOutMediaTypes();

  hr = pHandler->GetMediaTypeCount(&numOutMediaTypes);
  if (FAILED(hr))
  {
      std::cerr << "Fail to GetMediaTypeCount" << std::endl;
    return hr;
  }

  /*
   * Store available IMFMediaTypes and VideoFormat
   */
  outMediaTypes = (IMFMediaType **)malloc(sizeof(IMFMediaType *)*numOutMediaTypes);
  for (i=0; i < numOutMediaTypes; i++)
  {
    hr = pHandler->GetMediaTypeByIndex(i, &(outMediaTypes[i]));
    if (FAILED(hr)) { break; }

    IMFMediaType *type = outMediaTypes[i];
    UINT32 w, h;

    if( CheckMediaFormat(type, MFVideoFormat_RGB24))
    {
      hr = GetTwoUINT32_FromMediaType(type, MF_MT_FRAME_SIZE, &w, &h);
      if(video_format->isMember(FMT_RGB24, w, h) == FALSE){
        video_format->append(FMT_RGB24, w, h, i);
      }
    }
    
    if( CheckMediaFormat(type, MFVideoFormat_YUY2))
    {
      hr = GetTwoUINT32_FromMediaType(type, MF_MT_FRAME_SIZE, &w, &h);
      if(video_format->isMember(FMT_YUY2, w, h) == FALSE){
        video_format->append(FMT_YUY2, w, h, i);
      }
    }
  }

  return hr;
}

/*
 * Set Device Format
 */
HRESULT 
MFCapture::SetDeviceFormat(DWORD idx)
{
  IMFMediaType *type = NULL;

  if(pHandler == NULL)
  {
    hr = GetMediaTypeHandler();
    if (FAILED(hr))
    {
      return hr;
    }
  }

#if 0
  printf("Call SetDeviceFormat id = %d\n",idx);
#endif

  hr = pHandler->GetMediaTypeByIndex(idx, &type);
  if (FAILED(hr))
  {
    std::cerr << "Fail to GetMediaTypeByIndex "<< std::endl;
    return hr;
  }

  hr = pHandler->SetCurrentMediaType(type);
  if (FAILED(hr))
  {
    std::cerr << "Fail to SetCurrentMediaType "<< std::endl;
    SafeRelease(&type);
    return hr;
  }
  SafeRelease(&pType);
  pType = type;
#if 0
  printf("INDEX = %d\n", video_format->getIndex(idx));
#endif
  hr = GetMediaTypeInfo();

  hr = CreateSourceReader();
  if (FAILED(hr))
  {
    std::cerr << "Fail to SetDeviceFormat "<< std::endl;
    return hr;
  }

  return hr;
}
/*
 * Set Device Format
 */
INT32 
MFCapture::SetDeviceFormatByName(const char *name)
{
  INT32 idx = this->video_format->getIndexByName(name);

  if(idx >= 0){ 
    hr = SetDeviceFormat(idx);
    if(FAILED(hr)){ return -1; }
  }

  return idx;
}

/*
 * Display Summary of IMFMediaType's Attribute 
 */
HRESULT
MFCapture::ShowMediaTypeInfo(IMFMediaType *type, int idx)
{
  GUID format;
  INT32 stride;
  UINT32 length;
  UINT32 width, height;

  if(type == NULL)
  {
    type = pType;
    printf("== Current MediaType ==\n");
  }else{
    printf("== MediaType[ %d ] ==\n",idx);
  }

  hr = GetGUID_FromMediaType(type, MF_MT_SUBTYPE, &format);
  if (FAILED(hr)){ return hr; }
  printf("Format = %s\n", GetFormatName(format));

  hr = GetTwoUINT32_FromMediaType(type, MF_MT_FRAME_SIZE, &width, &height);
  if (FAILED(hr)){ return hr; }
  printf("frame size = %d x %d \n", width, height);

  hr = GetINT32_FromMediaType(type, MF_MT_DEFAULT_STRIDE, &stride);
  if (SUCCEEDED(hr)){  printf("stride  = %d \n", stride); }
  
  hr = GetUINT32_FromMediaType(type, MF_MT_SAMPLE_SIZE, &length);
  if (SUCCEEDED(hr)) { printf("length  = %d \n",length); }

  UINT32 val;
  hr = GetUINT32_FromMediaType(type, MF_MT_VIDEO_PRIMARIES, &val);
  if (SUCCEEDED(hr)) { printf("video_primitives  = %d \n",val); }

  hr = GetUINT32_FromMediaType(type, MF_MT_YUV_MATRIX, &val);
  if (SUCCEEDED(hr)) { printf("yuv_matrix  = %d \n",val); }

  hr = GetUINT32_FromMediaType(type, MF_MT_VIDEO_NOMINAL_RANGE, &val);
  if (SUCCEEDED(hr)) { printf("video nominal range  = %d \n",val); }

  printf("------------------------\n");
  return hr;
}


#if 0
int 
MFCapture::FindMediaTypeIndex()
{

}
#endif

/*
 *  Select available capture device by id.
 *  This function doesn't need. Please use 'ActiveDevide' alternatively.
 */
HRESULT 
MFCapture::SelectDevice(UINT32 idx)
{
  if (nDevs == 0)
  {
    std::cerr << "No Devices Found.";
    return  E_INVALIDARG;
  }
  if (idx < 0 || nDevs <= idx){
    std::cerr << "Invalid Index.";
    return   E_INVALIDARG;
  }

  // Create the media source object.
  hr = ppDevices[idx]->ActivateObject(IID_PPV_ARGS(&pSource));
    
  if (SUCCEEDED(hr))
  {
    pSource->AddRef();
  }
  devId = idx;
  
  hr = EnumerateCaptureFormats();
  if (FAILED(hr))
  {
    std::cout << "ERROR: Fail to EnumerateCaptureForamt. " << std::endl;
    return hr;
  }

  hr = GetCurrentType();
  if (FAILED(hr))
  {
    std::cout << "ERROR: Fail to GetCurrentType " << std::endl;
    return hr;
  }
  ShowMediaTypeInfo(NULL, 0);
  return hr;
}

//////// Test implementation for the MediaFoudation Transform
/*
 * Test code for MFT
 */
HRESULT 
MFCapture::EnumerateMFT(GUID inType, GUID outType)
{
  /// Step1: Create MFT
  ///
  const MFT_REGISTER_TYPE_INFO InType={ MFMediaType_Video, inType};
  const MFT_REGISTER_TYPE_INFO OutType={ MFMediaType_Video, outType};
  UINT32 unFlag = MFT_ENUM_FLAG_SYNCMFT |
                  MFT_ENUM_FLAG_LOCALMFT |
                  MFT_ENUM_FLAG_SORTANDFILTER | MFT_ENUM_FLAG_HARDWARE;

  IMFActivate **ppMFTActive;
  UINT32 cMFTActive;

  hr = MFTEnumEx(MFT_CATEGORY_VIDEO_DECODER,
               unFlag, &InType, &OutType, &ppMFTActive, &cMFTActive);

  if(FAILED(hr)){
    std::cerr << "Fail to MFTEnumEx" << std::endl;
    return hr;
  }
    if(cMFTActive < 1) {
    std::cerr << "Fail to MFTEnumEx: No MFActiveate." << std::endl;
   //  exit(1);
    return hr;
  }
  std::cerr << cMFTActive << " MFT Found." << std::endl;

  hr = ppMFTActive[0]->ActivateObject(__uuidof(IMFTransform), (void **)&pMFT);
  if(FAILED(hr)){
    std::cerr << "Fail to MFTEnumEx: in ActivateObject." << std::endl;
    return hr;
  }

  /// Step2: Get Stream Identification
  DWORD iMin, iMax, oMin, oMax;

  hr = pMFT->GetStreamLimits(&iMin, &iMax, &oMin, &oMax);
  printf("GetStreamLimits = in(%d %d), out(%d %d) =\n",
                   iMin, iMax, oMin, oMax);

  DWORD cIn, cOut;
  DWORD InIDs, OutIDs;
  hr = pMFT->GetStreamCount(&cIn, &cOut);
  printf("GetStreamCount: Input=%d, Output=%d \n", cIn, cOut);

  hr = pMFT->GetStreamIDs(cIn, &InIDs, cOut, &OutIDs);
  if(cIn > 0){
    printf("IN IDs: %x\n", (&InIDs)[0]);
  }
  if(cOut > 0){
    printf("Out ID= %x\n", (&OutIDs)[0]);
  }
  return hr;
}

/*
 * This is a test implemetation for MFT
 */
IMFSample *
MFCapture::ConvertToRGB()
{
  if(pMFT)
  {
    MFT_OUTPUT_STREAM_INFO osi;

    hr = pMFT->ProcessMessage(MFT_MESSAGE_NOTIFY_BEGIN_STREAMING, 0);
    pMFT->ProcessInput(0, pSample, 0);
    hr = pMFT->GetOutputStreamInfo(0, &osi);
    hr = pMFT->GetOutputStreamInfo(0, &osi);

    DWORD status = 0;
    hr = pMFT->GetOutputStatus(&status);

    if (status = MFT_OUTPUT_STATUS_SAMPLE_READY) {
    }

    IMFSample *outputSample;
    hr = MFCreateSample(&outputSample);

    DWORD outStatus = 0;
    MFT_OUTPUT_DATA_BUFFER odf;
    odf.dwStreamID = 0;
    odf.pSample = outputSample;
    odf.dwStatus = 0;
    odf.pEvents = NULL;;

    hr = pMFT->ProcessOutput(0, 1, &odf, &outStatus);
    hr = pMFT->ProcessMessage(MFT_MESSAGE_NOTIFY_END_OF_STREAM, 0);
    hr = pMFT->ProcessMessage(MFT_MESSAGE_COMMAND_DRAIN, 0);
  }
  return NULL;
}

/*** end of MfCapture.cpp ***/
