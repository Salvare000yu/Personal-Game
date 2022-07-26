class Vector3 : public VECTOR
{
	public:
		
		//float x;//x成分
		//float y;//y成分
		//float z;//z成分

	public:
		//コンストラクタ
		Vector3();//零ベクトル
		Vector3(float x, float y, float z);//xy成分として生成

		//メンバ関数
		float length()const;//ノルム（長さ）求める
		Vector3& normalize();//正規化
		float dot(const Vector3& v)const;//内積
		Vector3 cross(const Vector3& v)const;//外積

			//単項演算子オーバーロード
		Vector3 operator+() const;
		Vector3 operator-() const;

		//代入演算子
		Vector3& operator+=(const Vector3& v);
		Vector3& operator-=(const Vector3& v);
		Vector3& operator*=(float s);
		Vector3& operator/=(float s);

	};
	//2項演算子
	//いろんな引数
	const Vector3 operator+(const Vector3& v1, const Vector3& v2);
	const Vector3 operator-(const Vector3& v1, const Vector3& v2);
	const Vector3 operator*(const Vector3& v, float s);
	const Vector3 operator*(float s, const Vector3& v);
	const Vector3 operator/(const Vector3& v, float s);
	//補間関数
	//線形補間（一次元関数補間）
	const Vector3 lerp(const Vector3& start, const Vector3& end, const float t);

