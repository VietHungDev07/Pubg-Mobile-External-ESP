#pragma once
#include<Windows.h>
#include<iostream>
#include<string>
#include<string>
#include<vector>
#include"Tools.h"
#include "Data.h"
#include <d3d9types.h>
#define M_PI 3.14159265358979323846f
#define RAD2DEG( x )  ( (float)(x) * (float)(180.f / IM_PI) )
#define DEG2RAD( x ) ( (float)(x) * (float)(IM_PI / 180.f) )
using namespace std;
struct ActorsEncryption {
	uint32_t Enc_1; //0x0340 
	uint32_t Enc_2; //0x0344 
	uint32_t Enc_3; //0x0348 
	uint32_t Enc_4; //0x034C 
};
struct Encryption_Chunk {
	uint32_t val_1, val_2, val_3, val_4;
};
struct ObjectName
{
	char Data[64];
};
struct VECTOR2
{
	FLOAT X;
	FLOAT Y;
};

struct VECTOR3
{


	float X;
	float Y;
	float Z;


	//bool operator==(const VECTOR3& other) const {
	//	return X == other.X && Y == other.Y && Z == other.Z;
	//}

	float  Size()
	{
		return sqrt((this->X * this->X) + (this->Y * this->Y) + (this->Z * this->Z));
	}


	VECTOR3 FVector(float _X, float _Y, float _Z)
	{
		VECTOR3 emmm;
		emmm.X = _X;
		emmm.Y = _Y;
		emmm.Z = _Z;
		return emmm;
	}

	VECTOR3  operator+ (const VECTOR3& A)
	{
		return FVector(this->X + A.X, this->Y + A.Y, this->Z + A.Z);
	}

	VECTOR3 operator+ (const float A)
	{
		return FVector(this->X + A, this->Y + A, this->Z + A);
	}

	VECTOR3  operator+= (const VECTOR3& A)
	{
		this->X += A.X;
		this->Y += A.Y;
		this->Z += A.Z;
		return *this;
	}

	VECTOR3  operator+= (const float A)
	{
		this->X += A;
		this->Y += A;
		this->Z += A;
		return *this;
	}

	VECTOR3  operator- (const VECTOR3& A)
	{
		return FVector(this->X - A.X, this->Y - A.Y, this->Z - A.Z);
	}

	VECTOR3  operator- (const float A)
	{
		return FVector(this->X - A, this->Y - A, this->Z - A);
	}

	VECTOR3 operator-= (const VECTOR3& A)
	{
		this->X -= A.X;
		this->Y -= A.Y;
		this->Z -= A.Z;
		return *this;
	}

	VECTOR3 operator-= (const float A)
	{
		this->X -= A;
		this->Y -= A;
		this->Z -= A;
		return *this;
	}

	VECTOR3  operator* (const VECTOR3& A)
	{
		return FVector(this->X * A.X, this->Y * A.Y, this->Z * A.Z);
	}

	VECTOR3  operator* (const float A)
	{
		return FVector(this->X * A, this->Y * A, this->Z * A);
	}

	VECTOR3  operator*= (const VECTOR3& A)
	{
		this->X *= A.X;
		this->Y *= A.Y;
		this->Z *= A.Z;
		return *this;
	}

	VECTOR3 operator*= (const float A)
	{
		this->X *= A;
		this->Y *= A;
		this->Z *= A;
		return *this;
	}

	VECTOR3  operator/ (const VECTOR3& A)
	{
		return FVector(this->X / A.X, this->Y / A.Y, this->Z / A.Z);
	}

	VECTOR3 operator/ (const float A)
	{
		return FVector(this->X / A, this->Y / A, this->Z / A);
	}

	VECTOR3  operator/= (const VECTOR3& A)
	{
		this->X /= A.X;
		this->Y /= A.Y;
		this->Z /= A.Z;
		return *this;
	}

	VECTOR3 operator/= (const float A)
	{
		this->X /= A;
		this->Y /= A;
		this->Z /= A;
		return *this;
	}

	bool  operator== (const VECTOR3& A)
	{
		if (this->X == A.X
			&& this->Y == A.Y
			&& this->Z == A.Z)
			return true;

		return false;
	}

	bool operator!= (const VECTOR3& A)
	{
		if (this->X != A.X
			|| this->Y != A.Y
			|| this->Z != A.Z)
			return true;

		return false;
	}
	float Distance(VECTOR3 v)
	{
		return float(sqrtf(powf(v.X - X, 2.0) + powf(v.Y - Y, 2.0) + powf(v.Z - Z, 2.0)));
	}
};


