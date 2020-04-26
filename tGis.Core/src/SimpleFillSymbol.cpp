#include "SimpleFillSymbol.h"
#include "SimpleSymbolLibrary.h"
#include <memory>

#include "ISurface.h"

#include "glad.h"
#include "glutess.h"

extern "C"
{
#include "bi_list.h"
}

BEGIN_NAME_SPACE(tGis, Core)

class gluTessHelper
{
private:
	static gluTessHelper* _instance;
public:
	static GLUtesselator* GetTesselator()
	{
		if (_instance == nullptr)
		{
			_instance = new gluTessHelper();
			static std::unique_ptr<gluTessHelper> shit(_instance);
		}

		return _instance->_tesselator;
	}

	static void TessEnd()
	{
		bi_list_release(&(_instance->_newVertexList));
	}


	~gluTessHelper()
	{
		gluDeleteTess(_tesselator);
		bi_list_release(&_newVertexList);
	}

private:
	static void GLAPIENTRY tGisCombine(
		const GLdouble newVertex[3],
		const GLdouble *neighborVertex[4],
		const GLfloat neighborWeight[4],
		GLdouble **outData)
	{
		*outData = (GLdouble*)bi_list_insert_head(&(_instance->_newVertexList), newVertex);
	}

private:
	gluTessHelper()
	{
		_tesselator = gluNewTess();
		gluTessCallback(_tesselator, GLU_TESS_BEGIN, (_GLUfuncptr)glBegin);
		gluTessCallback(_tesselator, GLU_TESS_END, (_GLUfuncptr)glEnd);
		gluTessCallback(_tesselator, GLU_TESS_VERTEX, (_GLUfuncptr)glVertex3dv);
		gluTessCallback(_tesselator, GLU_TESS_COMBINE, (_GLUfuncptr)tGisCombine);
		gluTessProperty(_tesselator, GLU_TESS_TOLERANCE, GL_TRUE);
		gluTessProperty(_tesselator, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);

		bi_list_init(&_newVertexList,
			3 * sizeof(GLdouble), //自相交形成的顶点
			malloc,
			free,
			nullptr,
			nullptr);
	}

	GLUtesselator* _tesselator;
	BI_LIST _newVertexList;
};

gluTessHelper* gluTessHelper::_instance = nullptr;

SimpleFillSymbol::SimpleFillSymbol()
	:SimpleFillSymbol(SimpleFillSymbol::Solid)
{
	_lib = SimpleSymbolLibrary::GetFillSymbolLibrary();
}

SimpleFillSymbol::SimpleFillSymbol(int t)
{
	_lib = SimpleSymbolLibrary::GetFillSymbolLibrary();

	_type = t;
	_r = 30;
	_g = 236;
	_b = 63;
	_a = 255;
}


SimpleFillSymbol::~SimpleFillSymbol()
{
}

const int SimpleFillSymbol::GetId()
{
	return _type;
}

void SimpleFillSymbol::Paint(ISurface * surf, int count, int * x, int * y)
{
	size_t ndcSize = count * 3 * sizeof(GLdouble);
	GLdouble* ndc = (GLdouble*)malloc(ndcSize);

	//if (ndc == nullptr)
	//	return;

	GLfloat red = _r / 255.0f;
	GLfloat green = _g / 255.0f;
	GLfloat blue = _g / 255.0f;
	GLfloat alpha = _a / 255.0f;

	glColor4f(red, green, blue, alpha);

	GLUtesselator* tess = gluTessHelper::GetTesselator();
	GLdouble* ndc1 = ndc;
	gluTessBeginPolygon(tess, nullptr);
	gluTessBeginContour(tess);
	for (int i = 0; i < count; i++)
	{
		ndc1[2] = 0.0;
		surf->Surface2glndc(x[i], y[i], ndc1, ndc1 +1);
		gluTessVertex(tess, ndc1, ndc1);
		ndc1 += 3;
	}
	gluTessEndContour(tess);
	gluTessEndPolygon(tess);
	free(ndc);
	gluTessHelper::TessEnd();
}

void SimpleFillSymbol::Paint(ISurface* surf, int contourCount, int* ptCount, int** x, int** y)
{
	size_t ndcSize = 0;
	for (int c = 0; c < contourCount; c++)
		ndcSize += (ptCount[c] * 3 * sizeof(GLdouble));
	GLdouble* ndc = (GLdouble*)malloc(ndcSize);


	GLfloat red = _r / 255.0f;
	GLfloat green = _g / 255.0f;
	GLfloat blue = _g / 255.0f;
	GLfloat alpha = _a / 255.0f;

	glColor4f(red, green, blue, alpha);
	GLUtesselator* tess = gluTessHelper::GetTesselator();
	GLdouble* ndc1 = ndc;
	gluTessBeginPolygon(tess, 0);

	for (int c = 0; c < contourCount; c++)
	{
		int* xc = x[c];
		int* yc = y[c];
		gluTessBeginContour(tess);
		for (int i = 0; i < ptCount[c]; i++)
		{
			ndc1[2] = 0.0;
			surf->Surface2glndc(xc[i], yc[i], ndc1, ndc1 + 1);
			gluTessVertex(tess, ndc1, ndc1);
			ndc1 += 3;
		}
		gluTessEndContour(tess);
	}

	gluTessEndPolygon(tess);

	free(ndc);
	gluTessHelper::TessEnd();
}

void SimpleFillSymbol::GetColor(unsigned char * r, unsigned char * g, unsigned char * b, unsigned char * a)
{
	*r = _r;
	*g = _g;
	*b = _b;
	*a = _a;
}

void SimpleFillSymbol::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}


END_NAME_SPACE(tGis, Core)

