#define WX2_NEW new
#define WX2_DELETE delete

#define WX2_CONSTEXPR constexpr

#define WX2_NOEXCEPT noexcept

/**
 * @brief �f�t�H���g�R�s�[�R���X�g���N�^�ƃf�t�H���g������Z�q���`����
 * @param name �N���X��
 */
#define WX2_COPY_AND_ASSIGNABLE(name)		\
	name(const name&) = default;			\
	name& operator = (const name&) = default

/**
 * @brief �f�t�H���g���[�u�R���X�g���N�^�ƃf�t�H���g�ړ�������Z�q���`����
 * @param name �N���X��
 */
#define WX2_MOVE_AND_ASSIGNABLE(name)		\
	name(name&&) = default;					\
	name& operator = (name&&) = default

/**
 * @brief �R�s�[�R���X�g���N�^�Ƒ�����Z�q�̌Ăяo�����֎~����
 * @param name �N���X��
 */
#define WX2_DISALLOW_COPY_AND_ASSIGN(name)	\
	name(const name&) = delete;				\
	name& operator = (const name&) = delete

/**
 * @brief ���[�u�R���X�g���N�^�ƈړ�������Z�q�̌Ăяo�����֎~����
 * @param name �N���X��
 */
#define WX2_DISALLOW_MOVE_AND_ASSIGN(name)	\
	name(name&&) = delete;					\
	name& operator = (name&&) = delete