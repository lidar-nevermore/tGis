#include "Palette.h"
#include "MyGDALRasterDataset.h"
#include "RasterBandStorageBlockWalker.h"
#include "GradientColor.h"

#include <cassert>
#include <vector>

using namespace std;

extern "C"
{
#include "hashmap.h"
}


static size_t _int32_hash_code(const void* key)
{
	return _uint32_hash(key);
}

static int _int32_is_equal(const void * a, const void * b)
{
	return _32bit_equals(a, b);
}

BEGIN_NAME_SPACE(tGis, Core)

static const int UNI_COLOR = 5000;
static const int MAX_COLOR = 50000;

class PaletteImpl
{
public:
	PaletteImpl(Palette* owner)
	{
		_owner = owner;
		hashmap_init(&_color,
			10000,
			sizeof(int),
			sizeof(unsigned char)*3,
			malloc,
			free,
			_int32_hash_code,
			_int32_is_equal,
			NULL,
			NULL);
	}

	~PaletteImpl()
	{
		hashmap_release(&_color);
	}

	Palette* _owner;
	HASHMAP _color;
};

Palette::Palette()
{
	_impl_ = new PaletteImpl(this);
}

Palette::~Palette()
{
	delete _impl_;
}

struct UserData
{
	Palette* palette;
	ProgressEventHandler* progress;
	union
	{
		GradientColor* color;
		unsigned char* flag;
	};
};

static inline void HSL2RGB(double h, double s, double v, unsigned char* r_, unsigned char* g_, unsigned char* b_)
{
	double r = v;
	double g = v;
	double b = v;
	if (s == 0)
	{
		unsigned char c = unsigned char(255 * v);
		*r_ = c;
		*g_ = c;
		*b_ = c;
	}

	double q = 0;
	if (v < 0.5)
		q = v * (1 + s);
	else
		q = v + s - (v * s);
	double p = 2 * v - q;
	double hk = h / 360;

	// r,g,b colors
	double tc[] = { hk + (1.0 / 3.0), hk, hk - (1.0 / 3.0) };
	double colors[] = { 0, 0, 0 };

	for (int color = 0; color < 3; color++)
	{
		if (tc[color] < 0)
			tc[color] += 1;
		if (tc[color] > 1)
			tc[color] -= 1;

		if (tc[color] < (1.0 / 6.0))
			colors[color] = p + ((q - p) * 6.0 * tc[color]);
		else
			if (tc[color] >= (1.0 / 6.0) && tc[color] < (1.0 / 2.0))
				colors[color] = q;
			else
				if (tc[color] >= (1.0 / 2.0) && tc[color] < (2.0 / 3.0))
					colors[color] = p + ((q - p) * 6.0 * (2.0 / 3.0 - tc[color]));
				else
					colors[color] = p;

		colors[color] *= 255; // convert to value expected by Color
	}

	*r_ = unsigned char(colors[0]);
	*g_ = unsigned char(colors[1]);
	*b_ = unsigned char(colors[2]);
}

static inline void RandomRGB(UserData* ud, unsigned char *r, unsigned char *g, unsigned char *b)
{
	double rmax = RAND_MAX;

	int nh = 0;
	if (ud != nullptr && ud->palette->GetColorCount() < UNI_COLOR)
	{
		//entry数量小于5000个时,找到不重复的色度
		do
		{
			nh = rand() % MAX_COLOR;
		} while (ud->flag[nh] > 0);

		ud->flag[nh] = 1;
	}
	else
	{
		nh = rand() % MAX_COLOR;
	}

	//色度
	double h = 360.0* nh / MAX_COLOR;
	//亮度大于等于0.4
	double v = 0.3 + 0.5*rand() / rmax;
	//饱和度大于等于0.5
	double s = 0.5 + 0.5*rand() / rmax;

	HSL2RGB(h, s, v, r, g, b);
}

static bool __stdcall RandomColorPalette(void* user,
	GDALRasterBand* band,
	StorageBlockInfo* blockInfo, void* blockBuffer,
	GDALDataType dataType, int dataBytes,
	unsigned char* aoiBuffer, double aoiNoDataValue,
	double progress)
{
	UserData* ud = (UserData*)user;
	char* inPix = (char*)blockBuffer;

	for (int i = 0; i < blockInfo->yBlockSize; i++)
	{
		for (int j = 0; j < blockInfo->xBlockSize; j++)
		{
			int v = (int)MyGDALGetPixelValue(dataType, inPix);

			if (!ud->palette->ExistsColor(v))
			{
				unsigned char r, g, b;
				RandomRGB(ud, &r, &g, &b);
				ud->palette->SetColor(v, r, g, b);

				//调色板中最多有50000个颜色
				if (ud->palette->GetColorCount() > MAX_COLOR)
					return false;
			}

			inPix += dataBytes;
		}
	}

	if (ud->progress != nullptr)
	{
		int progi = int(progress * 100);
		Progress prog(progi, progi);
		ud->progress->Raise(prog);
	}

	return true;
}

