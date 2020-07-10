/******************************************************************************
 *
 * Project:  GDAL Utilities
 * Purpose:  Command line application to list info about a file.
 * Author:   Frank Warmerdam, warmerdam@pobox.com
 *
 * ****************************************************************************
 * Copyright (c) 1998, Frank Warmerdam
 * Copyright (c) 2007-2015, Even Rouault <even.rouault at spatialys.com>
 * Copyright (c) 2015, Faza Mahamood
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

#undef STARTS_WITH_CI
#define STARTS_WITH_CI(a,b) (_strnicmp(a, b, strlen(b)))

#undef EQUAL
#define EQUAL(a,b) (_stricmp(a, b))

#include "cpl_port.h"

#include <cmath>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include <string>
#include <vector>

#include <wx/wx.h>
#include <wx/string.h>

#include "gdal.h"
#include "gdal_alg.h"
#include "gdal_priv.h"
#include "gdal_rat.h"


using std::vector;


/************************************************************************/
/*                           GDALInfoOptions                            */
/************************************************************************/

/** Options for use with GDALInfo(). GDALInfoOptions* must be allocated and
 * freed with GDALInfoOptionsNew() and GDALInfoOptionsFree() respectively.
 */
struct GDALInfoOptions
{
    int bComputeMinMax;

    /*! report histogram information for all bands */
    int bReportHistograms;

    /*! report a PROJ.4 string corresponding to the file's coordinate system */
    int bReportProj4;

    /*! read and display image statistics. Force computation if no statistics
        are stored in an image */
    int bStats;

    /*! read and display image statistics. Force computation if no statistics
        are stored in an image.  However, they may be computed based on
        overviews or a subset of all tiles. Useful if you are in a hurry and
        don't want precise stats. */
    int bApproxStats;

    int bSample;

    /*! force computation of the checksum for each band in the dataset */
    int bComputeChecksum;

    /*! allow or suppress ground control points list printing. It may be useful
        for datasets with huge amount of GCPs, such as L1B AVHRR or HDF4 MODIS
        which contain thousands of them. */
    int bShowGCPs;

    /*! allow or suppress metadata printing. Some datasets may contain a lot of
        metadata strings. */
    int bShowMetadata;

    /*! allow or suppress printing of raster attribute table */
    int bShowRAT;

    /*! allow or suppress printing of color table */
    int bShowColorTable;

    /*! list all metadata domains available for the dataset */
    int bListMDD;

    /*! display the file list or the first file of the file list */
    int bShowFileList;

    /*! report metadata for the specified domains. "all" can be used to report
        metadata in all domains.
        */
    char **papszExtraMDDomains;

    bool bStdoutOutput;
};

#ifndef _info_str_Concat_
#define _info_str_Concat_

static std::string _str_format(const char* format, va_list args)
{
	int count = _vsnprintf(NULL, 0, format, args);
	size_t bufsz = count * sizeof(wchar_t);
	char* buff = (char*)malloc(bufsz);
	_vsnprintf(buff, bufsz, format, args);

	std::string str(buff, count);
	free(buff);
	return str;
}

static void Concat( wxString& osRet, bool bStdoutOutput,
                    const char* pszFormat, ... )
{
    va_list args;
    va_start( args, pszFormat );

    if( bStdoutOutput )
    {
        vfprintf(stdout, pszFormat, args );
    }
    else
    {
        try
        {
            wxString osTarget(_str_format(pszFormat,args).c_str());

			osRet += osTarget;
        }
        catch( const std::bad_alloc& )
        {
            CPLError(CE_Failure, CPLE_OutOfMemory, "Out of memory");
        }
    }

    va_end( args );
}

#endif // !_info_str_Concat_


static void GDALInfoReportMetadata( const GDALInfoOptions* psOptions,
                                    GDALMajorObjectH hObject,
                                    bool bIsBand,
                                    wxString& osStr );

static int
GDALInfoReportCorner( const GDALInfoOptions* psOptions,
                      GDALDatasetH hDataset,
                      OGRCoordinateTransformationH hTransform,
                      const char * corner_name,
                      double x,
                      double y,
                      wxString& osStr );

/************************************************************************/
/*                             GDALInfo()                               */
/************************************************************************/

