#pragma once


#include "BaseParser.h"
#include "../NParser.h"
#include "../../base/NAutoPtr.h"

class NParserImpl : public nui::Parser::NParser
{
public:
    virtual nui::Base::NAutoPtr<nui::Base::NBaseObj> Parse(LPCTSTR packFilePath);

private:
    bool GetStyleParam(LPCTSTR packFilePath, nui::Base::NString& filePath, nui::Base::NString& styleName);
    nui::Base::NAutoPtr<nui::Data::NDataReader> FindStyleNode(nui::Base::NAutoPtr<nui::Data::NDataReader> dataReader, LPCTSTR styleName);
};
