namespace Offset
{
	const DWORD PointerUe4 = 0xE0C3860;  // never change
	const DWORD Gword = 0xA7DAD54;
	//Signature
	//Gword: 8b 52 ? 89 d3 89 74 24
	//Gword: 8b 7f ? c7 45 ? ? ? ? ? 89 7d ? 8b 35
	//Gword: 8b 7f ? c7 45 ? ? ? ? ? 83 ff ? 9f 0f 90 ? 0f 84 ? ? ? ? 8b 4d
	//Gword: 8b 7f ? c7 45 ? ? ? ? ? 8b bf
	//Gword: 8b 7f ? 8b 4d ? 8b 51 ? 8b 8f ? ? ? ? 89 4d ? 8b 4d ? 89 4d ? 8b 8a ? ? ? ? 89 4d ? c7 45 ? ? ? ? ? 8b 4d
	//Gword: 8b 7f ? bb ? ? ? ? 83 ff ? 0f 84 ? ? ? ? 89 fe b9
	//Gword: 8b 52 ? c7 45 ? ? ? ? ? 8b 0d


	const DWORD Gnames = 0xA3E2004;
	//gNames Signature: 00 60 F7 2F 00 F0 24 57 00 A0 39 57 00 00 C9 79 00 E0 53 7A 00 90 35 7D 00 A0 25 7D 00 D0 EC 7D

	const DWORD VIEWMATRIX = 0xA7B96B0;// Nah No Need Update this , this method is shit



	const DWORD PersistentLevel = 0x20; // never change
	const DWORD EntityList = 0x70; // never change
	const DWORD EntityCount = EntityList + 4; // never change	
	const DWORD ActorsClass = 0x384; // never change
	const DWORD isDead = 0xa6c; //bool bDead;
	const DWORD NetDriver = 0x24; // never change
	const DWORD ServerConnection = 0x64;//NetConnection* ServerConnection; //Never Change
	const DWORD PlayerController = 0x20;//PlayerController* PlayerController; // never change


	const DWORD AcknowledgedPawn = 0x3a8;//Pawn* AcknowledgedPawn; // Change
	//SIGNATURE
	//AcknowledgedPawn: 8b b7 ? ? ? ? 89 75 ? 29 f2
	//AcknowledgedPawn: 8b b9 ? ? ? ? 89 7d ? 39 f7 9f 0f 90 ? 0f 84
	const DWORD STExtraBaseCharacter = 0x1eb8;//STExtraBaseCharacter* STExtraBaseCharacter; // Change
	//SIGNATURE
	//STExtraBaseCharacter: 8b 8f ? ? ? ? 89 8d ? ? ? ? 89 5d
	//STExtraBaseCharacter: 8b b7 ? ? ? ? 8d 97 ? ? ? ? 89 55 ? 89 f3 89 75

	const DWORD PlayerCameraManager = 0x3b8;
	//SIGNATURE:
	//PlayerCameraManager: 8b be ? ? ? ? 83 ff ? 0f 84 ? ? ? ? 8b 57 ? 8d 4c 24
	//PlayerCameraManager: 8b ba ? ? ? ? 83 ff ? 0f 84 ? ? ? ? b9 ? ? ? ? 89 4d ? 89 54 24
	//PlayerCameraManager: 8b ba ? ? ? ? 89 7d ? 83 ff ? 9f 0f 90 ? 90 0f 84 ? ? ? ? 8d b4 24
	//PlayerCameraManager: 8b ba ? ? ? ? 89 7d ? 66 0f d6 7e
	//PlayerCameraManager: 8b b2 ? ? ? ? 89 75 ? 83 fe ? 0f 84 ? ? ? ? 8b 96
	//PlayerCameraManager: 8b bb ? ? ? ? 89 7d ? 83 ff ? 0f 84 ? ? ? ? 8b 8f
	//PlayerCameraManager: 8b 8f ? ? ? ? 89 4d ? 83 f9 ? 9f 0f 90 ? 74 ? 8b 4d ? 89 4d
	//PlayerCameraManager: 8b b9 ? ? ? ? 89 7d ? 8d 74 24 ? 89 75 ? c7 45
	//PlayerCameraManager: 8b 91 ? ? ? ? 89 55 ? 83 7d ? ? 89 5d
	//PlayerCameraManager: 8b 99 ? ? ? ? 89 5d ? 83 fb ? 0f 84 ? ? ? ? 8b 8b ? ? ? ? 8d 93
	//PlayerCameraManager: 8b be ? ? ? ? 89 f3 89 75 ? 83 ff
	//PlayerCameraManager: 8b 9b ? ? ? ? 89 5d ? 83 fb ? 0f 84 ? ? ? ? 8b 8b ? ? ? ? 8d bb ? ? ? ? 89 7d ? 89 ca 83 f9 ? 9f 0f 90 ? 74 ? 8b b3 ? ? ? ? 89 75 ? 89 7d ? c7 45 ? ? ? ? ? e9
	//PlayerCameraManager: 8b ba ? ? ? ? 89 7d ? 83 ff ? 0f 84 ? ? ? ? 8b 97
	//PlayerCameraManager: 8b b2 ? ? ? ? 83 fe ? 0f 84 ? ? ? ? c7 45 ? ? ? ? ? 8b 55
	const DWORD CameraCache = 0x3c0;
	//SIGNATURE:
	//CameraCache: 8b 9a ? ? ? ? 89 9a ? ? ? ? 8d 9a ? ? ? ? 89 5d
	//CameraCache: 89 97 ? ? ? ? 8d 97 ? ? ? ? 89 55 ? f3 0f 7e 7c 24

	const DWORD TeamId = 0x6e8; // ?
	//SIGNATURE:
	//TeamId: 8b 92 ? ? ? ? 8b b3 ? ? ? ? 89 75 ? 39 f2
	//TeamId: 8b b3 ? ? ? ? 89 75 ? 39 f2 9f 0f 90 ? 74
	
	const DWORD Mesh = 0x398; // ?
	//SIGNATURE:
	//Mesh: 8b 9a ? ? ? ? 89 5c 24 ? 83 fb ? 9f
	//Mesh: 8b 9a ? ? ? ? 83 fb ? be
	//Mesh: 8b b6 ? ? ? ? 83 fe ? 9f 0f 90 ? 74 ? bf
	//Mesh: 8b b3 ? ? ? ? bf ? ? ? ? eb ? 89 75
	//Mesh: 8b 92 ? ? ? ? 89 c3 74
	//Mesh: 8b 8e ? ? ? ? 89 4d ? b8 ? ? ? ? c7 45
	//Mesh: 8b 93 ? ? ? ? 89 55 ? 83 fa ? 9f 0f 90 ? 0f 84 ? ? ? ? 8b 53
	const DWORD BodyAddv = 0x150; // This Very Stupid :) // u will no need update

	const DWORD MinLOD = 0x730;
	//SIGNATURE:
	//MinLod: 8b 8a ? ? ? ? 89 8d ? ? ? ? 89 f1
	const DWORD Health = 0xa54;
	//SIGNATURE
	//Health: 8B 7D 40 81 C7 ?? ?? ?? ?? 8B 4D 40 66 0F 6E 91 ?? ?? ?? ?? 66 0F 7E 95 80 00 00 00
	const DWORD HealthMax = 0xa58; // = Health+=0x4;
	const DWORD Name = 0x6c0;  // = )) try Search idk lazy:(


	const DWORD RootComponent = 0x158; // neverchange
	const DWORD Position = 0x160; // neverchange

}