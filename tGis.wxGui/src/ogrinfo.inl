/******************************************************************************
 *
 * Project:  OpenGIS Simple Features Reference Implementation
 * Purpose:  Simple client for viewing OGR driver data.
 * Author:   Frank Warmerdam, warmerdam@pobox.com
 *
 ******************************************************************************
 * Copyright (c) 1999, Frank Warmerdam
 * Copyright (c) 2008-2013, Even Rouault <even dot rouault at mines-paris dot org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ****************************************************************************/

#include "ogr_api.h"
#include "ogrsf_frmts.h"
#include "ogr_p.h"
#include "cpl_conv.h"
#include "cpl_string.h"
#include "cpl_multiproc.h"

#include <set>

#include <wx/wx.h>
#include <wx/string.h>

#undef EQUAL
#define EQUAL(a,b) (_stricmp(a, b))

#ifndef _info_str_Concat_
#define _info_str_Concat_

std::string _str_format(const char* format, va_list args)
{
	int count = _vsnprintf(NULL, 0, format, args);
	size_t bufsz = count * sizeof(wchar_t);
	char* buff = (char*)malloc(bufsz);
	_vsnprintf(buff, bufsz, format, args);

	std::string str(buff, count);
	free(buff);
	return str;
}

static void Concat(wxString& osRet, bool bStdoutOutput,
	const char* pszFormat, ...)
{
	va_list args;
	va_start(args, pszFormat);

	if (bStdoutOutput)
	{
		vfprintf(stdout, pszFormat, args);
	}
	else
	{
		try
		{
			wxString osTarget(_str_format(pszFormat, args).c_str());

			osRet += osTarget;
		}
		catch (const std::bad_alloc&)
		{
			CPLError(CE_Failure, CPLE_OutOfMemory, "Out of memory");
		}
	}

	va_end(args);
}

#endif // !_info_str_Concat_


static void ReportOnLayer( OGRLayer *, const char *, const char* pszGeomField,
                           OGRGeometry *,
                           int bListMDD,
                           int bShowMetadata,
                           char** papszExtraMDDomains,
                           int bFeatureCount,
                           int bExtent,
	                       wxString& osStr);

static void GDALInfoReportMetadata( GDALMajorObjectH hObject,
                                    int bListMDD,
                                    int bShowMetadata,
                                    char **papszExtraMDDomains,
	                                wxString& osStr);

/************************************************************************/
/*                             RemoveBOM()                              */
/************************************************************************/

/* Remove potential UTF-8 BOM from data (must be NUL terminated) */
static void RemoveBOM(GByte* pabyData)
{
    if( pabyData[0] == 0xEF && pabyData[1] == 0xBB && pabyData[2] == 0xBF )
    {
        memmove(pabyData, pabyData + 3, strlen((const char*)pabyData + 3) + 1);
    }
}


//"Usage: ogrinfo [--help-general] [-ro] [-q] [-where restricted_where|@filename]\n"
//"               [-spat xmin ymin xmax ymax] [-geomfield field] [-fid fid]\n"
//"               [-sql statement|@filename] [-dialect sql_dialect] [-al] [-rl] [-so] [-fields={YES/NO}]\n"
//"               [-geom={YES/NO/SUMMARY}] [-formats] [[-oo NAME=VALUE] ...]\n"
//"               [-nomd] [-listmdd] [-mdd domain|`all`]*\n"
//"               [-nocount] [-noextent]\n"
//"               datasource_name [layer [layer ...]]\n"


int     bVerbose = TRUE;
bool    bSuperQuiet = false;
int     bSummaryOnly = FALSE;
GIntBig nFetchFID = OGRNullFID;
char**  papszOptions = NULL;

