#pragma once

#include <cstdint>
#include <rax/shared.hpp>
#include <rax/collections/templates/map.hpp>

namespace rax::text
{
	

	enum class encoding_page : std::int32_t
	{
		Default = 0,
		NoOEM = 1,
		NoMac = 2,
		NoThread = 3,

		NoSymbol = 42,

		DuplicateEUCCN = 936,

		UTF16 = 1200,
		UTF16BE = 1201,

		Windows1252 = 1252,

		MacKorean = 10003,
		MacGB2312 = 10008,

		UTF32 = 12000,
		UTF32BE = 12001,

		ASCII = 20127,

		GB2312 = 20936,
		DLLKorean = 20949,

		ISO_8859_1 = 28591,
		ISO_8859_8_Visual = 28598,
		
		ISO_8859_8I = 38598,

		ISO2022JP = 50220,
		ISO2022JPESC = 50221,
		ISO2022JPSISO = 50222,
		ISOKorean = 50225,
		ISOSimplifiedCN = 50227,
		ENC50229 = 50229,

		EUCJP = 51932,
		EUCCN = 51936,
		EUCKR = 51949,

		ChineseHZ = 52936,
		
		GB18030 = 54936,

		ISCIIDevanagari = 57002,
		ISCIIBengali = 57003,
		ISCIITamil = 57004,
		ISCIITelugu = 57005,
		ISCIIAssemese = 57006,
		ISCIIOriya = 57007,
		ISCIIKannada = 57008,
		ISCIIMalayalam = 57009,
		ISCIIGujarathi = 57010,
		ISCIIPanjabi = 57011,

		UTF7 = 65000,
		UTF8 = 65001,
	};

	class encoding
	{
	private:
		static collections::templates::map<std::int32_t, encoding> converters_;

	public:
		static auto convert(encoding_page from, encoding_page to, const void* src, void* dest) -> std::int32_t;
		static auto convert(encoding_page from, encoding_page to, const void* src, void* dest, std::uint32_t start, std::uint32_t length) -> std::int32_t;

		
	};
}
