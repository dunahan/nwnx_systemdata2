#include "stdafx.h"
#include "sys_utility.h"
#include "read_ini.h"
#include "write_ini.h"
#include "SystemData2.h"
#include "EnumProc.h"
#include "madCHook.h"
#include <sys/stat.h>	
#include <time.h>


WIN32_FIND_DATA FileData;
HANDLE hFile;
long START_SEC; 

CNWNXSystemData2::CNWNXSystemData2()
{
}

CNWNXSystemData2::~CNWNXSystemData2()
{
	OnRelease();
}

char* CNWNXSystemData2::OnRequest( char *gameObject, char* Request, char* Parameters )
{
	string sLog = "";

	CSysData CSysData( Parameters );

	if( strcmp( Request,"CHECK" ) == 0 ){

		char	cData[254];
		sLog = CSysData.GetParameter( 1 );
		strcpy( cData, sLog.c_str( ) );
		sLog = CSysData.GetParameter( 2 );
		strcat( cData, sLog.c_str( ) );
		sLog = CSysData.GetParameter( 3 );
		strcat( cData, sLog.c_str( ) );
		sLog = CSysData.GetParameter( 4 );
		strcat( cData, sLog.c_str( ) );
		sLog = CSysData.GetParameter( 5 );
		strcat( cData, sLog.c_str( ) );

		sLog = CSysData.ReturnStringToNWN( Parameters, cData, "CHECK" );
		Log( "%s\n", sLog.c_str( ) );
		return NULL;
	}

	else if( strcmp( Request,"TIME" ) == 0 ){

		char Time[50]; 	
		long lTime = time( NULL );

		if( CSysData.GetParameter( 1 ) == "1" )
			lTime -= 1199174400;

		ltoa( lTime, Time, 10 );
		sLog = CSysData.ReturnStringToNWN( Parameters, Time, "TIME" );
		Log( "%s\n", sLog.c_str( ) );
		return NULL;
	}

	else if( strcmp( Request,"READINIINT" ) == 0 ){

		string sData1 = CSysData.GetParameter( 1 );
		string sData2 = CSysData.GetParameter( 2 );
		string sData3 = CSysData.GetParameter( 4 );

		char cData1[255];
		char cData2[255];
		char cData3[255];

		strcpy( cData1, sData1.c_str( ) ); 
		strcpy( cData2, sData2.c_str( ) ); 
		strcpy( cData3, sData3.c_str( ) ); 

		CIniReader CIni( cData3 );
		int nInt = CIni.ReadInteger( cData1, cData2, CSysData.StringToInt( CSysData.GetParameter( 3 ) ) );

		sData1 = CSysData.IntToString( nInt );
		strcpy( cData1, sData1.c_str( ) );

		sLog = CSysData.ReturnStringToNWN( Parameters, cData1, "READINIINT" );
		Log( "%s\n", sLog.c_str( ) );
		return NULL;
	}

	else if( strcmp( Request,"WRITEINIINT" ) == 0 ){

		string sData1 = CSysData.GetParameter( 1 );
		string sData2 = CSysData.GetParameter( 2 );
		string sData3 = CSysData.GetParameter( 4 );

		char cData1[255];
		char cData2[255];
		char cData3[255];

		strcpy( cData1, sData1.c_str( ) ); 
		strcpy( cData2, sData2.c_str( ) ); 
		strcpy( cData3, sData3.c_str( ) ); 

		CIniWriter CIni( cData3 );
		CIni.WriteInteger( cData1, cData2, CSysData.StringToInt( CSysData.GetParameter( 3 ) ) );	

		sLog = CSysData.ReturnStringToNWN( Parameters, "VOID", "WRITEINIINT" );
		Log( "%s\n", sLog.c_str( ) );

		return NULL;

	}

	else if( strcmp( Request,"READINISTRING" ) == 0 ){

		string sData1 = CSysData.GetParameter( 1 );
		string sData2 = CSysData.GetParameter( 2 );
		string sData3 = CSysData.GetParameter( 4 );
		string sData4 = CSysData.GetParameter( 3 );

		char cData1[255];
		char cData2[255];
		char cData3[255];

		strcpy( cData1, sData1.c_str( ) ); 
		strcpy( cData2, sData2.c_str( ) ); 
		strcpy( cData3, sData3.c_str( ) );

		CIniReader CIni( cData3 );
		sData1 = CIni.ReadString( cData1, cData2, sData4.c_str( ) ); 

		strcpy( cData1, sData1.c_str( ) );

		sLog = CSysData.ReturnStringToNWN( Parameters, cData1, "READINISTRING" );
		Log( "%s\n", sLog.c_str( ) );
		return NULL;
	}

	else if( strcmp( Request,"WRITEINISTRING" ) == 0 ){

		string sData1 = CSysData.GetParameter( 1 );
		string sData2 = CSysData.GetParameter( 2 );
		string sData3 = CSysData.GetParameter( 4 );
		string sData4 = CSysData.GetParameter( 3 );

		char cData1[255];
		char cData2[255];
		char cData3[255];
		char cData4[255];

		strcpy( cData1, sData1.c_str( ) ); 
		strcpy( cData2, sData2.c_str( ) ); 
		strcpy( cData3, sData3.c_str( ) ); 
		strcpy( cData4, sData4.c_str( ) ); 

		CIniWriter CIni( cData3 );
		CIni.WriteString( cData1, cData2, cData4 );	

		sLog = CSysData.ReturnStringToNWN( Parameters, "VOID", "WRITEINISTRING" );
		Log( "%s\n", sLog.c_str( ) );

		return NULL;

	}
	
	else if( strcmp( Request,"READINIFLOAT" ) == 0 ){

		string sData1 = CSysData.GetParameter( 1 );
		string sData2 = CSysData.GetParameter( 2 );
		string sData3 = CSysData.GetParameter( 4 );
		string sData4 = CSysData.GetParameter( 3 );

		char cData1[255];
		char cData2[255];
		char cData3[255];

		strcpy( cData1, sData1.c_str( ) ); 
		strcpy( cData2, sData2.c_str( ) ); 
		strcpy( cData3, sData3.c_str( ) );

		CIniReader CIni( cData3 );

		sData1 = CSysData.FloatToString( CIni.ReadFloat( cData1, cData2, atof( sData4.c_str( ) ) ) );  

		strcpy( cData1, sData1.c_str( ) );

		sLog = CSysData.ReturnStringToNWN( Parameters, cData1, "READINIFLOAT" );
		Log( "%s\n", sLog.c_str( ) );
		return NULL;
	}

	else if( strcmp( Request,"WRITEINIFLOAT" ) == 0 ){

		string sData1 = CSysData.GetParameter( 1 );
		string sData2 = CSysData.GetParameter( 2 );
		string sData3 = CSysData.GetParameter( 4 );
		string sData4 = CSysData.GetParameter( 3 );

		char cData1[255];
		char cData2[255];
		char cData3[255];

		strcpy( cData1, sData1.c_str( ) ); 
		strcpy( cData2, sData2.c_str( ) ); 
		strcpy( cData3, sData3.c_str( ) ); 
		
		CIniWriter CIni( cData3 );
		CIni.WriteFloat( cData1, cData2, atof( sData4.c_str( ) ) );

		sLog = CSysData.ReturnStringToNWN( Parameters, "VOID", "WRITEINIFLOAT" );
		Log( "%s\n", sLog.c_str( ) );

		return NULL;

	}

	else if( strcmp( Request,"READINIBOOL" ) == 0 ){

		string sData1 = CSysData.GetParameter( 1 );
		string sData2 = CSysData.GetParameter( 2 );
		string sData3 = CSysData.GetParameter( 4 );

		char cData1[255];
		char cData2[255];
		char cData3[255];

		strcpy( cData1, sData1.c_str( ) ); 
		strcpy( cData2, sData2.c_str( ) ); 
		strcpy( cData3, sData3.c_str( ) ); 

		bool bData = false;

		string sData4 = CSysData.GetParameter( 3 );

		if( sData4 == "1" )
			bData = true;

		CIniReader CIni( cData3 );		

		if( CIni.ReadBoolean( cData1, cData2, bData ) )
			sLog = CSysData.ReturnStringToNWN( Parameters, "1", "READINIBOOL" );
		else
			sLog = CSysData.ReturnStringToNWN( Parameters, "0", "READINIBOOL" );

		Log( "%s\n", sLog.c_str( ) );
		return NULL;
	}

	else if( strcmp( Request,"WRITEINIBOOL" ) == 0 ){

		string sData1 = CSysData.GetParameter( 1 );
		string sData2 = CSysData.GetParameter( 2 );
		string sData3 = CSysData.GetParameter( 4 );

		char cData1[255];
		char cData2[255];
		char cData3[255];

		strcpy( cData1, sData1.c_str( ) ); 
		strcpy( cData2, sData2.c_str( ) ); 
		strcpy( cData3, sData3.c_str( ) ); 

		bool bData = false;

		if( CSysData.GetParameter( 3 ) == "1" )
			bData = true;

		CIniWriter CIni( cData3 );
		CIni.WriteBoolean( cData1, cData2, bData ); 	

		sLog = CSysData.ReturnStringToNWN( Parameters, "VOID", "WRITEINIBOOL" );
		Log( "%s\n", sLog.c_str( ) );

		return NULL;

	}

	else if( strcmp( Request,"STRFTIME" ) == 0 ){

		string sData1 = CSysData.GetParameter( 1 );
		string sData2 = CSysData.GetParameter( 2 );

		char cData1[255];

		time_t rawtime;
		struct tm * timeinfo;

		long nTime = CSysData.StringToInt( sData2 );

		if( nTime > 0 )
			rawtime = nTime;
		else
			time ( &rawtime );

		timeinfo = localtime ( &rawtime );

		strftime( cData1,255,sData1.c_str( ),timeinfo );

		sLog = CSysData.ReturnStringToNWN( Parameters, cData1, "STRFTIME" );
		Log( "%s\n", sLog.c_str( ) );

		return NULL;

	}

	else if( strcmp( Request,"SYSTEM" ) == 0 ){

		string sData1 = CSysData.GetParameter( 1 );

		char cData1[255];

		sData1 = CSysData.IntToString( system( sData1.c_str( ) ) );

		strcpy( cData1, sData1.c_str( ) );

		sLog = CSysData.ReturnStringToNWN( Parameters, cData1, "SYSTEM" );
		Log( "%s\n", sLog.c_str( ) );

		return NULL;

	}

	else if( strcmp( Request,"GETENV" ) == 0 ){

		string sData1 = CSysData.GetParameter( 1 );

		char cData1[255];

		sData1 = getenv( sData1.c_str( ) );

		strcpy( cData1, sData1.c_str( ) );

		sLog = CSysData.ReturnStringToNWN( Parameters, cData1, "GETENV" );
		Log( "%s\n", sLog.c_str( ) );

		return NULL;

	}

	else if( strcmp( Request,"INTTOASCII" ) == 0 ){

		string sData = "";
		string sData1 = "";

		for( int nLoop = 1; nLoop <= 5; nLoop++ ){

			if( CSysData.GetParameter( nLoop ) != "-1" ){
				sData = CSysData.GetParameter( nLoop );
				sData1 += (char)CSysData.StringToInt( sData );
			}
			else
				break;

		}

		char cData1[255];
		
		strcpy( cData1, sData1.c_str( ) );

		sLog = CSysData.ReturnStringToNWN( Parameters, cData1, "INTTOASCII" );
		Log( "%s\n", sLog.c_str( ) );

		return NULL;

	}

	else if( strcmp( Request,"ASCIITOINT" ) == 0 ){

		string sData1 = CSysData.GetParameter( 1 );
		
		int nData = (int)sData1.at( 0 );

		char cData1[255];

		sData1 = CSysData.IntToString( nData );

		strcpy( cData1, sData1.c_str( ) );

		sLog = CSysData.ReturnStringToNWN( Parameters, cData1, "ASCIITOINT" );
		Log( "%s\n", sLog.c_str( ) );

		return NULL;

	}

	else if( strcmp( Request,"GETFIRSTFILE" ) == 0 ){

		string sData1 = CSysData.GetParameter( 1 );

		char cData1[255];

		hFile = FindFirstFile( TEXT( sData1.c_str( ) ), &FileData );

		if( hFile == INVALID_HANDLE_VALUE )
			sData1 = "0";
		else
			sData1 = FileData.cFileName;

		strcpy( cData1, sData1.c_str( ) );

		sLog = CSysData.ReturnStringToNWN( Parameters, cData1, "GETFIRSTFILE" );
		Log( "%s\n", sLog.c_str( ) );

		return NULL;

	}

	else if( strcmp( Request,"GETNEXTFILE" ) == 0 ){

		string sData1 = CSysData.GetParameter( 1 );

		char cData1[255];

		if( hFile == INVALID_HANDLE_VALUE )
			sData1 = "0";

		else if( !FindNextFile( hFile, &FileData ) )
			sData1 = "0";

		else
			sData1 = FileData.cFileName;

		strcpy( cData1, sData1.c_str( ) );

		sLog = CSysData.ReturnStringToNWN( Parameters, cData1, "GETNEXTFILE" );
		Log( "%s\n", sLog.c_str( ) );

		return NULL;

	}

	else if( strcmp( Request,"GETFILEEXISTS" ) == 0 ){

		string sData1 = CSysData.GetParameter( 1 );

		WIN32_FIND_DATA dData;
		HANDLE haFile = FindFirstFile( TEXT( sData1.c_str( ) ), &dData );

		if( haFile == INVALID_HANDLE_VALUE )
			sData1 = "0";
		else
			sData1 = dData.cFileName;

		char * cData1 = new char[ sData1.length()+1 ];

		strcpy( cData1, sData1.c_str( ) );
		sLog = CSysData.ReturnStringToNWN( Parameters, cData1, "GETFILEEXISTS" );
		Log( "%s\n", sLog.c_str( ) );
		
		delete cData1;
		CloseHandle( haFile );
		return NULL;
	}

	else if( strcmp( Request,"GETLASTUSEDFILE" ) == 0 ){

		string sData1 = CSysData.GetParameter( 1 );

		WIN32_FIND_DATA dData;
		HANDLE haFile = FindFirstFile( TEXT( sData1.c_str( ) ), &dData );

		long nLatest = 0, nTemp = 0;
		string sReturn = "0";

		struct tm* clock;
		struct stat attrib;
		string sFile = "";

		while( haFile != INVALID_HANDLE_VALUE ){
			sFile = CSysData.StripFileName( sData1 )+dData.cFileName;
			stat( sFile.c_str(), &attrib );		
	
			clock = gmtime(&(attrib.st_mtime));	

			nTemp = mktime( clock );

			if( nTemp > nLatest ){

				nLatest = nTemp;
				sReturn = dData.cFileName;
			}
			
			if( !FindNextFile( haFile, &dData ) )
				haFile = INVALID_HANDLE_VALUE;
		}
			
		char * cData1 = new char[ sReturn.length()+1 ];

		strcpy( cData1, sReturn.c_str( ) );

		sLog = CSysData.ReturnStringToNWN( Parameters, cData1, "GETLASTUSEDFILE" );
		Log( "%s\n", sLog.c_str( ) );
		
		delete cData1;
		CloseHandle( haFile );
		return NULL;

	}

	else if( strcmp( Request,"GETFILEUNIX" ) == 0 ){

		string sData1 = CSysData.GetParameter( 1 );
		string sData2 = CSysData.GetParameter( 2 );

		WIN32_FIND_DATA dData;
		HANDLE haFile = CreateFile( sData2.c_str( ), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
	
		char cData1[255];

		SYSTEMTIME stTime;
		FILETIME ftCreate,ftAccess,ftWrite,ftTime;

		if( GetFileTime( haFile, &ftCreate, &ftAccess, &ftWrite ) == 0 )
			sData1 = "0"; 

		if( sData1 == "1" )			
			ftTime = ftCreate;

		else if( sData1 == "2" )	
			ftTime = ftAccess;
		
		else if( sData1 == "3" )
			ftTime = ftWrite;

		else{

			sLog = CSysData.ReturnStringToNWN( Parameters, "0", "GETFILEUNIX" );
			Log( "%s\n", sLog.c_str( ) );

			return NULL;
		}
	
		FileTimeToSystemTime(&ftTime, &stTime);	

		sData1 = CSysData.IntToString( CSysData.SystemTimeToUnix( stTime ) );
		
		strcpy( cData1, sData1.c_str( ) );

		sLog = CSysData.ReturnStringToNWN( Parameters, cData1, "GETFILEUNIX" );
		Log( "%s\n", sLog.c_str( ) );
		CloseHandle( haFile );
		return NULL;
	}

	else if( strcmp( Request,"GETFILESIZE" ) == 0 ){

		string sData1 = CSysData.GetParameter( 1 );
		string sData2 = CSysData.GetParameter( 2 );

		WIN32_FIND_DATA dData;
		HANDLE haFile;

		haFile = FindFirstFile( sData2.c_str(), &dData );	
		char cData1[255];

		int nReturn = -1;

		if( sData1 == "1" ){
			
			nReturn = dData.nFileSizeLow;
		}

		else if( sData1 == "2" ){	

			nReturn = dData.nFileSizeHigh;
		}
		
		sData1 = CSysData.IntToString( nReturn );
		
		strcpy( cData1, sData1.c_str( ) );

		sLog = CSysData.ReturnStringToNWN( Parameters, cData1, "GETFILESIZE" );
		Log( "%s\n", sLog.c_str( ) );
		CloseHandle( haFile );
		return NULL;
	}

	else if( strcmp( Request,"DELETEFILE" ) == 0 ){

		string sData1 = CSysData.GetParameter( 1 );
		char cData1[255];

		if( DeleteFile( sData1.c_str( ) ) == 0 )
			strcpy( cData1, "0" );

		else
			strcpy( cData1, "1" );

		sLog = CSysData.ReturnStringToNWN( Parameters, cData1, "DELETEFILE" );
		Log( "%s\n", sLog.c_str( ) );

	}

	else if( strcmp( Request,"MOVEFILE" ) == 0 ){
		
		string sData1 = CSysData.GetParameter( 1 );
		string sData2 = CSysData.GetParameter( 2 );
		string sData3 = CSysData.GetParameter( 3 );

		bool bOverWrite = false;

		if( sData3 == "1" )
			bOverWrite = true;

		char cData1[255];

		if( CopyFile( sData1.c_str( ), sData2.c_str( ), bOverWrite ) == 0 )
			strcpy( cData1, "0" );

		else{
			strcpy( cData1, "1" );
			DeleteFile( sData1.c_str( ) );
		}

		sLog = CSysData.ReturnStringToNWN( Parameters, cData1, "MOVEFILE" );
		Log( "%s\n", sLog.c_str( ) );

	}

	else if( strcmp( Request,"COPYFILE" ) == 0 ){
	
		string sData1 = CSysData.GetParameter( 1 );
		string sData2 = CSysData.GetParameter( 2 );
		string sData3 = CSysData.GetParameter( 3 );

		bool bOverWrite = false;

		if( sData3 == "1" )
			bOverWrite = true;

		char cData1[255];

		if( CopyFile( sData1.c_str( ), sData2.c_str( ), bOverWrite ) == 0 )
			strcpy( cData1, "0" );

		else
			strcpy( cData1, "1" );

		sLog = CSysData.ReturnStringToNWN( Parameters, cData1, "COPYFILE" );
		Log( "%s\n", sLog.c_str( ) );

	}

	else if( strcmp( Request,"RENAMEFILE" ) == 0 ){

		string sData1 = CSysData.GetParameter( 1 );
		string sData2 = CSysData.GetParameter( 2 );

		char cData1[255];

		if( rename( sData1.c_str( ), sData2.c_str( ) ) != 0 )
			strcpy( cData1, "0" );

		else
			strcpy( cData1, "1" );

		sLog = CSysData.ReturnStringToNWN( Parameters, cData1, "RENAMEFILE" );
		Log( "%s\n", sLog.c_str( ) );

	}

	else if( strcmp( Request,"GETRUNTIME" ) == 0 ){

		char Time[50]; 	

		long lTime = time( NULL ) - START_SEC;

		ltoa( lTime, Time, 10 );

		sLog = CSysData.ReturnStringToNWN( Parameters, Time, "GETRUNTIME" );

		Log( "%s\n", sLog.c_str( ) );

		return NULL;
	}

	else if( strcmp( Request,"TERMINATE" ) == 0 ){

		string sData1 = CSysData.GetParameter( 1 );

		char cData1[255];

		strcpy( cData1, sData1.c_str( ) );

		CFindKillProcess *pProcess = new CFindKillProcess( cData1 );

		if( pProcess->m_hp ) 
			pProcess->KillProcess( 0 ); 

		delete pProcess;
		pProcess = NULL;

		sLog = CSysData.ReturnStringToNWN( Parameters, "VOID", "TERMINATE" );

		Log( "%s\n", sLog.c_str( ) );
		return NULL;
	}	

	else if( strcmp( Request,"WRITETOTEXTFILE" ) == 0 ){

		string sData = CSysData.GetParameter( 2 );

		if( CSysData.GetParameter( 3 ) == "1" )
			sData += "\n"; 

		if( CSysData.WriteTextToFile( CSysData.GetParameter( 1 ), sData ) )
			sLog = CSysData.ReturnStringToNWN( Parameters, "1", "WRITETOTEXTFILE" );
		else
			sLog = CSysData.ReturnStringToNWN( Parameters, "0", "WRITETOTEXTFILE" );

		Log( "%s\n", sLog.c_str( ) );

		return NULL;
	}

	else if( strcmp( Request,"READFROMTEXTFILE" ) == 0 ){

		string sData = CSysData.ReadTextFromFile( CSysData.GetParameter( 1 ) );

		char * cData;

		cData = new char[CSysData.GetSize( )];

		if( sData.length( ) > CSysData.GetSize( ) )
			sData = sData.substr( 0, CSysData.GetSize( ) );
		
		strcpy( cData, sData.c_str( ) );

		sLog = CSysData.ReturnStringToNWN( Parameters, cData, "READFROMTEXTFILE" );

		Log( "%s\n", sLog.c_str( ) );

		delete cData;

		return NULL;
	}

	else if( strcmp( Request,"SETVALUE" ) == 0 ){

		int nValue = CSysData.StringToInt( CSysData.GetParameter( 1 ) );

		if (*(gameObject+0x8) == 0x6){

			*(int*)(gameObject+0x1F4) = nValue;

			sLog = CSysData.ReturnStringToNWN( Parameters, "1", "SETVALUE" );

			Log( "%s\n", sLog.c_str( ) );

			return NULL;
		}

		sLog = CSysData.ReturnStringToNWN( Parameters, "0", "SETVALUE" );

		Log( "%s\n", sLog.c_str( ) );

		return NULL;
	}

	//---Nogo event
	//-------------
	else{

		sLog = "X INVALID COMMAND <- ";
		sLog += Request;
		sLog += "( "+CSysData.GetParameter( 1 );
		sLog += ", "+CSysData.GetParameter( 2 );
		sLog += ", "+CSysData.GetParameter( 3 );
		sLog += ", "+CSysData.GetParameter( 4 );
		sLog += ", "+CSysData.GetParameter( 5 );
		sLog += " );";
		Log( "%s\n", sLog.c_str( ) );
		return NULL;
	}
	//-------------
	//-------------
	//-------------

	return NULL;
}

BOOL CNWNXSystemData2::OnCreate(const char* LogDir)
{	
	//Catch the start time
	//To return the number of seconds nwserver.exe has been running
	START_SEC = time( NULL );

	// call the base class function
	char log[MAX_PATH];
	sprintf (log, "%s\\nwnx_SystemData2.txt", LogDir);
	if (!CNWNXBase::OnCreate(log))
		return false;	

	WriteLogHeader();

	return true;
}

BOOL CNWNXSystemData2::OnRelease()
{
	Log("\no Shutting down.\n");

	CloseHandle( hFile );
	// call base class function
	return CNWNXBase::OnRelease();
}

void CNWNXSystemData2::WriteLogHeader()
{		
	Log( "NWNX Systemdata V.2.2\nCreated By Terra_777\n\n" );
}
