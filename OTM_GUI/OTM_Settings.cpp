/*
This file is part of OpenTexMod.


OpenTexMod is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenTexMod is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with OpenTexMod.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "OTM_Main.h"


OTM_Settings::OTM_Settings(void)
{
  XSize=600;
  YSize=400;
  Language="English";
}

#define SETTINGS_FILE "OTM_Settings.txt"

int OTM_Settings::Load(void)
{
  wxFile file;

  if (!file.Access(SETTINGS_FILE, wxFile::read)) {return -1;}
  file.Open(SETTINGS_FILE, wxFile::read);
  if (!file.IsOpened()) return -1;

  unsigned len = file.Length();

  unsigned char* buffer;
  try {buffer = new unsigned char [len+2];}
  catch (...) {return -1;}

  unsigned int result = file.Read( buffer, len);
  file.Close();

  if (result != len) {delete [] buffer; return -1;}

  wchar_t *buff = (wchar_t*)buffer;
  len/=2;
  buff[len]=0;

  wxString content;
  content =  buff;
  delete [] buffer;

  wxStringTokenizer token( content, "\n");

  int num = token.CountTokens();
  wxString line;
  wxString command;
  wxString value;
  for (int i=0; i<num; i++)
  {
    line = token.GetNextToken();
    command = line.BeforeFirst(':');
    value = line.AfterFirst(':');
    value.Replace( "\r", "");
    value.Replace( "\n", "");
    if (command == "Lang") Language = value;
    else if (command == "x_size")
    {
      long x;
      if (value.ToLong( &x)) XSize=x;
    }
    else if (command == "y_size")
    {
      long y;
      if (value.ToLong( &y)) YSize=y;
    }
  }

  return 0;
}

int OTM_Settings::Save(void)
{
  wxFile file;
  file.Open(SETTINGS_FILE, wxFile::write);
  if (!file.IsOpened()) return -1;

  wxString content;

  content = "Lang:";
  content << Language << "\n";
  file.Write( content.wc_str(), content.Len()*2);

  content.Printf("x_size:%d\n", XSize);
  file.Write( content.wc_str(), content.Len()*2);

  content.Printf("y_size:%d\n", YSize);
  file.Write( content.wc_str(), content.Len()*2);
  file.Close();

  return 0;

}

