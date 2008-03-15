/*
   ComBlock - class representing a block of an Atari COM/EXE file

   Copyright (C) 2008 Matthias Reichl <hias@horus.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <sstream>
#include <iostream>
#include <iomanip>
#include "string.h"
#include "ComBlock.h"
#include "Error.h"
#include "AtariDebug.h"

ComBlock::ComBlock(RCPtr<FileIO>& f)
	: fData(0), fFileOffset(0)
{
	unsigned int endadr;
	Assert(f.IsNotNull());
	Assert(f->IsOpen());

	if (!f->ReadWord(fStartAddress)) {
		throw EOFError();
	}
	if (fStartAddress == 0xffff) {
		if (!f->ReadWord(fStartAddress)) {
			throw ReadError();
		}
	}
	if (!f->ReadWord(endadr)) {
		throw ReadError();
	}

	if (endadr < fStartAddress) {
		throw ErrorObject("invalid header in COM file");
	}

	fLen = endadr - fStartAddress + 1;

	fData = new unsigned char[fLen];

	fFileOffset = f->Tell();

	if (!f->ReadBlock(fData, fLen)) {
		ClearData();
		throw ReadError();
	}
}

ComBlock::ComBlock(const unsigned char* data, unsigned int len, unsigned int start_address)
	: fData(0), fFileOffset(0)
{
	if (!data) {
		Assert(false);
	}
	if (!len) {
		Assert(false);
	}
	fStartAddress = start_address;
	fLen = len;
	fData = new unsigned char[fLen];
	memcpy(fData, data, fLen);
}

ComBlock::~ComBlock()
{
	ClearData();
}

void ComBlock::ClearData()
{
	if (fData) {
		delete[] fData;
		fData = 0;
	}
}

bool ComBlock::WriteToFile(RCPtr<FileIO>& f, bool include_ffff) const
{
	unsigned int tmp;
	Assert(f.IsNotNull());
	Assert(f->IsOpen());

	if (include_ffff) {
		tmp = 0xffff;
		if (!f->WriteWord(tmp)) {
			return false;
		}
	}
	if (!f->WriteWord(fStartAddress)) {
		return false;
	}
	tmp = fStartAddress + fLen - 1;
	if (!f->WriteWord(tmp)) {
		return false;
	}

	return WriteRawToFile(f);
}

bool ComBlock::WriteRawToFile(RCPtr<FileIO>& f) const
{
	Assert(f.IsNotNull());
	Assert(f->IsOpen());
	if (!f->WriteBlock(fData, fLen)) {
		return false;
	}
	return true;
}

std::string ComBlock::GetDescription() const
{
	std::ostringstream s;

	s << std::hex
		<< std::setw(4) << std::setfill('0')
		<< fStartAddress
		<< '-'
		<< std::setw(4)
		<< (fStartAddress + fLen - 1)
		<< std::dec << std::setfill(' ')
		<< " (bytes: " 
		<< std::setw(5)
		<< fLen
	;
	if (fFileOffset > 0) {
		s << ", offset: "
			<< std::setw(6)
			<< fFileOffset
		;
	}
	s << ")";
	return s.str();
}
