#pragma once


class TestStringBundle : public testing::Test
{
public:
    TestStringBundle()
    {
    }
    virtual void SetUp()
    {
    }
    virtual void TearDown()
    {
    }
};

TEST_F(TestStringBundle, Basic)
{
    nui::Base::NInstPtr<nui::Data::NStringBundle> stringBundle(MemToolParam);
    struct
    {
        LPCTSTR id;
        LPCTSTR value;
    } testData[] =
    {
        {   _T("@1:11"), _T("111") },
        {   _T("@1:12"), _T("112") },
        {   _T("@1:13"), _T("113") },
        {   _T("@1:14"), _T("114") },
        {   _T("@1:15"), _T("115") },
        {   _T("@1:16"), _T("116") },
        {   _T("@1:17"), _T("117") },
        {   _T("@1:18"), _T("118") },
        {   _T("@1:19"), _T("119") },
        {   _T("@1:10"), _T("110") },
        {   _T("@1:Test"), _T("我只是一个测试") },
        {   _T("@1:31"), _T("") },
        {   _T("@2:11"), _T("") },
    };
    for(int i=0; i<_countof(testData); ++ i)
    {
        nui::Base::NString value = stringBundle->GetString(testData[i].id);
        EXPECT_EQ(value, testData[i].value);
    }
}
