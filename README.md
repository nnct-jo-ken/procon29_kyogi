��29�񍂐�v���R���@���Z����
====

## Description
[��W�v��](http://www.procon.gr.jp/wp-content/uploads//2016/12/266299c2bb26295c0ada037e8f235cb4.pdf "��29�񍂐�v���R���@��W�v��")

## Investment policy

### �u�����`�̖���
�uGit flow�v�̂悤�ȃu�����`�̊Ǘ������܂��B

* master
	- ���C���u�����`
	- ����ł̂�
* develop
	- �J���u�����`
* feature/{�@�\��}
	- �@�\�̒ǉ�
	- develop���番�򂵁Adevelop�Ƀ}�[�W
* hotfix
	- master���番�򂵁Amaster��develop�Ƀ}�[�W

### �R�~�b�g
#### �R�~�b�g���b�Z�[�W
���{��ł��ł��B�킩��₷�����b�Z�[�W�����Ă��������B

#### �^�C�~���O
�@�\�̒ǉ���C���A�폜�ȂǁA�ׂ����P�ʂŃR�~�b�g���Ă��������B

### �R�[�h��ύX����ꍇ
���l�̃R�[�h�Ƌ������N�����ꍇ�́A���̐l�Ƙb�������܂��B
�S�̂ɉe�����邱�Ƃł���΁A�S�̂ɘA�����܂��B

### �V�����@�\���J�����n�߂�Ƃ�
```
git pull origin 
git checkout develop
git checkout -b feature/{�@�\��}
```

### �@�\�J�����I����Ƃ�
'feature/{�@�\��}'�u�����`�ɂĊJ�����I�����鎞�́A�ȉ��̂悤�ɂ��Ă��������B

```
git commit -m 'commit message'
git push origin
```

push��ɁAPull Request�𔭍s���ă}�[�W�̐\�������Ă��������B
��肪�Ȃ���΁A�}�[�W����܂��B

### issue�̎g����
��������@�\�A�o�O�Ȃǂɂ��āA��̓I�ȃ��m��issue�Ƃ��č쐬���Ă��������B