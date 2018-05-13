//example


#include <opencv2/core.hpp>    // core���W���[���̃w�b�_�[���C���N���[�h
#include <opencv2/highgui.hpp> // highgui���W���[���̃w�b�_�[���C���N���[�h
#include <iostream>

int main(int argc, const char* argv[])
{
	// 1��320px�A����240px�ŐԐF�̉摜�f�[�^�𐶐�
	cv::Mat redImg(cv::Size(320, 240), CV_8UC3, cv::Scalar(0, 0, 255));

	// 2�摜�\���p�̃E�B���h�E�𐶐�
	cv::namedWindow("red", cv::WINDOW_AUTOSIZE);

	// 3�E�B���h�E�ɉ摜��\��
	cv::imshow("red", redImg);

	// 4�L�[���͂�ҋ@
	cv::waitKey(0);

	// 5�쐬�����E�B���h�E��S�Ĕj��
	cv::destroyAllWindows();

	return 0;
}