void OGRInfo(GDALDataset* poDS,  wxString& osStr)
{
    char *pszWHERE = NULL;
    char        **papszLayers = NULL;
    OGRGeometry *poSpatialFilter = NULL;
    int         nRepeatCount = 1, bAllLayers = TRUE;
    char  *pszSQLStatement = NULL;
    const char  *pszDialect = NULL;
    int          nRet = 0;
    const char* pszGeomField = NULL;
    char      **papszExtraMDDomains = NULL;
    int                 bListMDD = TRUE;
    int                  bShowMetadata = TRUE;
    int         bFeatureCount = TRUE, bExtent = TRUE;
    bool        bDatasetGetNextFeature = false;

	bool bStdoutOutput = false;

	/* -------------------------------------------------------------------- */
	/*      Report general info.                                            */
	/* -------------------------------------------------------------------- */
	GDALDriverH hDriver = GDALGetDatasetDriver(poDS);

	Concat(osStr, bStdoutOutput, "Driver: %s/%s\n",
		GDALGetDriverShortName(hDriver),
		GDALGetDriverLongName(hDriver));

	char **papszFileList = GDALGetFileList(poDS);

	if (papszFileList == NULL || *papszFileList == NULL)
	{
		Concat(osStr, bStdoutOutput,
			"Files: none associated\n");
	}
	else
	{
		Concat(osStr, bStdoutOutput,
			"Files: %s\n", papszFileList[0]);

		for (int i = 1; papszFileList[i] != NULL; i++)
			Concat(osStr, bStdoutOutput,
				"       %s\n", papszFileList[i]);
	}
	CSLDestroy(papszFileList);

/* -------------------------------------------------------------------- */
/*      Some information messages.                                      */
/* -------------------------------------------------------------------- */

    GDALInfoReportMetadata( (GDALMajorObjectH)poDS,
                            bListMDD,
                            bShowMetadata,
                            papszExtraMDDomains,
		                    osStr);

    if( bDatasetGetNextFeature )
    {
        nRepeatCount = 0;  // skip layer reporting.

/* -------------------------------------------------------------------- */
/*      Set filters if provided.                                        */
/* -------------------------------------------------------------------- */
        if( pszWHERE != NULL || poSpatialFilter != NULL )
        {
            for( int iLayer = 0; iLayer < poDS->GetLayerCount(); iLayer++ )
            {
                OGRLayer        *poLayer = poDS->GetLayer(iLayer);

                if( poLayer == NULL )
                {
                    printf( "FAILURE: Couldn't fetch advertised layer %d!\n",
                            iLayer );
                    exit( 1 );
                }

                if( pszWHERE != NULL )
                {
                    if (poLayer->SetAttributeFilter( pszWHERE ) != OGRERR_NONE )
                    {
                        printf( "WARNING: SetAttributeFilter(%s) failed on layer %s.\n",
                                pszWHERE, poLayer->GetName() );
                    }
                }

                if( poSpatialFilter != NULL )
                {
                    if( pszGeomField != NULL )
                    {
                        OGRFeatureDefn      *poDefn = poLayer->GetLayerDefn();
                        int iGeomField = poDefn->GetGeomFieldIndex(pszGeomField);
                        if( iGeomField >= 0 )
                            poLayer->SetSpatialFilter( iGeomField, poSpatialFilter );
                        else
                            printf("WARNING: Cannot find geometry field %s.\n",
                                pszGeomField);
                    }
                    else
                        poLayer->SetSpatialFilter( poSpatialFilter );
                }
            }
        }

        std::set<OGRLayer*> oSetLayers;
        while( true )
        {
            OGRLayer* poLayer = NULL;
            OGRFeature* poFeature = poDS->GetNextFeature( &poLayer, NULL,
                                                          NULL, NULL );
            if( poFeature == NULL )
                break;
            if( papszLayers == NULL || poLayer == NULL ||
                CSLFindString(papszLayers, poLayer->GetName()) >= 0 )
            {
                if( bVerbose && poLayer != NULL &&
                    oSetLayers.find(poLayer) == oSetLayers.end() )
                {
                    oSetLayers.insert(poLayer);
                    int bSummaryOnlyBackup = bSummaryOnly;
                    bSummaryOnly = TRUE;
                    ReportOnLayer( poLayer, NULL, NULL, NULL,
                                   bListMDD, bShowMetadata,
                                   papszExtraMDDomains,
                                   bFeatureCount,
                                   bExtent,
						           osStr);
                    bSummaryOnly = bSummaryOnlyBackup;
                }
                if( !bSuperQuiet && !bSummaryOnly )
                    poFeature->DumpReadable( NULL, papszOptions );
            }
            OGRFeature::DestroyFeature( poFeature );
        }
    }

/* -------------------------------------------------------------------- */
/*      Special case for -sql clause.  No source layers required.       */
/* -------------------------------------------------------------------- */
    else if( pszSQLStatement != NULL )
    {
        nRepeatCount = 0;  // skip layer reporting.

        if( CSLCount(papszLayers) > 0 )
            printf( "layer names ignored in combination with -sql.\n" );

        OGRLayer *poResultSet
            = poDS->ExecuteSQL( pszSQLStatement,
                                (pszGeomField == NULL) ? poSpatialFilter: NULL,
                                pszDialect );

        if( poResultSet != NULL )
        {
            if( pszWHERE != NULL )
            {
                if (poResultSet->SetAttributeFilter( pszWHERE ) != OGRERR_NONE )
                {
                    printf( "FAILURE: SetAttributeFilter(%s) failed.\n", pszWHERE );
					return;
                }
            }

            if( pszGeomField != NULL )
                ReportOnLayer( poResultSet, NULL, pszGeomField, poSpatialFilter,
                               bListMDD, bShowMetadata, papszExtraMDDomains,
                               bFeatureCount, bExtent, osStr );
            else
                ReportOnLayer( poResultSet, NULL, NULL, NULL,
                               bListMDD, bShowMetadata, papszExtraMDDomains,
                               bFeatureCount, bExtent, osStr );
            poDS->ReleaseResultSet( poResultSet );
        }
    }

    /* coverity[tainted_data] */
    for( int iRepeat = 0; iRepeat < nRepeatCount; iRepeat++ )
    {
        if ( papszLayers == NULL || *papszLayers == NULL )
        {
            if( iRepeat == 0 )
                CPLDebug( "OGR", "GetLayerCount() = %d\n", poDS->GetLayerCount() );

/* -------------------------------------------------------------------- */
/*      Process each data source layer.                                 */
/* -------------------------------------------------------------------- */
            for( int iLayer = 0; iLayer < poDS->GetLayerCount(); iLayer++ )
            {
                OGRLayer        *poLayer = poDS->GetLayer(iLayer);

                if( poLayer == NULL )
                {
                    printf( "FAILURE: Couldn't fetch advertised layer %d!\n",
                            iLayer );
					return;
                }

                if (!bAllLayers)
                {
					Concat(osStr, bStdoutOutput, "%d: %s",
                            iLayer+1,
                            poLayer->GetName() );

                    int nGeomFieldCount =
                        poLayer->GetLayerDefn()->GetGeomFieldCount();
                    if( nGeomFieldCount > 1 )
                    {
						Concat(osStr, bStdoutOutput, " (");
                        for(int iGeom = 0;iGeom < nGeomFieldCount; iGeom ++ )
                        {
                            if( iGeom > 0 )
								Concat(osStr, bStdoutOutput, ", ");
                            OGRGeomFieldDefn* poGFldDefn =
                                poLayer->GetLayerDefn()->GetGeomFieldDefn(iGeom);
							Concat(osStr, bStdoutOutput, "%s",
                                OGRGeometryTypeToName(
                                    poGFldDefn->GetType() ) );
                        }
						Concat(osStr, bStdoutOutput, ")");
                    }
                    else if( poLayer->GetGeomType() != wkbUnknown )
						Concat(osStr, bStdoutOutput, " (%s)",
                                OGRGeometryTypeToName(
                                    poLayer->GetGeomType() ) );

					Concat(osStr, bStdoutOutput, "\n" );
                }
                else
                {
                    if( iRepeat != 0 )
                        poLayer->ResetReading();

                    ReportOnLayer( poLayer, pszWHERE, pszGeomField, poSpatialFilter,
                                   bListMDD, bShowMetadata, papszExtraMDDomains,
                                   bFeatureCount, bExtent, osStr );
                }
            }
        }
        else
        {
/* -------------------------------------------------------------------- */
/*      Process specified data source layers.                           */
/* -------------------------------------------------------------------- */

            for( char** papszIter = papszLayers;
                 *papszIter != NULL;
                 ++papszIter )
            {
                OGRLayer        *poLayer = poDS->GetLayerByName(*papszIter);

                if( poLayer == NULL )
                {
                    printf( "FAILURE: Couldn't fetch requested layer %s!\n",
                            *papszIter );
                    return;
                }

                if( iRepeat != 0 )
                    poLayer->ResetReading();

                ReportOnLayer( poLayer, pszWHERE, pszGeomField, poSpatialFilter,
                               bListMDD, bShowMetadata, papszExtraMDDomains,
                               bFeatureCount, bExtent, osStr );
            }
        }
    }

    CSLDestroy( papszLayers );
    CSLDestroy( papszOptions );
    CSLDestroy( papszExtraMDDomains );
    if (poSpatialFilter)
        OGRGeometryFactory::destroyGeometry( poSpatialFilter );
    CPLFree(pszSQLStatement);
    CPLFree(pszWHERE);
}


