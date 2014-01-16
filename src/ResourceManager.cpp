#include "ResourceManager.h"

using namespace mzj;

std::string ResourceManager::GetResourcePath( unsigned int RES_ID )
{
	switch ( RES_ID )
	{
		case RES_TEXTURE:	return "data/texture/";
		case RES_OBJECT:	return "data/object/";
		case RES_AUDIO:		return "data/audio/";
		case RES_TEXT:		return "data/text/";
		case RES_DATA:
		default:			return "data/";
	}
}