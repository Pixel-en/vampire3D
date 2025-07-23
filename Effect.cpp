#include "Effect.h"
#include "Engine/CsvReader.h"

Effect::Effect(GameObject* parent)
	:GameObject(parent, "Effect")
{
}

Effect::~Effect()
{
}

void Effect::Initialize()
{
	LoadFile();
}

void Effect::Release()
{
}

int Effect::PlayEffect(std::string ObjName, std::string EmName, XMFLOAT3 _pos)
{
	for (int i = 0;i < emitterList_.size();i++)
	{
		//リストの中にオブジェクト名とエミッター名が一致するものがあるなら
		if (emitterList_[i].objectName == ObjName && emitterList_[i].emitterName == EmName) {
			
			EmitterData data = emitterList_[i].emitterData;
			data.position = _pos + data.position;
			int handle = VFX::Start(data);
			return handle;
		}
	}

}


void Effect::LoadFile()
{
	//なんかもっといい方法がある気がする
	enum EmitterFileData {
		OBJECTNAME,
		EMITTERNAME,
		FILENAME,
		POSITIONX,
		POSITIONY,
		POSITIONZ,
		POSITIONRNDX,
		POSITIONRNDY,
		POSITIONRNDZ,
		DIRECTIONX,
		DIRECTIONY,
		DIRECTIONZ,
		DIRECTIONRNDX,
		DIRECTIONRNDY,
		DIRECTIONRNDZ,
		SPEED,
		SPEEDRND,
		ACCEL,
		DELAY,
		NUMBER,
		GRAVITY,
		LIFETIME,
		COLORR,
		COLORG,
		COLORB,
		COLORA,
		DELTACOLORR,
		DELTACOLORG,
		DELTACOLORB,
		DELTACOLORA,
		ROTATEX,
		ROTATEY,
		ROTATEZ,
		ROTATERNDX,
		ROTATERNDY,
		ROTATERNDZ,
		SPINX,
		SPINY,
		SPINZ,
		SIZEX,
		SIZEY,
		SIZERNDX,
		SIZERNDY,
		SCALEX,
		SCALEY,
		ISBILLBOARD,
		LOOPCOUNT,
		MAX
	};

	CsvReader csv;
	csv.Load("Assets\\CSV\\EmitterFile.csv");

	//パーティクルのデータを読み込む
	for (int i = 1; i < csv.GetHeight(); i++) {
		EmitterFile ef;
		for (int j = 0; j < csv.GetWidth() || j < MAX; j++) {
			if (csv.GetString(j, i) == "def")
				continue; //defは無視
			switch (j)
			{
			case OBJECTNAME:
				ef.objectName = csv.GetString(j, i);
				break;
			case EMITTERNAME:
				ef.emitterName = csv.GetString(j, i);
				break;
			case FILENAME:
				ef.emitterData.textureFileName = "Assets\\Image\\Paticle\\" + csv.GetString(j, i) + ".png";
				break;
			case POSITIONX:
				ef.emitterData.position.x = csv.GetValue(j, i);
				break;
			case POSITIONY:
				ef.emitterData.position.y = csv.GetValue(j, i);
				break;
			case POSITIONZ:
				ef.emitterData.position.z = csv.GetValue(j, i);
				break;
			case POSITIONRNDX:
				ef.emitterData.positionRnd.x = csv.GetValue(j, i);
				break;
			case POSITIONRNDY:
				ef.emitterData.positionRnd.y = csv.GetValue(j, i);
				break;
			case POSITIONRNDZ:
				ef.emitterData.positionRnd.z = csv.GetValue(j, i);
				break;
			case DIRECTIONX:
				ef.emitterData.direction.x = csv.GetValue(j, i);
				break;
			case DIRECTIONY:
				ef.emitterData.direction.y = csv.GetValue(j, i);
				break;
			case DIRECTIONZ:
				ef.emitterData.direction.z = csv.GetValue(j, i);
				break;
			case DIRECTIONRNDX:
				ef.emitterData.directionRnd.x = csv.GetValue(j, i);
				break;
			case DIRECTIONRNDY:
				ef.emitterData.directionRnd.y = csv.GetValue(j, i);
				break;
			case DIRECTIONRNDZ:
				ef.emitterData.directionRnd.z = csv.GetValue(j, i);
				break;
			case SPEED:
				ef.emitterData.speed = csv.GetValue(j, i);
				break;
			case SPEEDRND:
				ef.emitterData.speedRnd = csv.GetValue(j, i);
				break;
			case ACCEL:
				ef.emitterData.accel = csv.GetValue(j, i);
				break;
			case DELAY:
				ef.emitterData.delay = csv.GetValue(j, i);
				break;
			case NUMBER:
				ef.emitterData.number = csv.GetValue(j, i);
				break;
			case GRAVITY:
				ef.emitterData.gravity = csv.GetValue(j, i);
				break;
			case LIFETIME:
				ef.emitterData.lifeTime = csv.GetValue(j, i);
				break;
			case COLORR:
				ef.emitterData.color.x = csv.GetValue(j, i);
				break;
			case COLORG:
				ef.emitterData.color.y = csv.GetValue(j, i);
				break;
			case COLORB:
				ef.emitterData.color.z = csv.GetValue(j, i);
				break;
			case COLORA:
				ef.emitterData.color.w = csv.GetValue(j, i);
				break;
			case DELTACOLORR:
				ef.emitterData.deltaColor.x = csv.GetValue(j, i);
				break;
			case DELTACOLORG:
				ef.emitterData.deltaColor.y = csv.GetValue(j, i);
				break;
			case DELTACOLORB:
				ef.emitterData.deltaColor.z = csv.GetValue(j, i);
				break;
			case DELTACOLORA:
				ef.emitterData.deltaColor.w = csv.GetValue(j, i);
				break;
			case ROTATEX:
				ef.emitterData.rotate.x = csv.GetValue(j, i);
				break;
			case ROTATEY:
				ef.emitterData.rotate.y = csv.GetValue(j, i);
				break;
			case ROTATEZ:
				ef.emitterData.rotate.z = csv.GetValue(j, i);
				break;
			case ROTATERNDX:
				ef.emitterData.rotateRnd.x = csv.GetValue(j, i);
				break;
			case ROTATERNDY:
				ef.emitterData.rotateRnd.y = csv.GetValue(j, i);
				break;
			case ROTATERNDZ:
				ef.emitterData.rotateRnd.z = csv.GetValue(j, i);
				break;
			case SPINX:
				ef.emitterData.spin.x = csv.GetValue(j, i);
				break;
			case SPINY:
				ef.emitterData.spin.y = csv.GetValue(j, i);
				break;
			case SPINZ:
				ef.emitterData.spin.z = csv.GetValue(j, i);
				break;
			case SIZEX:
				ef.emitterData.size.x = csv.GetValue(j, i);
				break;
			case SIZEY:
				ef.emitterData.size.y = csv.GetValue(j, i);
				break;
			case SIZERNDX:
				ef.emitterData.sizeRnd.x = csv.GetValue(j, i);
				break;
			case SIZERNDY:
				ef.emitterData.sizeRnd.y = csv.GetValue(j, i);
				break;
			case SCALEX:
				ef.emitterData.scale.x = csv.GetValue(j, i);
				break;
			case SCALEY:
				ef.emitterData.scale.y = csv.GetValue(j, i);
				break;
			case ISBILLBOARD:
				if (csv.GetValue(j, i) >= 1)
					ef.emitterData.isBillBoard = true;
				else
					ef.emitterData.isBillBoard = false;
				break;
			case LOOPCOUNT:
				break;
			case MAX:
				break;
			default:
				break;
			}

		}

		emitterList_.push_back(ef);
	}

}