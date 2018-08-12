#pragma once
#include "define.h"
#include <iostream>
#include <stdlib.h>

//ファイルからフィールド情報を取得
int load(const char filename[], struct field *field);
//フィールド情報の初期化
void init_field(struct field *field);
//フィールド情報の表示
void view(struct field *field);