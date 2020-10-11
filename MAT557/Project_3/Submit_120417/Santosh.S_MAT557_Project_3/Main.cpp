#include <math.h>
#include <cv.h>
#include <highgui.h>
#include <iostream>

#define PI 3.14159265
#define IMAGE_PATH "Input_Image.jpg"

float alpha(int k)
{
	if(k==0) return 0.353553;
	return 0.5;
}

typedef struct{
  unsigned char b,g,r;
} RgbPixel;

typedef struct{
  float b,g,r;
} RgbPixelFloat;

template <typename T> 
T **AllocateDynamicArray( int nRows, int nCols)
{
      T **dynamicArray;
	  dynamicArray = new T*[nRows];
      for( int i = 0 ; i < nRows ; i++ )
		dynamicArray[i] = new T [nCols];
	  for(int j = 0 ; j < nRows ; j++)
		  for(int k = 0 ; k < nRows ; k++)
			  dynamicArray[j][k]=0;
	  return dynamicArray;
}

template <typename T>
void FreeDynamicArray(T** dArray)
{
      delete [] *dArray;
      delete [] dArray;
}

template<class T> class Image
{
  private:
	IplImage* imgp;
  public:
	Image(IplImage* img=0) 
	{
		imgp=img;
		height=img->height;
		width=img->width;
		step=img->widthStep;
		channels=img->nChannels;
	}
	~Image(){imgp=0;}
	void operator=(IplImage* img) 
	{
		imgp=img;
		height=img->height;
		width=img->width;
		step=img->widthStep;
		channels=img->nChannels;
	}
	inline T* operator[](const int rowIndx) 
	{
		return ((T *)(imgp->imageData + rowIndx*step));
	}

	int height,width,step,channels;
};

typedef Image<RgbPixel>       RgbImage;
typedef Image<RgbPixelFloat>  RgbImageFloat;
typedef Image<unsigned char>  BwImage;
typedef Image<float>          BwImageFloat;

int main()
{
  IplImage* img = 0; 
  int imgheight,imgwidth,step,channels;
  uchar *data;
  int i,j,k,m,n,x,y;
  int mask[8][8]={ {1,1,1,1,1,0,0,0},
				   {1,1,1,1,0,0,0,0},
				   {1,1,1,0,0,0,0,0},
				   {1,1,0,0,0,0,0,0},
				   {1,0,0,0,0,0,0,0},
				   {0,0,0,0,0,0,0,0},
				   {0,0,0,0,0,0,0,0},
				   {0,0,0,0,0,0,0,0}};
  // load an image  
  img=cvLoadImage(IMAGE_PATH,-1);
  if(!img){
    std::cout<<"Could not load image file: "<<IMAGE_PATH<<"\n";
    exit(0);
  }
  BwImage imgA(img);

  // get the image data
  imgheight    = imgA.height;
  imgwidth     = imgA.width;
  step      = imgA.step;
  channels  = imgA.channels;
  data      = (uchar *)img->imageData;
  std::cout<<"Processing image: "<<IMAGE_PATH<<", Dim: "<<imgheight<<"x"<<imgwidth<<", Channels: "<<channels;

  int height,width;
  height=imgheight;
  width=imgwidth;

  while(height%8) ++height;
  while(width%8) ++width;
  
  int **input = AllocateDynamicArray<int>(height,width);
  uchar **output = AllocateDynamicArray<uchar>(imgheight,imgwidth);
  float **A = AllocateDynamicArray<float>(height,width);
  float **B = AllocateDynamicArray<float>(height,width);
  
  for(i=0;i<imgheight;i++) 
	  for(j=0;j<imgwidth;j++) 
		  input[i][j]=(int)imgA[i][j];

  for(m=0;m<height;m+=8) 
  {
	  for(n=0;n<width;n+=8)
	  {
		  for(j=0;j<8;j++)
		  {
			  for(k=0;k<8;k++)
			  {
				  for(x=0;x<8;x++)
				  {
					  for(y=0;y<8;y++)
					  {
						  A[m+j][n+k]+=( input[m+x][n+y] * cos((2*x+1)*j*PI/(2*8)) * cos((2*y+1)*k*PI/(2*8) ));
					  }
				  }
				  A[m+j][n+k]*=(alpha(j)*alpha(k));
				  A[m+j][n+k]*=mask[j][k];
			      //std::cout<<A[m+j][n+k]<<"   ";
			  }
		  }
	      for(x=0;x<8;x++)
		  {
			  for(y=0;y<8;y++)
			  {
				  for(j=0;j<8;j++)
				  {
					  for(k=0;k<8;k++)
					  {
						  B[m+x][n+y]+=(alpha(k)*alpha(j)*A[m+j][n+k]*cos((2*x+1)*j*PI/(2*8))*cos((2*y+1)*k*PI/(2*8)));
					  }
				  }
				  if(B[m+x][n+y]<0.0) B[m+x][n+y]=0.0;
				  if(B[m+x][n+y]>255.0) B[m+x][n+y]=255.0;
			  }
		  }
	  }
  }

 for(i=0;i<imgheight;i++) 
	for(j=0;j<imgwidth;j++) 
	  output[i][j]=B[i][j];

  IplImage* imgout=cvCreateImage(cvSize(imgwidth,imgheight),IPL_DEPTH_8U,1);
  BwImage imgB(imgout);

  for(i=0;i<imgheight;i++) 
	  for(j=0;j<imgwidth;j++) 
		  imgB[i][j]=output[i][j];
  
  cvSaveImage("Converted_Image.jpg",imgout);

  //show the images
  cvNamedWindow("Converted Image", CV_WINDOW_AUTOSIZE); 
  cvMoveWindow("Converted Image", 550, 10);
  cvShowImage("Converted Image", imgout );
  cvNamedWindow("Input", CV_WINDOW_AUTOSIZE); 
  cvMoveWindow("Input", 10, 10);
  cvShowImage("Input", img );

  // wait for a key
  cvWaitKey(0);

  // release the images
  cvReleaseImage(&img);
  cvReleaseImage(&imgout);

  //clear the dynamically allocated arrays
  FreeDynamicArray<int>(input);
  FreeDynamicArray<uchar>(output);
  FreeDynamicArray<float>(A);
  FreeDynamicArray<float>(B);
  return 0;
}