/**
 * Lists various information about a GDAL supported raster dataset.
 *
 * This is the equivalent of the <a href="gdalinfo.html">gdalinfo</a> utility.
 *
 * GDALInfoOptions* must be allocated and freed with GDALInfoOptionsNew()
 * and GDALInfoOptionsFree() respectively.
 *
 * @param hDataset the dataset handle.
 * @param psOptions the options structure returned by GDALInfoOptionsNew() or NULL.
 * @return string corresponding to the information about the raster dataset (must be freed with CPLFree()), or NULL in case of error.
 *
 * @since GDAL 2.1
 */

void GDALInfo( GDALDatasetH hDataset, const GDALInfoOptions *psOptions, wxString& osStr)
{

/* -------------------------------------------------------------------- */
/*      Report general info.                                            */
/* -------------------------------------------------------------------- */
    GDALDriverH hDriver = GDALGetDatasetDriver( hDataset );

	Concat(osStr, psOptions->bStdoutOutput, "Driver: %s/%s\n",
		GDALGetDriverShortName(hDriver),
		GDALGetDriverLongName(hDriver));

    char **papszFileList = GDALGetFileList( hDataset );

    if( papszFileList == NULL || *papszFileList == NULL )
    {
		Concat(osStr, psOptions->bStdoutOutput,
			"Files: none associated\n");
    }
    else
    {
		Concat(osStr, psOptions->bStdoutOutput,
			"Files: %s\n", papszFileList[0]);
		if (psOptions->bShowFileList)
		{
			for (int i = 1; papszFileList[i] != NULL; i++)
				Concat(osStr, psOptions->bStdoutOutput,
					"       %s\n", papszFileList[i]);
		}
    }
    CSLDestroy( papszFileList );

	Concat(osStr, psOptions->bStdoutOutput,
		"Size is %d, %d\n",
		GDALGetRasterXSize(hDataset),
		GDALGetRasterYSize(hDataset));

/* -------------------------------------------------------------------- */
/*      Report projection.                                              */
/* -------------------------------------------------------------------- */
    if( GDALGetProjectionRef( hDataset ) != NULL )
    {
        char *pszProjection =
            const_cast<char *>( GDALGetProjectionRef( hDataset ) );

        OGRSpatialReferenceH hSRS =
            OSRNewSpatialReference(NULL);
        if( OSRImportFromWkt( hSRS, &pszProjection ) == CE_None )
        {
            char *pszPrettyWkt = NULL;

            OSRExportToPrettyWkt( hSRS, &pszPrettyWkt, FALSE );

			Concat(osStr, psOptions->bStdoutOutput,
				"Coordinate System is:\n%s\n",
				pszPrettyWkt);

            CPLFree( pszPrettyWkt );
        }
        else
        {
			Concat(osStr, psOptions->bStdoutOutput,
				"Coordinate System is `%s'\n",
				GDALGetProjectionRef(hDataset));
        }

        if ( psOptions->bReportProj4 )
        {
            char *pszProj4 = NULL;
            OSRExportToProj4( hSRS, &pszProj4 );

			Concat(osStr, psOptions->bStdoutOutput,
				"PROJ.4 string is:\n\'%s\'\n", pszProj4);

            CPLFree( pszProj4 );
        }

        OSRDestroySpatialReference( hSRS );
    }

/* -------------------------------------------------------------------- */
/*      Report Geotransform.                                            */
/* -------------------------------------------------------------------- */
    double adfGeoTransform[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    if( GDALGetGeoTransform( hDataset, adfGeoTransform ) == CE_None )
    {
		if (adfGeoTransform[2] == 0.0 && adfGeoTransform[4] == 0.0)
		{
			Concat(osStr, psOptions->bStdoutOutput,
				"Origin = (%.15f,%.15f)\n",
				adfGeoTransform[0], adfGeoTransform[3]);

			Concat(osStr, psOptions->bStdoutOutput,
				"Pixel Size = (%.15f,%.15f)\n",
				adfGeoTransform[1], adfGeoTransform[5]);
		}
		else
		{
			Concat(osStr, psOptions->bStdoutOutput, "GeoTransform =\n"
				"  %.16g, %.16g, %.16g\n"
				"  %.16g, %.16g, %.16g\n",
				adfGeoTransform[0],
				adfGeoTransform[1],
				adfGeoTransform[2],
				adfGeoTransform[3],
				adfGeoTransform[4],
				adfGeoTransform[5]);
		}
    }

/* -------------------------------------------------------------------- */
/*      Report GCPs.                                                    */
/* -------------------------------------------------------------------- */
    if( psOptions->bShowGCPs && GDALGetGCPCount( hDataset ) > 0 )
    {
        if (GDALGetGCPProjection(hDataset) != NULL)
        {
            char *pszProjection =
                const_cast<char *>( GDALGetGCPProjection( hDataset ) );

            OGRSpatialReferenceH hSRS =
                OSRNewSpatialReference(NULL);
            if( OSRImportFromWkt( hSRS, &pszProjection ) == CE_None )
            {
                char *pszPrettyWkt = NULL;

                OSRExportToPrettyWkt( hSRS, &pszPrettyWkt, FALSE );

				Concat(osStr, psOptions->bStdoutOutput,
					"GCP Projection = \n%s\n", pszPrettyWkt);

                CPLFree( pszPrettyWkt );
            }
            else
            {
				Concat(osStr, psOptions->bStdoutOutput,
					"GCP Projection = %s\n",
					GDALGetGCPProjection(hDataset));
            }

            OSRDestroySpatialReference( hSRS );
        }

        for( int i = 0; i < GDALGetGCPCount(hDataset); i++ )
        {
            const GDAL_GCP *psGCP = GDALGetGCPs( hDataset ) + i;

			Concat(osStr, psOptions->bStdoutOutput,
				"GCP[%3d]: Id=%s, Info=%s\n"
				"          (%.15g,%.15g) -> (%.15g,%.15g,%.15g)\n",
				i, psGCP->pszId, psGCP->pszInfo,
				psGCP->dfGCPPixel, psGCP->dfGCPLine,
				psGCP->dfGCPX, psGCP->dfGCPY, psGCP->dfGCPZ);
        }
    }

/* -------------------------------------------------------------------- */
/*      Report metadata.                                                */
/* -------------------------------------------------------------------- */

    GDALInfoReportMetadata( psOptions, hDataset, false, osStr );

/* -------------------------------------------------------------------- */
/*      Setup projected to lat/long transform if appropriate.           */
/* -------------------------------------------------------------------- */
    const char  *pszProjection = NULL;
    if( GDALGetGeoTransform( hDataset, adfGeoTransform ) == CE_None )
        pszProjection = GDALGetProjectionRef(hDataset);

    OGRCoordinateTransformationH hTransform = NULL;
    bool bTransformToWGS84 = false;

    if( pszProjection != NULL && strlen(pszProjection) > 0 )
    {
        OGRSpatialReferenceH hLatLong = NULL;

        OGRSpatialReferenceH hProj = OSRNewSpatialReference( pszProjection );
        if( hProj != NULL )
        {
            OGRErr eErr = OGRERR_NONE;
            // Check that it looks like Earth before trying to reproject to wgs84...
			hLatLong = OSRCloneGeogCS(hProj);
        }

        if( hLatLong != NULL )
        {
            CPLPushErrorHandler( CPLQuietErrorHandler );
            hTransform = OCTNewCoordinateTransformation( hProj, hLatLong );
            CPLPopErrorHandler();

            OSRDestroySpatialReference( hLatLong );
        }

        if( hProj != NULL )
            OSRDestroySpatialReference( hProj );
    }

/* -------------------------------------------------------------------- */
/*      Report corners.                                                 */
/* -------------------------------------------------------------------- */
	Concat(osStr, psOptions->bStdoutOutput, "Corner Coordinates:\n");
	GDALInfoReportCorner(psOptions, hDataset, hTransform,
		"Upper Left",
		0.0, 0.0, osStr);
	GDALInfoReportCorner(psOptions, hDataset, hTransform,
		"Lower Left",
		0.0, GDALGetRasterYSize(hDataset),
		osStr);
	GDALInfoReportCorner(psOptions, hDataset, hTransform,
		"Upper Right",
		GDALGetRasterXSize(hDataset), 0.0,
		osStr);
	GDALInfoReportCorner(psOptions, hDataset, hTransform,
		"Lower Right",
		GDALGetRasterXSize(hDataset),
		GDALGetRasterYSize(hDataset),
		osStr);
	GDALInfoReportCorner(psOptions, hDataset, hTransform,
		"Center",
		GDALGetRasterXSize(hDataset) / 2.0,
		GDALGetRasterYSize(hDataset) / 2.0,
		osStr);

    if( hTransform != NULL )
    {
        OCTDestroyCoordinateTransformation( hTransform );
        hTransform = NULL;
    }

/* ==================================================================== */
/*      Loop over bands.                                                */
/* ==================================================================== */
    for( int iBand = 0; iBand < GDALGetRasterCount( hDataset ); iBand++ )
    {

        GDALRasterBandH const hBand = GDALGetRasterBand( hDataset, iBand+1 );

        if( psOptions->bSample )
        {
            vector<float> ofSample(10000, 0);
            float * const pafSample = &ofSample[0];
            const int nCount =
                GDALGetRandomRasterSample( hBand, 10000, pafSample );
			Concat(osStr, psOptions->bStdoutOutput,
				"Got %d samples.\n", nCount);
        }

        int nBlockXSize = 0;
        int nBlockYSize = 0;
        GDALGetBlockSize( hBand, &nBlockXSize, &nBlockYSize );

		Concat(osStr, psOptions->bStdoutOutput,
			"Band %d Block=%dx%d Type=%s, ColorInterp=%s\n",
			iBand + 1,
			nBlockXSize, nBlockYSize,
			GDALGetDataTypeName(
				GDALGetRasterDataType(hBand)),
			GDALGetColorInterpretationName(
				GDALGetRasterColorInterpretation(hBand)));

        if( GDALGetDescription( hBand ) != NULL
            && strlen(GDALGetDescription( hBand )) > 0 )
        {
			Concat(osStr, psOptions->bStdoutOutput, "  Description = %s\n",
				GDALGetDescription(hBand));
        }

        {
            int bGotMin = FALSE;
            int bGotMax = FALSE;
            const double dfMin = GDALGetRasterMinimum( hBand, &bGotMin );
            const double dfMax = GDALGetRasterMaximum( hBand, &bGotMax );
            if( bGotMin || bGotMax || psOptions->bComputeMinMax )
            {
				Concat(osStr, psOptions->bStdoutOutput, "  ");
                    
                if( bGotMin )
                {
					Concat(osStr, psOptions->bStdoutOutput,
						"Min=%.3f ", dfMin);
                }
                if( bGotMax )
                {
					Concat(osStr, psOptions->bStdoutOutput,
						"Max=%.3f ", dfMax);
                }

                if( psOptions->bComputeMinMax )
                {
                    CPLErrorReset();
                    double adfCMinMax[2] = {0.0, 0.0};
                    GDALComputeRasterMinMax( hBand, FALSE, adfCMinMax );
                    if( CPLGetLastErrorType() == CE_None )
                    {
						Concat(osStr, psOptions->bStdoutOutput,
							"  Computed Min/Max=%.3f,%.3f",
							adfCMinMax[0], adfCMinMax[1]);
                    }
                }

				Concat(osStr, psOptions->bStdoutOutput, "\n");                    
            }
        }

        double dfMinStat = 0.0;
        double dfMaxStat = 0.0;
        double dfMean = 0.0;
        double dfStdDev = 0.0;
        CPLErr eErr = GDALGetRasterStatistics( hBand, psOptions->bApproxStats,
                                               psOptions->bStats,
                                               &dfMinStat, &dfMaxStat,
                                               &dfMean, &dfStdDev );
        if( eErr == CE_None )
        {
			Concat(osStr, psOptions->bStdoutOutput,
				"  Minimum=%.3f, Maximum=%.3f, Mean=%.3f, StdDev=%.3f\n",
				dfMinStat, dfMaxStat, dfMean, dfStdDev);
        }

        if( psOptions->bReportHistograms )
        {
            int nBucketCount = 0;
            GUIntBig *panHistogram = NULL;

            eErr = GDALGetDefaultHistogramEx( hBand, &dfMinStat, &dfMaxStat,
                                              &nBucketCount, &panHistogram,
                                              TRUE, GDALTermProgress,
                                              NULL );

            if( eErr == CE_None )
            {
				Concat(osStr, psOptions->bStdoutOutput,
					"  %d buckets from %g to %g:\n  ",
					nBucketCount, dfMinStat, dfMaxStat);

                for( int iBucket = 0; iBucket < nBucketCount; iBucket++ )
                {
					Concat(osStr, psOptions->bStdoutOutput,
						CPL_FRMT_GUIB " ", panHistogram[iBucket]);
                        
                }

				Concat(osStr, psOptions->bStdoutOutput, "\n");

                CPLFree( panHistogram );
            }
        }

        if ( psOptions->bComputeChecksum)
        {
            const int nBandChecksum =
                GDALChecksumImage(hBand, 0, 0,
                                  GDALGetRasterXSize(hDataset),
                                  GDALGetRasterYSize(hDataset));

			Concat(osStr, psOptions->bStdoutOutput,
				"  Checksum=%d\n", nBandChecksum);

        }

        int bGotNodata = FALSE;
        const double dfNoData = GDALGetRasterNoDataValue( hBand, &bGotNodata );
        if( bGotNodata )
        {
            if( CPLIsNan(dfNoData) )
            {
				Concat(osStr, psOptions->bStdoutOutput,
					"  NoData Value=nan\n");
            }
            else
            {
				Concat(osStr, psOptions->bStdoutOutput,
					"  NoData Value=%.18g\n", dfNoData);
            }
        }

        if( GDALGetOverviewCount(hBand) > 0 )
        {
            Concat(osStr, psOptions->bStdoutOutput, "  Overviews: " );

            for( int iOverview = 0;
                 iOverview < GDALGetOverviewCount(hBand);
                 iOverview++ )
            {
                if( iOverview != 0 )
                    Concat(osStr, psOptions->bStdoutOutput, ", " );

                GDALRasterBandH hOverview = GDALGetOverview( hBand, iOverview );
                if (hOverview != NULL)
                {
					Concat(osStr, psOptions->bStdoutOutput, "%dx%d",
						GDALGetRasterBandXSize(hOverview),
						GDALGetRasterBandYSize(hOverview));

                    const char *pszResampling =
                         GDALGetMetadataItem( hOverview, "RESAMPLING", "" );

                    if( pszResampling != NULL 
                        && STARTS_WITH_CI(pszResampling, "AVERAGE_BIT2") )
                        Concat(osStr, psOptions->bStdoutOutput, "*" );
                }
                else
                {
                    Concat(osStr, psOptions->bStdoutOutput, "(null)" );
                }
            }
            
			Concat(osStr, psOptions->bStdoutOutput, "\n" );

            if ( psOptions->bComputeChecksum )
            {
                Concat(osStr, psOptions->bStdoutOutput,
                       "  Overviews checksum: " );

                for( int iOverview = 0;
                     iOverview < GDALGetOverviewCount(hBand);
                     iOverview++ )
                {
                    GDALRasterBandH hOverview;

                    if( iOverview != 0 )
                        Concat(osStr, psOptions->bStdoutOutput, ", " );

                    hOverview = GDALGetOverview( hBand, iOverview );
                    if (hOverview)
                    {
                        Concat(osStr, psOptions->bStdoutOutput, "%d",
                                GDALChecksumImage(hOverview, 0, 0,
                                        GDALGetRasterBandXSize(hOverview),
                                        GDALGetRasterBandYSize(hOverview)));
                    }
                    else
                    {
                        Concat(osStr, psOptions->bStdoutOutput, "(null)" );
                    }
                }
                Concat(osStr, psOptions->bStdoutOutput, "\n" );
            }
        }

        if( GDALHasArbitraryOverviews( hBand ) )
        {
            Concat(osStr, psOptions->bStdoutOutput,
                   "  Overviews: arbitrary\n" );
        }

        const int nMaskFlags = GDALGetMaskFlags( hBand );
        if( (nMaskFlags & (GMF_NODATA|GMF_ALL_VALID)) == 0 )
        {
            GDALRasterBandH hMaskBand = GDALGetMaskBand(hBand) ;
            
			Concat(osStr, psOptions->bStdoutOutput, "  Mask Flags: " );

            if( nMaskFlags & GMF_PER_DATASET )
            {
                 Concat(osStr, psOptions->bStdoutOutput, "PER_DATASET " );
            }
            if( nMaskFlags & GMF_ALPHA )
            {
                Concat(osStr, psOptions->bStdoutOutput, "ALPHA " );
            }
            if( nMaskFlags & GMF_NODATA )
            {
				Concat(osStr, psOptions->bStdoutOutput, "NODATA ");
            }
            if( nMaskFlags & GMF_ALL_VALID )
            {
                Concat(osStr, psOptions->bStdoutOutput, "ALL_VALID " );
            }
            
			Concat(osStr, psOptions->bStdoutOutput, "\n" );

            if( hMaskBand != NULL &&
                GDALGetOverviewCount(hMaskBand) > 0 )
            {
                Concat(osStr, psOptions->bStdoutOutput,
                           "  Overviews of mask band: " );

                for( int iOverview = 0;
                     iOverview < GDALGetOverviewCount(hMaskBand);
                     iOverview++ )
                {
                    GDALRasterBandH hOverview;

					if (iOverview != 0)
						Concat(osStr, psOptions->bStdoutOutput, ", ");

                    hOverview = GDALGetOverview( hMaskBand, iOverview );

					Concat(osStr, psOptions->bStdoutOutput, "%dx%d",
						GDALGetRasterBandXSize(hOverview),
						GDALGetRasterBandYSize(hOverview));
                }
                Concat(osStr, psOptions->bStdoutOutput, "\n" );
            }
        }

        if( strlen(GDALGetRasterUnitType(hBand)) > 0 )
        {
			Concat(osStr, psOptions->bStdoutOutput,
				"  Unit Type: %s\n", GDALGetRasterUnitType(hBand));
        }

        if( GDALGetRasterCategoryNames(hBand) != NULL )
        {
            char **papszCategories = GDALGetRasterCategoryNames(hBand);

            Concat(osStr, psOptions->bStdoutOutput, "  Categories:\n" );

            for( int i = 0; papszCategories[i] != NULL; i++ )
            {
                Concat(osStr, psOptions->bStdoutOutput,
                           "    %3d: %s\n", i, papszCategories[i] );
            }
        }

        int bSuccess = FALSE;
        if( GDALGetRasterScale( hBand, &bSuccess ) != 1.0
            || GDALGetRasterOffset( hBand, &bSuccess ) != 0.0 )
        {
			Concat(osStr, psOptions->bStdoutOutput,
				"  Offset: %.15g,   Scale:%.15g\n",
				GDALGetRasterOffset(hBand, &bSuccess),
				GDALGetRasterScale(hBand, &bSuccess));
        }

        GDALInfoReportMetadata( psOptions, hBand, true, osStr );

        GDALColorTableH hTable;
        if( GDALGetRasterColorInterpretation(hBand) == GCI_PaletteIndex
            && (hTable = GDALGetRasterColorTable( hBand )) != NULL )
        {
            Concat( osStr, psOptions->bStdoutOutput,
                    "  Color Table (%s with %d entries)\n",
                    GDALGetPaletteInterpretationName( GDALGetPaletteInterpretation( hTable )),
                    GDALGetColorEntryCount( hTable ) );

            if (psOptions->bShowColorTable)
            {
                for( int i = 0; i < GDALGetColorEntryCount( hTable ); i++ )
                {
                    GDALColorEntry sEntry;

                    GDALGetColorEntryAsRGB( hTable, i, &sEntry );

					Concat(osStr, psOptions->bStdoutOutput,
						"  %3d: %d,%d,%d,%d\n",
						i,
						sEntry.c1,
						sEntry.c2,
						sEntry.c3,
						sEntry.c4);
                }
            }
        }

        if( psOptions->bShowRAT && GDALGetDefaultRAT( hBand ) != NULL )
        {
            GDALRasterAttributeTableH hRAT = GDALGetDefaultRAT( hBand );

			CPLXMLNode *psTree =
				((GDALRasterAttributeTable *)hRAT)->Serialize();
			char *pszXMLText = CPLSerializeXMLTree(psTree);
			CPLDestroyXMLNode(psTree);
			Concat(osStr, psOptions->bStdoutOutput, "%s\n", pszXMLText);
			CPLFree(pszXMLText);
        }
    }
}

/************************************************************************/
/*                        GDALInfoReportCorner()                        */
/************************************************************************/

static int
GDALInfoReportCorner( const GDALInfoOptions* psOptions,
                      GDALDatasetH hDataset,
                      OGRCoordinateTransformationH hTransform,
                      const char * corner_name,
                      double x,
                      double y,
                      wxString& osStr )

{
   Concat(osStr, psOptions->bStdoutOutput, "%-11s ", corner_name );

/* -------------------------------------------------------------------- */
/*      Transform the point into georeferenced coordinates.             */
/* -------------------------------------------------------------------- */
    double adfGeoTransform[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    double dfGeoX = 0.0;
    double dfGeoY = 0.0;

    if( GDALGetGeoTransform( hDataset, adfGeoTransform ) == CE_None )
    {
        dfGeoX = adfGeoTransform[0] + adfGeoTransform[1] * x
            + adfGeoTransform[2] * y;
        dfGeoY = adfGeoTransform[3] + adfGeoTransform[4] * x
            + adfGeoTransform[5] * y;
    }
    else
    {
		Concat(osStr, psOptions->bStdoutOutput, "(%7.1f,%7.1f)\n", x, y);

        return FALSE;
    }

/* -------------------------------------------------------------------- */
/*      Report the georeferenced coordinates.                           */
/* -------------------------------------------------------------------- */
    if( std::abs(dfGeoX) < 181 && std::abs(dfGeoY) < 91 )
    {
		Concat(osStr, psOptions->bStdoutOutput,
			"(%12.7f,%12.7f) ", dfGeoX, dfGeoY);
    }
    else
    {
		Concat(osStr, psOptions->bStdoutOutput,
			"(%12.3f,%12.3f) ", dfGeoX, dfGeoY);
    }

/* -------------------------------------------------------------------- */
/*      Transform to latlong and report.                                */
/* -------------------------------------------------------------------- */
	double dfZ = 0.0;
	if (hTransform != NULL
		&& OCTTransform(hTransform, 1, &dfGeoX, &dfGeoY, &dfZ))
	{
		Concat(osStr, psOptions->bStdoutOutput,
			"(%s,", GDALDecToDMS(dfGeoX, "Long", 2));
		Concat(osStr, psOptions->bStdoutOutput,
			"%s)", GDALDecToDMS(dfGeoY, "Lat", 2));
	}
	Concat(osStr, psOptions->bStdoutOutput, "\n");

    return TRUE;
}

/************************************************************************/
/*                       GDALInfoPrintMetadata()                        */
/************************************************************************/
static void GDALInfoPrintMetadata( const GDALInfoOptions* psOptions,
                                   GDALMajorObjectH hObject,
                                   const char *pszDomain,
                                   const char *pszDisplayedname,
                                   const char *pszIndent,
                                   wxString& osStr )
{
    const bool bIsxml =
        pszDomain != NULL &&
        STARTS_WITH_CI(pszDomain, "xml:");
    const bool bMDIsJson =
        pszDomain != NULL &&
        STARTS_WITH_CI(pszDomain, "json:");

    char **papszMetadata = GDALGetMetadata( hObject, pszDomain );
    if( papszMetadata != NULL && *papszMetadata != NULL )
    {
		Concat(osStr, psOptions->bStdoutOutput, "%s%s:\n", pszIndent,
			pszDisplayedname);

        for( int i = 0; papszMetadata[i] != NULL; i++ )
        {
			if (bIsxml || bMDIsJson)
				Concat(osStr, psOptions->bStdoutOutput,
					"%s%s\n", pszIndent, papszMetadata[i]);
			else
				Concat(osStr, psOptions->bStdoutOutput,
					"%s  %s\n", pszIndent, papszMetadata[i]);
        }
    }
}

/************************************************************************/
/*                       GDALInfoReportMetadata()                       */
/************************************************************************/
static void GDALInfoReportMetadata( const GDALInfoOptions* psOptions,
                                    GDALMajorObjectH hObject,
                                    bool bIsBand,
                                    wxString& osStr )
{
    const char* const pszIndent = bIsBand ? "  " : "";

    /* -------------------------------------------------------------------- */
    /*      Report list of Metadata domains                                 */
    /* -------------------------------------------------------------------- */
    if( psOptions->bListMDD )
    {
        char** papszMDDList = GDALGetMetadataDomainList( hObject );
        char** papszIter = papszMDDList;

        if( papszMDDList != NULL )
        {
			Concat(osStr, psOptions->bStdoutOutput,
				"%sMetadata domains:\n", pszIndent);
        }

        while( papszIter != NULL && *papszIter != NULL )
        {
            if( EQUAL(*papszIter, "") )
            {
				Concat(osStr, psOptions->bStdoutOutput,
					"%s  (default)\n", pszIndent);
            }
            else
            {
				Concat(osStr, psOptions->bStdoutOutput,
					"%s  %s\n", pszIndent, *papszIter);
            }

            papszIter ++;
        }

        CSLDestroy(papszMDDList);
    }

    if (!psOptions->bShowMetadata)
        return;

    /* -------------------------------------------------------------------- */
    /*      Report default Metadata domain.                                 */
    /* -------------------------------------------------------------------- */
    GDALInfoPrintMetadata( psOptions, hObject, NULL, "Metadata",
                           pszIndent, osStr );

    /* -------------------------------------------------------------------- */
    /*      Report extra Metadata domains                                   */
    /* -------------------------------------------------------------------- */
    if( psOptions->papszExtraMDDomains != NULL )
    {
        char **papszExtraMDDomainsExpanded = NULL;

        if( EQUAL(psOptions->papszExtraMDDomains[0], "all") &&
            psOptions->papszExtraMDDomains[1] == NULL )
        {
            char ** papszMDDList = GDALGetMetadataDomainList( hObject );
            char * const * papszIter = papszMDDList;

            while( papszIter != NULL && *papszIter != NULL )
            {
                if( !EQUAL(*papszIter, "") &&
                    !EQUAL(*papszIter, "IMAGE_STRUCTURE") &&
                    !EQUAL(*papszIter, "SUBDATASETS") &&
                    !EQUAL(*papszIter, "GEOLOCATION") &&
                    !EQUAL(*papszIter, "RPC") )
                {
                    papszExtraMDDomainsExpanded =
                        CSLAddString(papszExtraMDDomainsExpanded, *papszIter);
                }
                papszIter ++;
            }
            CSLDestroy(papszMDDList);
        }
        else
        {
            papszExtraMDDomainsExpanded =
                CSLDuplicate(psOptions->papszExtraMDDomains);
        }

        for( int iMDD = 0; papszExtraMDDomainsExpanded != NULL &&
                           papszExtraMDDomainsExpanded[iMDD] != NULL; iMDD++ )
        {
			wxString osDisplayedname =
				"Metadata (" +
				wxString(papszExtraMDDomainsExpanded[iMDD]) + ")";

			GDALInfoPrintMetadata(
				psOptions, hObject, papszExtraMDDomainsExpanded[iMDD],
				osDisplayedname.c_str(), pszIndent, osStr);
        }

        CSLDestroy(papszExtraMDDomainsExpanded);
    }

    /* -------------------------------------------------------------------- */
    /*      Report various named metadata domains.                          */
    /* -------------------------------------------------------------------- */
    GDALInfoPrintMetadata( psOptions, hObject, "IMAGE_STRUCTURE",
                           "Image Structure Metadata", pszIndent, osStr );

    if (!bIsBand)
    {
        GDALInfoPrintMetadata( psOptions, hObject, "SUBDATASETS", "Subdatasets",
                               pszIndent, osStr );
        GDALInfoPrintMetadata( psOptions, hObject, "GEOLOCATION", "Geolocation",
                               pszIndent, osStr );
        GDALInfoPrintMetadata( psOptions, hObject, "RPC", "RPC Metadata",
                               pszIndent, osStr );
    }
}

