#define WX2_NEW new
#define WX2_DELETE delete

#define WX2_CONSTEXPR constexpr

#define WX2_NOEXCEPT noexcept

/**
 * @brief デフォルトコピーコンストラクタとデフォルト代入演算子を定義する
 * @param name クラス名
 */
#define WX2_COPY_AND_ASSIGNABLE(name)		\
	name(const name&) = default;			\
	name& operator = (const name&) = default

/**
 * @brief デフォルトムーブコンストラクタとデフォルト移動代入演算子を定義する
 * @param name クラス名
 */
#define WX2_MOVE_AND_ASSIGNABLE(name)		\
	name(name&&) = default;					\
	name& operator = (name&&) = default

/**
 * @brief コピーコンストラクタと代入演算子の呼び出しを禁止する
 * @param name クラス名
 */
#define WX2_DISALLOW_COPY_AND_ASSIGN(name)	\
	name(const name&) = delete;				\
	name& operator = (const name&) = delete

/**
 * @brief ムーブコンストラクタと移動代入演算子の呼び出しを禁止する
 * @param name クラス名
 */
#define WX2_DISALLOW_MOVE_AND_ASSIGN(name)	\
	name(name&&) = delete;					\
	name& operator = (name&&) = delete