/************************************************************************/
/*                           ReportOnLayer()                            */
/************************************************************************/

static void ReportOnLayer( OGRLayer * poLayer, const char *pszWHERE,
                           const char* pszGeomField,
                           OGRGeometry *poSpatialFilter,
                           int bListMDD,
                           int bShowMetadata,
                           char** papszExtraMDDomains,
                           int bFeatureCount,
                           int bExtent,
	                       wxString& osStr)

{
    OGRFeatureDefn      *poDefn = poLayer->GetLayerDefn();
	bool bStdoutOutput = false;

/* -------------------------------------------------------------------- */
/*      Set filters if provided.                                        */
/* -------------------------------------------------------------------- */
    if( pszWHERE != NULL )
    {
        if (poLayer->SetAttributeFilter( pszWHERE ) != OGRERR_NONE )
        {
			printf("FAILURE: SetAttributeFilter(%s) failed.\n", pszWHERE );
            return;
        }
    }

    if( poSpatialFilter != NULL )
    {
        if( pszGeomField != NULL )
        {
            int iGeomField = poDefn->GetGeomFieldIndex(pszGeomField);
            if( iGeomField >= 0 )
                poLayer->SetSpatialFilter( iGeomField, poSpatialFilter );
            else
				printf("WARNING: Cannot find geometry field %s.\n",
                       pszGeomField);
        }
        else
            poLayer->SetSpatialFilter( poSpatialFilter );
    }

/* -------------------------------------------------------------------- */
/*      Report various overall information.                             */
/* -------------------------------------------------------------------- */
    if( !bSuperQuiet )
    {
		Concat(osStr, bStdoutOutput, "\n" );

		Concat(osStr, bStdoutOutput, "Layer name: %s\n", poLayer->GetName() );
    }

    GDALInfoReportMetadata( (GDALMajorObjectH)poLayer,
                            bListMDD,
                            bShowMetadata,
                            papszExtraMDDomains,
		                    osStr);

    if( bVerbose )
    {
        int nGeomFieldCount =
            poLayer->GetLayerDefn()->GetGeomFieldCount();
        if( nGeomFieldCount > 1 )
        {
            for(int iGeom = 0;iGeom < nGeomFieldCount; iGeom ++ )
            {
                OGRGeomFieldDefn* poGFldDefn =
                    poLayer->GetLayerDefn()->GetGeomFieldDefn(iGeom);
				Concat(osStr, bStdoutOutput, "Geometry (%s): %s\n", poGFldDefn->GetNameRef(),
                    OGRGeometryTypeToName( poGFldDefn->GetType() ) );
            }
        }
        else
        {
			Concat(osStr, bStdoutOutput, "Geometry: %s\n",
                    OGRGeometryTypeToName( poLayer->GetGeomType() ) );
        }

        if( bFeatureCount )
			Concat(osStr, bStdoutOutput, "Feature Count: " CPL_FRMT_GIB "\n", poLayer->GetFeatureCount() );

        OGREnvelope oExt;
        if( bExtent && nGeomFieldCount > 1 )
        {
            for(int iGeom = 0;iGeom < nGeomFieldCount; iGeom ++ )
            {
                if (poLayer->GetExtent(iGeom, &oExt, TRUE) == OGRERR_NONE)
                {
                    OGRGeomFieldDefn* poGFldDefn =
                        poLayer->GetLayerDefn()->GetGeomFieldDefn(iGeom);
					Concat(osStr, bStdoutOutput, "Extent (%s): (%f, %f) - (%f, %f)\n",
                           poGFldDefn->GetNameRef(),
                           oExt.MinX, oExt.MinY, oExt.MaxX, oExt.MaxY);
                }
            }
        }
        else if ( bExtent && poLayer->GetExtent(&oExt, TRUE) == OGRERR_NONE)
        {
			Concat(osStr, bStdoutOutput, "Extent: (%f, %f) - (%f, %f)\n",
                   oExt.MinX, oExt.MinY, oExt.MaxX, oExt.MaxY);
        }

        char    *pszWKT;

        if( nGeomFieldCount > 1 )
        {
            for(int iGeom = 0;iGeom < nGeomFieldCount; iGeom ++ )
            {
                OGRGeomFieldDefn* poGFldDefn =
                    poLayer->GetLayerDefn()->GetGeomFieldDefn(iGeom);
                OGRSpatialReference* poSRS = poGFldDefn->GetSpatialRef();
                if( poSRS == NULL )
                    pszWKT = CPLStrdup( "(unknown)" );
                else
                {
                    poSRS->exportToPrettyWkt( &pszWKT );
                }

				Concat(osStr, bStdoutOutput, "SRS WKT (%s):\n%s\n",
                        poGFldDefn->GetNameRef(), pszWKT );
                CPLFree( pszWKT );
            }
        }
        else
        {
            if( poLayer->GetSpatialRef() == NULL )
                pszWKT = CPLStrdup( "(unknown)" );
            else
            {
                poLayer->GetSpatialRef()->exportToPrettyWkt( &pszWKT );
            }

			Concat(osStr, bStdoutOutput, "Layer SRS WKT:\n%s\n", pszWKT );
            CPLFree( pszWKT );
        }

        if( strlen(poLayer->GetFIDColumn()) > 0 )
			Concat(osStr, bStdoutOutput, "FID Column = %s\n",
                    poLayer->GetFIDColumn() );

        for(int iGeom = 0;iGeom < nGeomFieldCount; iGeom ++ )
        {
            OGRGeomFieldDefn* poGFldDefn =
                poLayer->GetLayerDefn()->GetGeomFieldDefn(iGeom);
            if( nGeomFieldCount == 1 &&
                EQUAL(poGFldDefn->GetNameRef(), "")  && poGFldDefn->IsNullable() )
                break;
			Concat(osStr, bStdoutOutput, "Geometry Column ");
            if( nGeomFieldCount > 1 )
				Concat(osStr, bStdoutOutput, "%d ", iGeom + 1);
            if( !poGFldDefn->IsNullable() )
				Concat(osStr, bStdoutOutput, "NOT NULL ");
			Concat(osStr, bStdoutOutput, "= %s\n", poGFldDefn->GetNameRef() );
        }

        for( int iAttr = 0; iAttr < poDefn->GetFieldCount(); iAttr++ )
        {
            OGRFieldDefn    *poField = poDefn->GetFieldDefn( iAttr );
            const char* pszType = (poField->GetSubType() != OFSTNone) ?
                CPLSPrintf("%s(%s)",
                           poField->GetFieldTypeName( poField->GetType() ),
                           poField->GetFieldSubTypeName(poField->GetSubType())) :
                poField->GetFieldTypeName( poField->GetType() );
			Concat(osStr, bStdoutOutput, "%s: %s (%d.%d)",
                    poField->GetNameRef(),
                    pszType,
                    poField->GetWidth(),
                    poField->GetPrecision() );
            if( !poField->IsNullable() )
				Concat(osStr, bStdoutOutput, " NOT NULL");
            if( poField->GetDefault() != NULL )
				Concat(osStr, bStdoutOutput, " DEFAULT %s", poField->GetDefault() );
			Concat(osStr, bStdoutOutput, "\n" );
        }
    }

/* -------------------------------------------------------------------- */
/*      Read, and dump features.                                        */
/* -------------------------------------------------------------------- */
    OGRFeature  *poFeature = NULL;

    if( nFetchFID == OGRNullFID && !bSummaryOnly )
    {
        while( (poFeature = poLayer->GetNextFeature()) != NULL )
        {
            if( !bSuperQuiet )
                poFeature->DumpReadable( NULL, papszOptions );
            OGRFeature::DestroyFeature( poFeature );
        }
    }
    else if( nFetchFID != OGRNullFID )
    {
        poFeature = poLayer->GetFeature( nFetchFID );
        if( poFeature == NULL )
        {
            printf( "Unable to locate feature id " CPL_FRMT_GIB " on this layer.\n",
                    nFetchFID );
        }
        else
        {
            poFeature->DumpReadable( NULL, papszOptions );
            OGRFeature::DestroyFeature( poFeature );
        }
    }
}

