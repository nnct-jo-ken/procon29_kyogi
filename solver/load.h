#pragma once
#include "define.h"
#include <iostream>
#include <stdlib.h>

//�t�@�C������t�B�[���h�����擾
int load(const char filename[], struct field *field);
//�t�B�[���h���̏�����
void init_field(struct field *field);
//�t�B�[���h���̕\��
void view(struct field *field);