#include<opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp>        
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

//leer un imagen
Mat leeimagen(const string& filename){
	return imread(filename);
}
//------------------
//imprimir imagen en una ventana
void pintaI(Mat im){
	namedWindow("ventana",1);
	imshow("ventana",im);
	waitKey();
	destroyWindow("ventana");
}
//------------------

//funcion para imprimir los valores de un valor tipo float
void printVector(const vector<float>& v){
	int tam=v.size();
	for(int i=0;i<tam;i++){
	cout<<v[i]<<" ";
	}
}
//---------------------
//funcion para imprimir los valores de una matriz tipo float
void printMatriz(const vector<vector<float> >& v ){
	int filas=v.size(), columnas=v[0].size();
	for(int i=0;i<filas;i++){
		for(int j=0;j<columnas;j++){
			cout<<v[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
}
//------------------------------------------------------------------------
int main ( int argc, char* argv[]){
	Mat src,src_gray,myHarris_dst,Mc,myHarris_copy;
	src= leeimagen("imagenes/Yosemite1.jpg");
	cvtColor(src,src_gray,COLOR_BGR2GRAY);
	int blockSize=3, ksize=3;
	//matrices de Harris (6 matrices) 
	myHarris_dst=Mat::zeros(src_gray.size(),CV_32FC(6));
	Mc=Mat::zeros(src_gray.size(),CV_32FC1);
	//llamar a la funcion que nos devuelve los 6 matrices con el tamaño de la ventana blockSize=3
	cornerEigenValsAndVecs(src_gray,myHarris_dst,blockSize,ksize);
	//calcular Mc que va a almacenar los valores Harris para cada pixel
	float lambda1,lambda2;
	for(int i=0;i<src_gray.rows;i++){
		for(int j=0;j<src_gray.cols;j++){
			lambda1=myHarris_dst.at<Vec6f>(i,j)[0];
			lambda2=myHarris_dst.at<Vec6f>(i,j)[1];
			Mc.at<float>(i,j)=(lambda1*lambda2) - 0.04f*pow((lambda1+lambda2),2);
		}
	}
	
	//encontrar el menor y el mayor valor en la matriz que contiene los valores de Harris (Mc)
	double myHarris_minVal,myHarris_maxVal;
	minMaxLoc(Mc, &myHarris_minVal, &myHarris_maxVal,0,0,Mat());
	
	//dibujar los circulos
	RNG rng(12345);
	int myHarris_qualityLevel=50,max_qualityLevel=100;
	myHarris_copy=src.clone();
	for(int i=0;i<src_gray.rows;i++){
		for(int j=0;j<src_gray.cols;j++){
			if(Mc.at<float>(i,j) > 0.001){//(myHarris_minVal+(myHarris_maxVal-myHarris_minVal)*myHarris_qualityLevel/max_qualityLevel)
				circle(myHarris_copy,Point(j,i),4,Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255)),-1,8,0);
			}
		}
	}

	//pintaI(src_gray);
	pintaI(myHarris_copy);
	return 0;
}