/************************************************************************/
/*                       GDALInfoPrintMetadata()                        */
/************************************************************************/
static void GDALInfoPrintMetadata( GDALMajorObjectH hObject,
                                   const char *pszDomain,
                                   const char *pszDisplayedname,
                                   const char *pszIndent,
	                               wxString& osStr)
{
    bool bIsxml = false;
	bool bStdoutOutput = false;

    if (pszDomain != NULL && STARTS_WITH_CI(pszDomain, "xml:"))
        bIsxml = true;

    char **papszMetadata = GDALGetMetadata( hObject, pszDomain );
    if( CSLCount(papszMetadata) > 0 )
    {
		Concat(osStr, bStdoutOutput, "%s%s:\n", pszIndent, pszDisplayedname );
        for( int i = 0; papszMetadata[i] != NULL; i++ )
        {
            if (bIsxml)
				Concat(osStr, bStdoutOutput, "%s%s\n", pszIndent, papszMetadata[i] );
            else
				Concat(osStr, bStdoutOutput, "%s  %s\n", pszIndent, papszMetadata[i] );
        }
    }
}

/************************************************************************/
/*                       GDALInfoReportMetadata()                       */
/************************************************************************/
static void GDALInfoReportMetadata( GDALMajorObjectH hObject,
                                    int bListMDD,
                                    int bShowMetadata,
                                    char **papszExtraMDDomains,
	                                wxString& osStr)
{
    const char* pszIndent = "";
	bool bStdoutOutput = false;
    /* -------------------------------------------------------------------- */
    /*      Report list of Metadata domains                                 */
    /* -------------------------------------------------------------------- */
    if( bListMDD )
    {
        char** papszMDDList = GDALGetMetadataDomainList( hObject );
        char** papszIter = papszMDDList;

        if( papszMDDList != NULL )
			Concat(osStr, bStdoutOutput, "%sMetadata domains:\n", pszIndent );
        while( papszIter != NULL && *papszIter != NULL )
        {
            if( EQUAL(*papszIter, "") )
				Concat(osStr, bStdoutOutput, "%s  (default)\n", pszIndent);
            else
				Concat(osStr, bStdoutOutput, "%s  %s\n", pszIndent, *papszIter );
            papszIter ++;
        }
        CSLDestroy(papszMDDList);
    }

    if (!bShowMetadata)
        return;

    /* -------------------------------------------------------------------- */
    /*      Report default Metadata domain.                                 */
    /* -------------------------------------------------------------------- */
    GDALInfoPrintMetadata( hObject, NULL, "Metadata", pszIndent, osStr );

    /* -------------------------------------------------------------------- */
    /*      Report extra Metadata domains                                   */
    /* -------------------------------------------------------------------- */
    if (papszExtraMDDomains != NULL) {
        char **papszExtraMDDomainsExpanded = NULL;

        if( EQUAL(papszExtraMDDomains[0], "all") &&
            papszExtraMDDomains[1] == NULL )
        {
            char** papszMDDList = GDALGetMetadataDomainList( hObject );
            char** papszIter = papszMDDList;

            while( papszIter != NULL && *papszIter != NULL )
            {
                if( !EQUAL(*papszIter, "") )
                {
                    papszExtraMDDomainsExpanded = CSLAddString(papszExtraMDDomainsExpanded, *papszIter);
                }
                papszIter ++;
            }
            CSLDestroy(papszMDDList);
        }
        else
        {
            papszExtraMDDomainsExpanded = CSLDuplicate(papszExtraMDDomains);
        }

        for( int iMDD = 0; papszExtraMDDomainsExpanded != NULL &&
                           papszExtraMDDomainsExpanded[iMDD] != NULL; iMDD++ )
        {
            char pszDisplayedname[256];
            snprintf(pszDisplayedname, 256, "Metadata (%s)", papszExtraMDDomainsExpanded[iMDD]);
            GDALInfoPrintMetadata( hObject, papszExtraMDDomainsExpanded[iMDD], pszDisplayedname, pszIndent, osStr );
        }

        CSLDestroy(papszExtraMDDomainsExpanded);
    }
}