struct VECTOR4
{
	FLOAT X;
	FLOAT Y;
	FLOAT Z;
	FLOAT W;
};

uint32_t DecryptActorsArrayZakaria(uint32_t uLevel, int Actors_Offset, int EncryptedActors_Offset)
{
	if (uLevel < 0x10000000)
		return 0;

	if (ReadMemoryEx<uint32_t>(uLevel + Actors_Offset) > 0)
		return uLevel + Actors_Offset;
	else
	{
		std::cout << "Try Find Next Decryption Address 1" << std::endl;

	}

	if (ReadMemoryEx<uint32_t>(uLevel + EncryptedActors_Offset) > 0)
		return uLevel + EncryptedActors_Offset;
	else
	{
		std::cout << "Try Find Next Decryption Address 2" << std::endl;

	}
	auto Encryption = ReadMemoryEx<ActorsEncryption>(uLevel + EncryptedActors_Offset + 0x0C);

	if (Encryption.Enc_1 > 0)
	{
		auto Enc = ReadMemoryEx<Encryption_Chunk>(Encryption.Enc_1 + 0x80);

		return ((ReadMemoryEx<unsigned __int8>(Encryption.Enc_1 + Enc.val_1))
			| ((ReadMemoryEx<unsigned __int8>(Encryption.Enc_1 + Enc.val_2)) << 8)
			| ((ReadMemoryEx<unsigned __int8>(Encryption.Enc_1 + Enc.val_3)) << 16)
			| ((ReadMemoryEx<unsigned __int8>(Encryption.Enc_1 + Enc.val_4)) << 24));
	} //ok
	else if (Encryption.Enc_2 > 0)
	{
		auto Encrypted_Actors = ReadMemoryEx<uint32_t>(Encryption.Enc_2);
		if (Encrypted_Actors > 0)
		{
			return (Encrypted_Actors - 0x4000000) & 0xFF000000
				| (Encrypted_Actors + 0xFC0000) & 0xFF0000
				| ((unsigned __int16)Encrypted_Actors - 0x400) & 0xFF00
				| (unsigned __int8)(Encrypted_Actors - 4);
		}
	} //ok
	else if (Encryption.Enc_3 > 0)
	{
		auto Encrypted_Actors = ReadMemoryEx<uint32_t>(Encryption.Enc_3);
		if (Encrypted_Actors > 0)
			return _rotr(Encrypted_Actors, 0x18);
	}
	else if (Encryption.Enc_4 > 0)
	{
		auto Encrypted_Actors = ReadMemoryEx<uint32_t>(Encryption.Enc_4);
		if (Encrypted_Actors > 0)
			return Encrypted_Actors ^ 0xCDCD00;
	}
	else
	{
		std::cout << "Cant Find Decryption Address" << std::endl;
	}
	return 0;
}




string GetEntities(LONG GNames, LONG Id)
{
	DWORD GName = ReadMemoryEx<DWORD>(GNames);
	DWORD NamePtr = ReadMemoryEx<DWORD>(GName + int(Id / 16384) * 0x4);
	DWORD Name = ReadMemoryEx<DWORD>(NamePtr + int(Id % 16384) * 0x4);
	ObjectName pBuffer = ReadMemoryEx<ObjectName>(Name + 0x8);
	return string(pBuffer.Data);
}
BOOL
IsPlayer(string str)
{
	if (str.find(("BP_PlayerPawn")) != std::string::npos ||
		str.find(("PlayerCharacter")) != std::string::npos ||
		str.find(("BP_Plane_Character_C")) != std::string::npos ||
		str.find(("BP_PlayerState")) != std::string::npos ||

		str.find(("PlanET_FakePlayer")) != std::string::npos ||//new
		str.find(("PlayerPawn_Infec")) != std::string::npos ||//new
		str.find(("Bp_XTPlayerState_C")) != std::string::npos ||
		str.find(("BP_CharacterModelTaget_C")) != std::string::npos)
	{
		return true;
	}
	return FALSE;
}