Palette * Palette::CreatePalette(MyGDALRasterDataset * dt, int band, ProgressEventHandler * progressHanler)
{
	Palette* pal = new Palette();
	UserData ud;
	ud.palette = pal;
	ud.progress = progressHanler;
	ud.flag = (unsigned char*)malloc(MAX_COLOR);
	memset(ud.flag, 0, MAX_COLOR);

	RasterBandStorageBlockWalker pixWalker(dt->GetGDALDataset(), band);
	srand((unsigned int)time(NULL));
	pixWalker.ForEachBlock(RandomColorPalette, &ud);

	free(ud.flag);

	return pal;
}

static bool __stdcall GradientColorPalette(void* user,
	GDALRasterBand* band,
	StorageBlockInfo* blockInfo, void* blockBuffer,
	GDALDataType dataType, int dataBytes,
	unsigned char* aoiBuffer, double aoiNoDataValue,
	double progress)
{
	UserData* ud = (UserData*)user;
	char* inPix = (char*)blockBuffer;
	double rmax = RAND_MAX;

	for (int i = 0; i < blockInfo->yBlockSize; i++)
	{
		for (int j = 0; j < blockInfo->xBlockSize; j++)
		{
			int v = (int)MyGDALGetPixelValue(dataType, inPix);

			if (!ud->palette->ExistsColor(v))
			{
				unsigned char r, g, b;
				double cpos = rand() / rmax;
				ud->color->GetColor(&r, &g, &b, cpos);
				ud->palette->SetColor(v, r, g, b);

				//调色板中最多有50000个颜色
				if (ud->palette->GetColorCount() > MAX_COLOR)
					return false;
			}

			inPix += dataBytes;
		}
	}

	if (ud->progress != nullptr)
	{
		int progi = int(progress * 100);
		Progress prog(progi, progi);
		ud->progress->Raise(prog);
	}

	return true;
}


Palette * Palette::CreatePalette(MyGDALRasterDataset * dt, int band, GradientColor * color, ProgressEventHandler * progressHanler)
{
	Palette* pal = new Palette();
	UserData ud;
	ud.palette = pal;
	ud.progress = progressHanler;
	ud.color = color;

	RasterBandStorageBlockWalker pixWalker(dt->GetGDALDataset(), band);
	srand((unsigned int)time(NULL));
	pixWalker.ForEachBlock(GradientColorPalette, &ud);

	return pal;
}

void Palette::GenerateColor(unsigned char * r, unsigned char * g, unsigned char * b)
{
	srand((unsigned int)time(NULL));
	RandomRGB(nullptr, r, g, b);
}

Palette * Palette::Clone()
{
	Palette* pal = new Palette();
	HASHMAP_ITER iter;

	hashmap_first(&_impl_->_color, &iter);
	while (iter.val_node != NULL)
	{
		HASHMAP_ENTRY e = hashmap_at(&_impl_->_color, &iter);
		hashmap_next(&_impl_->_color, &iter);
		unsigned char* c = (unsigned char*)e.val;
		pal->SetColor(*((int*)e.key), *c, *(c + 1), *(c + 2));
	}

	return pal;
}

size_t Palette::GetColorCount()
{
	return _impl_->_color.size;
}

void Palette::ForEachColor(void(__stdcall *pfunc)(int *entry, unsigned char *r, unsigned char *g, unsigned char *b, void *ud), void * ud)
{
	assert(pfunc != nullptr);

	HASHMAP_ITER iter;

	hashmap_first(&_impl_->_color, &iter);
	while (iter.val_node != NULL)
	{
		HASHMAP_ENTRY e = hashmap_at(&_impl_->_color, &iter);
		hashmap_next(&_impl_->_color, &iter);
		unsigned char* c = (unsigned char*)e.val;
		pfunc((int*)e.key, c, c + 1, c + 2, ud);
	}
}

void Palette::SetColor(int pos, unsigned char r, unsigned char g, unsigned char b)
{
	unsigned char* c = (unsigned char*)hashmap_set(&_impl_->_color, &pos, nullptr);
	c[0] = r;
	c[1] = g;
	c[2] = b;
}

void Palette::RemoveColor(int entry)
{
	hashmap_del(&_impl_->_color, &entry);
}

void Palette::GetColor(int pos, unsigned char * r, unsigned char * g, unsigned char * b)
{
	unsigned char* c = (unsigned char*)hashmap_get(&_impl_->_color, &pos, nullptr);
	if (c != nullptr)
	{
		*r = c[0];
		*g = c[1];
		*b = c[2];
	}
}

bool Palette::ExistsColor(int pos)
{
	return hashmap_exists(&_impl_->_color, &pos) > 0;
}

void Palette::ClearColor()
{
	hashmap_clear(&_impl_->_color);
}


END_NAME_SPACE(tGis, Core)
