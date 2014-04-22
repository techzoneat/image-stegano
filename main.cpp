#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include "EasyBMP.h"
#include <stdlib.h>
#include <fstream>

#include "utils.h"

using namespace std;

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;



void encode(string infile, string outfile, string payload)
{
	BMP input;
	if(!input.ReadFromFile(infile.c_str()))
	{
		cout << "The Bitmap doesn\'t exist!" << endl;
		return;
	}
	int msglength = payload.length() * 2;
	if(msglength > input.TellWidth() * input.TellHeight())
	{
		cout << "That message is too large for that image! (" << msglength << " as opposed to " << input.TellWidth() * input.TellHeight() << "; " << input.TellWidth() << " * " << input.TellHeight() << ")"<<endl;
		return;
	}
	stringstream msglength_ss;
	msglength_ss << setfill('0') << setw(8) << hex << msglength;
	string msglength_str = msglength_ss.str();

	uchar msgLength_split[8];
	for(uint i = 0; i < msglength_str.length(); i++)
	{
		msgLength_split[i] = (uchar)single_char_to_int(msglength_str[i]);
	}

	char* payload_split = new char[payload.length() * 2];
	for(uint i = 0; i < payload.length() * 2 - 1; i+=2)
	{
		payload_split[i] = payload[i / 2] >> 4;
		payload_split[i + 1] = payload[i/ 2] & 0x0F;
	}
	RGBApixel pix;
	for(int x = 0; x < 8; x++)
	{
		pix = input.GetPixel(x, 0);
		pix.Blue &= 0xF0;
		pix.Blue += msgLength_split[x];
		input.SetPixel(x, 0, pix);
	}

	for(int y = 0; y < input.TellHeight(); y++)
	{
		for(int x = 0; x < input.TellWidth(); x++)
		{
			if(y == 0 && x < 8)
				continue;
			pix = input.GetPixel(x, y);
			if(payload.length() * 2 > (uint)input.TellWidth() * y + x - 8)
			{
				pix.Blue &= 0xF0;
				pix.Blue += payload_split[input.TellWidth() * y + x - 8];
			}
			input.SetPixel(x, y, pix);
		}
	}
	fclose(fopen(outfile.c_str(),"w"));
	input.WriteToFile(outfile.c_str());
	delete[] payload_split;
}

string decode(string infile)
{
	BMP input;
	if(!input.ReadFromFile(infile.c_str()))
	{
		cout << "The input file didn't exist!" << endl;
		return "";
	}
	stringstream ret;
	int count = 0;
	char currentChar;
	RGBApixel pix;
	int msgLength = 0;
	stringstream msglength_ss;
	for(int x = 0; x < 8; x++)
	{
		pix = input.GetPixel(x, 0);
		msglength_ss << hex << (ushort)(pix.Blue & 0x0F);
	}
	msgLength = strtol(msglength_ss.str().c_str(), NULL, 16);
	for(int y = 0; y < input.TellHeight(); y++)
	{
		for(int x = 0; x < input.TellWidth(); x++)
		{
			if(input.TellWidth() * y + x - 8 > msgLength) goto end;
			if(y == 0 && x < 8)
				continue;
			pix = input.GetPixel(x, y);
			if(count % 2 == 0)
			{
				// Begin new Char
				currentChar = pix.Blue & 0x0F;
			}
			else
			{
				// Add to current char
				currentChar <<= 4;
				currentChar += pix.Blue & 0x0F;
				ret << currentChar;
			}
			count++;
		}
	}
	end:
		return ret.str();
}

int main(int argc, char **argv)
{
	if(argc != 5)
	{
		cout << "Usage: " << argv[0] << " [decode/encode] input.bmp output.bmp message.txt" << endl << endl;
		cout << "Note: output.bmp can be set to anything when decoding, but it has to be set to something" << endl;
		return 1;
	}
	if(strcmp(argv[1], "decode") == 0 || strcmp(argv[1], "d") == 0)
	{
		ofstream ofs(argv[4]);
		cout << "File open" << endl;
		ofs << decode(argv[2]);
		cout << "All done!" << endl;
		ofs.close();
	}
	else if(strcmp(argv[1], "encode") == 0 || strcmp(argv[1], "e") == 0)
	{
		ifstream ifs(argv[4]);
		if(!ifs)
		{
			cout << "The message file doesn't exist!" << endl;
			return 1;
		}
		string content( (istreambuf_iterator<char>(ifs) ),
						   (istreambuf_iterator<char>()    ) );
		cout << "File reading done" << endl;
		encode(argv[2], argv[3], content);
		cout << "All done!" << endl;
	}
	return 0;
}