VECTOR3 Subtract(VECTOR3 Src, VECTOR3 Dst)
{
	VECTOR3 Result;
	Result.X = Src.X - Dst.X;
	Result.Y = Src.Y - Dst.Y;
	Result.Z = Src.Z - Dst.Z;

	return Result;
}
float Magnitude(VECTOR3 Vec)
{
	return sqrtf(Vec.X * Vec.X + Vec.Y * Vec.Y + Vec.Z * Vec.Z);
}
float GetDistance(VECTOR3 Src, VECTOR3 Dst)
{
	VECTOR3 Result = Subtract(Src, Dst);
	return Magnitude(Result);
}
float Distance3D(VECTOR3 OurPosition, VECTOR3 EnemyPosition)
{
	float x_d = (EnemyPosition.X - OurPosition.X);
	float y_d = (EnemyPosition.Y - OurPosition.Y);
	float z_d = (EnemyPosition.Z - OurPosition.Z);
	return sqrt((x_d * x_d) + (y_d * y_d) + (z_d * z_d));
}
D3DMATRIX ToMatrixWithScale(VECTOR3 translation, VECTOR3 scale, VECTOR4 rot)
{
	D3DMATRIX m;
	m._41 = translation.X;
	m._42 = translation.Y;
	m._43 = translation.Z;

	float x2 = rot.X + rot.X;
	float y2 = rot.Y + rot.Y;
	float z2 = rot.Z + rot.Z;

	float xx2 = rot.X * x2;
	float yy2 = rot.Y * y2;
	float zz2 = rot.Z * z2;
	m._11 = (1.0f - (yy2 + zz2)) * scale.X;
	m._22 = (1.0f - (xx2 + zz2)) * scale.Y;
	m._33 = (1.0f - (xx2 + yy2)) * scale.Z;

	float yz2 = rot.Y * z2;
	float wx2 = rot.W * x2;
	m._32 = (yz2 - wx2) * scale.Z;
	m._23 = (yz2 + wx2) * scale.Y;

	float xy2 = rot.X * y2;
	float wz2 = rot.W * z2;
	m._21 = (xy2 - wz2) * scale.Y;
	m._12 = (xy2 + wz2) * scale.X;

	float xz2 = rot.X * z2;
	float wy2 = rot.W * y2;
	m._31 = (xz2 + wy2) * scale.Z;
	m._13 = (xz2 - wy2) * scale.X;

	m._14 = 0.0f;
	m._24 = 0.0f;
	m._34 = 0.0f;
	m._44 = 1.0f;

	return m;
}

D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
{
	D3DMATRIX pOut =
	{
		pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41,
		pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42,
		pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43,
		pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44,
		pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41,
		pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42,
		pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43,
		pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44,
		pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41,
		pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42,
		pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43,
		pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44,
		pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41,
		pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42,
		pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43,
		pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44
	};

	return pOut;
}

struct FTTransform
{
	VECTOR4 Rotation;
	VECTOR3 Translation;
	FLOAT _Padding;
	VECTOR3 Scale3D;
};
VECTOR3 GetBoneWorldPosition(DWORD ActorAddv, DWORD BoneAddv)
{
	FTTransform Bone = ReadMemoryEx<FTTransform>(BoneAddv);
	FTTransform Actor = ReadMemoryEx<FTTransform>(ActorAddv);
	D3DMATRIX BoneMatrix = ToMatrixWithScale(Bone.Translation, Bone.Scale3D, Bone.Rotation);
	D3DMATRIX ComponentToWorldMatrix = ToMatrixWithScale(Actor.Translation, Actor.Scale3D, Actor.Rotation);
	D3DMATRIX NewMatrix = MatrixMultiplication(BoneMatrix, ComponentToWorldMatrix);

	VECTOR3 BonePos;
	BonePos.X = NewMatrix._41;
	BonePos.Y = NewMatrix._42;
	BonePos.Z = NewMatrix._43;

	return BonePos;
}

enum BoneIndex
{
	Head = 5,
	Chest = 4,
	Pelvis = 1,
	LeftSholder = 11,
	RightSholder = 32,
	LeftElbow = 12,
	RightElbow = 33,
	LeftWrist = 63,
	RightWrist = 62,
	LeftThigh = 52,
	RightThigh = 56,
	LeftKnee = 53,
	RightKnee = 57,
	LeftAnkle = 54,
	RightAnkle = 58
};

static const BoneIndex BoneList[] = {
Head,Chest, Pelvis,
LeftSholder, RightSholder,
LeftElbow,   RightElbow,
LeftWrist,   RightWrist,
LeftThigh,   RightThigh,
LeftKnee,    RightKnee,
LeftAnkle,   RightAnkle
};


static const std::pair<BoneIndex, BoneIndex> BonePairs[] = {
	{ Head,        Chest },       
	{ Chest,       Pelvis },

	{ Chest,      LeftSholder }, 
	{ LeftSholder, LeftElbow },
	{ LeftElbow,   LeftWrist },

	{ Chest,      RightSholder },
	{ RightSholder,  RightElbow },
	{ RightElbow,      RightWrist },

	{ Pelvis,      LeftThigh },
	{ LeftThigh,   LeftKnee },
	{ LeftKnee,  LeftAnkle },

	{ Pelvis,      RightThigh },
	{ RightThigh,   RightKnee },
	{ RightKnee,  RightAnkle },

	{ LeftSholder,  RightSholder }


};

