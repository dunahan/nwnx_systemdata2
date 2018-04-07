#include "iostream"
#include "fstream"
#include "sstream"
#include "string"
#include "ctime"
//#include "SystemData2.h"
//#include "stdafx.h"
#include "windows.h"
#include "stdio.h"

using namespace std;

class CSysData{

public:

	CSysData ( char *sNwnParams ){

		string sParams = sNwnParams;

		int nLen = sParams.length();

		int nNext = 0, nPrev = 0, nArray = 0;

		while( true ){

			//Find the ";" which are used to seperate the parameters
			nPrev = nNext;
			nNext = sParams.find( ";", nPrev+1 ); 
			
			//Break loop and determine useable size; could be good to know
			if( nNext == -1 || nArray > 4 ){

				USEABLE_SIZE = sParams.find( "|", nPrev+1 );		

				break;
			}
			//Cutup & assign
			switch( nArray ){

				case 0: PARAMETER1 = sParams.substr( nPrev + 1, nNext - nPrev - 1 );break;
				case 1: PARAMETER2 = sParams.substr( nPrev + 1, nNext - nPrev - 1 );break;
				case 2: PARAMETER3 = sParams.substr( nPrev + 1, nNext - nPrev - 1 );break;
				case 3: PARAMETER4 = sParams.substr( nPrev + 1, nNext - nPrev - 1 );break;
				case 4: PARAMETER5 = sParams.substr( nPrev + 1, nNext - nPrev - 1 );break;

				default:return;

			}

			nArray++;
		}
	
	}

	//Get specific parameter
	void HandleParametersFromCall( char* sNwnParams );

	//Returns param 1,2,3,4 or 5
	string GetParameter( int nParam );

	//Returns the size of the memoryspace
	int GetSize( );

	//Preps up and merges Data into Space for nwn to use
	//Returns the string that supposed to go into the logfile
	string ReturnStringToNWN( char *CReturnMemorySpace, char *CData, char *CRequest );

	//Returns iInt as a string
	string IntToString( int iInt );

	//Returns sString as a number
	int StringToInt( string sString );

	//Returns fData as a string
	string FloatToString( float fData );

	//Returns wchar** as a string
	//Not actually used (multibyte set)
	string WCharToString( const wstring &wstr );

	long SystemTimeToUnix( SYSTEMTIME stTime );

	string StripFileName( string sFullFileName );

	string ReadTextFromFile( string sFile );

	bool WriteTextToFile( string sFile, string sData );

private:

	//Assigned in the constructor

	//The induvirual params
	string PARAMETER1;
	string PARAMETER2;
	string PARAMETER3;
	string PARAMETER4;
	string PARAMETER5;

	//Size we got to work with
	int USEABLE_SIZE;

	ifstream I_FILE;
	ofstream O_FILE;
	
protected:

};


bool CSysData::WriteTextToFile( string sFile, string sData ){

	O_FILE.open( sFile.c_str( ), ios_base::app );
	
	bool bRet = false;

	if( O_FILE.good( ) ){

		O_FILE << sData;
		bRet = true;
	}

	O_FILE.close( );

	return bRet;
}

string CSysData::ReadTextFromFile( string sFile ){

	I_FILE.open( sFile.c_str( ), ios_base::app );

	sFile = "";
	while( !I_FILE.eof( ) ){

		sFile += I_FILE.get( );		
	}

	I_FILE.close( );

	return sFile;
}

string CSysData::GetParameter( int nParam ){

	string sReturn = "";

	switch( nParam ){

		case 1: sReturn = PARAMETER1;break;
		case 2: sReturn = PARAMETER2;break;
		case 3: sReturn = PARAMETER3;break;
		case 4: sReturn = PARAMETER4;break;
		case 5: sReturn = PARAMETER5;break;
		default:return "";
	}

	return sReturn;
}

int CSysData::GetSize( ){

	return USEABLE_SIZE; 
}

string CSysData::ReturnStringToNWN( char *CReturnMemorySpace, char *CData, char *CRequest ){

	//Convert to string
	string sReturnData = CData;

	//Assemble and for returning
	string sLog = "o " + sReturnData + " <- " + CRequest + "( " + PARAMETER1 + ", " + PARAMETER2 + ", "  + PARAMETER3 + ", "  + PARAMETER4 + ", "  + PARAMETER5 + " );"; 
	
	//Add the endtag which the game looks for
	sReturnData += "|";

	//Asign CData its new value
	strncpy( CReturnMemorySpace, sReturnData.c_str( ) , sReturnData.length( ) );

	return sLog;

}

string CSysData::IntToString( int iInt ){
	
	ostringstream buffer;
	buffer << iInt;
	string sReturn = buffer.str( );
	return sReturn;
}

int CSysData::StringToInt( string sString ){
	
	istringstream buffer( sString );
	int iInt;
	buffer >> iInt;
	return iInt;
}

string CSysData::FloatToString( float fData ){

	ostringstream buffer;
	buffer << fData;
	string sReturn = buffer.str( );
	return sReturn;
}

string CSysData::WCharToString( const wstring &wstr )
{
	//Dirty but works 
    string sRet;
    char *cRet = new char[wstr.length() + 1];
    cRet[wstr.size()] = '\0';
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, cRet, (int)wstr.length(), NULL, NULL);
    sRet = cRet;
    delete[] cRet;
    return sRet;
}

long CSysData::SystemTimeToUnix( SYSTEMTIME stTime ){

		time_t tt_secs;

		struct tm * timeinfo;

		time ( &tt_secs );
		timeinfo = localtime ( &tt_secs );
	
		timeinfo->tm_mday	= stTime.wDay;
		timeinfo->tm_min	= stTime.wMinute;		
		timeinfo->tm_sec	= stTime.wSecond;
		timeinfo->tm_wday	= stTime.wDayOfWeek;
		timeinfo->tm_isdst  = -1;
		//Offset fix
		timeinfo->tm_hour	= stTime.wHour+2;
		timeinfo->tm_mon	= stTime.wMonth-1;
		timeinfo->tm_year	= stTime.wYear-1900;

		return mktime( timeinfo );
}

string CSysData::StripFileName( string sFullFileName ){

	int nPrev = 0, nNext = sFullFileName.find( "/", 0 );

	while( nNext != -1 ){
	
		nPrev = nNext;
	
		nNext = sFullFileName.find( "/", nPrev+1 );

	}

	return sFullFileName.substr( 0, nPrev+1 );
}