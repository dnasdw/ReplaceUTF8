#include <sdw.h>

int UMain(int argc, UChar* argv[])
{
	if (argc != 5)
	{
		return 1;
	}
	string sPattern = UToU8(argv[3]);
	string sReplacement = UToU8(argv[4]);
	FILE* fp = UFopen(argv[1], USTR("rb"), false);
	if (fp == nullptr)
	{
		return 1;
	}
	fseek(fp, 0, SEEK_END);
	u32 uFileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* pTemp = new char[uFileSize + 1];
	fread(pTemp, 1, uFileSize, fp);
	fclose(fp);
	pTemp[uFileSize] = '\0';
	bool bBOM = false;
	if (uFileSize >= 3 && pTemp[0] == '\xEF' && pTemp[1] == '\xBB' && pTemp[2] == '\xBF')
	{
		bBOM = true;
	}
	string sText;
	if (bBOM)
	{
		sText = pTemp + 3;
	}
	else
	{
		sText = pTemp;
	}
	delete[] pTemp;
	sText = Replace(sText, sPattern, sReplacement);
	fp = UFopen(argv[2], USTR("wb"), false);
	if (fp == nullptr)
	{
		return 1;
	}
	if (bBOM)
	{
		fwrite("\xEF\xBB\xBF", 3, 1, fp);
	}
	fwrite(sText.c_str(), 1, sText.size(), fp);
	fclose(fp);
	return 0;
}
