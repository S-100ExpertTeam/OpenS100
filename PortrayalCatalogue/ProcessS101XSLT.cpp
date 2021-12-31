#include "stdafx.h"
#include "../GeoMetryLibrary/ENCCommon.h"
#include "ProcessS101XSLT.h"
#include <sstream> 

ProcessS101XSLT::ProcessS101XSLT()
{
}


ProcessS101XSLT::~ProcessS101XSLT()
{
}

int checkParseError(IXMLDOMParseErrorPtr pError)
{
	long lineNumber;
	pError->get_line(&lineNumber);
	BSTR reason;
	pError->get_reason(&reason);
	_bstr_t parseError = _bstr_t("At line ") + _bstr_t(lineNumber) + _bstr_t("\n") + _bstr_t(reason);
	MessageBox(NULL, parseError, L"Parse Error", MB_OK);
	return -1;
}

void dump_com_error(_com_error &e)
{
	printf("Error\n");
	printf("\a\tCode = %08lx\n", e.Error());
	printf("\a\tCode meaning = %s", e.ErrorMessage());
	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());
	printf("\a\tSource = %s\n", (LPCSTR)bstrSource);
	printf("\a\tDescription = %s\n", (LPCSTR)bstrDescription);
}

char* XQ_ConvertBSTRToString(BSTR pSrc)
{
	if (!pSrc) return NULL;

	DWORD cb, cwch = ::SysStringLen(pSrc);//convert even embeded NULL

	char *szOut = NULL;

	if (cb = ::WideCharToMultiByte(CP_ACP, 0, pSrc, cwch + 1, NULL, 0, 0, 0))
	{
		szOut = new char[cb];
		if (szOut)
		{
			szOut[cb - 1] = '\0';

			if (!::WideCharToMultiByte(CP_ACP, 0, pSrc, cwch + 1, szOut, cb, 0, 0))
			{
				delete[]szOut;//clean up if failed;
				szOut = NULL;
			}
		}
	}

	return szOut;
};

int ProcessS101XSLT::ProcessS101XSLT2XML(std::wstring xsltPath, std::wstring inXMLPath, std::wstring outXMLPath)
{
	HRESULT hr;

	try{
		BOOL bResult = FALSE;
		short sResult = FALSE;

		MSXML2::IXMLDOMDocument2Ptr pStyleSheet = NULL;
		MSXML2::IXSLTemplatePtr pIXSLTemplate = NULL;
		MSXML2::IXSLProcessorPtr pXSLProcessor = NULL;

		hr = pIXSLTemplate.CreateInstance(__uuidof(MSXML2::XSLTemplate40));

		hr = pStyleSheet.CreateInstance(__uuidof(MSXML2::FreeThreadedDOMDocument40));
		pStyleSheet->async = VARIANT_FALSE;
		hr = pStyleSheet->load(xsltPath.c_str());
		//check on the parser error      
		if (hr != VARIANT_TRUE)
		{
			return checkParseError(pStyleSheet->parseError);
		}

		pIXSLTemplate->stylesheet = pStyleSheet.GetInterfacePtr();
		pXSLProcessor = pIXSLTemplate->createProcessor();

		MSXML2::IXMLDOMDocumentPtr   pInputDoc;
		MSXML2::IXMLDOMDocumentPtr   pOutputDoc;

		hr = pInputDoc.CreateInstance(__uuidof(MSXML2::FreeThreadedDOMDocument));
		pInputDoc->async = VARIANT_FALSE;
		hr = pInputDoc->load(inXMLPath.c_str());
		//check on the parser error      
		if (hr != VARIANT_TRUE)
		{
			return checkParseError(pInputDoc->parseError);
		}

		hr = pOutputDoc.CreateInstance(__uuidof(MSXML2::FreeThreadedDOMDocument));
		pOutputDoc->async = VARIANT_FALSE;
		
		//check on the parser error      
		//if (hr != VARIANT_TRUE)
		//{
		//	return checkParseError(pOutputDoc->parseError);
		//}

		pXSLProcessor->input = variant_t((IUnknown*)pInputDoc); //pInputDoc.GetInterfacePtr();
		pXSLProcessor->output = variant_t((IUnknown*)pOutputDoc);

		//<xsl:param name = "TWO_SHADES">true< / xsl:param>
		//<xsl:param name = "SAFETY_DEPTH" select = "30.0" / >
		//<xsl:param name = "SAFETY_CONTOUR" select = "30.0" / >
		//<xsl:param name = "SHALLOW_CONTOUR" select = "2.0" / >
		//<xsl:param name = "DEEP_CONTOUR" select = "30.0" / >
		//<xsl:param name = "SHALLOW_PATTERN">false< / xsl:param>
		//<xsl:param name = "SHOW_ISOLATED_DANGERS_IN_SHALLOW_WATERS">false< / xsl:param>
		//<xsl:param name = "FULL_SECTORS">false< / xsl:param>
		std::stringstream ss;
		std::string str = "";
		if (ENCCommon::TWO_SHADES)
			str = "true";
		else
			str = "false";
		hr = pXSLProcessor->addParameter("TWO_SHADES", str.c_str(), "");

		ss << ENCCommon::SAFETY_DEPTH;
		str = ss.str();
		hr = pXSLProcessor->addParameter("SAFETY_DEPTH", str.c_str(), "");
		ss.str(std::string());


		ss << ENCCommon::SAFETY_CONTOUR;
		str = ss.str();
		hr = pXSLProcessor->addParameter("SAFETY_CONTOUR", str.c_str(), "");
		ss.str(std::string());

		ss << ENCCommon::SHALLOW_CONTOUR;
		str = ss.str();
		hr = pXSLProcessor->addParameter("SHALLOW_CONTOUR", str.c_str(), "");
		ss.str(std::string());

		ss << ENCCommon::DEEP_CONTOUR;
		str = ss.str();
		hr = pXSLProcessor->addParameter("DEEP_CONTOUR", str.c_str(), "");
		ss.str(std::string());

		if (ENCCommon::SHALLOW_PATTERN)
			str = "true";
		else
			str = "false";
		hr = pXSLProcessor->addParameter("SHALLOW_PATTERN", str.c_str(), "");

		if (ENCCommon::SHOW_ISOLATED_DANGER_IN_SHALLOW_WATER)
			str = "true";
		else
			str = "false";
		hr = pXSLProcessor->addParameter("SHOW_ISOLATED_DANGERS_IN_SHALLOW_WATERS", str.c_str(), "");

		if (ENCCommon::FULL_SECTORS)
			str = "true";
		else
			str = "false";
		hr = pXSLProcessor->addParameter("FULL_SECTORS", str.c_str(), "");

		VARIANT_BOOL vtRet = pXSLProcessor->transform();
		if (vtRet != VARIANT_TRUE)
		{
			MessageBox(NULL, L"transformation failed", L"Error", MB_OK);
			return -1;
		}
		//_bstr_t bstrOutput = pXSLProcessor->Getoutput();
		//MessageBox(NULL, bstrOutput, L"Transformed Output", MB_OK);
		pOutputDoc->save(outXMLPath.c_str());

		pStyleSheet.Release();
		pIXSLTemplate.Release();
		pXSLProcessor.Release();
		pInputDoc.Release();
		pOutputDoc.Release();
	}
	catch (_com_error &e)
	{
		dump_com_error(e);
	}

	return 0;
}
