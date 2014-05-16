// OpenGLViewer.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "opencv.h"
#include "glview.h"

char filedir[] = "./data"; //�f�[�^���f�B���N�g��
char dir[] = "sample"; //�f�B���N�g����
char filename[] = "points.csv"; //�_�Q�t�@�C����

/// ---�ݒ�--- ///
float velocity = 0.002; //�ړ����x
float cubeSize = 0.01; //�����̂̈�ӂ̒���
float angle = 0.0; //�����̂̉�]�p
GLdouble cameraDistance = 3.0; //�����_�ƃJ�����Ƃ̋���
float axisSize = 0.25; //���W���̒���
float axisThick_ = 0.005; //���W���̑���
int window_w = 1000; //�E�C���h�E�T�C�Y
int window_h = 1000; //�E�C���h�E�T�C�Y
/// ---�����܂�--- ///

/// ---�}�E�X�������--- ///
//  ���h���b�O : ��]
//  �z�C�[���h���b�O : �c�C�������̕��s�ړ�
//  �z�C�[�� : ���s�������̕��s�ړ�
//  �E�h���b�O : �Y�[���C��/�A�E�g 
//  ���N���b�N�{�E�N���b�N : �_�̍��W�l(x,y,z)�擾 - ����������\�i�L�[�{�[�h����Q�Ɓj
/// ---�����܂�--- ///

/// ---�L�[�{�[�h�������--- ///
//  y : �ŏ��̈ʒu�ɖ߂�
//  o : ���W���\��/��\��
//  c : �J�����̒����_��_�Q�̒��S�Ɍ�����
//  j : ���̂����ɌX����
//  l : ���̂��E�ɌX����
//  0 : �����̂̃T�C�Y�傫��
//  9 : �����̂̃T�C�Y������
//  s : �_�̍��W�l(x,y,z)�ۑ� 2��ڂŋ������� 3��ڂŃ��Z�b�g
//  q : �E�B���h�E����� 
/// ---�����܂�--- ///


Mat dataPoints,dataPoints2;
int window1;
int rows, rows2;
const int cols = 3;
float axisThick;
float cubeSize_;
float velocityKey = 0.02;

int * FLAG;
FILE * fp;

/// ////////////////////////
///  �֐��v���g�^�C�v�錾
Mat csvread(const char* filename, const int rows, const int cols);
int readfileLine(char * filename);


/// ////////////////////////
///       main�֐�
int main(int argc, char** argv)
{
	axisThick = axisThick_;
	cubeSize_ = cubeSize/4.;

	/// Mat dataPoints
	char fname[255];
	sprintf(fname,"%s/%s/%s",filedir,dir,filename);
	rows = readfileLine(fname);
	dataPoints = csvread(fname,rows,cols); //csv����_�Q���W�擾

	/// �t�@�C����������
	sprintf(fname,"%s/%s/Distance_%s",filedir,dir,filename);
	errno_t error;
	error = fopen_s(&fp, fname, "w");
	if(error != 0){
		cout << "�t�@�C�����J���܂��� " << fname << endl;
		exit(1);
	}
	fprintf(fp,"����,�_1x,�_1y,�_1z,�_2x,�_2y,�_2z\n");

	/// OpenGL
	FLAG = (int *)malloc(sizeof(int) * rows);
	for(int i=0;i<rows;i++){
		FLAG[i] = 0;
	}

	/// OpenGL
	// GLUT initialize
	initFlag();
	initParam();
	Initialize();
	//window1
	glutInit(&argc, argv);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(window_w, window_h);
	glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );

	window1 = glutCreateWindow("Window1");
	glutDisplayFunc(disp);
	glutMouseFunc(mouse);
	glutMotionFunc(drag);
	glutPassiveMotionFunc(passive);
	glutMouseWheelFunc ( MouseWheel ) ;//�z�C�[���R�[���o�b�N
	glutIdleFunc(myGlutIdle);
	glutKeyboardFunc(glut_keyboard);
	glutIdleFunc(animate);
	glClearColor(0.0, 0.0, 0.0, 0.5); //�w�i�F

	//�`�ʊJ�n
	glutMainLoop();
	
	fclose(fp);

	return 0;
}

#pragma region // --- �t�@�C�������֐� ---
/// ////////////////////////////////////////////////
/// csvread : csv�t�@�C����cv::Mat�`���ŏo��
/// filename : �t�@�C����, rows : �s��, cols : ��
Mat csvread(const char* filename, const int rows, const int cols){
	cv::Mat_<float> m;
	m = cv::Mat::zeros(rows, cols, CV_32F);

	// CSV�t�@�C�����J��
	ifstream ifs(filename);
	string str;
	// �t�@�C�������݂��Ȃ��ꍇ
	if(ifs.fail()) {
		std::cout << "File does not exist" << std::endl;
		exit(0);
	}

	// �񐔂ɍ��킹�ēK�X�ϐ����w��
	float x,y,z;
	int line = 0;
	// 1�s���ǂ�ł���
	while(getline(ifs, str)) {
		sscanf_s(str.data(), "%f,%f,%f", &x, &y, &z);
		m.at<float>(line,0) = x;
		m.at<float>(line,1) = y;
		m.at<float>(line,2) = z;
		line ++;
		if(line>=rows){
			break;
		}
	}
	return m;
}


/// /////////////////////////////
/// �t�@�C���ǂݎ̂Ăčs���擾
int readfileLine(char * filename)
{
	int total = 0;

	FILE * fp;
	errno_t error;
	error = fopen_s(&fp, filename, "r");
	if(error != 0){
		cout << "�t�@�C�����J���܂��� " << filename << endl;
		exit(1);
	}
	char s[SHRT_MAX];
	while( fgets( s, SHRT_MAX, fp ) != NULL ){
		total++;
	}

	return total;
}

#pragma endregion