bool isnull(VECTOR3 dat)
{
	if (dat.X != NULL)
		if (dat.Y != NULL)
			return true;
	return false;
}
struct FRotator {
	float Pitch;
	float Yaw;
	float Roll;
};
typedef struct D3DMATRIX1 {
	float m[4][4];
}
D3DMATRIX1, * LPD3DMATRIX;
struct MinimalViewInfo {
	VECTOR3 Location;
	VECTOR3 LocationLocalSpace;
	FRotator Rotation;
	float FOV;
};
struct CameraCacheEntry {
	float TimeStamp;
	char chunks[0xC];
	MinimalViewInfo POV;
};

D3DMATRIX1 ToMatrix(FRotator rot = { 0,0,0 }, FRotator origin = { 0,0,0 })
{
	float radPitch = (rot.Pitch * float(M_PI) / 180.f);
	float radYaw = (rot.Yaw * float(M_PI) / 180.f);
	float radRoll = (rot.Roll * float(M_PI) / 180.f);

	float SP = sinf(radPitch);
	float CP = cosf(radPitch);
	float SY = sinf(radYaw);
	float CY = cosf(radYaw);
	float SR = sinf(radRoll);
	float CR = cosf(radRoll);

	D3DMATRIX1 matrix;
	matrix.m[0][0] = CP * CY;
	matrix.m[0][1] = CP * SY;
	matrix.m[0][2] = SP;
	matrix.m[0][3] = 0.f;

	matrix.m[1][0] = SR * SP * CY - CR * SY;
	matrix.m[1][1] = SR * SP * SY + CR * CY;
	matrix.m[1][2] = -SR * CP;
	matrix.m[1][3] = 0.f;

	matrix.m[2][0] = -(CR * SP * CY + SR * SY);
	matrix.m[2][1] = CY * SR - CR * SP * SY;
	matrix.m[2][2] = CR * CP;
	matrix.m[2][3] = 0.f;

	matrix.m[3][0] = origin.Pitch;
	matrix.m[3][1] = origin.Yaw;
	matrix.m[3][2] = origin.Roll;
	matrix.m[3][3] = 1.f;

	return matrix;
}
namespace Data
{
	 VECTOR3 MyLocalPosition;
	 CameraCacheEntry CameraCache;
}
bool WorldToScreenWithoutMatrix(VECTOR3 WorldLocation, VECTOR3& Screenlocation)
{
	FRotator Rotation = Data::CameraCache.POV.Rotation;
	D3DMATRIX1 tempMatrix = ToMatrix(Rotation);

	VECTOR3 vAxisX, vAxisY, vAxisZ;

	vAxisX.X = tempMatrix.m[0][0];
	vAxisX.Y = tempMatrix.m[0][1];
	vAxisX.Z = tempMatrix.m[0][2];
	vAxisY.X = tempMatrix.m[1][0];
	vAxisY.Y = tempMatrix.m[1][1];
	vAxisY.Z = tempMatrix.m[1][2];
	vAxisZ.X = tempMatrix.m[2][0];
	vAxisZ.Y = tempMatrix.m[2][1];
	vAxisZ.Z = tempMatrix.m[2][2];
	VECTOR3 vDelta = Subtract(WorldLocation, Data::CameraCache.POV.Location);

	VECTOR3 vTransformed;

	vTransformed.X = (vDelta.X * vAxisY.X) + (vDelta.Y * vAxisY.Y) + (vDelta.Z * vAxisY.Z);
	vTransformed.Y = (vDelta.X * vAxisZ.X) + (vDelta.Y * vAxisZ.Y) + (vDelta.Z * vAxisZ.Z);
	vTransformed.Z = (vDelta.X * vAxisX.X) + (vDelta.Y * vAxisX.Y) + (vDelta.Z * vAxisX.Z);


	if (vTransformed.Z < 0.0001f) return false;


	float FovAngle = Data::CameraCache.POV.FOV;

	float ScreenCenterX = Process::Width / 2;
	float ScreenCenterY = (Process::Height+40) / 2;

	Screenlocation.X = ScreenCenterX + vTransformed.X * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.Z;
	Screenlocation.Y = ScreenCenterY - vTransformed.Y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.Z;
	if (Screenlocation.X > Process::Width || Screenlocation.Y > Process::Height || Screenlocation.X < 0 || Screenlocation.Y < 0)
		return false;
	return true;
}

